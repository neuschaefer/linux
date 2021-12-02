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
 * $RCSfile: drv_async.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_TDTV_DRVIF_H
#define _DRV_TDTV_DRVIF_H

//=============================================================================
// Mode Change Callback Function Register
//=============================================================================
typedef void (*vTDTVModeChangeCB)(void *pvArgv);

typedef enum {
    E_TDTV_CB_MM        =0,
    E_TDTV_CB_HDMI      =1,
    E_TDTV_CB_OSD       =2,
    E_TDTV_CB_OSTG      =3,
    E_TDTV_CB_DQC       =4,
    E_TDTV_CB_TTD       =5,
    E_TDTV_CB_3D22D     =6,
    E_TDTV_NUM_OF_CB
}E_TDTV_CB_MODULE;


/**
 * @brief : Register 3DTV Mode Change Callback
 * @param eModule: The Callback module
 * @param fpTDTVModeChangeCB: The Callback Function
 * @retval: Return Previous Callback Function; 
 */
vTDTVModeChangeCB fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_MODULE eModule,vTDTVModeChangeCB fpTDTVModeChangeCB);

//=============================================================================
// TDTV Handling Mode Query for HDMI
//=============================================================================

//Decoder Tag for Decide TDTV System Config
#define LR_INVERSE_BIT 0x80

#define TDTV_DEC_IN(x) ((E_TDTV_DECODER_INPUT)(((UINT32)x)&0x7F))
#define TDTV_DEC_IN_INV(x) ((E_TDTV_UI_LR)((((UINT32)x)>>7)&0x1))

typedef enum { // Instead of AUTO Mode , LSB = 0 Means Interlace Frame

    E_TDTV_DECODER_INPUT_AUTO     =0,
    E_TDTV_DECODER_INPUT_RESERVE  =1,    
    E_TDTV_DECODER_INPUT_2D_I     =2,//2D
    E_TDTV_DECODER_INPUT_2D_P     =3,
    E_TDTV_DECODER_INPUT_TTD_I    =4,//2D to 3D
    E_TDTV_DECODER_INPUT_TTD_P    =5,
    E_TDTV_DECODER_INPUT_FS_I     =6,//Frame Seq
    E_TDTV_DECODER_INPUT_FS_P     =7,
    E_TDTV_DECODER_INPUT_FP_I     =8,//Frame Packing
    E_TDTV_DECODER_INPUT_FP_P     =9,
    E_TDTV_DECODER_INPUT_TB_I     =10,//Top and Bottom
    E_TDTV_DECODER_INPUT_TB_P     =11,
    E_TDTV_DECODER_INPUT_SBS_I    =12,//Side by Side
    E_TDTV_DECODER_INPUT_SBS_P    =13,
    E_TDTV_DECODER_INPUT_RD_I     =14,//RealD
    E_TDTV_DECODER_INPUT_RD_P     =15,
    E_TDTV_DECODER_INPUT_SS_I     =16,//Sensio
    E_TDTV_DECODER_INPUT_SS_P     =17, 
    E_TDTV_DECODER_INPUT_LI_I     =18,//Line Interleave
    E_TDTV_DECODER_INPUT_LI_P     =19,
    E_TDTV_DECODER_INPUT_CB_I     =20,//Checker Board
    E_TDTV_DECODER_INPUT_CB_P     =21,
    E_TDTV_DECODER_INPUT_DA_I     =22,//Dot Alternative
    E_TDTV_DECODER_INPUT_DA_P     =23,    
    E_TDTV_DECODER_INPUT_END,
    //Above Enum Should be fit into 8 Bit

    //Not Valid Input type Select,  Used for B2R to Send L/R Inverse
    E_TDTV_DECODER_INPUT_TB_INV_I  = E_TDTV_DECODER_INPUT_TB_I  | LR_INVERSE_BIT,
    E_TDTV_DECODER_INPUT_TB_INV_P  = E_TDTV_DECODER_INPUT_TB_P  | LR_INVERSE_BIT,
    E_TDTV_DECODER_INPUT_SBS_INV_I = E_TDTV_DECODER_INPUT_SBS_I | LR_INVERSE_BIT,
    E_TDTV_DECODER_INPUT_SBS_INV_P = E_TDTV_DECODER_INPUT_SBS_P | LR_INVERSE_BIT,
    
}E_TDTV_DECODER_INPUT;

#define NON_2D_PACKING_START E_TDTV_DECODER_INPUT_FS_I
#define NUM_OF_PACKING_TYPE E_TDTV_DECODER_INPUT_END

//Decoder output configuation
typedef enum {
    E_TDTV_DECODER_OUTPUT_NATIVE       =0,// Output Packing Like Original Content
    E_TDTV_DECODER_OUTPUT_3D_FS        =1,// Convert Packing to Frame Sequencial
    E_TDTV_DECODER_OUTPUT_3D_2_2D      =2,// Convert Packing to Single View( Drop R View)
    E_TDTV_DECODER_OUTPUT_END
}E_TDTV_DECODER_OUTPUT;

//TDTV Video Engine Input Configuration
typedef enum { // Instead of AUTO Mode , LSB = 0 Means Interlace Frame

    E_TD_IN_AUTO     =  E_TDTV_DECODER_INPUT_AUTO,     
    E_TD_IN_RESERVE  =  E_TDTV_DECODER_INPUT_RESERVE,  
    E_TD_IN_2D_I     =  E_TDTV_DECODER_INPUT_2D_I,     
    E_TD_IN_2D_P     =  E_TDTV_DECODER_INPUT_2D_P,     
    E_TD_IN_TTD_I    =  E_TDTV_DECODER_INPUT_TTD_I,    
    E_TD_IN_TTD_P    =  E_TDTV_DECODER_INPUT_TTD_P,    
    E_TD_IN_FS_I     =  E_TDTV_DECODER_INPUT_FS_I,     
    E_TD_IN_FS_P     =  E_TDTV_DECODER_INPUT_FS_P,     
    E_TD_IN_FP_I     =  E_TDTV_DECODER_INPUT_FP_I,     
    E_TD_IN_FP_P     =  E_TDTV_DECODER_INPUT_FP_P,     
    E_TD_IN_TB_I     =  E_TDTV_DECODER_INPUT_TB_I,     
    E_TD_IN_TB_P     =  E_TDTV_DECODER_INPUT_TB_P,     
    E_TD_IN_SBS_I    =  E_TDTV_DECODER_INPUT_SBS_I,    
    E_TD_IN_SBS_P    =  E_TDTV_DECODER_INPUT_SBS_P,   
    E_TD_IN_RD_I     =  E_TDTV_DECODER_INPUT_RD_I,     
    E_TD_IN_RD_P     =  E_TDTV_DECODER_INPUT_RD_P,     
    E_TD_IN_SS_I     =  E_TDTV_DECODER_INPUT_SS_I,     
    E_TD_IN_SS_P     =  E_TDTV_DECODER_INPUT_SS_P,     
    E_TD_IN_LI_I     =  E_TDTV_DECODER_INPUT_LI_I,     
    E_TD_IN_LI_P     =  E_TDTV_DECODER_INPUT_LI_P,     
    E_TD_IN_CB_I     =  E_TDTV_DECODER_INPUT_CB_I,     
    E_TD_IN_CB_P     =  E_TDTV_DECODER_INPUT_CB_P,     
    E_TD_IN_DA_I     =  E_TDTV_DECODER_INPUT_DA_I,     
    E_TD_IN_DA_P     =  E_TDTV_DECODER_INPUT_DA_P,     
    E_TD_IN_END      =  E_TDTV_DECODER_INPUT_END,
    //Above Enum Should be fit into 8 Bit    
}E_TD_IN;

typedef enum {
    E_TD_OUT_NATIVE,
    E_TD_OUT_3D_FS,    //Frame Seq
    E_TD_OUT_3D_TB,    //Top and Bottom
    E_TD_OUT_3D_SBS,   //Side by Side
    E_TD_OUT_3D_LI,    //Line interleave, For PR
    E_TD_OUT_3D_LN_REP,//Line  Repeat
    E_TD_OUT_3D_FPR,      
    E_TD_OUT_END
}E_TD_OUT;

typedef enum {
    E_TD_RES_NORMAL =0,
    E_TD_RES_HALF   =1,
    E_TD_RES_DOUBLE =2,
    E_TD_RES_END
}E_TDTV_RES_OUTPUT;

//=============================================================================
// Configuration Table for Companion 3D FRC Chip Support
//=============================================================================

typedef enum{
    E_DEC_MOD_MM     =0,
    E_DEC_MOD_HDMI   =1,
    E_DEC_MOD_OTHERS =2,
    NUM_OF_DECOER
}E_DEC_MOD;


#define NUM_OF_BIT_USED(x) (((x)>=(1<<7)) ? 8 : ((x)>=(1<<6)) ? 7 : ((x)>=(1<<5)) ? 6 : \
                            ((x)>=(1<<4)) ? 5 : ((x)>=(1<<3)) ? 4 : ((x)>=(1<<2)) ? 3 : \
                            ((x)>=(1<<1)) ? 2 : 1)

#define TD_MSK(Width)     ((0x1<<((UINT8)(Width)))-1)


#define TTT_IN_W  NUM_OF_BIT_USED(E_TD_IN_END)
#define TTT_OUT_W NUM_OF_BIT_USED(E_TD_OUT_END)
#define TTT_W     (DEC_IN_W + DEC_OUT_W)
                            
#define DEC_IN_W  NUM_OF_BIT_USED(E_TDTV_DECODER_INPUT_END)
#define DEC_OUT_W NUM_OF_BIT_USED(E_TDTV_DECODER_OUTPUT_END)
#define DEC_W     (DEC_IN_W + DEC_OUT_W)

#define DQC_IN_W  NUM_OF_BIT_USED(E_TD_IN_END)
#define DQC_OUT_W NUM_OF_BIT_USED(E_TD_OUT_END)
#define DQC_W     (DQC_IN_W + DQC_OUT_W)

#define MIB_IN_W  NUM_OF_BIT_USED(E_TD_IN_END)
#define MIB_OUT_W NUM_OF_BIT_USED(E_TD_OUT_END)
#define MIB_W     (MIB_IN_W + MIB_OUT_W)

#define SCL_IN_W   NUM_OF_BIT_USED(E_TD_IN_END)
#define SCL_OUT_W  NUM_OF_BIT_USED(E_TD_OUT_END)
#define SCL_RES_W  NUM_OF_BIT_USED(E_TD_RES_END)
#define SCL_W       (SCL_IN_W + SCL_OUT_W + SCL_RES_W*4 )

#define TTD_IN_W  NUM_OF_BIT_USED(E_TD_IN_END)
#define TTD_OUT_W NUM_OF_BIT_USED(E_TD_OUT_END)
#define TTD_W     (TTD_IN_W + TTD_OUT_W)

#define MJC_IN_W  NUM_OF_BIT_USED(E_TD_IN_END)
#define MJC_OUT_W NUM_OF_BIT_USED(E_TD_OUT_END)
#define MJC_W     (MJC_IN_W + MJC_OUT_W)

#define TD_DEC_IN(x)        ((E_TDTV_DECODER_INPUT) (((x))&TD_MSK(DEC_IN_W)))
#define TD_DEC_OUT(x)       ((E_TDTV_DECODER_OUTPUT)(((x)>>DEC_IN_W)&TD_MSK(DEC_OUT_W)))

#define TD_MIB_IN(x)        ((E_TD_IN) (((x))&TD_MSK(MIB_IN_W)))
#define TD_MIB_OUT(x)       ((E_TD_OUT)(((x)>>MIB_IN_W)&TD_MSK(MIB_OUT_W)))

#define TD_DQC_IN(x)        ((E_TD_IN) (((x))&TD_MSK(DQC_IN_W)))
#define TD_DQC_OUT(x)       ((E_TD_OUT)(((x)>>DQC_IN_W)&TD_MSK(DQC_OUT_W)))

#define TD_SCL_IN(x)        ((E_TD_IN) ((((UINT32)(x)))&TD_MSK(SCL_IN_W)))
#define TD_SCL_OUT(x)       ((E_TD_OUT)((((UINT32)(x))>>SCL_IN_W)&TD_MSK(SCL_OUT_W)))
#define TD_SCL_PRE_H(x)     ((E_TDTV_RES_OUTPUT)((((UINT32)(x))>>(SCL_IN_W+SCL_OUT_W+SCL_RES_W*0))&TD_MSK(SCL_RES_W)))
#define TD_SCL_PRE_V(x)     ((E_TDTV_RES_OUTPUT)((((UINT32)(x))>>(SCL_IN_W+SCL_OUT_W+SCL_RES_W*1))&TD_MSK(SCL_RES_W)))
#define TD_SCL_PST_H(x)     ((E_TDTV_RES_OUTPUT)((((UINT32)(x))>>(SCL_IN_W+SCL_OUT_W+SCL_RES_W*2))&TD_MSK(SCL_RES_W)))
#define TD_SCL_PST_V(x)     ((E_TDTV_RES_OUTPUT)((((UINT32)(x))>>(SCL_IN_W+SCL_OUT_W+SCL_RES_W*3))&TD_MSK(SCL_RES_W)))

#define TD_TTD_IN(x)        ((E_TD_IN) ((((UINT32)(x)))&TD_MSK(TTD_IN_W)))
#define TD_TTD_OUT(x)       ((E_TD_OUT)((((UINT32)(x))>>TTD_IN_W)&TD_MSK(TTD_OUT_W)))

#define TD_MJC_IN(x)        ((E_TD_IN) ((((UINT32)(x)))&TD_MSK(MJC_IN_W)))
#define TD_MJC_OUT(x)       ((E_TD_OUT)((((UINT32)(x))>>MJC_IN_W)&TD_MSK(MJC_OUT_W)))

typedef enum
{
    TD_TBL_POS_DEC= 0,                       //Decoder Configuration
    TD_TBL_POS_DQC= (TD_TBL_POS_DEC + DEC_W), //De-Quen Cunx Engine Configuration
    TD_TBL_POS_MIB= (TD_TBL_POS_DQC + DQC_W),  //DI,NR Configuration
    TD_TBL_POS_SCL= (TD_TBL_POS_MIB + MIB_W), //Scaler Configuration
    TD_TBL_POS_TTD= (TD_TBL_POS_SCL + SCL_W), //2D to 3D Engine Configuration
    TD_TBL_POS_MJC= (TD_TBL_POS_TTD + TTD_W), //MJC Configuration
}TD_TBL_POS;

#define TTT(in,out)                     ((UINT32)((in)| ((out)<<TTT_IN_W)))

#define DEC(in,out)                     ((UINT32)((in)| ((out)<<DEC_IN_W)))
#define DQC(in,out)                     ((UINT32)((in)| ((out)<<DQC_IN_W)))
#define MIB(in,out)                     ((UINT32)((in)| ((out)<<MIB_IN_W)))
#define SCL(preh,prev,in,out,psth,pstv) ((UINT32)((in)| ((out)<<SCL_IN_W) \
        | ((preh)<<(SCL_IN_W + SCL_OUT_W)) | ((prev)<<(SCL_IN_W + SCL_OUT_W +SCL_RES_W*1))\
        | ((psth)<<(SCL_IN_W + SCL_OUT_W +SCL_RES_W*2) ) | ((pstv)<<(SCL_IN_W + SCL_OUT_W + SCL_RES_W*3))))
        
#define TTD(in,out)                     ((UINT32)((in)| ((out)<<TTD_IN_W)))
#define MJC(in,out)                     ((UINT32)((in)| ((out)<<MJC_IN_W)))

#define TD_TBL_PACK(dec,dqc,mib,scl,ttd,mjc) \
    ((((UINT64)(dec)) << TD_TBL_POS_DEC) | (((UINT64)(dqc)) << TD_TBL_POS_DQC) | (((UINT64)(mib)) << TD_TBL_POS_MIB)\
    |(((UINT64)(scl)) << TD_TBL_POS_SCL) | (((UINT64)(ttd)) << TD_TBL_POS_TTD) | (((UINT64)(mjc)) << TD_TBL_POS_MJC))


//Macro to MM/HDMI/Scaler Query Interface
#define TDTV_UINT32_DEC_INPUT(x) ((E_TDTV_DECODER_INPUT)((x)&0xFFFF))
#define TDTV_UINT32_DEC_OUTPUT(x) ((E_TDTV_DECODER_OUTPUT)(((x)>>16)&0xFFFF))


//=============================================================================
// TDTV Handling Mode Query for HDMI
//=============================================================================

/**
 * @brief :3D Processing Mode Query
 * @param eDetectedMode: Set The Decoder detected Mode According to HDMI Flag
 * @retval:  (E_TDTV_HDMI_OUTPUT << 16) || E_TDTV_HDMI_INPUT
 
     E_TDTV_DECODER_INPUT_AUTO     =0,//Not Support
     E_TDTV_DECODER_INPUT_RESERVE  =1,//Not Support
     E_TDTV_DECODER_INPUT_2D_I     =2,//Support bypass native Packing
     E_TDTV_DECODER_INPUT_2D_P     =3,//Support bypass native Packing
     E_TDTV_DECODER_INPUT_TTD_I    =4,//Not Support
     E_TDTV_DECODER_INPUT_TTD_P    =5,//Not Support
     E_TDTV_DECODER_INPUT_FS_I     =6,//Not Support
     E_TDTV_DECODER_INPUT_FS_P     =7,//Not Support
     E_TDTV_DECODER_INPUT_FP_I     =8,
     E_TDTV_DECODER_INPUT_FP_P     =9,
     E_TDTV_DECODER_INPUT_TB_I     =10,//Only Support Convert 3D to 2D
     E_TDTV_DECODER_INPUT_TB_P     =11,//Support Bypass Packing and 3D to 2D
     E_TDTV_DECODER_INPUT_SBS_I    =12,//Support Bypass Packing and 3D to 2D
     E_TDTV_DECODER_INPUT_SBS_P    =13,//Support Bypass Packing and 3D to 2D
     E_TDTV_DECODER_INPUT_RD_I     =14,//Support Bypass Packing and 3D to 2D
     E_TDTV_DECODER_INPUT_RD_P     =15,//Support Bypass Packing and 3D to 2D
     E_TDTV_DECODER_INPUT_SS_I     =16,//Support Bypass Packing and 3D to 2D
     E_TDTV_DECODER_INPUT_SS_P     =17,//Support Bypass Packing and 3D to 2D 

 
     E_TDTV_DECODER_OUTPUT_NATIVE       =0,// Output Packing Like Original Content
     E_TDTV_DECODER_OUTPUT_3D_FS        =1,// Convert Packing to Frame Sequencial
     E_TDTV_DECODER_OUTPUT_3D_2_2D      =2 // Convert Packing to Single View( Drop R View)
 */

UINT32 u4DrvTDTVHDMIModeQuery(E_TDTV_DECODER_INPUT eDetectedMode);

//=============================================================================
// TDTV Handling Mode Query for MM
//=============================================================================

/**
 * @brief :3D Processing Mode Query
 * @param eDetectedMode: Set the Decoder Detected mode
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT


     E_TDTV_DECODER_INPUT_AUTO     =0,//Not Support
     E_TDTV_DECODER_INPUT_RESERVE  =1,//Not Support
     E_TDTV_DECODER_INPUT_2D_I     =2 //Support Bypass Native Packing
     E_TDTV_DECODER_INPUT_2D_P     =3 //Support Bypass Native Packing
     E_TDTV_DECODER_INPUT_TTD_I    =4,//Not Support
     E_TDTV_DECODER_INPUT_TTD_P    =5,//Not Support
     E_TDTV_DECODER_INPUT_FS_I     =6,
     E_TDTV_DECODER_INPUT_FS_P     =7,
     E_TDTV_DECODER_INPUT_FP_I     =8,//Not Support
     E_TDTV_DECODER_INPUT_FP_P     =9,//Not support
     E_TDTV_DECODER_INPUT_TB_I     =10,
     E_TDTV_DECODER_INPUT_TB_P     =11,
     E_TDTV_DECODER_INPUT_SBS_I    =12,
     E_TDTV_DECODER_INPUT_SBS_P    =13,
     E_TDTV_DECODER_INPUT_RD_I     =14,
     E_TDTV_DECODER_INPUT_RD_P     =15,
     E_TDTV_DECODER_INPUT_SS_I     =16,
     E_TDTV_DECODER_INPUT_SS_P     =17, 
 
 
     E_TDTV_DECODER_OUTPUT_NATIVE       =0,// Output Packing Like Original Content
     E_TDTV_DECODER_OUTPUT_3D_FS        =1,// Convert Packing to Frame Sequencial
     E_TDTV_DECODER_OUTPUT_3D_2_2D      =2 // Convert Packing to Single View( Drop R View)
     
 */
UINT32 u4DrvTDTVMMModeQuery(E_TDTV_DECODER_INPUT eDetectedMode);

//=============================================================================
// TDTV Handling Mode Query for DI
//=============================================================================

UINT32 u4DrvTDTVDIModeQuery(void);

//=============================================================================
// TDTV Handling Mode Query for Scaler
//=============================================================================

/**
 * @brief :3D Processing Mode Query
 * @param eDetectedMode: For HDMI Input Source, Scaler Detect RealD Tag, 
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT

     E_TDTV_DECODER_INPUT_AUTO     =0,//Not Support
     E_TDTV_DECODER_INPUT_RESERVE  =1,//Not Support
     E_TDTV_DECODER_INPUT_2D_I     =2,//Not Support
     E_TDTV_DECODER_INPUT_2D_P     =3, //Support Bypass Native Packing
     E_TDTV_DECODER_INPUT_TTD_I    =4,//Not Support
     E_TDTV_DECODER_INPUT_TTD_P    =5,//Not Support
     E_TDTV_DECODER_INPUT_FS_I     =6,//Not Support
     E_TDTV_DECODER_INPUT_FS_P     =7,//Not Support
     E_TDTV_DECODER_INPUT_FP_I     =8,//Not Support
     E_TDTV_DECODER_INPUT_FP_P     =9 ,//Not Support
     E_TDTV_DECODER_INPUT_TB_I     =10,//Not Support
     E_TDTV_DECODER_INPUT_TB_P     =11,//Support Convert to FS
     E_TDTV_DECODER_INPUT_SBS_I    =12 ,//Not Support
     E_TDTV_DECODER_INPUT_SBS_P    =13,//Support Convert to FS
     E_TDTV_DECODER_INPUT_RD_I     =14 ,//Not Support
     E_TDTV_DECODER_INPUT_RD_P     =15 ,//Support Convert to FS
     E_TDTV_DECODER_INPUT_SS_I     =16 ,//Not Support
     E_TDTV_DECODER_INPUT_SS_P     =17 ,//Support Convert to FS

     typedef enum {
         E_TDTV_SCAlER_OUTPUT_NATIVE       =0,//Output Packing Like Original Content
         E_TDTV_SCAlER_OUTPUT_3D_FS        =1,//Convert Packing to Frame Sequencial
         E_TDTV_SCAlER_OUTPUT_3D_2_2D      =2 //Not Support
     }E_TDTV_DECODER_OUTPUT;

 
 */
UINT32 u4DrvTDTVScalerModeQuery(void);


/**
 * @brief :3D Processing Mode Query Extension for Scaler 
 * @param u4Path: Video Path Src
 * @param eDetectedMode: For HDMI Input Source, Scaler Detect RealD Tag, 
 * @retval:  (E_TDTV_MM_OUTPUT << 16) || E_TDTV_MM_INPUT
 */
UINT32 u4DrvTDTVScalerModeQueryEx(UINT32 u4Path);

//=============================================================================
// TDTV Handling Mode Query for MJC
//=============================================================================

typedef enum {
    E_TDTV_MJC_LR_NORMAL  =0,
    E_TDTV_MJC_LR_INVERSE =1
}E_TDTV_MJC_LR;

// TODO:MJC Sync to Chiehhan
typedef enum {
    E_TDTV_MJC_2D  =0,// Normal MJC Function
    E_TDTV_MJC_3D  =1,// Pack into line Interleave or Pair Repeat According to the Panel
    E_TDTV_MJC_TTD =2,
    E_TDTV_MJC_BYPASS
}E_TDTV_MJC;

/*
* @brief :3D Processing Mode Query for MJC
* @retval:  (E_TDTV_MJC_LR << 16) || E_TDTV_MJC
*/
UINT32 u4DrvTDTVMJCModeQuery(void);

//=============================================================================
// TDTV Handling Mode Query for 2D to 3D
//=============================================================================


/*
* @brief : TTD Processing Mode Query
* @retval: E_TDTV_MJC_2D
*/
UINT32 u4DrvTDTVTTDModeQuery(void);


//=============================================================================
// TDTV Handling Mode Query for DQC
//=============================================================================

/*
* @brief : De-QuenCunx Processing Mode Query
* @retval: E_TDTV_MJC_2D
*/
UINT32 u4DrvTDTVDQCModeQuery(void);

//=============================================================================
// TDTV Handling Mode Query for OSD
//=============================================================================

typedef enum {
    E_TDTV_OSD_2D  =0,
    E_TDTV_OSD_FS  =1,
    E_TDTV_OSD_SBS =2,
    E_TDTV_OSD_TB  =3
}E_TDTV_OSD;

typedef struct{
    E_TDTV_OSD eOSDType;
    UINT32 u4PanelWidth;
    UINT32 u4PanelHeight;
}S_TDTV_OSD_CONFIG;

/*
* @brief :Mode Query for OSD
* @retval:  S_TDTV_OSD_CONFIG
*/
S_TDTV_OSD_CONFIG eDrvTDTVOSDModeQuery(void);

//=============================================================================
// TDTV Handling Mode Query for OSTG
//=============================================================================

typedef enum {
    E_TDTV_OSTG_CFG_BY_PANEL_TBL  =0, // Use Panel Config by Default Panel Tbl
    E_TDTV_OSTG_CFG_BY_TDTV       =1
}E_TDTV_OSTG;

typedef struct{
    E_TDTV_OSTG eOSTGCfg;
    UINT32 u4Clk;    
    UINT32 u4HTotal;
    UINT32 u4VTotal;
    UINT32 u4FrameRate;
    UINT32 u4Width;
    UINT32 u4Height;
}S_TDTV_OSTG_CONFIG;

/*
* @brief :Mode Query for OSTG
* @retval:  S_TDTV_OSD_CONFIG
*/
S_TDTV_OSTG_CONFIG eDrvTDTVOSTGModeQuery(void);


//=============================================================================
// TDTV Dram Mode Query for SRM Display Mode Decision
//=============================================================================

/**
 * @brief : Query Function for SRM check force Scaler Dram Mode
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */
typedef enum {
    E_TDTV_DRAM_NORMAL =0,
    E_TDTV_DRAM_3FB    =1,
    E_TDTV_DRAM_6FB    =2
}E_TDTV_DRAM;

UINT32 u4DrvTDTVForceDramModeQuery(void);

/**
 * @brief : Query Function for SRM check force Scaler Dram Mode Extension
 * @Param u4Path: SV_VP_MAIN/ SV_VP_PIP
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */
UINT32 u4DrvTDTVForceDramModeQueryEx(UINT32 u4Path);


/**
 * @brief : Query Function for SRM check force PSCAN Dram Mode
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */
typedef enum {
    E_TDTV_DI_DRAM_NORMAL =0,
    E_TDTV_DI_DRAM_0FB    =1,
    E_TDTV_DI_DRAM_DOUBLE =2
}E_TDTV_DI_DRAM;

UINT32 u4DrvTDTVForceDIDramModeQuery(void);

/**
 * @brief : Query Function for SRM check force PSCAN Dram Mode
 * @Param u4Path: SV_VP_MAIN/ SV_VP_PIP
 * @retval: SV_TRUE => Force Scaler Dram Mode
 */

UINT32 u4DrvTDTVForceDIDramModeQueryEx(UINT32 u4Path);

/**
 * @brief : Query Function for Scanning Backlight On /Off
 * @retval: SV_TRUE => Enable Scanning Backlight
 */
UINT32 u4DrvTDTVIsScanningBLEnabled(void);

//=============================================================================
// TDTV  UI Interface 
//=============================================================================

typedef enum{
    E_TDTV_UI_CTRL_3D_MODE      =0,
    E_TDTV_UI_CTRL_LR_INVERSE   =1,
    E_TDTV_UI_CTRL_DEPTH_CTRL   =2,
    E_TDTV_UI_CTRL_PROTRUDE     =3,
    E_TDTV_UI_CTRL_DISTANCE_TV  =4,
    E_TDTV_UI_CTRL_3D_TO_2D     =5,
    E_TDTV_UI_CTRL_OSD_DEPTH    =6,
    E_TDTV_UI_CTRL_FPR          =7,
    E_TDTV_UI_CTRL_TDNAVI       =8,
    E_TDTV_UI_CTRL_IMG_SAFETY   =9,
}E_TDTV_UI_CTRL;


typedef enum{
    E_TDTV_UI_HIDE      =0,
    E_TDTV_UI_GREYOUT   =1,
    E_TDTV_UI_RSV       =2,//Reserve
    E_TDTV_UI_ENABLE    =3,
}E_TDTV_UI_CAP;


typedef enum {
    E_TDTV_UI_3D_MODE_OFF  = 0,
    E_TDTV_UI_3D_MODE_AUTO = 1,
    E_TDTV_UI_3D_MODE_TTD  = 2,//2D -> 3D Conversion
    E_TDTV_UI_3D_MODE_FS   = 3,//Frame Seq
    E_TDTV_UI_3D_MODE_SBS  = 4,//Side by Side    
    E_TDTV_UI_3D_MODE_TB   = 5,//Top Bottom    
    E_TDTV_UI_3D_MODE_RD   = 6,//Reald
    E_TDTV_UI_3D_MODE_SS   = 7,//Sensio    
    E_TDTV_UI_3D_MODE_LI   = 8,//Line Interleave
    E_TDTV_UI_3D_MODE_DA   = 9,//Dot Alternative
    E_TDTV_UI_3D_MODE_CB   =10,//Checker Board    
    E_TDTV_UI_3D_MODE_END    
}E_TDTV_UI_3D_PACKING;    

E_TDTV_UI_3D_PACKING u4DrvTDTV3DModeQuery(void);

UINT32 u4DrvTDTV3DFMTCapQuery(UINT32 bPath);
UINT32 u4DrvTDTV3DCtrlCapQuery(UINT32 bPath);
UINT8  u1DrvTDTVMJCEnableQuery(void);


/**
 * @brief :Interface for UI to force 3D mode
 * @param eTDTVUI3DMode
 */

void vDrvTDTVPackingDbg(E_TDTV_UI_3D_PACKING eTDTVUI3DMode);
void vDrvTDTVPacking(E_TDTV_UI_3D_PACKING eTDTVUI3DMode);


/**
 * @brief :Interface for UI to inverse L R View
 * @param eTDTVLRInverse:
 */
 typedef enum {
    E_TDTV_UI_NAVI_MANAUL    =0,
    E_TDTV_UI_NAVI_SEMI_AUTO =1,
    E_TDTV_UI_NAVI_FULL_AUTO =2,   
}E_TDTV_UI_NAVI;  

void vDrvTDTVTDNAVI(E_TDTV_UI_NAVI eTDTVNavi);  

/**
 * @brief : Query Function for TDNavi Mode
 * @retval: E_TDTV_UI_NAVI
 */

E_TDTV_UI_NAVI eDrvTDTVTDNavimModeQuery(void);



/**
 * @brief :Interface for UI to Set Image Safety 
 * @param eTDTVLRInverse:
 */
void vDrvTDTVTImgSafety(UINT8 u1Level);

/**
 * @brief : Query Function for Img Safety
 * @retval: E_TDTV_UI_NAVI
 */

UINT8 eDrvTDTVTDImgSafetyLvlQuery(void);

/**
 * @brief :Interface for UI to inverse L R View
 * @param eTDTVLRInverse:
 */
 typedef enum {
    E_TDTV_UI_LR_NORMAL    =0,
    E_TDTV_UI_LR_INVERSE   =1
}E_TDTV_UI_LR;  

void vDrvTDTVLRInverse(E_TDTV_UI_LR eTDTVLRInverse);
/**
 * @brief :Interface for UI to decide Depth difference of 3D Content
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVDepth(UINT32 u4DepthField);


/**
 * @brief :Interface for UI to decide Protrude Sceen for 2D to 3D
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVProtrude(UINT32 u4Protrude);


/**
 * @brief :Interface for UI to decide Distance beteen TV and Viewer
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVDistanceFromTV(UINT32 u4Distant);

/**
 * @brief :Interface for UI to decide OSD depth
 * @param u4DepthField: 0~32 , Larger mean differ more
 */
void vDrvTDTVOSDDepth(UINT32 u4OSDDepth);



typedef enum {
    E_TDTV_UI_3D_2_2D_OFF  =0,
    E_TDTV_UI_3D_2_2D_L    =1,
    E_TDTV_UI_3D_2_2D_R    =2,
}E_TDTV_UI_3D_2_2D;    

typedef enum {
    E_TDTV_UI_3D_FPR_OFF  =0,
    E_TDTV_UI_3D_FPR_V1   =1,
    E_TDTV_UI_3D_FPR_V2   =2,
}E_TDTV_UI_3D_FPR;   

/**
 * @brief :Interface for UI to force convert 3D Video into 2D
 * @param e3D22DMode:
 */
 
void vDrvTDTV3D22Ddbg(E_TDTV_UI_3D_2_2D e3D22DMode);
void vDrvTDTV3D22D(E_TDTV_UI_3D_2_2D e3D22DMode);

/**
 * @brief :Interface for UI to Chose FPR Mode
 * @param e3D22DMode:E_TDTV_UI_3D_FPR_OFF,E_TDTV_UI_3D_FPR_V1,E_TDTV_UI_3D_FPR_V2
 */
void vDrvTDTV3DFPRdbg(E_TDTV_UI_3D_FPR e3DFPRMode);
void vDrvTDTV3DFPR(E_TDTV_UI_3D_FPR e3DFPRMode);

void vDrvTDTVQueryConfig(void);


typedef enum
{
    TDTV_TAG3D_2D = 0,
    TDTV_TAG3D_MVC,          //MVC = Multi-View Codec 
    TDTV_TAG3D_FP,           //FP = Frame Packing
    TDTV_TAG3D_FS,           //FS = Frame Sequential 
    TDTV_TAG3D_TB,           //TB = Top-and-Bottom 
    TDTV_TAG3D_SBS,          //SBS = Side-by-Side
    TDTV_TAG3D_REALD,        //RealD
    TDTV_TAG3D_SENSIO,       //Sensio
    TDTV_TAG3D_LI,           //LI = Line Interleave 
    TDTV_TAG3D_CB,           //CB = Checker Board
    TDTV_TAG3D_DA,           //DA = Dot Alternative
    TDTV_TAG3D_TTDO,         //TTD only
    TDTV_TAG3D_NOT_SUPPORT
}   TDTV_TAG3D_TYPE_T;

/**
 * @brief :Interface for MW to know the Decoder Detected 3D Tag
 * @param bPath: SV_VP_MAIN / SV_VP_PIP
 */
TDTV_TAG3D_TYPE_T u4DrvTDTVGetTag3D(UINT8 bPath);
TDTV_TAG3D_TYPE_T u4DrvTDTVGetDecTag3D(E_DEC_MOD eDec);
TDTV_TAG3D_TYPE_T u4DrvTDTVGetNaviTag(void);

/**
 * @brief :Main loop of TDTV Process
 */
void vDrvTDTVProc(void);

void vDrvTDTVInit(UINT8 forceinit);

UINT32 u4DrvTDTVModelWithoutMJC(void);

//=============================================================================
//Obsolete Interface , Maybe Changed in the Future
//=============================================================================

typedef enum
{
    TV3D_TYPE_FRAME_PACKING,
    TV3D_TYPE_FRAME_SEQUENTIAL,
    TV3D_TYPE_FIELD_ALTERNATIVE,
    TV3D_TYPE_SIDE_BY_SIDE_FULL,
    TV3D_TYPE_TOP_N_BOTTOM,
    TV3D_TYPE_SIDE_BY_SIDE_HALF,
    TV3D_TYPE_LINE_INTERLEAVE,
    TV3D_TYPE_NS,
} TV3D_TYPE_T;

typedef struct 
{
    UINT8 u13DEnable;
    UINT8 u1Force2D;
    UINT8 u1DisplayLR;
    TV3D_TYPE_T r3DType;
} TV3D_STATUS_T;

// TV3D functions
extern void vTV3DGetForce2D(UINT8 bPath, UINT8 *pu1Force2D, UINT8 *pu1LR);

#endif

