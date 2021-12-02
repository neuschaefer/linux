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
 * $RCSfile: drv_di.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////

#include "hw_ckgen.h"
#include "hw_di.h"
#include "hw_nr.h"
#include "hw_dvi.h"
#include "hw_ycproc.h"

#include "sv_const.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "video_timing.h"
#include "vga_table.h"

#include "drv_di.h"
#include "drv_di_int.h"
#include "di_debug.h"
#include "drv_hdtv.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "drv_tdtv_drvif.h"
#include "drv_display.h"

#include "mute_if.h"
#include "source_select.h"

#include "fbm_drvif.h"
#include "srm_drvif.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN


#include "x_ckgen.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_util.h"
#include "x_hal_5381.h"

#ifdef CC_MT5396
#include "drv_mjc.h"
#endif


////////////////////////////////////////////////////////////////////////////////
//  Macro & Define
////////////////////////////////////////////////////////////////////////////////

#define SCL_DISP_MODE_WIDTH_MIN   240
#define RA_ADAPTIVE_MIN_WIDTH     208
#define MIB_DISP_HBLANK_MIN       32

#define DRAM_REFILL_DELAY 4

//VGA Source HTotal maybe adjust by UI, not follow VGA Table
#define MIB_VGA_HTOTAL_BOOST (0x80)


#define DMOD_M  1<<0    //Support Main
#define DMOD_S  1<<1    //Support Sub

////////////////////////////////////////////////////////////////////////////////
//  External Function
////////////////////////////////////////////////////////////////////////////////

static void _vDrvDICfgPQ(UINT8 u1VdpId);

////////////////////////////////////////////////////////////////////////////////
//  Global variables enum
////////////////////////////////////////////////////////////////////////////////
VDP_MDDI_PRM_T _arMDDiPrm[VDP_NS];
static HANDLE_T _hDIPQIsrSema;

UINT8 u1FlipMirrorConfig = DI_NORMAL_MODE;

#ifdef DYNAMIC_RES_VERIFY
UINT8 u1ForceClipEn = 0;
UINT8 u1EnDynRes = 0;
#endif

#ifdef CC_FPGA_DEBUG
#define MDDI_Y_BASE_ADDR_M 0x3000000
#define MDDI_Y_BASE_ADDR_P 0x5000000
EXTERN UINT32 u4DiInWidth;
EXTERN UINT32 u4DiInHeight;
#endif

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
MDDI_REGTBL_T const CODE MDDI_MIF_INIT[] =
{
    { MCVP_KC_17,      0x00000000, 0x00001000}, // DTV00084276: Set Sub Path AUTO_TRICK_ON to be 0
    { MDDI_REGTBL_END, 0x00000000, 0x00000000}
};
 
///////////NR Still Need To Be Update ~~

const rDataMode aDataMode[]=
{
    //Interlace
    DATAMODE_ARA(E_MIB_DMODE_UNKNOWN,0xFF,0xFF),
    //Scaler FRC
    DATAMODE_ARA(E(I,NR_OFF   ,DI_BOB ,FRC_SCL), 0,DMOD_M|DMOD_S),
    DATAMODE_ARA(E(I,NR_OFF   ,DI_Y3D ,FRC_SCL),24,DMOD_S),    
    DATAMODE_ARA(E(I,NR_YC    ,DI_Y3D ,FRC_SCL),24,DMOD_M),
    DATAMODE_ARA(E(I,NR_OFF   ,DI_YC3D,FRC_SCL),32,DMOD_S),    
    DATAMODE_ARA(E(I,NR_YC    ,DI_YC3D,FRC_SCL),32,DMOD_M),
    //MIB FRC
    DATAMODE_ARA(E(I,NR_OFF   ,DI_BOB ,FRC_MIB),16,DMOD_M),
    DATAMODE_ARA(E(I,NR_Y_ONLY,DI_OFF ,FRC_MIB),20,0),
    DATAMODE_ARA(E(I,NR_YC    ,DI_OFF ,FRC_MIB),24,0),
    DATAMODE_ARA(E(I,NR_OFF   ,DI_Y3D ,FRC_MIB),28,DMOD_M),
    DATAMODE_ARA(E(I,NR_OFF   ,DI_YC3D,FRC_MIB),40,DMOD_M),
    DATAMODE_ARA(E(I,NR_YC    ,DI_Y3D ,FRC_MIB),40,DMOD_M),
    DATAMODE_ARA(E(I,NR_YC    ,DI_YC3D,FRC_MIB),48,DMOD_M),
    //Progressive
    DATAMODE_ARA(E(P,NR_OFF   ,DI_OFF ,FRC_SCL), 0,DMOD_M),
    DATAMODE_ARA(E(P,NR_Y_ONLY,DI_OFF ,FRC_SCL),16,0),
    DATAMODE_ARA(E(P,NR_YC    ,DI_OFF ,FRC_SCL),32,DMOD_M),
    DATAMODE_ARA(E(P,NR_OFF   ,DI_OFF ,FRC_MIB),32,DMOD_M),    
    DATAMODE_ARA(E(P,NR_Y_ONLY,DI_OFF ,FRC_MIB),40,0),
    DATAMODE_ARA(E(P,NR_YC    ,DI_OFF ,FRC_MIB),48,DMOD_M),
    DATAMODE_ARA(E_MIB_DMODE_END,0xFF,0),
 };

/**
 * @brief Get NR Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return NR mode
 */
static inline UINT8 _u1DrvDIGetNRDataMode(UINT8 u1VdpId)
{        
    UINT8 bVdpIdOther = (u1VdpId == VDP_1) ? VDP_2 : VDP_1;
    UINT32 u4Mode;

    PATH_CHECK(u1VdpId);
    u4Mode = _arMDDiPrm[u1VdpId].rFbmPool.u4Mode; 

    if (_arMDDiPrm[u1VdpId].u1DebugNRDataMode != E_MIB_NR_UNKNOWN)        
    {
        if (_arMDDiPrm[bVdpIdOther].u1DebugNRDataMode != E_MIB_NR_UNKNOWN)
        {
            _arMDDiPrm[bVdpIdOther].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;
        }
        return _arMDDiPrm[u1VdpId].u1DebugNRDataMode;
    }

    if (u4Mode & FBM_POOL_MODE_MDDI_NR_OFF)
    {
        return E_MIB_NR_OFF;
    }
    else 
    {
        if (bIsScalerInput444(u1VdpId))
        {
            return E_MIB_NR_OFF;
        }
        else
        {
            if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_C)
            {
                return E_MIB_NR_YC;
            }
            else if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_ONLY)
            {
                //Before ECO , NR Y Only is not Avaliable
                //return IS_ECO_IC() ? E_MIB_NR_Y_ONLY : E_MIB_NR_YC;
                return E_MIB_NR_YC; //NR_Y_ONLY is not supported. this modification is to prevent wrong mode assignment
            }
            else
            {
                return E_MIB_NR_UNKNOWN;
            }
        }        
    }
}


/**
 * @brief Get DI Display Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return DI Display mode
 */
static inline UINT8 _u1DrvDIGetFrcMode(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);
    
    if (_arMDDiPrm[u1VdpId].u1DebugFrcMode != E_MIB_FRC_UNKNOWN)
    {       
        return _arMDDiPrm[u1VdpId].u1DebugFrcMode;
    }

    if (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & FBM_POOL_MODE_MDDI_DISP)
    {
        return E_MIB_FRC_MIB;
    }
    else
    {
        return E_MIB_FRC_SCL;
    }
}

static inline UINT8 _u1DrvDIGetDIDataMode(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);
    
    if(bDrvVideoIsSrcInterlace(u1VdpId))
    {
        switch (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & 0x00070000)
        {
            case FBM_POOL_MODE_MDDI_FULL:
                return E_MIB_DI_YC3D;
            case FBM_POOL_MODE_MDDI_BOB:
                return E_MIB_DI_BOB;
            case FBM_POOL_MODE_MDDI_CBOB:
                return E_MIB_DI_Y3D;
            default:
                return E_MIB_DI_OFF;
        }
    }
    else
    {
        return E_MIB_DI_OFF;
    }
}

/**
 * @brief Get DI Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return DI mode
 */
static inline E_MIB_DMODE _u1DrvDIGetMIBDataMode(UINT8 u1VdpId)
{

    //Please Note that Data Mode Query should always use latest Info instead of MIB Batched Status
    UINT8 u1FrcMode = _u1DrvDIGetFrcMode(u1VdpId);
    UINT8 u1NRMode = _u1DrvDIGetNRDataMode(u1VdpId);
    UINT8 u1DIMode = _u1DrvDIGetDIDataMode(u1VdpId);
    UINT8 u1IsIntr = bDrvVideoIsSrcInterlace(u1VdpId);
    UINT32 u4MIBDateMode;        
    UINT32 i;
    
    PATH_CHECK(u1VdpId);

    if ((u1VdpId == VDP_2)&&(!u1IsIntr))
    {
        _arMDDiPrm[u1VdpId].u1DataModeIdx = 0;
        return E_MIB_DMODE_UNKNOWN;
    }
   
    if((!IS_ECO_IC())&& IS_FRAME_SEQ(u1VdpId) && u1IsIntr)
    {
      //Before ECO , Frame Seq Interlace 3D Deinterlace is not support
      u1NRMode = E_MIB_NR_OFF;
      u1DIMode = E_MIB_DI_BOB;
      u1FrcMode = E_MIB_FRC_SCL;
    }
    
    if (_arMDDiPrm[u1VdpId].eDebugDataMode != E_MIB_DMODE_UNKNOWN)
    {
        u4MIBDateMode =  _arMDDiPrm[u1VdpId].eDebugDataMode; 
    }
    else
    {
        u4MIBDateMode = DMODE(u1IsIntr,u1NRMode,u1DIMode,u1FrcMode);
    }

    //Find MID Data Mode Configuration
    for(i=0;aDataMode[i].eMIBMode!= E_MIB_DMODE_END;++i)
    {
        if((aDataMode[i].eMIBMode == u4MIBDateMode) &&
            (((u1VdpId==VDP_1)&&(aDataMode[i].u1Flag&DMOD_M))||((u1VdpId==VDP_2)&&(aDataMode[i].u1Flag&DMOD_S)))
            )
        {           
            _arMDDiPrm[u1VdpId].u1DataModeIdx=i;
            return aDataMode[i].eMIBMode;
        }    
    }

    LOG(6,"[MIB]FBM CONFIGURATION UNREASONABLE\n");
    _arMDDiPrm[u1VdpId].u1DataModeIdx = 0;    
    return E_MIB_DMODE_UNKNOWN;
}

static inline UINT32 _u4DrvDIGetBandwidthUpperBound(UINT8 u1VdpId)
{
    return u1VdpId==VDP_1 ?
        IO32ReadFldAlign(PSCAN_MISC_00, BANDWIDTH_PER_PXL_1) : IO32ReadFldAlign(PSCAN_MISC_00, BANDWIDTH_PER_PXL_2);
}

/**
 * @brief Get DI Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return DI mode
 */
static inline E_MIB_DMODE _u1DrvDIGetMIBBandWidthMode(UINT8 u1VdpId)
{
   // UINT32 u4MIBDateMode = _arMDDiPrm[u1VdpId].eDataMode;        
    UINT32 u4BandWidthUpperBound = _u4DrvDIGetBandwidthUpperBound(u1VdpId);
    UINT8 i;
    UINT8 u1SizeCheck = sizeof(aDataMode) / sizeof(rDataMode);
    
    PATH_CHECK(u1VdpId);

    if ( _arMDDiPrm[u1VdpId].u1DataModeIdx > u1SizeCheck-1 )
    {
        return E_MIB_DMODE_UNKNOWN;
    }
    //Find MID Data Mode Configuration
    for(i=_arMDDiPrm[u1VdpId].u1DataModeIdx; 
        (aDataMode[i].eMIBMode!= E_MIB_DMODE_UNKNOWN)&&((aDataMode[i].eMIBMode&0xF) == FRC_MODE(u1VdpId));
        --i)
    {
        
        if((aDataMode[i].u1Bandwith <= u4BandWidthUpperBound)&&
            (((u1VdpId==VDP_1)&&(aDataMode[i].u1Flag&DMOD_M))||((u1VdpId==VDP_2)&&(aDataMode[i].u1Flag&DMOD_S)))
            )            
        {  
            _arMDDiPrm[u1VdpId].u1BandwidthDataModeIdx=i;
            return aDataMode[i].eMIBMode;
        }    
    }

    if(aDataMode[i].eMIBMode!= E_MIB_DMODE_UNKNOWN)
    {
        LOG(6,"UNABLE TO REACH MIB Bandwidth TARGET Restrict BY FBM !!\n");
    }
    
    _arMDDiPrm[u1VdpId].u1BandwidthDataModeIdx = _arMDDiPrm[u1VdpId].u1DataModeIdx;
    return _arMDDiPrm[u1VdpId].eDataMode;
}

/**
 * @brief Set DI Dram Control
 * @param u1VdpId VDP_1/VDP_2
 * @param u1DramCOnOff Enable/Disable
 */
static void _vDrvDISetDramCtrl(UINT8 u1VdpId, E_MIB_DMODE eDataMode)
{
    UINT32 u4DRAM_RW_EN;
    UINT32 u4BYPASS_EN;  
    UINT32 u4AF_TO_CF;   
    UINT32 u4BF_TO_CF;   
    UINT32 u4AF_FROM_SRC;
    UINT32 u4ABCD_YC_EN; 
    UINT32 u4I2P_EN; 
    UINT32 u4KERNEL_SYNC;
    UINT32 u4OUTBUF_SYNC;   
    UINT32 u4DA_SYNC_MODE;   
    UINT32 u4OUT_TG_MODE;
    UINT32 u4DA_SC_READ_ENABLE;  
    UINT32 u4DA_PROG_MODE;   
    UINT32 u4DA_MODE_YUV;
    UINT32 u4PROG_MODE;    
    static UINT32 u4PrevDMODE[VDP_NS]={0xFFFF,0xFFFF};
    static UINT32 u4PreAF_TO_CF[VDP_NS]={0xFF,0xFF};
    
    PATH_CHECK(u1VdpId);
    
    switch (eDataMode)
    {                          
        //Scaler FRC
        case E(I,NR_OFF   ,DI_BOB ,FRC_SCL):
            u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x00;
            u4DRAM_RW_EN    =E_ALL_RW_OFF; // E_SRC_W = 0x01,  E_SRC_R = 0x02, E_LM_W = 0x04,E_LM_R = 0x08,E_MV_W = 0x10,E_MV_R = 0x20, E_NR_W = 0x40,              
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break; 
        case E(I,NR_OFF   ,DI_Y3D ,FRC_SCL)://This Mode Only Used under Sub
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x74;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;        
        case E(I,NR_YC    ,DI_Y3D ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x74;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;
        case E(I,NR_OFF   ,DI_YC3D,FRC_SCL)://This Mode Only Used under sub
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0xFC;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;          
        case E(I,NR_YC    ,DI_YC3D,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0xFC;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;             
        //MIB FRC
        case E(I,NR_OFF   ,DI_BOB ,FRC_MIB):
            u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_DRAM;
            u4ABCD_YC_EN    =0x03;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_OUTPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;            
            if(_arMDDiPrm[u1VdpId].u1DataModeIdx!= _arMDDiPrm[u1VdpId].u1BandwidthDataModeIdx)
            { //BandWidth Savinf Mode, DRAM Size Enough       
                u4AF_TO_CF      =SV_OFF;                
                u4ABCD_YC_EN    = IS_ECO_IC() ? 0x30 :0x34;
            }            
            break;             
        case E(I,NR_Y_ONLY,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x10;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_ON;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;            
        case E(I,NR_YC    ,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x30;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_ON;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;              
        case E(I,NR_OFF   ,DI_Y3D ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_DRAM;
            u4ABCD_YC_EN    =0x75;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_OUTPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;             
        case E(I,NR_OFF   ,DI_YC3D,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_DRAM;
            u4ABCD_YC_EN    =0xFF;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_OUTPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;              
        case E(I,NR_YC    ,DI_Y3D ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_DRAM;
            u4ABCD_YC_EN    =0x77;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_OUTPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;            
        case E(I,NR_YC    ,DI_YC3D,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_DRAM;
            u4ABCD_YC_EN    =0xFF;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_OUTPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;                            
        //Progressive Scaler FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x0;
            u4DRAM_RW_EN    =E_ALL_RW_OFF;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;              
        case E(P,NR_Y_ONLY,DI_OFF ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x4;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_Y_ONLY;
            break;            
        case E(P,NR_YC    ,DI_OFF ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =IS_FRAME_SEQ(u1VdpId) ? SV_OFF : SV_ON;
            u4ABCD_YC_EN    =IS_FRAME_SEQ(u1VdpId) ? 0x30   : 0xC;                 
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;            
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;             
        //Progressive MIB FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x0;
            u4DRAM_RW_EN    =E_NR_W;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_ON;
            u4DA_PROG_MODE  =SV_ON;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;            
        case E(P,NR_Y_ONLY,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x4;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_ON;
            u4DA_PROG_MODE  =SV_ON;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break;            
        case E(P,NR_YC    ,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x0C;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_ON;
            u4DA_PROG_MODE  =SV_ON;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            break; 
        case E_MIB_DMODE_UNKNOWN:                
        default:
            //Turn Off PSCAN DRAM Engine
            u4BYPASS_EN     =SV_ON;
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4ABCD_YC_EN    =0x0;
            u4DRAM_RW_EN    =0x0;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
    }

    //DRAM Engine 444/422 Mode
    if (bIsScalerInput444(u1VdpId)&&(eDataMode != E_MIB_DMODE_UNKNOWN)&&(IS_MIB_FRC(u1VdpId)))
    {
        u4BYPASS_EN     =SV_OFF;
        u4AF_TO_CF      =SV_OFF;
        u4BF_TO_CF      =SV_OFF;   
        u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
        u4ABCD_YC_EN    =0x0;
        u4DRAM_RW_EN    =E_SRC_W;
        u4I2P_EN        =SV_OFF;                               
        u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
        u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
        u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
        u4OUT_TG_MODE   =E_TG_MIB_DISP;
        u4DA_SC_READ_ENABLE=SV_ON;
        u4DA_PROG_MODE  =SV_ON;    
        u4DA_MODE_YUV = MIB_YUV_MODE_YUV;
    }

    u4PROG_MODE = IS_INTERLACE(u1VdpId)? SV_OFF : SV_ON ;

    u4BYPASS_EN = (
        !((_arMDDiPrm[u1VdpId].u1OnOff)
            &&((IS_420TO422(u1VdpId)&&(VDP_1==u1VdpId))
                || IS_NR_ON(u1VdpId) 
                || IS_MIB_FRC(u1VdpId) 
                || IS_INTERLACE(u1VdpId))))
        || (IS_FRAME_SEQ(u1VdpId) && IS_INTERLACE(u1VdpId) &&(!IS_ECO_IC(void)))
        || (eDataMode ==E_MIB_DMODE_UNKNOWN );

    u4DRAM_RW_EN = (_arMDDiPrm[u1VdpId].u1OnOff) ? u4DRAM_RW_EN : E_ALL_RW_OFF;

    //When Turn OFF A->C , Need to wait for DRAM Refill
    if((SV_ON==u4PreAF_TO_CF[u1VdpId])&&(SV_OFF==u4AF_TO_CF))
    {
        _arMDDiPrm[u1VdpId].u1VSyncCnt=DRAM_REFILL_DELAY;  
    }    

    u4PreAF_TO_CF[u1VdpId] = u4AF_TO_CF;
    
    if((_arMDDiPrm[u1VdpId].u1VSyncCnt==0)||(SV_ON==u4AF_TO_CF))
    {
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17,u4AF_TO_CF        ,AF_TO_CF);
    }

    u4DRAM_RW_EN &= (UINT32)_arMDDiPrm[u1VdpId].u1FreezeWriteFilter;

    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17   ,u4BF_TO_CF         ,BF_TO_CF);
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17   ,u4AF_FROM_SRC      ,AF_FROM_SRC);
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17   ,u4ABCD_YC_EN       ,ABCD_YC_EN);
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4DRAM_RW_EN       ,EN_DRAM_RW);
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_00   ,u4I2P_EN           ,I2P_EN);
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4KERNEL_SYNC      ,KERNEL_SYNC);
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4OUTBUF_SYNC      ,OUTBUF_SYNC);
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_13 ,u4DA_SYNC_MODE     ,DA_SYNC_MODE);
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_1F   ,u4OUT_TG_MODE      ,OUT_TG_MODE);    
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_18 ,u4DA_SC_READ_ENABLE,DA_SC_READ_ENABLE);
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_13 ,u4DA_PROG_MODE     ,DA_PROG_MODE);
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_06 ,u4DA_MODE_YUV      ,DA_MODE_YUV);    
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4PROG_MODE        ,PROG_MODE);

    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_00   ,u4BYPASS_EN        ,BYPASS_EN);    
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_0A   ,u4BYPASS_EN        ,MUTE_EN);

    if(u1VdpId == VDP_1)
    {
        vRegWriteFldAlign(MCVP_GC_00 , u4BYPASS_EN, M_SYNC_FULL_SPEED);
        vRegWriteFldAlign(TOP_MAIN_00, u4BYPASS_EN, C_M2H_INV);
    }
    else
    {
        vRegWriteFldAlign(MCVP_GC_00 , u4BYPASS_EN, P_SYNC_FULL_SPEED);
        vRegWriteFldAlign(TOP_PIP_00 , u4BYPASS_EN, C_P2H_INV);
    }
    
    if((u4PrevDMODE[u1VdpId]!=eDataMode)||_arMDDiPrm[u1VdpId].u1ModeChg)
    {
        u4PrevDMODE[u1VdpId]=eDataMode;
        _vDrvDICfgPQ(u1VdpId);             
    } 
}

/**
 * @brief Thread Pending Until PSCAN Interrupt Finished
 * @param None
 */
void vDrvDIWaitDIVSyncSema(void)
{
    VERIFY (x_sema_lock(_hDIPQIsrSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

/**
 * @brief Set DI interrupt
 * @param u2Vector
 */
static void _vDrvDIISR(UINT16 u2Vector)
{
    ASSERT(u2Vector == VECTOR_PSCAN);

    // Input Vsync Rising
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MIVS_R_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MIVS_R_CLR);//Clear interrupt

        _arMDDiPrm[VDP_1].u2Vcnt = MDDI_READ_FLD(VDP_1, MCVP_KC_2D, OUT_VDE_CNT);
        _arMDDiPrm[VDP_1].u1VSyncCnt = _arMDDiPrm[VDP_1].u1VSyncCnt ? _arMDDiPrm[VDP_1].u1VSyncCnt -1 : 0;
       
#ifdef PSCAN_AUTO_VERIFY
        vDrvDIAutoVerifyInputISR(VDP_1);
#endif        

    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PIVS_R_STA))
    {
        _arMDDiPrm[VDP_2].u1VSyncCnt = _arMDDiPrm[VDP_2].u1VSyncCnt ? _arMDDiPrm[VDP_2].u1VSyncCnt -1 : 0;
#ifdef CC_SUPPORT_VENC
        vDrvDIVEncISR();
#endif
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PIVS_R_CLR);//Clear interrupt
    }

    // Input Vsync Falling
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MIVS_F_STA))
    {
#ifdef PSCAN_AUTO_VERIFY
         vDrvDIISRAutoVerifyChk();
#endif    
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MIVS_F_CLR);//Clear interrupt
    }
    
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PIVS_F_STA))
    {  
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PIVS_F_CLR);//Clear interrupt
    }

    // Input Vsync Adj
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MI_ADJ_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MI_ADJ_CLR);//Clear interrupt
    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PI_ADJ_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PI_ADJ_CLR);//Clear interrupt
    }

    // Output Vsync Rising
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MOVS_R_STA))
    {
        vScpipOnDIOutputISR();
        vDrvDISetQualityISR_Ex();
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MOVS_R_CLR);//Clear interrupt
    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_POVS_R_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_POVS_R_CLR);//Clear interrupt
    }

    // Output Vsync Adj
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MO_ADJ_STA))
    {
        vDrvNRVBlankProc();
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MO_ADJ_CLR);//Clear interrupt
    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PO_ADJ_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PO_ADJ_CLR);//Clear interrupt
    }

    // Output Vsync Falling
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MOVS_F_STA))
    {        
        vDrvDIGetCSStatusISR();
        vDrvDISetQualityISR();
        vDrvNRISR();
		#ifdef CC_MT5396
#if MIB_PORTING_TODO                
		if (fgDrvMJCFilmDetectDIOrMJC() == SV_TRUE)
		{
           vDrvMJCFrmSchFilmDetect();
		}
#endif
		#endif
#if MIB_PORTING_TODO        
        vApiIsrLog(NPTV_ISR_PSCAN); 
#endif

#ifdef PSCAN_AUTO_VERIFY
        vDrvDIAutoVerifyISR(VDP_1);
#endif
        x_sema_unlock(_hDIPQIsrSema);    
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MOVS_F_CLR);//Clear interrupt
    }

    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_POVS_F_STA))
    {
#ifdef PSCAN_AUTO_VERIFY
        vDrvDIAutoVerifyISR(VDP_2);
#endif
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_POVS_F_CLR);//Clear interrupt
    }
    
    VERIFY(BIM_ClearIrq(VECTOR_PSCAN));
}


/**
 * @brief Set DI init interrupt
 */
static void _vDrvDIInitISR(void)
{
    static UINT8 u1Initialized = SV_FALSE;
    x_os_isr_fct pfnOldIsr;

    if (!u1Initialized)
    {
        VERIFY(x_sema_create(&_hDIPQIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        
        if (x_reg_isr(VECTOR_PSCAN, _vDrvDIISR, &pfnOldIsr) != OSR_OK)
        {
            LOG(7, "Error: fail to register PSCAN ISR!\n");
        }
        else
        {
            LOG(7, "Register PSCAN ISR OK!\n");
        }

        u1Initialized = SV_TRUE;
        LOG(7, "PSCAN ISR registered!\n");
    }
}


/**
 * @brief Get DI output clock rate
 * @param u1VdpId VDP_1/VDP_2
 * @return u4ClkRate clock rate
 */

static inline VOID _u4DrvDICfgColorFmt(UINT8 u1VdpId)
{
    UINT32 u4Is444,u4Is422;
    UINT32 u4NR420;
    
    if(bIsScalerInput444(u1VdpId))
    {
        u4Is444=SV_ON;
        u4Is422=SV_OFF;
    }
    else if(IS_420TO422(u1VdpId))
    {
        u4Is444=SV_OFF;
        u4Is422=SV_OFF;
    }
    else
    {    
        u4Is444=SV_OFF;
        u4Is422=SV_ON;
    }

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u4Is444, M444);    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u4Is422, M422);

    u4NR420 = (IS_420TO422(u1VdpId)&&(IS_PROGRESSIVE(u1VdpId))) ? SV_ON : SV_OFF;
	
	if(u1VdpId == VDP_1)
	{
	    vDrvNRSetChromaUpSample(u4NR420);
	}
}

/**
 * @brief Set DI Misc
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDISetMisc(UINT8 u1VdpId)
{
    UINT32 u4FldRegenOnOff;
    UINT32 u4DispHBlank, u4PanelPxlClk;
    
    PATH_CHECK(u1VdpId);

    // Reset SW counter
    _arMDDiPrm[u1VdpId].u1StatusCnt = 0;
    _arMDDiPrm[u1VdpId].u1FreezeCnt = 0;
    _arMDDiPrm[u1VdpId].u1SRCWAutoStopDisableCnt = 0;

    //Fld Info Regen
    u4FldRegenOnOff = ((SV_VD_MPEGHD ==bGetVideoDecType(u1VdpId)) 
        || (TDTV_TAG3D_FS==u4DrvTDTVGetTag3D(u1VdpId))
        || (TDTV_TAG3D_FP==u4DrvTDTVGetTag3D(u1VdpId))
        || (TDTV_TAG3D_MVC==u4DrvTDTVGetTag3D(u1VdpId))    
        )
        ? SV_OFF : SV_ON;
    
    if(SV_VP_MAIN==u1VdpId)
    {
        vRegWriteFldAlign(LPF_PIP_00, u4FldRegenOnOff, MAIN_PROG_FLD_EN);
    }
    else
    {
        vRegWriteFldAlign(LPF_PIP_00, u4FldRegenOnOff, PIP_PROG_FLD_EN);
    }

    //Oversample Mode    
    MDDI_WRITE_FLD(u1VdpId, MCVP_MULTI_00, IS_OVERSAMPLE(u1VdpId), DET_SUB); 

    
    if (IS_ECO_2_IC() && IS_MIB_FRC(u1VdpId) && IS_INTERLACE(u1VdpId) && !IS_SD_HEIGHT(u1VdpId))
    {//// solve prelen issue
        // 252*50*2200*1125 = H_Blank*PxlClk =>   H_Blank = 123750000*252 / PxlClk
        u4PanelPxlClk = (bDrvVideoGetRefreshRate(u1VdpId) == 60) ? PANEL_GetPixelClk60Hz() : PANEL_GetPixelClk50Hz();
        // H_Blank = 123750000*14*18 / PxlClk = 1732500000 / (PxlClk / 18)
        u4DispHBlank = 1732500000 / (u4PanelPxlClk / 18);
        u4DispHBlank = (u4DispHBlank > 255) ? 255 : u4DispHBlank;
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, u4DispHBlank, DISPMODE_H_BLANK); 
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, IS_OVERSAMPLE(u1VdpId)? 0xA0 : 0x8A, DISPMODE_H_BLANK); 
    }

    vDrvDISetStdVMask(u1VdpId, (IS_PROG_DISPMODE(u1VdpId)) ? SV_OFF : SV_ON);

    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, IS_INTERLACE(u1VdpId), DA_SNAKE_MAPPING);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, IS_INTERLACE(u1VdpId), DA_NR_SNAKE_MAPPING);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, IS_INTERLACE(u1VdpId), DA_SC_SNAKE_MAPPING);
   
}


/**
 * @brief Set DI field parameter
 * @param u1VdpId VDP_1/VDP_2
 */
static void inline _vDrvDICfgFlipMirrorParam(UINT8 u1VdpId)
{
    if(u1VdpId ==VDP_1)//Only Main do Flip/Mirror , Sub always done by scaler
    {
        if(IS_FLIP_MIRROR(u1VdpId))
        {
            vDrvDISetFlipMirror(u1VdpId, u1FlipMirrorConfig);
        }
        else
        {
            vDrvDISetFlipMirror(u1VdpId, DI_NORMAL_MODE);
        }
    }
}

/**
 * @brief Set DI Speed Configuration
 * @param u1VdpId VDP_1/VDP_2
 * @param u2HTotal
 * @param u2Width
 */
static inline void _vDrvDICfgDeRatio(UINT8 u1VdpId)
{
    #define DE_BASE_M 25
    #define CHROMA_LINE_AVG_LOWEST_SPEED 2
    #define NON_STD_SPEED_BOOST_IN_BLANK 4
    
    UINT32 u4SigClk,u4OutDERatio,u4DERatio,u4DELmt;

    UINT32 u4OutHT =  MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, HDEW) + MIB_DISP_HBLANK_MIN;
    UINT32 u4PanelVT      = wDrvGetOutputVTotal();
    UINT32 u4PanelVActive = wDrvGetOutputVActive();
    UINT16 u2NSTDRatio;
    
    //Output DE Config
    u4SigClk = u4OutHT * (( _arMDDiPrm[u1VdpId].u2Height * u4PanelVT + u4PanelVActive -1 )/ u4PanelVActive)  * u2DrvGetInternalFrameRate();
    
    u4OutDERatio = (u4SigClk * DE_BASE_M +  (u4DrvDIGetClkRate(u1VdpId)*2) -1)/ (u4DrvDIGetClkRate(u1VdpId)*2);    
    u4OutDERatio = MIN(DE_BASE_M/2,(u4OutDERatio * 11 + 9)/ 10); // Run A Little Faster than scl lower bound

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2E, DE_BASE_M , OUT_TG_DE_M);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2E, u4OutDERatio , OUT_TG_DE_N);
    
    //Kernel DE Config
    u4SigClk  = _arMDDiPrm[u1VdpId].u2VTotal * _arMDDiPrm[u1VdpId].u2HTotal * bDrvVideoGetRefreshRate(u1VdpId);
    u4SigClk  = IS_INTERLACE(u1VdpId) ? (u4SigClk +1)>>1 : u4SigClk;
    
    //59.94 Correction
    if(MODE_525I == bDrvVideoGetTiming(u1VdpId))
    {
        u4SigClk = u4SigClk / 1001 * 1000; //Using *1000/1001 will overflow
    }
    
    u4DERatio = MIN((u4SigClk * DE_BASE_M +  u4DrvDIGetClkRate(u1VdpId) -1)/ u4DrvDIGetClkRate(u1VdpId),DE_BASE_M);
    
    if(_arMDDiPrm[u1VdpId].u1IsNSTDSignal)
    {
        u2NSTDRatio = (IS_PROGRESSIVE(u1VdpId)) ? DE_BASE_M : (DE_BASE_M/2);
        u4DERatio = MIN(u4DERatio+NON_STD_SPEED_BOOST_IN_BLANK, u2NSTDRatio);
    }
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_03, u4DERatio, IPTG_DE_N); 
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_04, DE_BASE_M, IPTG_DE_M); 

    if(IS_MIB_FRC(u1VdpId))
    {
        u4OutDERatio = u4OutDERatio<<1;
        u4DERatio = MAX(u4DERatio,u4OutDERatio);
    }
    
    u4DELmt = u4DERatio ? (DE_BASE_M  / u4DERatio) : 1;    
    u4DELmt = u4DELmt ? (u4DELmt -1) : 0 ;    
    u4DELmt = IS_INTERLACE(u1VdpId) ? MAX(u4DELmt,1) : u4DELmt ;
    
    //u4DELmt = IS_MIB_FRC(u1VdpId) ? MIN(1,u4DELmt) : u4DELmt;

    if((MODE_625I == bDrvVideoGetTiming(u1VdpId))&&(SV_VD_TVD3D == bGetVideoDecType(u1VdpId)))
    {
        u4DELmt = MAX(u4DELmt,CHROMA_LINE_AVG_LOWEST_SPEED);       
    }
   
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, u4DELmt , K_DE_LOW_MIN);
    
}



/**
 * @brief Set H/V clip
 * @param u1VdpId VDP_1/VDP_2
 * @param wSrcXOffset X_offset
 * @param wSrcYOffset Y_offset
 * @param wSrcWidth width
 * @param wSrcHeight height
 */
static inline void _vDrvDISetActiveWindow(UINT8 u1VdpId, UINT16 wSrcXOffset, UINT16 wSrcYOffset, UINT16 wSrcWidth, UINT16 wSrcHeight)
{
    UINT32 u4Height=wDrvVideoInputHeight(u1VdpId);
    UINT32 u4PreDownWidth = u4ScpipGetPreScaleWidth(u1VdpId);

#ifdef DYNAMIC_RES_VERIFY
    if (u1ForceClipEn)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_09, SV_ON, IPTG_CLIP_EN);
    }
#endif

    if (IS_MIB_FRC(u1VdpId) && wSrcXOffset + wSrcWidth > u4PreDownWidth)
    {   // protection;  adjust here if wrong overscan setting found
        wSrcWidth = u4PreDownWidth - wSrcXOffset;
    }
    //-----------------
    //Set Content Width    
    //-----------------

    //NXNR DISP_R Delay will depend on Width
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, wSrcWidth > 960,DISP_R_OUT_P);

    vDrvDISetRegionMotionOnOff(u1VdpId,(wSrcWidth > RA_ADAPTIVE_MIN_WIDTH));
    
    //Not Clip Right side Clipping because of MIB HW Limitation 
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_02, wSrcWidth, IPTG_H_ACT);    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, wSrcWidth, HDEW);   

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1F, MIB_DISP_HBLANK_MIN , OUT_TG_H_BLANK);

    //-----------------
    //Set Content Height    
    //-----------------

    u4Height = IS_INTERLACE(u1VdpId) ? (u4Height+1)>>1 : u4Height ;
    u4Height = (IS_420TO422(u1VdpId)) ? (((u4Height+1)>>1)<<1) : u4Height;
    
    if((VDP_2==u1VdpId)&&IS_PROGRESSIVE(u1VdpId))
    {   //SW WA for Sub Path Writing DRAM For Progressive Src Video Enc,
        u4Height = u4Height <<1;
        wSrcHeight = wSrcHeight <<1;
    }
    
    wSrcHeight = IS_420TO422(u1VdpId)? (((wSrcHeight + 1) >> 1) << 1) : wSrcHeight; //vdew needs to be even when 420 mode 
  
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, wSrcHeight , VDEW);
#ifdef DYNAMIC_RES_VERIFY
    if (u1EnDynRes && IS_INTERLACE(u1VdpId))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, wDrvVideoInputHeight(u1VdpId), VDEW);
    }
#endif

    if (VDP_1==u1VdpId)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0B, (wSrcHeight+(IS_SD_HEIGHT(u1VdpId)?6:18)) , OUT_ADJ_INT_LINE);
    }
}

/**
 * @brief Configure DI Flip-Mirror
 */
static inline void _vDrvDIFlipMirrorInit(void)
{
#ifdef CC_FLIP_MIRROR_SUPPORT    
    UINT8 u1Config = u1GetFlipMirrorConfig();

    if ((u1Config & SYS_MIRROR_CONFIG_ON) 
            && (u1Config & SYS_FLIP_CONFIG_ON))
    {
        u1FlipMirrorConfig = DI_FLIP_MIRROR_ON_MODE;
    }
    else if (u1Config & SYS_MIRROR_CONFIG_ON)
    {
        u1FlipMirrorConfig = DI_MIRROR_ON_MODE;
    }
    else if (u1Config & SYS_FLIP_CONFIG_ON)
    {
        u1FlipMirrorConfig = DI_FLIP_ON_MODE;
    }
    else
    {
        u1FlipMirrorConfig = DI_NORMAL_MODE;
    }
#else
        u1FlipMirrorConfig = DI_NORMAL_MODE;
#endif

}

/**
 * @brief Set DI OnOff
 * @param u1VdpId VDP_1/VDP_2
 */
static UINT8 _u1DrvDIMaskedOnOff(UINT8 u1VdpId)
{
    UINT8 u1IsChannelOn = (u1VdpId == VDP_1) ? _rMChannel.bIsChannelOn : _rPChannel.bIsChannelOn;
    UINT8 u1SigStatus = bDrvVideoSignalStatus(u1VdpId);
    UINT8 u1SigType = bGetSignalType(u1VdpId);
    UINT8 u1IsIntr = bDrvVideoIsSrcInterlace(u1VdpId);

    // 1. Channel is off
    if ((u1IsChannelOn == SV_FALSE)
        // 2. Timing is not supported
        || (u1SigStatus == SV_VDO_NOSUPPORT)
        // 3. Snow screen should be displayed even signal is not stable (TV)
        || ((u1SigType != SV_ST_TV) && ((u1SigStatus == SV_VDO_NOSIGNAL) || (u1SigStatus == SV_VDO_UNKNOWN)))
        || ((u1VdpId == VDP_2)&&(!u1IsIntr))
        // Signal Clk higher than Kernal CLK
        || (!u1DrvDIIsSupportedSignal(u1VdpId))     
        )    
    {
        return SV_OFF;
    }
    else
    {
        return SV_ON;
    }
}

static UINT8 _u1DrvDIGetMIB420Mode(UINT8 u1VdpId)
{    
    return (IS_MPEG(u1VdpId)&&IS_ECO_IC()) || _arMDDiPrm[u1VdpId].u1Force420Mode;
}

static VOID _vDrvDICfgPQ(UINT8 u1VdpId)
{    
    vDrvDISetInterpMode(u1VdpId, _arMDDiPrm[u1VdpId].eDataMode);
    vDrvDISetROIPixel(u1VdpId, MPY_AND_CEIL(_arMDDiPrm[u1VdpId].u2Width, _arMDDiPrm[u1VdpId].u2Height/2, 16));    

    if(SV_VP_MAIN==u1VdpId)
    {
        if(bIsScalerInput444(u1VdpId))
        {
            vDrvNRSet2DNRMode(NXNR_MODE_OFF);
            vDrvNRSet3DNRMode(MCNR_MODE_OFF);                            
        }
        else
        {
#ifdef __MODEL_slt__
            vDrvNRSet2DNRMode(NXNR_MODE_OFF);
            vDrvNRSet3DNRMode(MCNR_MODE_OFF);   
#else
            if(IS_DQ(SV_VP_MAIN)&&(_arMDDiPrm[SV_VP_MAIN].u2SigWidth>960))
            {   //NXNR SRAM is used by DQC , Should be turn off
                vDrvNRSet2DNRMode(NXNR_MODE_OFF);
            }
            else
            {
                vDrvNRSet2DNRMode(NXNR_MODE_ON);
            }    
            
            switch(NR_MODE(u1VdpId))
            {         
                case E_MIB_NR_YC:
                    if(IS_SD_HEIGHT(u1VdpId))
                    {
                        vDrvNRSet3DNRMode(MCNR_MODE_MC);                        
                    }
                    else
                    {
                        vDrvNRSet3DNRMode((IS_PROGRESSIVE(VDP_1)&&_arMDDiPrm[VDP_1].u2Height >= 1080) ? MCNR_MODE_MA : MCNR_MODE_MV);                        
                    }
                    break;
                case E_MIB_NR_OFF:     
                case E_MIB_NR_Y_ONLY:
                case E_MIB_NR_UNKNOWN: 
                default:
                    vDrvNRSet3DNRMode(MCNR_MODE_OFF);            
            }
#endif            
        }
        vDrvNRSetQuality(IS_INTERLACE(VDP_1), _arMDDiPrm[VDP_1].u2Width, _arMDDiPrm[VDP_1].u2Height);
        
    }
    
    vDrvDISetQuality(u1VdpId);
}


/**
 * @brief DI mode change check loop
 * @param u1VdpId VDP_1/VDP_2
 */
static UINT8 _u1DrvDIModeChange(UINT8 u1VdpId)
{
#ifdef CC_FPGA_DEBUG
    if ((_arMDDiPrm[u1VdpId].eDataMode != _arMDDiPrm[u1VdpId].eDebugDataMode))
    {
        _arMDDiPrm[u1VdpId].u1OnOff = SV_TRUE;
        _arMDDiPrm[u1VdpId].u1Interlace = SV_TRUE;
        _arMDDiPrm[u1VdpId].rFbmPool.u4Addr = (u1VdpId == VDP_1) ? MDDI_Y_BASE_ADDR_M : MDDI_Y_BASE_ADDR_P;
        _arMDDiPrm[u1VdpId].u2Height = u4DiInHeight;
        _arMDDiPrm[u1VdpId].u2HTotal = (u4DiInWidth > 720) ? 1716 :  ((u4DiInHeight > 480) ? 864 : 858);
        _arMDDiPrm[u1VdpId].u2VTotal=  (u4DiInHeight > 480) ? 625 : 525;
        _arMDDiPrm[u1VdpId].u1420Mode = SV_FALSE;
        _arMDDiPrm[u1VdpId].u2HClip = 0;
        _arMDDiPrm[u1VdpId].u2VClip = 0;
        _arMDDiPrm[u1VdpId].u1BitRes = 10;   // FPGA hdmi rx limitation
        _arMDDiPrm[u1VdpId].eDataMode = _arMDDiPrm[u1VdpId].eDebugDataMode;

        return TRUE;
    }
    else
    {
        return FALSE;
    }    
#else

    UINT8 u1Ret, u1OnOff, u1DecType, u1Interlace, u1BitRes;
    UINT8 u1FrcMode;
    E_MIB_DMODE eDMode;    
    UINT8 u1FlipMorrorByMIB;
    UINT8 u1420Mode;
    UINT8 u1IsDQ, u1IsPOP;
    UINT8 u1IsNSTDSignal = bDrvVideoIsNonStandardSignal(u1VdpId);
    UINT8 u1STDSigNotChg = 1;
    
    UINT16 u2SigWidth = u4ScpipGetPreScaleWidth(u1VdpId);// Signal Native Width
    UINT16 u2SigHeight = wDrvVideoInputHeight(u1VdpId);// Signal Native Width
    
    UINT16 u2HTotal, u2VTotal, u2Width, u2Height;
    UINT16 wSrcXOffset, wSrcYOffset, u2BufWidth, u2BufHeight;
    UINT32 u4BufAddr;
    UINT32 u4BufSize;
    UINT8 u1FrameRate = bDrvVideoGetRefreshRate(u1VdpId);
    UINT32 u4PanelVT = wDrvGetOutputVTotal();
    
    PSCAN_CLIP_INFO_T ScalerOverscanInfo;
    E_TD_IN eDQCIn= TD_DQC_IN(u4DrvTDTVDQCModeQuery());
    
    PATH_CHECK(u1VdpId);

    u1OnOff = _u1DrvDIMaskedOnOff(u1VdpId);   

    //Get SRM/ FBM Information
    u4BufAddr = _arMDDiPrm[u1VdpId].rFbmPool.u4Addr;
    u4BufSize = _arMDDiPrm[u1VdpId].rFbmPool.u4Size;
    
    u2BufWidth = _arMDDiPrm[u1VdpId].rFbmPool.u4Width;
    u2BufHeight = _arMDDiPrm[u1VdpId].rFbmPool.u4Height;
    
    u1BitRes = (_arMDDiPrm[u1VdpId].rFbmPool.fg10bit) ? 10 : 8;

    u1FrcMode = _u1DrvDIGetFrcMode(u1VdpId);

    if(_arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB == SV_TRUE)
    {
        u1FlipMorrorByMIB = SV_TRUE;
    }
    else
    {
#ifdef CC_FLIP_MIRROR_SUPPORT
        u1FlipMorrorByMIB = u4GetFlipMirrorModule(u1VdpId) == FLIP_BY_PSCAN;
#else
        u1FlipMorrorByMIB = 0;
#endif
    }

    u1420Mode =_u1DrvDIGetMIB420Mode(u1VdpId);
    eDMode    =_u1DrvDIGetMIBDataMode(u1VdpId);    
        
    u1IsPOP = (SRM_IsPopVideo() == 1)? SV_TRUE : SV_FALSE;
    u1DecType = bGetVideoDecType(u1VdpId);

    u1IsDQ =(E_TD_IN_SS_P==eDQCIn)||(E_TD_IN_SS_I==eDQCIn);
    
    u1Interlace = bDrvVideoIsSrcInterlace(u1VdpId);
    u2VTotal = wDrvVideoGetVTotal(u1VdpId);
    u2HTotal = wDrvVideoGetHTotal(u1VdpId);

    if(SV_VD_VGA==bGetVideoDecType(u1VdpId))
    {
        u2HTotal+=MIB_VGA_HTOTAL_BOOST; 
    }

    if (u1ScpipGetClipInfo(u1VdpId, &ScalerOverscanInfo))
    {
        //MIB Width Height Always come from Scaler, Don't query by itself
        u2Width = ScalerOverscanInfo.wWidth;          	
        u2Height = ScalerOverscanInfo.wHeight;
        wSrcXOffset = ScalerOverscanInfo.wX;
        wSrcYOffset = ScalerOverscanInfo.wY;

        if (u1420Mode)
        {
            // The height should be the multiple of 4 under 420 mode.
            u2Height = (u2Height + 3) & (~0x3);
        }
    }
    else
    {
        //If Currently W/H not Avaliable, Keep Previous Value
        wSrcXOffset = _arMDDiPrm[u1VdpId].u2HClip;
        wSrcYOffset = _arMDDiPrm[u1VdpId].u2VClip;
        u2Width = _arMDDiPrm[u1VdpId].u2Width;
        u2Height = _arMDDiPrm[u1VdpId].u2Height;
    }

    u1STDSigNotChg = (!u1IsNSTDSignal && _arMDDiPrm[u1VdpId].u1IsNSTDSignal == u1IsNSTDSignal) ? 1 : 0;
#ifdef DYNAMIC_RES_VERIFY
    u1STDSigNotChg = 0;
#endif   
    // handle non-standard signal at dram mode  
    if ((_arMDDiPrm[u1VdpId].u1OnOff != u1OnOff)
            || (_arMDDiPrm[u1VdpId].u1DecType != u1DecType)
            || (_arMDDiPrm[u1VdpId].u1Interlace != u1Interlace)
            || (_arMDDiPrm[u1VdpId].u2SigWidth != u2SigWidth)
            || (_arMDDiPrm[u1VdpId].u2SigHeight != u2SigHeight)

            || ((_arMDDiPrm[u1VdpId].u2Width != u2Width) && u1STDSigNotChg)
            || ((_arMDDiPrm[u1VdpId].u2Height != u2Height) && u1STDSigNotChg)
            
            || (_arMDDiPrm[u1VdpId].u1420Mode != u1420Mode)
            || (_arMDDiPrm[u1VdpId].u4BufAddr != u4BufAddr)
            || (_arMDDiPrm[u1VdpId].u2BufWidth != u2BufWidth)
            || (_arMDDiPrm[u1VdpId].u4BufSize != u4BufSize)
        
            || (_arMDDiPrm[u1VdpId].u1BitRes != u1BitRes)
            || (_arMDDiPrm[u1VdpId].eDataMode != eDMode)// Not Update When Data Mode Change For Bandwidth Saving
            || (_arMDDiPrm[u1VdpId].u1FrcMode != u1FrcMode)
            || (_arMDDiPrm[u1VdpId].u1IsDQ != u1IsDQ)
            || (_arMDDiPrm[u1VdpId].u1IsPOP != u1IsPOP)
            || (_arMDDiPrm[u1VdpId].u1FlipMorrorByMIB != u1FlipMorrorByMIB)  
            || (_arMDDiPrm[u1VdpId].u1FrameRate != u1FrameRate)
            )
    {       


        _arMDDiPrm[u1VdpId].u1OnOff = u1OnOff;
        _arMDDiPrm[u1VdpId].u1DecType = u1DecType;
        _arMDDiPrm[u1VdpId].u1Interlace = u1Interlace;
        _arMDDiPrm[u1VdpId].u1420Mode = u1420Mode;        
        
        _arMDDiPrm[u1VdpId].u4BufAddr = u4BufAddr;
        _arMDDiPrm[u1VdpId].u4BufSize = u4BufSize;        
        _arMDDiPrm[u1VdpId].u2BufWidth = u2BufWidth;
        _arMDDiPrm[u1VdpId].u2BufHeight = u2BufHeight;
                
        _arMDDiPrm[u1VdpId].u1BitRes = u1BitRes;
        _arMDDiPrm[u1VdpId].eDataMode = (E_MIB_DMODE)eDMode;
        _arMDDiPrm[u1VdpId].u1FrcMode = (E_MIB_FRC_DMODE) u1FrcMode; 
        _arMDDiPrm[u1VdpId].u1IsDQ = u1IsDQ;
        _arMDDiPrm[u1VdpId].u1IsPOP = u1IsPOP;
        _arMDDiPrm[u1VdpId].u1FlipMorrorByMIB = u1FlipMorrorByMIB;

        _arMDDiPrm[u1VdpId].u2SigWidth = u2SigWidth;
        _arMDDiPrm[u1VdpId].u2SigHeight = u2SigHeight;
        _arMDDiPrm[u1VdpId].u1FrameRate = u1FrameRate;

        _arMDDiPrm[u1VdpId].u1ModeChg =SV_TRUE;
        u1Ret = SV_TRUE;

       // The following code is to fix the problem that picture will be abnormal when adjust 
       //  the VGA clock in sub path(1600*1200). Force pre-scaling factor is to avoid mute
       // that will occur when change scaling factor during adjust VGA clock.
        #if defined(CC_MT5389) 
			if(u1VdpId ==VDP_2)
			{
		        if(bGetSignalType(u1VdpId) == SV_ST_VGA && Get_VGAMODE_ICLK(_bVgaTiming) == 1620)
		        {
		            vScpipSetForcedHpsFactor(u1VdpId,0x4000);
		        }
		        else
		        {
	            	vScpipSetForcedHpsFactor(u1VdpId,0x0);
		        }
			}
        #endif
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1ModeChg =SV_FALSE;    
        u1Ret = SV_FALSE;
    }

    //Check Bandwidth Limitation
    _arMDDiPrm[u1VdpId].eBandwidthDataMode  =_u1DrvDIGetMIBBandWidthMode(u1VdpId);
    
    //We don't need to do mode change to reflect offset adjustment
    //We check if offsets are changed here and set registers directly
    if ((_arMDDiPrm[u1VdpId].u2HClip != wSrcXOffset)
            || (_arMDDiPrm[u1VdpId].u2VClip != wSrcYOffset)
            || (_arMDDiPrm[u1VdpId].u2Width != u2Width)
            || (_arMDDiPrm[u1VdpId].u2Height != u2Height)
            || (_arMDDiPrm[u1VdpId].u1IsNSTDSignal != u1IsNSTDSignal)
            
            || (_arMDDiPrm[u1VdpId].u2HTotal != u2HTotal)
            || (_arMDDiPrm[u1VdpId].u2VTotal != u2VTotal)            
            || (_arMDDiPrm[u1VdpId].u4PanelVT != u4PanelVT)
            
            || (SV_TRUE==_arMDDiPrm[u1VdpId].u1ModeChg)
            )
    {
        _arMDDiPrm[u1VdpId].u2HClip = wSrcXOffset;
        _arMDDiPrm[u1VdpId].u2VClip = wSrcYOffset;

        _arMDDiPrm[u1VdpId].u2Width = u2Width;
        _arMDDiPrm[u1VdpId].u2Height= u2Height;

        _arMDDiPrm[u1VdpId].u1IsNSTDSignal = u1IsNSTDSignal;

        _arMDDiPrm[u1VdpId].u2HTotal = u2HTotal;
        _arMDDiPrm[u1VdpId].u2VTotal = u2VTotal;        
        _arMDDiPrm[u1VdpId].u4PanelVT = u4PanelVT;

        if(bGetSignalType(u1VdpId) == SV_ST_MPEG)
        {
            //For  H/V position change after DTV pause. Temporary solution.        
            _arMDDiPrm[u1VdpId].u1SRCWAutoStopDisableCnt= 1; 
            vDrvDISetAutoStop(u1VdpId, SV_OFF);
        }
        
        //Change Input Active Data
        _vDrvDISetActiveWindow(u1VdpId, wSrcXOffset, wSrcYOffset,u2Width,u2Height);

        vScpipSetPscanPrelen(u1VdpId, u2GetMainPrelenValue(u1VdpId)); 

        //Update Output Daa Rate
        _vDrvDICfgDeRatio(u1VdpId);
    }
    return u1Ret;
#endif
}

/**
 * @brief Set DI Parameters
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDISetParam(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);

    vDrvDILoadRegTbl(u1VdpId, MDDI_MIF_INIT);

    vDrvDIFrameRepeat(u1VdpId, SV_OFF);

    _u4DrvDICfgColorFmt(u1VdpId);

    vDrvDISetFrameBuf(u1VdpId, &_arMDDiPrm[u1VdpId].rFbmPool);

    //_vDrvDICfgDeRatio(u1VdpId);

    _vDrvDICfgFlipMirrorParam(u1VdpId);

    _vDrvDISetMisc(u1VdpId);

}

/**
 * @brief Set DI buffer information
 * @param u1VdpId VDP_1/VDP_2
 * @param u4Mode fbm configure mode
 * @param u4Address
 * @param u4Size
 * @param u4Width
 * @param u4Height
 */
static void _vDrvSetDIBufferInfo(UINT8 u1Path, UINT32 u4Mode, 
        UINT32 u4Address, UINT32 u4Size, UINT32 u4Width, UINT32 u4Height)
{
    _arMDDiPrm[u1Path].rFbmPool.u4Addr = u4Address;
    _arMDDiPrm[u1Path].rFbmPool.u4Size = u4Size;
    _arMDDiPrm[u1Path].rFbmPool.u4Width = u4Width;
    _arMDDiPrm[u1Path].rFbmPool.u4Height = u4Height;
    _arMDDiPrm[u1Path].rFbmPool.fg10bit = (u4Mode & FBM_POOL_MODE_10BIT) > 0;
    _arMDDiPrm[u1Path].rFbmPool.fg422 = (u4Mode & FBM_POOL_MODE_422_FB) > 0;
    _arMDDiPrm[u1Path].rFbmPool.u4Mode = u4Mode;

}

/**
 * @brief Init DI Clock
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDIInitClock(UINT8 u1VdpId)
{
    if (u1VdpId == VDP_1)
    {
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, SV_OFF, FLD_MAIN_CLK_STOP);
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, SV_OFF, FLD_PIP_CLK_STOP);
    }

#if defined(CC_MT5389)
    //MT5389 use SYSPLL/2 to prevent ATV Spur
    _vDrvDISetClock(VDP_1, MIB_CLK_TYPESYSPLL_D2_CK);    
#else
    _vDrvDISetClock(VDP_1, MIB_CLK_TYPEENETPLL_D2_CK);    
#endif

    vDrvDISetClockEnable(u1VdpId, SV_ON);
}

/**
 * @brief MIB Main / Sub configuration Loop
 * @param u1VdpId VDP_1/VDP_2
 */
static void inline _vDrvDIChkImpl(UINT8 u1VdpId)
{ 
    PATH_CHECK(u1VdpId);

#ifndef CC_FPGA_DEBUG
    if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_MAIN) && (VDP_1 == u1VdpId))
            || (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_SUB) && (VDP_2 ==u1VdpId)))
#endif
    {
        if (_u1DrvDIModeChange(u1VdpId))
        {                        
            _vDrvVideoSetMute(MUTE_MODULE_MDDI01, u1VdpId, MDDI_MUTE_PERIOD, TRUE);

            if (_arMDDiPrm[u1VdpId].eDebugDataMode != E_MIB_DMODE_UNKNOWN)
            {
#ifdef CC_MT5396
                VDP_SetDispmode(u1VdpId, IS_MIB_FRC(u1VdpId)? 3 : 5 );
#else
                VDP_SetDispmode(u1VdpId, IS_MIB_FRC(u1VdpId)? 3 : 0 );
#endif
            }

            vDrvDISetDoubleBuffer(u1VdpId, SV_OFF);

            _vDrvDISetDramCtrl(u1VdpId, E_MIB_DMODE_UNKNOWN);

            vDrvDISetClkReset(u1VdpId, SV_ON); 

            _vDrvDISetParam(u1VdpId);

            vDrvDISetClkReset(u1VdpId, SV_OFF);   
            
            vDrvDISetDoubleBuffer(u1VdpId, SV_ON);  
                        
        }

        _vDrvDISetDramCtrl(u1VdpId, _arMDDiPrm[u1VdpId].eBandwidthDataMode);        

        if (SV_ON==_arMDDiPrm[u1VdpId].u1OnOff)
        {
            vDrvDIChkStatus(u1VdpId);
        }
    }
}

/**
 * @brief MIB Sram Sharing Mode configuration
 * @param u1VdpId VDP_1/VDP_2
 */
static inline void _vDrvDISramShareCfg(void)
{
    E_SRAM_SHARE_MODE eSramMode;
    UINT32 u4Width =  MDDI_READ_FLD(SV_VP_MAIN, MCVP_DRAM_09, DA_DRAM_LINE_LENGTH) * 16;

#ifndef CC_FPGA_DEBUG
    if(IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_MAIN)&&
            (SV_TRUE==_arMDDiPrm[SV_VP_MAIN].u1ModeChg))
#endif
    {
        //Put Sub in Extra Buffer, But Sub only CBOB Avaliable under this mode
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00,   (IS_POP(SV_VP_MAIN)) ? 0 : 1, EXTRA_PIP_BUF_EN);

        //DRAM FIFO Control
        MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_06, (IS_POP(SV_VP_MAIN)) ? 1 : 0, DA_PIP_POP);
 
        //Config SRAM Sharing Mode for other model
        if((u4Width < 960)|| bDrvVideoGetWidthOverSample(SV_VP_MAIN)) 
        {
            if(IS_INTERLACE(SV_VP_MAIN))
            {
                //SD Interlace
                eSramMode= IS_DQ(SV_VP_MAIN) ? E_SRAM_SHARE_HD_I_HALF_DQ
                 : IS_OVERSAMPLE(SV_VP_MAIN) ? E_SRAM_SHARE_SD_I_OS : E_SRAM_SHARE_SD_I;
            }
            else
            {
                //SD Progressive
                eSramMode= IS_DQ(SV_VP_MAIN) ? E_SRAM_SHARE_HD_P_FULL_DQ
                : IS_OVERSAMPLE(SV_VP_MAIN) ? E_SRAM_SHARE_SD_P_OS : E_SRAM_SHARE_SD_P;
            }
        }
        else 
        {
            if(IS_INTERLACE(SV_VP_MAIN))
            {
                //HD Interlace
                eSramMode= ((IS_POP(SV_VP_MAIN)) ? E_SRAM_SHARE_HD_I_HALF_DQ : E_SRAM_SHARE_HD_I);
            }
            else
            {
                //HD Progressive
                eSramMode= IS_DQ(SV_VP_MAIN) ? E_SRAM_SHARE_HD_P_FULL_DQ : E_SRAM_SAHRE_HD_P;
            }
        }
                
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_19, eSramMode, SRAM_MODE);

        if((!IS_ECO_IC())&&(E_SRAM_SHARE_HD_I==eSramMode))
        {
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_33 , 0x3, KC_RSV);                       
        }
        else
        {
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_33 , 0x0, KC_RSV);                       
        }
    }    
}

/**
 * @brief PSCAN SRM callback function
 * @param u4Arg1 VDP_1/VDP_2
 * @param u4Arg2 configuration
 * @param u4SramMode sram mode
 */
void vDrvDISrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4TDTV)
{
    FBM_POOL_T* prPool;

    if (u4Arg1 == VDP_1)
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);

        _arMDDiPrm[u4Arg1].eTDInput  = TD_MIB_IN(u4TDTV);
        _arMDDiPrm[u4Arg1].eTDOutput = TD_MIB_OUT(u4TDTV);                
    }
    else
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
        _arMDDiPrm[u4Arg1].eTDInput  = E_TD_IN_2D_I;
        _arMDDiPrm[u4Arg1].eTDOutput = E_TD_OUT_NATIVE;        
    }
   
    if (prPool != NULL)
    {
        //Lock the DRAM Write to prevent DRAM Overlap
        if(prPool->u4Addr >  (MDDI_READ_FLD(u4Arg1, MCVP_DRAM_0F, DA_WADDR_LO_LIMIT)<<11))
        {
            MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_0F, prPool->u4Addr>>11, DA_WADDR_LO_LIMIT);
        }

        if(prPool->u4Addr >  (MDDI_READ_FLD(u4Arg1, MCVP_DRAM_04, DA_FLAG_WADDR_LO_LIMIT)<<4))
        {
            MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_04, prPool->u4Addr>>4, DA_FLAG_WADDR_LO_LIMIT);
        }            

        if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(u4Arg1, MCVP_DRAM_0E, DA_WADDR_HI_LIMIT)<<11)
        {
            MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_0E, (prPool->u4Addr + prPool->u4Size)>>11, DA_WADDR_HI_LIMIT);
        }
        
        if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(u4Arg1, MCVP_DRAM_03, DA_FLAG_WADDR_HI_LIMIT)<<4)
        {
            MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_03, (prPool->u4Addr + prPool->u4Size)>>4, DA_FLAG_WADDR_HI_LIMIT);
        }        
    
        _vDrvSetDIBufferInfo((UINT8)u4Arg1, u4Arg2, prPool->u4Addr, prPool->u4Size, prPool->u4Width, prPool->u4Height);
    }
    else
    {
        LOG(1, "vDrvDISrmCb: Fail to get pool\n");
    }
}

/**
 * @brief DI mode change done
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIModeChangeDone(UINT8 u1VdpId)
{

}


/**
 * @brief Initialize PSCAN register values
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIInit(UINT8 u1VdpId)
{
    _vDrvDIInitClock(u1VdpId); 

    vDrvDISetDoubleBuffer(u1VdpId, SV_OFF);

    _vDrvDISetDramCtrl(u1VdpId, E_MIB_DMODE_UNKNOWN);

    vDrvDISetClkReset(u1VdpId, SV_ON);

    _vDrvDIFlipMirrorInit();

    //Initialize PSCAN structure
    _arMDDiPrm[u1VdpId].u1OnOff = SV_OFF;
    _arMDDiPrm[u1VdpId].u2Width = 0xFFFF;
    _arMDDiPrm[u1VdpId].u2Height = 0xFFFF;
    _arMDDiPrm[u1VdpId].u1FreezeCnt = 0;
    _arMDDiPrm[u1VdpId].u1StatusCnt = 0;
    _arMDDiPrm[u1VdpId].eDebugDataMode = E_MIB_DMODE_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugFrcMode = E_MIB_FRC_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB = SV_OFF;    
    _arMDDiPrm[u1VdpId].u2HClip = 0xFFFF;
    _arMDDiPrm[u1VdpId].u2VClip = 0xFFFF;
    _arMDDiPrm[u1VdpId].u1FreezeWriteFilter = 0xFF;
    _arMDDiPrm[u1VdpId].u1SRCWAutoStopDisableCnt= 0;

    vDrvDIInitQuality(u1VdpId);

    vDrvDISetAutoStop(u1VdpId, SV_OFF);

    _vDrvDIInitISR();

    vDrvDIEnableISR(u1VdpId, SV_ON);

    vDrvDISetClkReset(u1VdpId, SV_OFF);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, SV_ON, FLD_OUT_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, SV_ON, AUTO_TNR_OFF_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, SV_ON, AUTO_BOB_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_09, SV_ON, HBLANK_EXT_EN);     
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, 0x8, SRCW_V_BGN);

    vIO32WriteFldAlign(PSCAN_MISC_00, 0xFF, BANDWIDTH_PER_PXL_1); 
    vIO32WriteFldAlign(PSCAN_MISC_00, 0xFF, BANDWIDTH_PER_PXL_2);        
}

/**
 * @brief MIB main loop
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvMIBProc()
{
    //Ctrl main loop
    _vDrvDIChkImpl(VDP_1);
    _vDrvDIChkImpl(VDP_2);
    _vDrvDISramShareCfg();
	
    //Quality main loop
#ifndef __MODEL_slt__
    vDrvDIQtyMainLoop(VDP_1);
    vDrvDIQtyMainLoop(VDP_2);    
#endif
}
