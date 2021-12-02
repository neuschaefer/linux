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
 * $RCSfile: drv_display.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_display.c
 *  Brief of file drv_display.c.
 *  Details of file drv_display.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#ifdef CC_SCPOS_3DTV_SUPPORT

//SUPPORT FEATURE
#ifndef SUPPORT_FS_FREERUN
#define SUPPORT_FS_FREERUN (0)
#endif

#ifndef SUPPORT_3D_EXT_COMP_TRL
#if defined(CC_MT5365)
#define SUPPORT_3D_EXT_COMP_TRL (1)
#else
#define SUPPORT_3D_EXT_COMP_TRL (0)
#endif
#endif

#ifndef SUPPORT_3D_SG_CTRL
#define SUPPORT_3D_SG_CTRL (0)
#endif

#ifndef SUPPORT_3D_BL_CTRL
#define SUPPORT_3D_BL_CTRL (0)
#endif

#ifndef SUPPORT_3D_WITHOUT_MJC
#define SUPPORT_3D_WITHOUT_MJC (0)
#endif

#ifndef SUPPORT_3D_EMBEDED_OSD
#define SUPPORT_3D_EMBEDED_OSD (0)
#endif

#ifndef SUPPORT_3D_ANALOG_SRC
#define SUPPORT_3D_ANALOG_SRC (1)
#endif

#ifndef SUPPORT_3D_FOR_HD_ONLY
#ifdef CC_MT5365
#define SUPPORT_3D_FOR_HD_ONLY (0)
#else
#define SUPPORT_3D_FOR_HD_ONLY (0)
#endif
#endif

#ifndef SUPPORT_PSEUDO_MVC
#define SUPPORT_PSEUDO_MVC (0)
#endif

#else

#define SUPPORT_FS_FREERUN (0)
#define SUPPORT_3D_EXT_COMP_TRL (0)
#define SUPPORT_3D_SG_CTRL (0)
#define SUPPORT_3D_BL_CTRL (0)
#define SUPPORT_3D_WITHOUT_MJC (0)
#define SUPPORT_3D_EMBEDED_OSD (0)
#define SUPPORT_3D_ANALOG_SRC (0)
#define SUPPORT_3D_FOR_HD_ONLY (0)
#define SUPPORT_PSEUDO_MVC (0)

#endif

UINT64 u8VdoPathProcCfg[NUM_OF_DECOER][NUM_OF_PACKING_TYPE]=
{
#if defined(MT5366_SUPPORT_3D_PR)
    //B2R
    {
        0,//E_TDTV_DECODER_INPUT_AUTO
        0,//E_TDTV_DECODER_INPUT_RESERVE
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE   ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE   ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        //TTD
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_60_PR,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_60_PR,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_P                  
        //FS
#if SUPPORT_PSEUDO_MVC        
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FS_I ,E_TDTV_DECODER_OUTPUT_3D_2_2D ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_60_PR,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FS_P ,E_TDTV_DECODER_OUTPUT_3D_2_2D ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_60_PR,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_P                  
#else
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FS_I ,E_TDTV_DECODER_OUTPUT_3D_2_2D ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FS_P ,E_TDTV_DECODER_OUTPUT_3D_2_2D ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
#endif
        0,//E_TDTV_DECODER_INPUT_FP_I
        0,//E_TDTV_DECODER_INPUT_FP_P
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_TB_P   ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_TB_P   ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        //SBS
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_SBS_P  ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_SBS_P  ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        //REAL_D
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_RD_P  ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_RD_P  ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        //Sensio
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BYPASS,E_TDTV_DECODER_INPUT_SBS_Q_I,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D    ,E_TDTV_DECODER_INPUT_SBS_Q_P,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        //Quin Cunx
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BYPASS,E_TDTV_DECODER_INPUT_SBS_Q_I,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_3D_Q  ,E_TDTV_DECODER_INPUT_SBS_Q_P,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_2D),
        0,//E_TDTV_DECODER_INPUT_SBS_Q_I   =20,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_SBS_Q_P   =21,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_FS_Q_I    =22,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_FS_Q_P    =23,//Quin Cunx            
        //Line interleave            
        0,// E_TDTV_DECODER_INPUT_LI_I    =24,//Line Interleave
        0,// E_TDTV_DECODER_INPUT_LI_P    =25,//Line Interleave            
    },
    //HDMI
    {
        0,//E_TDTV_DECODER_INPUT_AUTO    
        0,//E_TDTV_DECODER_INPUT_RESERVE
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        //TTD
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_P                  
        //FS, Consider As Frame Packing
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_I ,E_TDTV_DECODER_OUTPUT_3D_FS  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_3D_FPI  ,E_TDTV_DECODER_INPUT_FS_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_P ,E_TDTV_DECODER_OUTPUT_3D_FS  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_3D_I,E_TDTV_DECODER_INPUT_FS_P,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        //FP,
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_I ,E_TDTV_DECODER_OUTPUT_NATIVE  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_FP_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_P ,E_TDTV_DECODER_OUTPUT_NATIVE  ,E_TDTV_NR_PROGRESSIVE  ,E_TDTV_DI_2D ,E_TDTV_DECODER_INPUT_FP_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        //TB
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_TB_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_TB_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        //SBS
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR  ,E_TDTV_MJC_3D),
        //REAL_D
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D  ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        //Sensio
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BYPASS,E_TDTV_DECODER_INPUT_SBS_Q_I ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D    ,E_TDTV_DECODER_INPUT_SBS_Q_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_3D),
        //Quin Cunx
        0,//E_TDTV_DECODER_INPUT_2D_Q_I    =18,//Quin Cunx ,Current No use
        0,//E_TDTV_DECODER_INPUT_2D_Q_P    =19,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_SBS_Q_I   =20,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_SBS_Q_P   =21,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_FS_Q_I    =22,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_FS_Q_P    =23,//Quin Cunx            
        //Line interleave            
        0,// E_TDTV_DECODER_INPUT_LI_I    =24,//Line Interleave
        0,// E_TDTV_DECODER_INPUT_LI_P    =25,//Line Interleave            
    },
    //Others
    {
        0,//E_TDTV_DECODER_INPUT_AUTO    
        0,//E_TDTV_DECODER_INPUT_RESERVE
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        //TTD
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_3D_60_PR ,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_P                  
        0,//E_TDTV_DECODER_INPUT_FS_I 
        0,//E_TDTV_DECODER_INPUT_FS_P 
        0,//E_TDTV_DECODER_INPUT_FP_I
        0,//E_TDTV_DECODER_INPUT_FP_P
        0,//E_TDTV_DECODER_INPUT_TB_I 
        0,//E_TDTV_DECODER_INPUT_TB_P 
        0,//E_TDTV_DECODER_INPUT_SBS_I
        0,//E_TDTV_DECODER_INPUT_SBS_P
        0,//E_TDTV_DECODER_INPUT_RD_I 
        0,//E_TDTV_DECODER_INPUT_RD_P 
        0,//E_TDTV_DECODER_INPUT_SS_I 
        0,//E_TDTV_DECODER_INPUT_SS_P 
        //Quin Cunx
        0,//E_TDTV_DECODER_INPUT_2D_Q_I    =18,//Quin Cunx ,Current No use
        0,//E_TDTV_DECODER_INPUT_2D_Q_P    =19,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_SBS_Q_I   =20,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_SBS_Q_P   =21,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_FS_Q_I    =22,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_FS_Q_P    =23,//Quin Cunx     
        //Line interleave            
        0,// E_TDTV_DECODER_INPUT_LI_I    =24,//Line Interleave
        0,// E_TDTV_DECODER_INPUT_LI_P    =25,//Line Interleave            
    },       
#else
    //B2R
    {
        0,//E_TDTV_DECODER_INPUT_AUTO
        0,//E_TDTV_DECODER_INPUT_RESERVE
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        //TTD
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_REPEAT,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_REPEAT,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_P                  
        //FS
#ifdef  CC_B2R_3D_MVC_INTERLACE_SUPPORT
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FS_I ,E_TDTV_DECODER_OUTPUT_3D_FS   ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_BYPASS,E_TDTV_DECODER_INPUT_FS_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_3D),
#else
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FS_I ,E_TDTV_DECODER_OUTPUT_3D_2_2D ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_REPEAT,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
#endif        
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FS_P ,E_TDTV_DECODER_OUTPUT_3D_FS  ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_3D_I       ,E_TDTV_DECODER_INPUT_FS_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_3D),
        0,//E_TDTV_DECODER_INPUT_FP_I
        0,//E_TDTV_DECODER_INPUT_FP_P
        //TB
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_TB_I   ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_SYNC_PADDING,E_TDTV_DI_BYPASS     ,E_TDTV_DECODER_INPUT_TB_P ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //SBS 
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I,E_TDTV_DECODER_OUTPUT_NATIVE  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_SBS_P  ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE  ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_SBS_P  ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //REAL_D
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_RD_P  ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_RD_P  ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //Sensio
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BYPASS     ,E_TDTV_DECODER_INPUT_SBS_Q_I,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_SBS_Q_P,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //Quin Cunx
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BYPASS     ,E_TDTV_DECODER_INPUT_SBS_Q_I,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_3D_Q       ,E_TDTV_DECODER_INPUT_SBS_Q_P,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_MJC_2D),
        0,//E_TDTV_DECODER_INPUT_SBS_Q_I   =20,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_SBS_Q_P   =21,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_FS_Q_I    =22,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_FS_Q_P    =23,//Quin Cunx            
        //Line interleave            
        0,//E_TDTV_DECODER_INPUT_LI_I    =24,//Line Interleave
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_SNR_TNR_BYPASS ,E_TDTV_DI_2D      ,E_TDTV_DECODER_INPUT_LI_P   ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
    },
    //HDMI
    {
        0,//E_TDTV_DECODER_INPUT_AUTO    
        0,//E_TDTV_DECODER_INPUT_RESERVE
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        //TTD
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_REPEAT,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_REPEAT,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_P                  
        //FS, Consider As Frame Packing
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_I ,E_TDTV_DECODER_OUTPUT_3D_FS  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_3D_FPI     ,E_TDTV_DECODER_INPUT_FS_P ,E_TDTV_DECODER_OUTPUT_3D_PREDOWN,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_P ,E_TDTV_DECODER_OUTPUT_3D_FS  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_3D_I       ,E_TDTV_DECODER_INPUT_FS_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_3D),
        //FP,
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_I ,E_TDTV_DECODER_OUTPUT_3D_FS  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_3D_FPI     ,E_TDTV_DECODER_INPUT_FS_P ,E_TDTV_DECODER_OUTPUT_3D_PREDOWN,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_FP_P ,E_TDTV_DECODER_OUTPUT_3D_FS  ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_3D_I       ,E_TDTV_DECODER_INPUT_FS_P ,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_3D),
        //TB
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_TB_I ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_SYNC_PADDING,E_TDTV_DI_BYPASS     ,E_TDTV_DECODER_INPUT_TB_P ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //SBS
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_SBS_P,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_SBS_P,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //REAL_D
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        //Sensio
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BYPASS     ,E_TDTV_DECODER_INPUT_SBS_Q_I ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D         ,E_TDTV_DECODER_INPUT_SBS_Q_P ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        //Quin Cunx
        0,//E_TDTV_DECODER_INPUT_2D_Q_I    =18,//Quin Cunx ,Current No use
        0,//E_TDTV_DECODER_INPUT_2D_Q_P    =19,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_SBS_Q_I   =20,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_SBS_Q_P   =21,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_FS_Q_I    =22,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_FS_Q_P    =23,//Quin Cunx            
        //Line interleave           
        0,//E_TDTV_DECODER_INPUT_LI_I    =24,//Line Interleave
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_SNR_TNR_BYPASS ,E_TDTV_DI_2D      ,E_TDTV_DECODER_INPUT_LI_P   ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
    },
    //Others
    {
        0,//E_TDTV_DECODER_INPUT_AUTO    
        0,//E_TDTV_DECODER_INPUT_RESERVE
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE,E_TDTV_MJC_2D),
        //TTD
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE ,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_REPEAT,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_I
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE,E_TDTV_DI_2D,E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_3D_REPEAT,E_TDTV_MJC_TTD),//E_TDTV_DECODER_INPUT_TTD_P                  
        //FS, Consider As Frame Packing
        0,
        0,
        //FP,
        0,
        0,
        //TB
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING  ,E_TDTV_DECODER_INPUT_TB_I ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_SYNC_PADDING ,E_TDTV_DI_2D          ,E_TDTV_DECODER_INPUT_TB_P ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //SBS
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING  ,E_TDTV_DECODER_INPUT_SBS_P,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D          ,E_TDTV_DECODER_INPUT_SBS_P,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
        //REAL_D
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BY_TIMMING  ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D          ,E_TDTV_DECODER_INPUT_SBS_P ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        //Sensio
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_I ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_INTERLEAVE  ,E_TDTV_DI_BYPASS      ,E_TDTV_DECODER_INPUT_SBS_Q_I ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_PROGRESSIVE ,E_TDTV_DI_2D          ,E_TDTV_DECODER_INPUT_SBS_Q_P ,E_TDTV_DECODER_OUTPUT_3D_FS,E_TDTV_MJC_3D),
        //Quin Cunx
        0,//E_TDTV_DECODER_INPUT_2D_Q_I    =18,//Quin Cunx ,Current No use
        0,//E_TDTV_DECODER_INPUT_2D_Q_P    =19,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_SBS_Q_I   =20,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_SBS_Q_P   =21,//Quin Cunx
        0,//E_TDTV_DECODER_INPUT_FS_Q_I    =22,//Quin Cunx , Current No use
        0,//E_TDTV_DECODER_INPUT_FS_Q_P    =23,//Quin Cunx            
        //Line interleave            
        0,//E_TDTV_DECODER_INPUT_LI_I    =24,//Line Interleave
        TTD_PROC_PACK(E_TDTV_DECODER_INPUT_2D_P ,E_TDTV_DECODER_OUTPUT_NATIVE ,E_TDTV_NR_SNR_TNR_BYPASS ,E_TDTV_DI_2D      ,E_TDTV_DECODER_INPUT_LI_P   ,E_TDTV_DECODER_OUTPUT_3D_FS ,E_TDTV_MJC_3D),
    },    
#endif
};    
