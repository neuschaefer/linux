/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_ttd.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "general.h"
#include "x_assert.h"
#include "hw_vdoin.h"
#include "hw_ttd.h"
#include "hw_sw.h"
#include "hw_scpos.h"
#include "hw_ycproc.h"
#include "nptv_debug.h"
#include "drv_ttd.h"
#include "drv_display.h"
#include "drv_video.h"
#include "drv_tdtv_feature.h"
#include "drv_tdtv_drvif.h"
#include "drv_scaler_drvif.h"
#include "drv_meter.h"
#include "drv_di.h"
#include "panel.h"
#include "fbm_drvif.h"

#ifdef CC_MT5396
#include "drv_mjc.h"
#include "hw_mjc.h"
#endif

/*---------------------------------------------------------------------*
 * definition
 *---------------------------------------------------------------------*/
#define Diff(a, b)  (((a)>(b))?((a)-(b)):((b)-(a)))

// LSV definition
typedef unsigned char PEL;
/*----------------------------------------------------------------------------------------
INCREASE = 1, PRECISION = 16 => UI_Zero = 16, UI_Min = 0, UI_Max = 32
INCREASE = 1, PRECISION = 32 => UI_Zero = 32, UI_Min = 0, UI_Max = 64
INCREASE = 0, PRECISION = 16 => UI_Zero =  0, UI_Min = 0, UI_Max = 16
INCREASE = 0, PRECISION = 32 => UI_Zero =  0, UI_Min = 0, UI_Max = 32

UI_Zero = (INCREASE) ? PRECISION : 0;
UI_Min = 0
UI_Max = (INCREASE) ? (PRECISION*2) : INCREASE
Gain Formula = (x-PRECISION*INCREASE)/PRECISION 
------------------------------------------------------------------------------------------*/
#define SUPPORT_DISPARITY_INCREASE	(1)   // user define : Turnkey = 1, LGE = 0
#define CONVG_GAIN_PRECISION       	(16)   // user define : Turnkey = 16, LGE = 32

#define LSV_DEF_DES_WIDTH       (704)   // user define : must 64 byte align
#define LSV_DEF_DES_HEIGHT      (16)    // user define

// MJC dump definition
#define MJC_DES_WIDTH           (704)   // user define : must 64 byte align
#define MJC_DES_HEIGHT          (16)    // user define
#define MJC_SUBSAMPLE           (32)    // user define : must 16 align 
#define MJC_DUMP_MAX_SIZE       (1920*16)  

#define MJC_FRAME_NUM_REPEAT    (3)  
//#define MJC_FRAME_NUM_NORMAL    (9)  
UINT32 u4MjcFrmNumNormal = 9;

#define MJC_UINT_LENGTH         (16)        // 128 bit
#define MJC_PACK_LENGTH         (16*4)

#define MJC_YC_UINT_SIZE        (MJC_UINT_LENGTH*2)     // YC pack length
#define MJC_PACK_SIZE_08BIT     (MJC_YC_UINT_SIZE*4)    // YC pack size
#define MJC_PACK_SIZE_10BIT     (MJC_YC_UINT_SIZE*5)
#define MJC_PACK_SIZE_LSB       (MJC_YC_UINT_SIZE)

#define MJC_UINT_SIZE_LSB_NORMAL    (MJC_UINT_LENGTH)   
#define MJC_UINT_SIZE_08BIT_NORMAL  (MJC_UINT_LENGTH*4)    // h0 h1 h2 h3
#define MJC_UINT_SIZE_10BIT_NORMAL  (MJC_UINT_LENGTH*5)    // LSB h0 h1 h2 h3
#define MJC_PACK_SIZE_10BIT_NORMAL  (MJC_UINT_LENGTH*5*4)  // YE CE YO CO
#define MJC_SIZE_TO_NEXT_BLK_2D     (MJC_PACK_SIZE_10BIT_NORMAL*u4MjcFrmNumNormal)
#define MJC_SIZE_TO_NEXT_BLK_3D     (MJC_SIZE_TO_NEXT_BLK_2D*2)

#define MJC_SRC_SIZE_10BIT(w)       (w*2*10/8)	    // YCbCr422 10bit, must 64 align
#define MJC_FRM_SIZE_10BIT(w, h)    (w*h*2*10/8)	// YCbCr422 10bit, must 64 align


// Scaler dump definition
#define SCL_DES_WIDTH           (704)
#define SCL_DES_HEIGHT          (16) 
#define SCL_SUBSAMPLE           (32)
#define SCL_DUMP_MAX_SIZE       (1920*16)

#define SCL_BLK_BIT             (11)
#define SCL_H_PXL_BIT           (8)
#define SCL_H_PXL_MASK          ((1<<SCL_H_PXL_BIT) - 1)
#define SCL_V_PXL_BIT           (SCL_BLK_BIT-SCL_H_PXL_BIT)
#define SCL_V_PXL_MASK          ((1<<SCL_V_PXL_BIT) - 1)

UINT32 u4LumaSumL, u4LumaSumR, u4LumaSumDiffThld;

/*----------------------------------------------------------------------*
 * Global/Static Variables
 *----------------------------------------------------------------------*/

// LSV variables
static UINT8 u1LsvInit = SV_FALSE;
static E_TD_IN e3DMode = E_TD_IN_END;
static INT32 iSclRatio = 0x100;

INT32 m_width,m_height;
INT32 m_mbSize = 4;
INT32 m_lsv_width;
INT32 m_lsv_height;
INT32 pred_de_dv[10];
INT32 * dv_meter_LH = NULL;
INT32 * DE_buf_L = NULL;
INT32 * pre_depth_buf_L = NULL;
INT32 shift_avg_H=0;    
INT32 cur_convg=0;
INT32 stab_cnt=0,stab_flag=0;
INT32 pre_shift_avg_H=0;  
INT32 SR_H=160, SR_V=0;
INT32 m_depth_width=32,m_depth_height=8;//W((384-256)/4)*2+1 H((15-3*2)/2)/2+1
INT32 step_v=1,step_h=2;
INT32 str_h;
INT32 cand_num;
INT32 var_cur;
UINT8 lsv_auto_gain=32;
UINT8 lsv_local_gain=0;
UINT8 lsv_focal_plen=128;
UINT8 d2d_convg =CONVERGENCE_DEPTH_3D; 
UINT8 d2d_ratio =DEPTH_RANGE_RATIO_3D;
UINT8 d2d_gain =DISPARITY_GAIN_3D;

INT32 var_en_th, var_pnlty_sel;
INT32 rand_dvpnlty_sel;
INT32 var_cor_th;
INT32 tmpr_dvpnlty_sel;
INT32 me_mvpnlty_sel;
INT32 ord_sel, obme_num;
INT32 zero_sel;
INT32 iIIR;
UINT32 u4DebugCnt;
UINT32 u4CalLoop;
UINT32 u4WidthDec, u4HeightDec;

const INT32  seed_1[9]={9,3,8,1,5,0,7,2,6};
const INT32  seed_2[9]={9,5,6,1,3,2,7,0,8};
const INT32  seed_3[9]={9,3,2,7,5,6,1,8,0};
const INT32  seed_4[9]={9,5,0,7,3,8,1,6,2};	
const INT32 LutT[9] ={1,-1,5,-5,11,-11,19,-19,0}; 

/*----------------------------------------------------------------------*
 * External Variables & Functions
 *----------------------------------------------------------------------*/
extern void HalFlushInvalidateDCache(void);
extern TTD_INFO gTTDInfo;
extern UINT32 u4MJCBaseAddress;

/*-----------------------------------------------------------------------*
 * Local Functions
 *-----------------------------------------------------------------------*/
void vDrvLSVModeChange(INT32 iWidth, INT32 iHeight, INT32 iStep_h);
INT32 ClipABC(INT32 p,INT32 max_val, INT32 min_val);

/*************************** MJC dump function ***************************/
#ifdef CC_MT5396
UINT8 u1DrvMJCGetDumpDramInfo(DUMP_3D_INFO_T *pDumpInfo)
{       
    UINT32 u4BufSize;
    static UINT32 u4DesAddrL = (UINT32)NULL;
    static UINT32 u4DesAddrR = (UINT32)NULL;

#if 0 // dram dump debug    
    FBM_POOL_T* prPool;
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_PQ_TOOL, NULL);

    if (prPool->u4Size > (MJC_DUMP_MAX_SIZE*2))
    {
        u4DesAddrL = VIRTUAL(prPool->u4Addr);
        u4DesAddrR = u4DesAddrL + (pDumpInfo->u4DumpSize/2);       
        u4BufSize = prPool->u4Size;
        
        vRegWrite4B(SWRW_01, prPool->u4Addr);
        vRegWrite4B(SWRW_02, u4BufSize);
    }    
    else
#endif
    {
        if ((u4DesAddrL == (UINT32)NULL) && (u4DesAddrR == (UINT32)NULL))
        {
            u4DesAddrL = (UINT32)x_mem_alloc(MJC_DUMP_MAX_SIZE);          
            u4DesAddrR = (UINT32)x_mem_alloc(MJC_DUMP_MAX_SIZE);  
        }     
        u4BufSize = MJC_DUMP_MAX_SIZE*2;
    }
    
    if ((u4BufSize < pDumpInfo->u4DumpSize) 
        || (u4DesAddrL == (UINT32)NULL) || (u4DesAddrR == (UINT32)NULL))
    {            
        LOG(5,"MJC Dump Dram Error  %d   \n", pDumpInfo->u4DumpSize);    
        return SV_FALSE; 
    }          
    
    pDumpInfo->u4DesAddrL = u4DesAddrL;
    pDumpInfo->u4DesAddrR = u4DesAddrR;  
    
    return SV_TRUE;
}


UINT8 u1DrvMJCGetDumpInfo(DUMP_3D_INFO_T *pDumpInfo)
{
    INT32 iWidth, iHeight, iStep_h;

    iWidth  = (e3DMode == E_TD_IN_SBS_P) ? MJC_DES_WIDTH : (MJC_DES_WIDTH*2); 
    iHeight = MJC_DES_HEIGHT; 
    iStep_h = (e3DMode == E_TD_IN_SBS_P) ? 4 : 8;   
    vDrvLSVModeChange(iWidth, iHeight, iStep_h);
    
    pDumpInfo->u4SrcWidth = MJC_READ_FLD(MJC_SYS_02, MJC_MJC_WIDTH);
    pDumpInfo->u4SrcHeight = MJC_READ_FLD(MJC_SYS_02, MJC_MJC_HEIGHT_FX);
    pDumpInfo->u4SrcAddr = u4MJCBaseAddress;
    pDumpInfo->u4DesWidth = (pDumpInfo->u4SrcWidth < 1920) ? MJC_DES_WIDTH : (MJC_DES_WIDTH*2);
    pDumpInfo->u4DesHeight = MJC_DES_HEIGHT;
    pDumpInfo->u4Subsample = (pDumpInfo->u4SrcHeight < 1080) ? (MJC_SUBSAMPLE/2) : MJC_SUBSAMPLE;
    
    u4MjcFrmNumNormal = MJC_READ_FLD(MJC_FB_03, MJC_DRAM_FRM_CNT_M1)+1;
    
    if (arMJCPrm.u1Dram422RPT)
    {        
        pDumpInfo->u4Index = MJC_READ_FLD(MJC_STA_FB_01, MJC_STA_RFIFO_WTLV)&0x3;
    }
    else
    {
        pDumpInfo->u4Index = (MJC_READ_FLD(MJC_STA_FM_03, MJC_STA_WRT_FRM_PTR_HW)&0xF)%3;
    }  

    // Error check
    if (pDumpInfo->u4SrcHeight < pDumpInfo->u4DesHeight*pDumpInfo->u4Subsample) 
    {
        LOG(5,"MJC Dump H Error  %d   \n", pDumpInfo->u4DesHeight);    
        pDumpInfo->u4DesHeight = pDumpInfo->u4SrcHeight/pDumpInfo->u4Subsample;
    }    

    if (pDumpInfo->u4SrcWidth < pDumpInfo->u4DesWidth)
    {
        LOG(5,"MJC Dump W Error  %d   \n", pDumpInfo->u4DesWidth);    
        pDumpInfo->u4DesWidth = pDumpInfo->u4SrcWidth;
    }    
  
    pDumpInfo->u4SrcStartX = 
        (((pDumpInfo->u4SrcWidth - pDumpInfo->u4DesWidth)/2)>>6)<<6;
    pDumpInfo->u4SrcStartY = 
        (((pDumpInfo->u4SrcHeight - pDumpInfo->u4DesHeight*pDumpInfo->u4Subsample)/2)>>4)<<4;

    // Only dump Y 8bit data
    pDumpInfo->u4DumpSize = pDumpInfo->u4DesWidth*pDumpInfo->u4DesHeight*2;

    if (u1DrvMJCGetDumpDramInfo(pDumpInfo) == SV_TRUE)
    {
        LOG(5, "==== MJC Dump %s mode ==== \n", arMJCPrm.u1Dram422RPT ? "Repeat" : "Normal");
        LOG(5, "Src    %d   %d   \t0x%08x   \n", 
            pDumpInfo->u4SrcWidth, pDumpInfo->u4SrcHeight, 
            pDumpInfo->u4SrcAddr);       
        LOG(5, "Des    %d   %d   \t0x%08x   0x%08x\n", 
            pDumpInfo->u4DesWidth, pDumpInfo->u4DesHeight, 
            pDumpInfo->u4DesAddrL, pDumpInfo->u4DesAddrR);       
        LOG(5, "Start    X %d  Y %d   Subsample  %d  DumpSize  0x%08x \n", 
            pDumpInfo->u4SrcStartX, pDumpInfo->u4SrcStartY, 
            pDumpInfo->u4Subsample, pDumpInfo->u4DumpSize);        
        LOG(5, "depth_width %d   depth_height %d  \n", 
            m_depth_width, m_depth_height); 

        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

void vDrvMJCDumpRepeat(DUMP_3D_INFO_T *pInfo)
{
    UINT32 i, j, u4LoopCnt;    
    UINT32 u4SrcLineStartL, u4SrcLineStartR;    
    UINT32 u4DesLineStartL, u4DesLineStartR;       
    UINT32 u4SrcAddrL = pInfo->u4SrcAddr;    
    UINT32 u4SrcAddrR = pInfo->u4SrcAddr
        +MJC_FRM_SIZE_10BIT(pInfo->u4SrcWidth, pInfo->u4SrcHeight)*MJC_FRAME_NUM_REPEAT;    
    
    u4LoopCnt = pInfo->u4DesWidth/MJC_PACK_LENGTH;

    for (i = 0; i < pInfo->u4DesHeight; i++)
    {
        u4SrcLineStartL = u4SrcAddrL + (pInfo->u4SrcStartY+i*pInfo->u4Subsample)*MJC_SRC_SIZE_10BIT(pInfo->u4SrcWidth);                
        u4SrcLineStartR = u4SrcAddrR + (pInfo->u4SrcStartY+i*pInfo->u4Subsample)*MJC_SRC_SIZE_10BIT(pInfo->u4SrcWidth);
        
        u4DesLineStartL = pInfo->u4DesAddrL + (i+1)*pInfo->u4DesWidth - MJC_UINT_LENGTH;                
        u4DesLineStartR = pInfo->u4DesAddrR + (i+1)*pInfo->u4DesWidth - MJC_UINT_LENGTH;
    
        u4SrcLineStartL += MJC_SRC_SIZE_10BIT(pInfo->u4SrcStartX);
        u4SrcLineStartR += MJC_SRC_SIZE_10BIT(pInfo->u4SrcStartX);
        
        for (j = 0; j < u4LoopCnt; j++)
        {
            u4SrcLineStartL += MJC_PACK_SIZE_LSB;                    
            u4SrcLineStartR += MJC_PACK_SIZE_LSB;
            
            x_memcpy((void *)u4DesLineStartL, (void *)u4SrcLineStartL, MJC_UINT_LENGTH);
            x_memcpy((void *)u4DesLineStartR, (void *)u4SrcLineStartR, MJC_UINT_LENGTH);
            
            u4SrcLineStartL += MJC_YC_UINT_SIZE;                    
            u4SrcLineStartR += MJC_YC_UINT_SIZE;
            u4DesLineStartL -= MJC_UINT_LENGTH;                    
            u4DesLineStartR -= MJC_UINT_LENGTH;
            
            x_memcpy((void *)u4DesLineStartL, (void *)u4SrcLineStartL, MJC_UINT_LENGTH);
            x_memcpy((void *)u4DesLineStartR, (void *)u4SrcLineStartR, MJC_UINT_LENGTH);
            
            u4SrcLineStartL += MJC_YC_UINT_SIZE;                    
            u4SrcLineStartR += MJC_YC_UINT_SIZE;
            u4DesLineStartL -= MJC_UINT_LENGTH;                    
            u4DesLineStartR -= MJC_UINT_LENGTH;
            
            x_memcpy((void *)u4DesLineStartL, (void *)u4SrcLineStartL, MJC_UINT_LENGTH);
            x_memcpy((void *)u4DesLineStartR, (void *)u4SrcLineStartR, MJC_UINT_LENGTH);
            
            u4SrcLineStartL += MJC_YC_UINT_SIZE;                    
            u4SrcLineStartR += MJC_YC_UINT_SIZE;
            u4DesLineStartL -= MJC_UINT_LENGTH;                    
            u4DesLineStartR -= MJC_UINT_LENGTH;
            
            x_memcpy((void *)u4DesLineStartL, (void *)u4SrcLineStartL, MJC_UINT_LENGTH);
            x_memcpy((void *)u4DesLineStartR, (void *)u4SrcLineStartR, MJC_UINT_LENGTH);
    
            u4SrcLineStartL += MJC_YC_UINT_SIZE;                    
            u4SrcLineStartR += MJC_YC_UINT_SIZE;
            u4DesLineStartL -= MJC_UINT_LENGTH;                    
            u4DesLineStartR -= MJC_UINT_LENGTH;
        }
    }   
}

void vDrvMJCDumpNormal(DUMP_3D_INFO_T *pInfo)
{
    UINT32 i, j;    
    UINT32 u4BlkPosX, u4BlkPosY;     
    UINT32 u4BlkStartL, u4BlkStartR;
    UINT32 u4DesLineStartL, u4DesLineStartR;
    UINT32 u4LoopCnt, u4HorBlkCnt;
    UINT32 u4Index = (pInfo->u4Index-1);
    
    u4LoopCnt = pInfo->u4DesWidth/MJC_UINT_LENGTH;
    u4HorBlkCnt = pInfo->u4SrcWidth/MJC_UINT_LENGTH;

    for (i = 0; i < pInfo->u4DesHeight; i++)
    {
        u4BlkPosX = (pInfo->u4SrcStartX/16);
        u4BlkPosY = (pInfo->u4SrcStartY+i*pInfo->u4Subsample)/8;
        
        u4BlkStartL = pInfo->u4SrcAddr + u4Index*MJC_PACK_SIZE_10BIT_NORMAL 
            + (u4BlkPosY*u4HorBlkCnt+u4BlkPosX)*MJC_SIZE_TO_NEXT_BLK_3D + MJC_UINT_SIZE_LSB_NORMAL;
        u4BlkStartR = u4BlkStartL + MJC_PACK_SIZE_10BIT_NORMAL*u4MjcFrmNumNormal;

        u4DesLineStartL = pInfo->u4DesAddrL + (i+1)*pInfo->u4DesWidth - MJC_UINT_LENGTH;                
        u4DesLineStartR = pInfo->u4DesAddrR + (i+1)*pInfo->u4DesWidth - MJC_UINT_LENGTH;
        
        for (j = 0; j < u4LoopCnt; j++)
        {         
            x_memcpy((void *)u4DesLineStartL, (void *)u4BlkStartL, MJC_UINT_LENGTH);
            x_memcpy((void *)u4DesLineStartR, (void *)u4BlkStartR, MJC_UINT_LENGTH);
            
            u4BlkStartL += MJC_SIZE_TO_NEXT_BLK_3D;                    
            u4BlkStartR += MJC_SIZE_TO_NEXT_BLK_3D;
            u4DesLineStartL -= MJC_UINT_LENGTH;                    
            u4DesLineStartR -= MJC_UINT_LENGTH; 
        }
    }
}

void vDrvMJCDump(DUMP_3D_INFO_T *pInfo)
{
    if (arMJCPrm.u1Dram422RPT)
    {
        vDrvMJCDumpRepeat(pInfo);
    }
    else
    {
        vDrvMJCDumpNormal(pInfo);
    }
}
#endif

/*************************** Scaler dump function ***************************/

UINT8 u1DrvSCLGetDumpDramInfo(DUMP_3D_INFO_T *pDumpInfo)
{       
    UINT32 u4BufSize;
    static UINT32 u4DesAddrL = 0;
    static UINT32 u4DesAddrR = 0;

#if 0 // dram dump debug    
    FBM_POOL_T* prPool;
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_PQ_TOOL, NULL);

    if (prPool->u4Size > pDumpInfo->u4DumpSize)
    {
       	u4DesAddrL = VIRTUAL(prPool->u4Addr);
        u4DesAddrR = u4DesAddrL + (pDumpInfo->u4DumpSize/2);       
        u4BufSize = prPool->u4Size;
        
        vRegWrite4B(SWRW_01, prPool->u4Addr);
        vRegWrite4B(SWRW_02, u4BufSize);
    }    
    else
#endif        
    {
        if ((u4DesAddrL == 0) && (u4DesAddrR == 0))
        {
            u4DesAddrL = (UINT32)x_mem_alloc(SCL_DUMP_MAX_SIZE);          
            u4DesAddrR = (UINT32)x_mem_alloc(SCL_DUMP_MAX_SIZE);  
        }     
        u4BufSize = SCL_DUMP_MAX_SIZE*2;
    }
    
    if ((u4BufSize < pDumpInfo->u4DumpSize) 
        || (u4DesAddrL == 0) || (u4DesAddrR == 0))
    {            
        LOG(5,"Scaler Dump Dram Error  %d   \n", pDumpInfo->u4DumpSize);    
        return SV_FALSE; 
    }          
    
    pDumpInfo->u4DesAddrL = u4DesAddrL;
    pDumpInfo->u4DesAddrR = u4DesAddrR;  
    
    return SV_TRUE;
}

UINT8 u1DrvSCLGetDumpInfo(DUMP_3D_INFO_T *pDumpInfo)
{       
    INT32 iStep_h;
    
    pDumpInfo->u4SrcWidth = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_DRAM_RD_WIDTH_H_1);
    pDumpInfo->u4SrcHeight = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_DRAM_RD_WIDTH_V_1);   
    pDumpInfo->u4SrcAddr = VIRTUAL(RegReadFldAlign(SCPIP_DRAM_M_05, DRAM_M_05_DA_ADDR_BASE_MSB_Y)<<11);
    pDumpInfo->u4Index = RegReadFldAlign(SCPIP_PIP_OA_RO_0A, PIP_OA_RO_0A_RD_W_POS_1);

    if (pDumpInfo->u4SrcWidth >= ((SCL_DES_WIDTH+128)*2)) 
    {
        pDumpInfo->u4DesWidth = SCL_DES_WIDTH*2;     
        iStep_h = 8;    //1024
    }
    else if (pDumpInfo->u4SrcWidth >= (SCL_DES_WIDTH+128)) 
    {        
        pDumpInfo->u4DesWidth = SCL_DES_WIDTH;    
        iStep_h = 4;    //512
    }
    else if (pDumpInfo->u4SrcWidth >= (512+64))
    {        
        pDumpInfo->u4DesWidth = 512;             
        iStep_h = 3;    //384
    }
    else
    {        
        pDumpInfo->u4DesWidth = 256;             
        iStep_h = 1;    //128
    }
    
    if (pDumpInfo->u4SrcHeight >= (512+256)) 
    {
        pDumpInfo->u4DesHeight = SCL_DES_HEIGHT;                            
        pDumpInfo->u4Subsample = 32;    //512
    }
    else if (pDumpInfo->u4SrcHeight >= (256+64)) 
    {        
        pDumpInfo->u4DesHeight = SCL_DES_HEIGHT;                            
        pDumpInfo->u4Subsample = 16;    //256
    }        
    else
    {        
        pDumpInfo->u4DesHeight = SCL_DES_HEIGHT/2;                            
        pDumpInfo->u4Subsample = 16;     //128
    }
        
    vDrvLSVModeChange(pDumpInfo->u4DesWidth, pDumpInfo->u4DesHeight, iStep_h);

    // Error check
    if (pDumpInfo->u4SrcHeight < pDumpInfo->u4DesHeight*pDumpInfo->u4Subsample) 
    {
        LOG(5, " Scaler Dump H Error  %d   \n", pDumpInfo->u4DesHeight);    
        pDumpInfo->u4DesHeight = pDumpInfo->u4SrcHeight/pDumpInfo->u4Subsample;
    }    

    if (pDumpInfo->u4SrcWidth < pDumpInfo->u4DesWidth)
    {
        LOG(5, "Scaler Dump W Error  %d   \n", pDumpInfo->u4DesWidth);    
        pDumpInfo->u4DesWidth = pDumpInfo->u4SrcWidth;
    }    
  
    pDumpInfo->u4SrcStartX = 
        (((pDumpInfo->u4SrcWidth - pDumpInfo->u4DesWidth)/2)>>2)<<2;        // 4 byte align
    pDumpInfo->u4SrcStartY = 
        (((pDumpInfo->u4SrcHeight - pDumpInfo->u4DesHeight*pDumpInfo->u4Subsample)/2)>>4)<<4;

    // Only dump Y 8bit data
    pDumpInfo->u4DumpSize = pDumpInfo->u4DesWidth*pDumpInfo->u4DesHeight*2;

    if (u1DrvSCLGetDumpDramInfo(pDumpInfo) == SV_TRUE)
    {
        LOG(5, "==== Scaler Dump mode ==== \n");
        LOG(5, "Src    %d   %d   \t0x%08x   \n", 
            pDumpInfo->u4SrcWidth, pDumpInfo->u4SrcHeight, pDumpInfo->u4SrcAddr);       
        LOG(5, "Des    %d   %d   \t0x%08x   0x%08x\n", 
            pDumpInfo->u4DesWidth, pDumpInfo->u4DesHeight, 
            pDumpInfo->u4DesAddrL, pDumpInfo->u4DesAddrR);       
        LOG(5, "Start    X %d  Y %d   Subsample  %d  DumpSize  0x%08x \n", 
            pDumpInfo->u4SrcStartX, pDumpInfo->u4SrcStartY, 
            pDumpInfo->u4Subsample, pDumpInfo->u4DumpSize);        
        LOG(5, "depth_width %d   depth_height %d  \n", 
            m_depth_width, m_depth_height); 

        return SV_TRUE;
    }
    else
    {
        LOG(5, "Scaler Get Dump Info Fail\n", pDumpInfo->u4DesWidth);    
        return SV_FALSE;
    }
}

void vDrvSCLDumpNormal(DUMP_3D_INFO_T *pInfo)
{
    UINT8 bYFrmNum;    
    UINT32 u4SrcL, u4SrcR, u4DesL, u4DesR;
    UINT32 u4Pitch, u4HPage;  	
    UINT32 i, j, temp, u4Pos;

    u4Pitch = (RegReadFldAlign(SCPIP_DRAM_M_00, DRAM_M_00_DA_DRAM_LINE_PITCH)<<4);
    bYFrmNum = RegReadFldAlign(SCPIP_DRAM_M_00, DRAM_M_00_DA_FRAME_NUM)+1;
    u4HPage = (u4Pitch+SCL_H_PXL_MASK)>>SCL_H_PXL_BIT;
    
    for (j=0; j < pInfo->u4DesHeight; j++)
    {
        u4DesL = pInfo->u4DesAddrL+pInfo->u4DesWidth*j;
        u4DesR = pInfo->u4DesAddrR+pInfo->u4DesWidth*j;
        
        temp = (pInfo->u4SrcStartY+j*pInfo->u4Subsample)*bYFrmNum;        
 
        u4SrcL = pInfo->u4SrcAddr + ((((temp+0)>>SCL_V_PXL_BIT)*u4HPage)<<SCL_BLK_BIT) 
                    + (((temp+0)&SCL_V_PXL_MASK)<<SCL_H_PXL_BIT);
        u4SrcR = pInfo->u4SrcAddr + ((((temp+1)>>SCL_V_PXL_BIT)*u4HPage)<<SCL_BLK_BIT) 
                    + (((temp+1)&SCL_V_PXL_MASK)<<SCL_H_PXL_BIT);        

        temp = pInfo->u4SrcStartX;
        
        for (i=0; i < pInfo->u4DesWidth; i+=4)
        {           
            u4Pos = ((temp>>SCL_H_PXL_BIT)<<SCL_BLK_BIT)+(temp&SCL_H_PXL_MASK);
            *(UINT32*)(u4DesL+i) = *(UINT32*)(u4SrcL+u4Pos);
            *(UINT32*)(u4DesR+i) = *(UINT32*)(u4SrcR+u4Pos);    
            temp += 4;
        }
    }
}

void vDrvSCLDump(DUMP_3D_INFO_T *pInfo)
{
    vDrvSCLDumpNormal(pInfo);    
}

/*************************** LSV function ***************************/

UINT8 u1DrvLSVGetDumpInfo(DUMP_3D_INFO_T *pDumpInfo)
{    
#if defined(CC_MT5396)    
    E_TD_IN e3DModeCur = TD_MJC_IN(u4DrvTDTVMJCModeQuery());
#else
    E_TD_IN e3DModeCur = TD_SCL_IN(u4DrvTDTVMJCModeQuery());
#endif

    if (e3DMode != e3DModeCur)
    {        
        LOG(3, "3D mode change to %d -> %d \n", e3DMode, e3DModeCur);
        e3DMode = e3DModeCur;
    }

    if ((e3DModeCur != (E_TD_IN)E_TDTV_DECODER_INPUT_SBS_P) 
        && (e3DModeCur != (E_TD_IN)E_TDTV_DECODER_INPUT_TB_P) 
        && (e3DModeCur != (E_TD_IN)E_TDTV_DECODER_INPUT_FS_P))
    {
        //LOG(5, "LSV Not support 3D mode %d\n", e3DMode);
        return SV_FALSE;
    }       
    
#if defined(CC_MT5396)
    return u1DrvMJCGetDumpInfo(pDumpInfo);
#else
    return u1DrvSCLGetDumpInfo(pDumpInfo);
#endif
}

void vDrvLSVDump(DUMP_3D_INFO_T *pInfo)
{
#if defined(CC_MT5396)
    vDrvMJCDump(pInfo);   
#else                 
    vDrvSCLDump(pInfo);
#endif
    vRegWriteFldAlign(SWRW_03, pInfo->u4DesHeight*2, DUMP_CTRL_HEIGHT);
    vRegWriteFldAlign(SWRW_03, pInfo->u4DesWidth, DUMP_CTRL_WIDTH);
    vRegWriteFldAlign(SWRW_03, DUMP_FMT_Y_8BIT, DUMP_CTRL_FORMAT);    
    vRegWriteFldAlign(SWRW_04, pInfo->u4DumpSize, DUMP_CTRL_SIZE); 
}

void vDrvLSVModeChange(INT32 iWidth, INT32 iHeight, INT32 iStep_h)
{
    if ((m_lsv_width != iWidth) || (m_lsv_height != iHeight)) 
    {  
        m_lsv_width = iWidth; 
        m_lsv_height = iHeight; 
        step_h = iStep_h;   
        str_h = (m_lsv_width-m_depth_width*step_h*m_mbSize)/2;        
        SR_H = ClipABC(str_h-step_h*4, 160, 0);            

        x_memset((void *)DE_buf_L, 0, sizeof(INT32)*m_depth_width*m_depth_height);
        x_memset((void *)pre_depth_buf_L, 0, sizeof(INT32)*m_depth_width*m_depth_height);
        x_memset((void *)dv_meter_LH, 0, sizeof(INT32)*(SR_H*2+1));

        vIO32WriteFldMulti(TTD_FW_REG_05, P_Fld(SR_H, TTD_SR_H) | P_Fld(step_h, TTD_STEP_H));     
        
        #if defined(CC_MT5396)
        iSclRatio = 0x100;
        #else                 
        if (e3DMode == (E_TD_IN)E_TDTV_DECODER_INPUT_SBS_P) 
        {
            iSclRatio = (-1)*(INT32)(0x400000/RegReadFldAlign(SCPIP_US1_00, US1_00_USCALE_H_1));
        }
        else
        {            
            iSclRatio = (-1)*(INT32)(0x800000/RegReadFldAlign(SCPIP_US1_00, US1_00_USCALE_H_1));
        }
        #endif
        
        LOG(2, "LSV Mode Change ==> iWidth %d   iHeight %d   iSr_h %d   iStep_h %d \n", 
                m_lsv_width, m_lsv_height, SR_H, step_h);  
        LOG(2, "LSV Mode Change ==> str_h %d   depth_width %d   step_h %d   mbSize %d   scaler ratio %d\n", 
                str_h, m_depth_width, step_h, m_mbSize, iSclRatio);       
    }
}


INT32 ClipABC(INT32 p,INT32 max_val, INT32 min_val)
{
    if ( p > max_val )		return max_val;
    else if ( p < min_val )   return min_val;
    else				    return p;
}

/* =========================================================================================== */
void AdaptiveMW(INT32 *avg_idx_H, UINT8 SR_H, UINT8 SR_V)
{
	INT32 sum_cnt_H=0;
	INT32 sum_total_H=0;
	INT32 i;
    UINT8 fg_weight = IO32ReadFldAlign(TTD_FW_REG_07, TTD_FG_WEIGHT); 
    UINT8 bg_weight = IO32ReadFldAlign(TTD_FW_REG_07, TTD_BG_WEIGHT); 
    UINT8 amw_focal_plen = IO32ReadFldAlign(TTD_FW_REG_07, AMW_FOCAL_PLEN); 
	
	for(i=-SR_H;i<=SR_H; i++)
	{
		if(i>=(amw_focal_plen-128))
		{
			sum_cnt_H+=dv_meter_LH[SR_H+i]*fg_weight;
			sum_total_H+=dv_meter_LH[SR_H+i]*i*fg_weight;
		}
		else
		{
			sum_cnt_H+=dv_meter_LH[SR_H+i]*bg_weight;
			sum_total_H+=dv_meter_LH[SR_H+i]*i*bg_weight;
		}
	}
	*avg_idx_H=sum_total_H*2/sum_cnt_H;

	x_memset((void *)dv_meter_LH, 0, sizeof(INT32)*(SR_H*2+1));
}

/* =========================================================================================== */
void prepare_dv(INT32 blk_x, INT32 blk_y, INT32* mv_frm_buf_bi_sp_sram, INT32* mv_frm_buf_bi_tp_sram,
                UINT8 SR_H, UINT8 SR_V, UINT8 fcount,UINT8 foreback)
{
	INT32 v_bound=m_depth_height;
	INT32 h_bound=m_depth_width;

	INT32* p_mv_frm_buf_bi_tp = mv_frm_buf_bi_tp_sram;
	INT32* p_mv_frm_buf_bi_sp = mv_frm_buf_bi_sp_sram;
	INT32* p_mv_frm_buf_bi;

	INT32 idx,idy;
	INT32 i,j,k = 0;
    
	// Generate random MV
	INT32 rand_cnt = fcount;
	INT32 rand_factor = ((blk_x+blk_y+rand_cnt)&0x07);
	INT32 rand_mv;
    
	if(foreback==0)
	{
		for(j=0;j<3;j++)
		{
			for(i=0;i<3;i++)
			{	
				if (blk_y==0)
                {   //1st line
					if ((j==1)&&(i==0))		        
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_sp;
					else							
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_tp;
				}
				else if ((blk_y%2)==0)
				{	//forward 
					if ((j==0) || ((j==1)&&(i==0)))		
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_sp;
					else							
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_tp;
				}
				else				
				{	//backward				
					if ((j==0) || ((j==1)&&(i==2)))	
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_sp;
					else							
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_tp;
				}
			
				idx = ClipABC((blk_x + i - 1), h_bound-1, 0);
				idy = ClipABC((blk_y + j - 1), v_bound-1, 0);
				pred_de_dv[k++] = p_mv_frm_buf_bi[idx + idy*h_bound];
			}	
		}
	}
	else
	{
		for(j=0;j<3;j++)
		{
			for(i=0;i<3;i++)
			{	
				if (blk_y==m_depth_height-1)
				{
					if ((j==1)&&(i==0))		
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_sp;
					else							
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_tp;
				}
				else if ((blk_y%2)==0)
				{	//backward 
					if ((j==2) || ((j==1)&&(i==2)))	
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_sp;
					else							
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_tp;
				}
				else				
				{	//foreward				
					if ((j==2) || ((j==1)&&(i==0)))	
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_sp;
					else							
                        p_mv_frm_buf_bi = p_mv_frm_buf_bi_tp;
				}
			
				idx = ClipABC((blk_x + i - 1), h_bound-1, 0);
				idy = ClipABC((blk_y + j - 1), v_bound-1, 0);
				pred_de_dv[k++] = p_mv_frm_buf_bi[idx + idy*h_bound];
			}	
		}
	}

    if( blk_x==0 &&blk_y==0)					
        rand_mv = pred_de_dv[4];   //tmpr
    else if ((blk_x==0 &&blk_y!=0&&(blk_y%2)==0) || (blk_x==m_depth_width-1&&blk_y!=0&&(blk_y%2)==1))	
        rand_mv = pred_de_dv[1];    //up
    else if ((blk_y%2)==0)
        rand_mv = pred_de_dv[3];   //left
    else														
        rand_mv = pred_de_dv[5];   //right
        
	pred_de_dv[9] = ClipABC( rand_mv + LutT[rand_factor], SR_H, -SR_H);

}
/* =========================================================================================== */
INT32 ORD_Quad(PEL *pCurFull, PEL *pRefFull, INT32 cur_x, INT32 cur_y, INT32 ref_x, INT32 ref_y, UINT8 step_v, UINT8 step_h)
{
    INT32 cur_avg, ref_avg, i_str, i_end;    
	INT32 ord_cost = 0;
	INT32 cur_acc=0,ref_acc=0;
	INT32 i, pos1, pos2;
	UINT8 ord_cur,ord_ref;
	INT32 ord_pnlty = 0;

    pos1 = (cur_y+step_v)*m_lsv_width+cur_x;
    pos2 = (ref_y+step_v)*m_lsv_width+ref_x;
    
	cur_acc+=pCurFull[pos1+1*step_h];
	cur_acc+=pCurFull[pos1+2*step_h];
	ref_acc+=pRefFull[pos2+1*step_h];
	ref_acc+=pRefFull[pos2+2*step_h];
	
	cur_avg=(cur_acc+1)>>1;	
	ref_avg=(ref_acc+1)>>1;

	i_str = -obme_num;
	i_end = m_mbSize+obme_num;

    pos1 += i_str*step_h;
    pos2 += i_str*step_h;
    
	for(i=i_str; i<i_end; i++)
    {
		ord_cur=(cur_avg>pCurFull[pos1])?1:0;
		ord_ref=(ref_avg>pRefFull[pos2])?1:0;
        
        pos1 += step_h;
        pos2 += step_h;

		if(ord_cur!=ord_ref)	ord_cost++;
	}

	if     (ord_sel == 0)		ord_pnlty = 0;
	else if(ord_sel == 1)		ord_pnlty = MIN(ord_cost, 36)*1;
	else if(ord_sel == 2)		ord_pnlty = MIN(ord_cost, 36)*2;
	else if(ord_sel == 3)		ord_pnlty = MIN(ord_cost, 36)*3;
	else if(ord_sel == 4)		ord_pnlty = MIN(ord_cost, 36)*4;
	else if(ord_sel == 5)		ord_pnlty = MIN(ord_cost, 36)*6;
	else if(ord_sel == 6)		ord_pnlty = MIN(ord_cost, 36)*8;

	return ord_pnlty;
}


/* =========================================================================================== */
void DVEC_sort(INT32* dvecand)
{
	INT32 sort_tmp;
	INT32 i,j;
    
	for(i = 0; i < 4; i++)
	{
    	for(j = i+1; j < 5; j++)
        {
    		if(dvecand[i] > dvecand[j])
    		{
    			sort_tmp = dvecand[i];
    			dvecand[i] = dvecand[j];
    			dvecand[j] = sort_tmp;
    		}
    	}
	}
}
/* =========================================================================================== */
INT32  smth_pnlty(INT32 dv_curr, INT32 *dv_in, UINT8 is_even_line)
{
	//  0  1  2  3  4
	//  5  6  7  8  9 
	// 10 11 12 13 14
	// 15 16 17 18 19
	// 20 21 22 23 24
	INT32 dvecand[5];
	INT32 sad_pnlty;
	INT32 mv_pred_dif, dv_med;    

    dvecand[0] = dv_in[1];   
    dvecand[1] = dv_in[3];   
    dvecand[2] = dv_in[5];   
    dvecand[3] = dv_in[7];   
    dvecand[4] = (is_even_line) ? dv_in[0] : dv_in[2];   

	//candidate median
	DVEC_sort(dvecand);
	dv_med = dvecand[2];
    
    mv_pred_dif=MIN(ABS(dv_curr - dv_med), 8);

	if     (me_mvpnlty_sel == 0)		sad_pnlty = 0;
	else if(me_mvpnlty_sel == 1)		sad_pnlty = mv_pred_dif* 1/8;
	else if(me_mvpnlty_sel == 2)		sad_pnlty = mv_pred_dif* 2/8;
	else if(me_mvpnlty_sel == 3)		sad_pnlty = mv_pred_dif* 3/8;
	else if(me_mvpnlty_sel == 4)		sad_pnlty = mv_pred_dif* 4/8;
	else if(me_mvpnlty_sel == 5)		sad_pnlty = mv_pred_dif* 6/8;
	else if(me_mvpnlty_sel == 6)		sad_pnlty = mv_pred_dif* 8/8;
	else if(me_mvpnlty_sel == 7)		sad_pnlty = mv_pred_dif*12/8;
    else                                sad_pnlty = 0;
    
	return sad_pnlty;
}
/* =========================================================================================== */
INT32  tmpr_pnlty(INT32 i)
{
	INT32 tmpr_pnlty;
	INT32 tmpr_cost;

	if(	i==2||i==4)		tmpr_cost=2;
	else				tmpr_cost=0;

    tmpr_pnlty = tmpr_cost*tmpr_dvpnlty_sel;

	return tmpr_pnlty;
}


INT32  zero_pnlty(INT32 dv_cur)
{
	INT32 mv_pred_dif = MIN(ABS(dv_cur), 1);
	INT32 sad_pnlty;

    sad_pnlty = mv_pred_dif*zero_sel;

	return sad_pnlty;
}


INT32  rand_pnlty(INT32 i)
{
	INT32 rand_pnlty;
	INT32 rand_cost;

	if(	i==0)		rand_cost=4;
	else			rand_cost=0;

    rand_pnlty = rand_cost*rand_dvpnlty_sel;

	return rand_pnlty;
}


void DepthBlend(INT32 *pre_depth_buf_R, INT32 *filter_buf_R)
{
    x_memcpy((void *)pre_depth_buf_R, (void *)filter_buf_R, sizeof(INT32)*m_depth_height*m_depth_width);
}


INT32 blk_var(PEL *pCurFull, UINT8 step_v, UINT8 step_h, INT32 str_h)
{
	INT32 i,j;
	INT32 abs_df1, abs_df2;
	
	var_cur=0;
	for(i=str_h;i<m_depth_width*step_h*m_mbSize;i+=4)
	{
		for(j=0;j<m_depth_height*step_v*2;j+=2)
		{
			abs_df1 = ABS(pCurFull[(i+step_h)+(j  )*m_lsv_width] - pCurFull[(i	     )+(j+1)*m_lsv_width]);
			abs_df2 = ABS(pCurFull[(i		)+(j+1)*m_lsv_width] - pCurFull[(i+step_h)+(j  )*m_lsv_width]);
			
			if(abs_df1>var_cor_th)		var_cur ++;
			if(abs_df2>var_cor_th)		var_cur ++;            
		}
	}
	return var_cur;
}   


INT32 var_pnlty(INT32 dv_cur)
{
	INT32 var_pnlty;
	INT32 var_cost = 0;

	if(var_cur<var_en_th)	
	{
        var_cost=ABS(dv_cur);
	}
    
    var_pnlty = var_cost*var_pnlty_sel;

	return var_pnlty;
}


void DVSearchLPS(PEL *pCurFull, PEL *pRefFull, INT32* cur_depth_buf, INT32* pre_depth_buf_cur,
						 UINT8 SR_H, UINT8 SR_V, UINT8 fcount, UINT8 step_v, UINT8 step_h, INT32 str_h)
{
	INT32 n, dx; //n = Search Loop For SRW ,  dx = best candidate
	INT32 ref_x, ref_y;
	INT32 smth_cost, ord_cost, rand_cost, tmpr_cost, zero_cost, var_cost;
	INT32 minimum, SAD_cand;
	INT32 V_cand_step=2*step_v; 
	INT32 H_cand_step=step_h*m_mbSize;
	INT32 i, cur_x, cur_y;
	UINT8 is_even_line;
			
	//  0  1  2  3  4
	//  5  6  7  8  9 
	// 10 11 12 13 14
	// 15 16 17 18 19
	// 20 21 22 23 24

	// Full search disparity estimation										
	for (cur_y=0; cur_y<m_depth_height; cur_y++)
	{
		is_even_line = ((cur_y%2)==0) ? SV_TRUE : SV_FALSE;
        
		if(is_even_line)
		{
			for (cur_x=0; cur_x<m_depth_width; cur_x++)
			{
				minimum = 65535;	dx=-256;
				prepare_dv(cur_x, cur_y, cur_depth_buf, pre_depth_buf_cur, SR_H, SR_V, fcount, 0);

				for (i=0; i < cand_num; i++)
				{
					n=pred_de_dv[seed_1[i]];

					ref_y = cur_y*V_cand_step;		        // Vertical   co-ordinate for reference block in previous frame
					ref_x = str_h+cur_x*H_cand_step+n;		// Horizontal co-ordinate for reference block in previous frame
				
					smth_cost = smth_pnlty(n, pred_de_dv, is_even_line);
					tmpr_cost = tmpr_pnlty(i);
					rand_cost =	rand_pnlty(i);
					zero_cost =	zero_pnlty(n);
					 var_cost =  var_pnlty(n);
					ord_cost  = ORD_Quad(pCurFull, pRefFull, str_h+cur_x*H_cand_step, cur_y*V_cand_step, 
                                        ref_x, ref_y, step_v, step_h);
                    
					SAD_cand =  smth_cost + ord_cost + zero_cost + rand_cost + tmpr_cost + var_cost;

					// Find the vector where the SAD is minimum and store it
					if(SAD_cand < minimum)
					{
						minimum = SAD_cand;
						dx = n;
					}
				}
                
				//write histogram
				cur_depth_buf[cur_x+cur_y*m_depth_width] = dx;
                
				if(dx!=-256)
				{
					dv_meter_LH[dx+SR_H]++;
				}
			}//LOOP BLOCK X :cur_x 0->width
		}//LOOP Even Line 
		else
		{
			for (cur_x=(m_depth_width-1); cur_x>=0; cur_x--)
			{
				minimum = 65535;	dx=-256;
				prepare_dv(cur_x, cur_y,cur_depth_buf, pre_depth_buf_cur, SR_H, SR_V, fcount, 0);

				for (i=0; i < cand_num; i++)
				{
					n=pred_de_dv[seed_2[i]];
					ref_y = cur_y*V_cand_step;		        // Vertical co-ordinate for reference block in previous frame
					ref_x = str_h+cur_x*H_cand_step+n;		// Horizontal co-ordinate for reference block in previous frame
				
					smth_cost = smth_pnlty(n, pred_de_dv, is_even_line);
					tmpr_cost = tmpr_pnlty(i);
					rand_cost =	rand_pnlty(i);
					zero_cost =	zero_pnlty(n);
					 var_cost =  var_pnlty(n);
					ord_cost  = ORD_Quad(pCurFull, pRefFull, str_h+cur_x*H_cand_step, cur_y*V_cand_step, 
                                        ref_x, ref_y, step_v, step_h);
                    
					SAD_cand =  smth_cost + ord_cost + zero_cost + rand_cost + tmpr_cost + var_cost;

					// Find the vector where the SAD is minimum and store it
					if(SAD_cand < minimum)
					{
						minimum = SAD_cand;
						dx = n;
					}
				}
                
				//write histogram
				cur_depth_buf[cur_x+cur_y*m_depth_width] = dx;
                
				if(dx!=-256)
				{
					dv_meter_LH[dx+SR_H]++;
				}
			}//LOOP BLOCK X :cur_x 0->width
		}
	}//LOOP BLOCK Y :cur_y
}


void DVSearchLPS_Back(PEL *pCurFull, PEL *pRefFull, INT32* cur_depth_buf, INT32* pre_depth_buf_cur,
						 UINT8 SR_H, UINT8 SR_V, UINT8 fcount, UINT8 step_v, UINT8 step_h, INT32 str_h)
{
	INT32 n, dx; //n = Search Loop For SRW ,  dx = best candidate
	INT32 ref_x, ref_y;
	INT32 smth_cost,ord_cost,rand_cost,tmpr_cost,zero_cost,var_cost;
	INT32 minimum, SAD_cand;
	INT32 V_cand_step=2*step_v; 
	INT32 H_cand_step=step_h*m_mbSize;
	INT32 cur_x,cur_y;
	UINT8 is_even_line;
	INT32 i;
						
	//  0  1  2  3  4
	//  5  6  7  8  9 
	// 10 11 12 13 14
	// 15 16 17 18 19
	// 20 21 22 23 24

	// Full search disparity estimation										
	for(cur_y=(m_depth_height-1); cur_y>=0; cur_y--)
	{
		is_even_line=((cur_y%2)==1) ? SV_TRUE : SV_FALSE;
        
		if(is_even_line)
		{
			for(cur_x=0; cur_x < m_depth_width; cur_x++)
			{
				minimum = 65535;	dx=-256;
				prepare_dv(cur_x, cur_y, cur_depth_buf, pre_depth_buf_cur, SR_H, SR_V, fcount, 1);
				
				for (i=0; i < cand_num; i++)
				{
					n=pred_de_dv[seed_3[i]];

					ref_y = cur_y*V_cand_step;		        // Vertical   co-ordinate for reference block in previous frame
					ref_x = str_h+cur_x*H_cand_step+ n;		// Horizontal co-ordinate for reference block in previous frame
				
					smth_cost = smth_pnlty(n, pred_de_dv, is_even_line);
					tmpr_cost = tmpr_pnlty(i);
					rand_cost =	rand_pnlty(i);
					zero_cost =	zero_pnlty(n);
					 var_cost =  var_pnlty(n);
					ord_cost  = ORD_Quad(pCurFull, pRefFull, str_h+cur_x*H_cand_step, cur_y*V_cand_step, 
                                        ref_x, ref_y, step_v, step_h);
					
					SAD_cand =  smth_cost + ord_cost + zero_cost + rand_cost + tmpr_cost + var_cost;

					// Find the vector where the SAD is minimum and store it
					if(SAD_cand < minimum)
                    {
						minimum = SAD_cand;
						dx = n;
					}
				}
                
				//write histogram
				cur_depth_buf[cur_x+cur_y*m_depth_width] = dx;
                
				if(dx!=-256)
                {
					dv_meter_LH[dx+SR_H]++;
				}
			}//LOOP cur_x 
		}//LOOP Even Line 
		else
		{
			for(cur_x=m_depth_width-1; cur_x>=0; cur_x--)
			{
				minimum = 65535;	dx=-256;
				prepare_dv(cur_x, cur_y, cur_depth_buf, pre_depth_buf_cur, SR_H, SR_V, fcount, 1);
				
				for (i=0; i < cand_num; i++)
				{
					n=pred_de_dv[seed_4[i]];
                    
					ref_y = cur_y*V_cand_step;		        // Vertical co-ordinate for reference block in previous frame
					ref_x = str_h+cur_x*H_cand_step+n;		// Horizontal co-ordinate for reference block in previous frame
				
					smth_cost = smth_pnlty(n, pred_de_dv, is_even_line);
					tmpr_cost = tmpr_pnlty(i);
					rand_cost =	rand_pnlty(i);
					zero_cost =	zero_pnlty(n);
					 var_cost =  var_pnlty(n);
					ord_cost  = ORD_Quad(pCurFull, pRefFull, str_h+cur_x*H_cand_step, cur_y*V_cand_step, 
                                        ref_x, ref_y, step_v, step_h);
					
					SAD_cand =  smth_cost + ord_cost + zero_cost + rand_cost + tmpr_cost + var_cost;

					// Find the vector where the SAD is minimum and store it
					if(SAD_cand < minimum)
                    {
						minimum = SAD_cand;
						dx = n;
					}
				}
                
				//write histogram
				cur_depth_buf[cur_x+cur_y*m_depth_width] = dx;
                
				if(dx!=-256)
                {
					dv_meter_LH[dx+SR_H]++;
				}
			}//LOOP cur_x 
		}
	}//LOOP BLOCK Y :cur_y
}

void vDrvTTDLSVUpdateParam(void)
{        
    static UINT32 u4HomoMo[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    static UINT8 u1MoCnt = 0;
    
    u4HomoMo[u1MoCnt] = u4DrvDIGetFusionMotionLevel();
    u1MoCnt = ((u1MoCnt+1)&0x3);   
    
    m_depth_width = IO32ReadFldAlign(TTD_FW_REG_04, TTD_DE_ARRAY_W); 
    m_depth_height = IO32ReadFldAlign(TTD_FW_REG_04, TTD_DE_ARRAY_H);    

    SR_H = IO32ReadFldAlign(TTD_FW_REG_05, TTD_SR_H); 

    step_h = IO32ReadFldAlign(TTD_FW_REG_05, TTD_STEP_H); 
    m_mbSize = IO32ReadFldAlign(TTD_FW_REG_06, TTD_MB_SIZE); 
    cand_num = IO32ReadFldAlign(TTD_FW_REG_06, TTD_CAND_NUM); 

    lsv_focal_plen =  IO32ReadFldAlign(TTD_FW_REG_08, LSV_CONVG_PLEN); 
     
    #if defined(CC_MT5396)
    lsv_auto_gain = MJC_READ_FLD(MJC_TTD_REG_00, TTD_LSV_CONVG_GAIN);
    lsv_local_gain = MJC_READ_FLD(MJC_TTD_REG_00, TTD_LSV_ENH_GAIN);
    #else    
    lsv_auto_gain = IO32ReadFldAlign(SW_TTD_REG_00, SW_LSV_CONVG_GAIN);
    lsv_local_gain = IO32ReadFldAlign(SW_TTD_REG_00, SW_LSV_ENH_GAIN);
    #endif

    d2d_convg =  IO32ReadFldAlign(TTD_FW_REG_10, D2D_CONVG); 
    d2d_ratio =  IO32ReadFldAlign(TTD_FW_REG_10, D2D_RATIO); 
    d2d_gain  =  IO32ReadFldAlign(TTD_FW_REG_10, D2D_GAIN); 

    var_en_th = IO32ReadFldAlign(TTD_FW_REG_06, TTD_VAR_EN_TH); 
    var_pnlty_sel = IO32ReadFldAlign(TTD_FW_REG_02, TTD_VAR_SEL);
    var_cor_th = IO32ReadFldAlign(TTD_FW_REG_02, TTD_VAR_COR_TH); 
    me_mvpnlty_sel = IO32ReadFldAlign(TTD_FW_REG_02, TTD_SMTH_SEL);
    ord_sel = IO32ReadFldAlign(TTD_FW_REG_02, TTD_ORD_QUAD_SEL); 
    obme_num= IO32ReadFldAlign(TTD_FW_REG_05, TTD_OBME_NUM); 
    zero_sel = IO32ReadFldAlign(TTD_FW_REG_02, TTD_ZERO_SEL);
    u4DebugCnt = IO32ReadFldAlign(TTD_FW_REG_16, TTD_DEBUG_CNT);
    u4CalLoop = IO32ReadFldAlign(SW_TTD_REG_00, SW_LSV_DELAY_TIMES);
    u4WidthDec = IO32ReadFldAlign(SW_TTD_REG_00, SW_LSV_WIDTH_DEC);    
    u4HeightDec = IO32ReadFldAlign(SW_TTD_REG_00, SW_LSV_HEIGHT_DEC);

    m_depth_width = (m_depth_width > (u4WidthDec*2)) ? (m_depth_width - (u4WidthDec*2)) : 2;
    m_depth_height = (m_depth_height>u4HeightDec) ? (m_depth_height-u4HeightDec) : 1; 
    if ((u4HomoMo[0] == 0) && (u4HomoMo[1] == 0) 
        && (u4HomoMo[2] == 0) && (u4HomoMo[3] == 0))
    {        
        rand_dvpnlty_sel = IO32ReadFldAlign(TTD_FW_REG_16, TTD_RAND_SEL_FREEZE);        
        tmpr_dvpnlty_sel = IO32ReadFldAlign(TTD_FW_REG_16, TTD_TMPR_SEL_FREEZE);
        iIIR = IO32ReadFldAlign(TTD_FW_REG_16, TTD_IIR_FREEZE);
    }
    else
    {
        rand_dvpnlty_sel = IO32ReadFldAlign(TTD_FW_REG_02, TTD_RAND_SEL);        
        tmpr_dvpnlty_sel = IO32ReadFldAlign(TTD_FW_REG_02, TTD_TMPR_SEL);        
        iIIR = IO32ReadFldAlign(TTD_FW_REG_16, TTD_IIR_NORMAL);        
    }
}


//16(depth, 194, 60, 20)
//24(depth, 216, 60, 15)
UINT8 dsp2dep(INT32 in_disparity, UINT8 cd, UINT8 gain, UINT8 ratio) 
{
	INT32 out_depth;
	UINT8 final_depth;
	INT32 inv_ratio;
	INT32 inv_cd_ratio;
	UINT8 floating_no = 3;
	
	inv_ratio = (1<<6)-ratio;
	inv_cd_ratio = (1<<14) - cd*inv_ratio;

	out_depth = ( (1<<12)*gain*inv_cd_ratio ) /
						 ( (in_disparity*inv_cd_ratio/(1<<floating_no))+(1<<12)*gain) ;

	out_depth = ((1<<14) - out_depth)/ inv_ratio;
	
	final_depth = (out_depth>255)?255:(out_depth<0)?0: out_depth;

	return final_depth;
}

void gen_cid_setting(INT32 ori_global_dsp, UINT8 ori_global_depth, UINT8 dsp_range, 
					 UINT8* local_gain, UINT8 *new_global_depth)
{
	INT32 low_global_dsp;
    //INT32 apl_sum = IO32ReadFldAlign(STA_TTDVP_20 ,STA_TTDVP_20_STA_TTD_IMAGE_APL); 
	//INT32 apl= apl_sum/120/135;   //SBS W_120 x H_135
	INT32 apl = IO32ReadFldAlign(TTD_FW_REG_08,LSV_CID_APL_AVG); 
	UINT8 is_inv =0;// IO32ReadFldAlign(TTD_FW_REG_00, TTD_FW_IS_INVERSE);

	apl = (is_inv)? (255-apl) : apl;

	//dsp_range = dsp_range*apl/128;

	low_global_dsp = ori_global_dsp - dsp_range;

	*new_global_depth = dsp2dep(low_global_dsp,d2d_convg, d2d_gain, d2d_ratio);

	apl = (apl<16)?16:apl;
	
	if(ori_global_depth < *new_global_depth)
		*local_gain = 0;
	else
		*local_gain = ((ori_global_depth - *new_global_depth)<<5)/apl;
}

static void vDrvTTDLumaSum(UINT32 size, PEL *SRYL, PEL *SRYR)
{      
    UINT32 i;

    u4LumaSumL = 0;
    u4LumaSumR = 0;
    u4LumaSumDiffThld = size*0x10;
    
    for (i=0; i < size; i++)
    {           
        u4LumaSumL += SRYL[i];
        u4LumaSumR += SRYR[i];
    }
}

static void vDrvTTDLSVCalculate(PEL *SRYL, PEL *SRYR)
{      
	INT32 i4DepthCtrlShift, i4TotalShift, i4GlobalShift, i4DisparityGain, i4CurConvgMc, i4NewDV;
	INT32 i4BlackFromTTD, i4BlackFromGBL, i4BlackFromTDDC, i4Temp, i4BlackNum, i4BlackSingleNum;
	UINT8 u1PrInfo, u1BlackSingleInv;
    static UINT32 u4DiffPat = 0;
    
#if defined(CC_MT5396)    
    E_TD_IN e3DModeCur = TD_MJC_IN(u4DrvTDTVMJCModeQuery());
#endif

    static UINT8 bCount = 0;  
	INT32 lsv_global_dspt;
	UINT8 lsv_global_depth;
	UINT8 lsv_final_global_depth;
	UINT8 lsv_final_cid_gain;
    UINT8 u1AplL, u1AplR;   
    UINT8 u1AslL, u1AslR; 
    static UINT8 u1LsvEnable = SV_FALSE;
        
    if (u1DrvTTDGet3DDepthCtrl())
    {           
        vDrvGetAPLLR(&u1AplL, &u1AplR);
        vDrvGetASLLR(&u1AslL, &u1AslR);
        
        vDrvTTDLSVUpdateParam();

        blk_var(SRYL, step_v,step_h,str_h);

        if(bCount%2==0)
            DVSearchLPS(SRYL, SRYR, DE_buf_L, pre_depth_buf_L, SR_H, SR_V, bCount, step_v, step_h, str_h);
        else
            DVSearchLPS_Back(SRYL, SRYR, DE_buf_L, pre_depth_buf_L, SR_H, SR_V, bCount, step_v, step_h, str_h);

        //global disparity analysis
        AdaptiveMW(&shift_avg_H, SR_H, SR_V); 

        //compensate upscaler effect
        shift_avg_H = ((shift_avg_H*iSclRatio)/256);      
               
        //disparity buffer store
        DepthBlend(pre_depth_buf_L, DE_buf_L);

        //analysis global disparity stability
        if(pre_shift_avg_H==shift_avg_H)
        {
         	if(stab_cnt>=8)		
            {
                stab_flag=1;
        	}
        	else
            {
                stab_cnt++;
        	}
        }
        else
        {
        	stab_cnt=0;
        	stab_flag=0;
        }

        //current global disparity decision
        if (u1LsvEnable == SV_TRUE)
        {
            if (cur_convg!=shift_avg_H)
            {
            	if (stab_flag==0)	
            	{                    
                    cur_convg = cur_convg+(shift_avg_H-cur_convg)/iIIR;        
            	}
                else 
            	{
            		if (shift_avg_H>cur_convg)		
                    {
                        cur_convg++;
            		}
            		else if(shift_avg_H<cur_convg)	
                    {
                        cur_convg--;
            		}
            	}
            }
        }
        else
        {
            cur_convg = shift_avg_H;
            u1LsvEnable = SV_TRUE;
            LOG(3, "LSV enable : cur_convg %d  shift_avg_H %d \n", cur_convg/2, shift_avg_H/2); 
        }

        // Disable 3DC when LR are different images 
        if (((Diff(u1AplL, u1AplR)) > 0x10) || ((Diff(u1AslL, u1AslR)) > 0x10) 
            || (Diff(u4LumaSumL, u4LumaSumR) > u4LumaSumDiffThld))
        {
            u4DiffPat = (u4DiffPat < 0x4) ? (u4DiffPat+1) : 0x4;
        }
        else
        {
            u4DiffPat = 0;
        }

        if (u4DiffPat >= 4) 
        {            
            cur_convg = 0;
            shift_avg_H = 0;                    
            LOG(3, "Disable 3DC : APL %d %d ASL %d %d  Sum %08x %08x   Diff %08x Thld %08x\n", 
                u1AplL, u1AplR, u1AslL, u1AslR, u4LumaSumL, u4LumaSumR, 
                Diff(u4LumaSumL, u4LumaSumR), u4LumaSumDiffThld);       
        }
        
        pre_shift_avg_H = shift_avg_H;	
    }
    else
    {         
        cur_convg = 0;
        shift_avg_H = 0;
        lsv_auto_gain = SUPPORT_DISPARITY_INCREASE*CONVG_GAIN_PRECISION;
        lsv_local_gain = 0;
        u1LsvEnable = SV_FALSE;
    }
        
    if (bCount++ >= u4DebugCnt)
    {       
        LOG(3, "cur_convg %d  shift_avg_H %d \n", cur_convg/2, shift_avg_H/2);         
        LOG(5, "IIR %d   Rand %d   Tmpr %d \n", iIIR, rand_dvpnlty_sel, tmpr_dvpnlty_sel); 
        bCount = 0;
    }       
    
#if 0    
 	lsv_global_dspt = (lsv_focal_plen-128)+(-cur_convg*(((INT32)lsv_auto_gain)-SUPPORT_DISPARITY_INCREASE*CONVG_GAIN_PRECISION)/CONVG_GAIN_PRECISION);
 	lsv_global_depth= dsp2dep(lsv_global_dspt,d2d_convg, d2d_gain, d2d_ratio);

	gen_cid_setting(lsv_global_dspt, lsv_global_depth, lsv_local_gain, 
					 &lsv_final_cid_gain, &lsv_final_global_depth);

	vDrvTTDSetLocalWeight(lsv_final_cid_gain);
	vDrvTTDSetDepthBias(lsv_global_depth);
#else

#if defined(CC_MT5396)    
    if (e3DModeCur != (E_TD_IN)E_TDTV_DECODER_INPUT_SBS_P)
    {
        i4CurConvgMc = (cur_convg + 2) >> 2;
    }
    else
    {
        i4CurConvgMc = (cur_convg + 4) >> 3;
    }
#else
    i4CurConvgMc = (cur_convg + 2) >> 2;
#endif    
    i4CurConvgMc = (i4CurConvgMc > 32) ? 32 : ((i4CurConvgMc < (-32)) ? (-32) : i4CurConvgMc);
	i4DepthCtrlShift = i4DrvUiToReg(32, 0, 16, i4CurConvgMc, -i4CurConvgMc, 0, lsv_auto_gain, 128, 7);
	i4TotalShift = lsv_focal_plen + i4DepthCtrlShift;

	i4NewDV = cur_convg + (i4TotalShift - 128)*2;

	if (i4NewDV > 2*g_i4dLimMax)
	{
		// we hope i4NewDV = 2*DLimMax, that means
		i4TotalShift = (g_i4dLimMax + 128) - cur_convg/2;
	}
	else if (i4NewDV < 2*g_i4dLimMin)
	{
		// we hope i4NewDV = 2*DLimMin, that means
		i4TotalShift = (g_i4dLimMin + 128) - cur_convg/2;
	}

	if ((VER_5368_MP == g_u1Ver) || (VER_5396_MP == g_u1Ver))
	{
		u1PrInfo = 0;
		i4DisparityGain = DISPARITY_GAIN_3D;
		i4GlobalShift = i4TotalShift;
		u1BlackSingleInv = u1DrvTTDSignAB(128, i4GlobalShift);
	}
	else
	{
		u1PrInfo = u1DrvTTDSignAB(i4TotalShift, 128);
		i4DisparityGain = i4DrvTTDDiffAB(i4TotalShift, 128);
		i4GlobalShift = 128;
		u1BlackSingleInv = 0;
	}

	vDrvTTDSetPrInfo(u1PrInfo^g_u1LREyeInverse);
	vDrvTTDSetDisparityGain((UINT8)i4DisparityGain);
	vDrvTTDSetNewGlobalShift((UINT8)i4GlobalShift);
	vDrvTTDSetBlackSingleInv(u1BlackSingleInv);
 	   
	i4BlackFromTDDC = 0;
	i4BlackFromGBL = i4DrvTTDDiffAB(i4GlobalShift, 128);
	i4BlackFromTTD = (i4DisparityGain > 0) ? (i4DisparityGain + 3) : 0;

	i4PreBlackNum[0] = i4BlackFromTTD + i4BlackFromGBL + i4BlackFromTDDC;
	i4PreBlackSingleNum[0] = 0;

#if SUPPORT_BLACK_SINGLE_MASK
	if ((VER_5368_MP == g_u1Ver) || (VER_5396_MP == g_u1Ver))
	{
		i4PreBlackNum[0] = i4BlackFromTTD;
		i4PreBlackSingleNum[0] = i4BlackFromTTD + i4BlackFromGBL + i4BlackFromTDDC;
	}
#endif

	i4BlackNum = i4DrvGetArrayMaxValue(i4PreBlackNum, 8);
	i4BlackSingleNum = i4DrvGetArrayMaxValue(i4PreBlackSingleNum, 8);

	for (i4Temp = 7; i4Temp > 0; i4Temp--)
	{
		i4PreBlackNum[i4Temp] = i4PreBlackNum[i4Temp-1];
		i4PreBlackSingleNum[i4Temp] = i4PreBlackSingleNum[i4Temp-1];
	}
	
#if defined(CC_MT5368) || defined(CC_MT5389)
	vDrvTTDSetBlackNum(0);
	vDrvTTDSetBlackSingleNum(0);
    UNUSED(i4BlackSingleNum);
#else
	vDrvTTDSetBlackNum(i4BlackNum);
	vDrvTTDSetBlackSingleNum(i4BlackSingleNum);
#endif
	vDrvTTDSetBlackByBorderWindow(i4BlackNum);

	UNUSED(lsv_final_cid_gain);
	UNUSED(lsv_final_global_depth);
	UNUSED(lsv_global_depth);
	UNUSED(lsv_global_dspt);
#endif

    vIO32WriteFldMulti(TTD_FW_REG_01, P_Fld(pre_shift_avg_H, TTD_PRE_SHIFT_AVG_H)
                                    | P_Fld(shift_avg_H, TTD_SHIFT_AVG_H));
                                        
    vIO32WriteFldAlign(TTD_FW_REG_07, cur_convg, TTD_CUR_CONVG);      
    vIO32WriteFldAlign(TTD_FW_REG_05, stab_cnt, TTD_STAB_CNT);  
}

/*-----------------------------------------------------------------------*
 * LSV interface
 *-----------------------------------------------------------------------*/

void vDrvTTDLSVInit(void)
{
    INT32 lsv_width = LSV_DEF_DES_WIDTH*2;
    INT32 lsv_height = LSV_DEF_DES_WIDTH;
    
    if (u1LsvInit != SV_FALSE)
    {
        LOG(0, "Lsv already init\n");
        return;
    }
    
    m_width = 1920;          // 1920
    m_height = 1080;        // 1080
    m_depth_width  = 32;     // 16
    m_depth_height = 8;    // 8

    m_lsv_width  = lsv_width; // dram width
    m_lsv_height = lsv_height; // dram height 
    str_h = (lsv_width-m_depth_width*step_h*m_mbSize)/2;
        
    DE_buf_L = (INT32 *)x_mem_alloc(sizeof(INT32)*(m_depth_width*m_depth_height));  
    pre_depth_buf_L = (INT32 *)x_mem_alloc(sizeof(INT32)*(m_depth_width*m_depth_height));    
    dv_meter_LH = (INT32 *)x_mem_alloc(sizeof(INT32)*(SR_H*2+1));  

    if ((DE_buf_L == NULL) || (pre_depth_buf_L == NULL) || (dv_meter_LH == NULL))
    {        
        LOG(0, "Lsv init fail : allocate memory fail\n");
        return;
    }

    x_memset((void *)DE_buf_L, 0, sizeof(INT32)*m_depth_width*m_depth_height);
    x_memset((void *)pre_depth_buf_L, 0, sizeof(INT32)*m_depth_width*m_depth_height);
    x_memset((void *)dv_meter_LH, 0, sizeof(INT32)*(SR_H*2+1));

    vIO32WriteFldMulti(TTD_FW_REG_02, P_Fld(3, TTD_ORD_QUAD_SEL)
                                    | P_Fld(2, TTD_SMTH_SEL)
                                    | P_Fld(1, TTD_TMPR_SEL)
                                    | P_Fld(3, TTD_ZERO_SEL)
                                    | P_Fld(3, TTD_VAR_SEL)
                                    | P_Fld(1, TTD_RAND_SEL)
                                    | P_Fld(4, TTD_VAR_COR_TH)); 
    
	vIO32WriteFldMulti(TTD_FW_REG_04, P_Fld(32, TTD_DE_ARRAY_W)
                                    | P_Fld(8, TTD_DE_ARRAY_H)); 
    
	vIO32WriteFldMulti(TTD_FW_REG_05, P_Fld(32, TTD_Y_SUBSAMPLE)
                                    | P_Fld(80, TTD_SR_H)
                                    | P_Fld(4, TTD_OBME_NUM)
                                    | P_Fld(4, TTD_STEP_H)); 
    
	vIO32WriteFldMulti(TTD_FW_REG_06, P_Fld(4, TTD_MB_SIZE)
                                    | P_Fld(4, TTD_CAND_NUM)
                                    | P_Fld(64,TTD_VAR_EN_TH)); 

	vIO32WriteFldMulti(TTD_FW_REG_07, P_Fld(2, TTD_FG_WEIGHT)
									| P_Fld(1, TTD_BG_WEIGHT)
									| P_Fld(128, AMW_FOCAL_PLEN)); 

	vIO32WriteFldMulti(TTD_FW_REG_08, P_Fld(128, LSV_CONVG_PLEN)
                                    | P_Fld(128, LSV_CID_APL_AVG)); 
    
    vIO32WriteFldMulti(TTD_FW_REG_16, P_Fld(4, TTD_IIR_NORMAL)
                                    | P_Fld(8, TTD_IIR_FREEZE)
                                    | P_Fld(0, TTD_TMPR_SEL_FREEZE)             
                                    | P_Fld(3, TTD_RAND_SEL)
                                    | P_Fld(8, TTD_DEBUG_CNT)); 
    
#if defined(CC_MT5396)    
    MJC_WRITE_FLD(MJC_TTD_REG_00, 32, TTD_LSV_CONVG_GAIN);    
    MJC_WRITE_FLD(MJC_TTD_REG_00, 0, TTD_LSV_ENH_GAIN);     
#else
    vIO32WriteFldMulti(SW_TTD_REG_00, P_Fld(32, SW_LSV_CONVG_GAIN)                                    
                                    | P_Fld(0, SW_LSV_ENH_GAIN));     
#endif

#ifdef DEF_3DC_LITE_VERSION
    vIO32WriteFldMulti(SW_TTD_REG_00, P_Fld(2, SW_LSV_WIDTH_DEC)
                                       | P_Fld(2, SW_LSV_HEIGHT_DEC)
                                       | P_Fld(1, SW_LSV_DELAY_TIMES));    
#else
    vIO32WriteFldMulti(SW_TTD_REG_00, P_Fld(0, SW_LSV_WIDTH_DEC)
                                       | P_Fld(0, SW_LSV_HEIGHT_DEC)
                                       | P_Fld(0, SW_LSV_DELAY_TIMES));    
#endif

	vIO32WriteFldMulti(TTD_FW_REG_10, P_Fld(CONVERGENCE_DEPTH_3D , D2D_CONVG)
                                    | P_Fld(DEPTH_RANGE_RATIO_3D  , D2D_RATIO)
                                    | P_Fld(DISPARITY_GAIN_3D  , D2D_GAIN)); 

    // CID inverse 
    vIO32WriteFldMulti(TTD_FW_REG_03, P_Fld(0 , TTD_HIST_THLD)
                                    | P_Fld(80, TTD_AVG_THLD)
                                    | P_Fld(61, TTD_DIVER_THLD_L)
                                    | P_Fld(146, TTD_DIVER_THLD_H));    

    u1LsvInit = SV_TRUE; 
}

void vDrvTTDLSVProc(void)
{    
    HAL_TIME_T TimeStart, TimeNow, TimeDelta;
    DUMP_3D_INFO_T stInfo;
    static UINT8 u1CalCount = 0;

    if (u1LsvInit == SV_FALSE)   
    {        
        vDrvTTDSet3DDepthCtrl(SV_OFF);
        LOG(0, "LSV init failed, stop Lsv");
        return;
    }

    if (u1DrvLSVGetDumpInfo(&stInfo) == SV_FALSE)   
    {
        return;
    }
          
    LOG(5, "==== LSV Proc ====  %d\n", stInfo.u4Index);
    
    if (stInfo.u4Index == 1)
    {            
        if (u1CalCount >= u4CalLoop)
        {      
            u1CalCount = 0;
        }
        else
        {
            u1CalCount++;
            return;
        }
        
        HAL_GetTime(&TimeStart);

        if (u1DrvTTDGet3DDepthCtrl())
        {            
            HalFlushInvalidateDCache();    
            vDrvLSVDump(&stInfo);
        }
 
        HAL_GetTime(&TimeNow);
        HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
        LOG(4, "Dump time  %d.%06d sec.\n", TimeDelta.u4Seconds, TimeDelta.u4Micros);
    }         
    else if (stInfo.u4Index == 2) 
    {             
        if (u1CalCount != 0)
        {
            return;
        }
        
        HAL_GetTime(&TimeStart);
        
        vDrvTTDLSVCalculate((UINT8 *)stInfo.u4DesAddrL, (UINT8 *)stInfo.u4DesAddrR);
        
        HAL_GetTime(&TimeNow);
        HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
        LOG(4, "Calculate time  %d.%06d sec.\n", TimeDelta.u4Seconds, TimeDelta.u4Micros);       
    }
    else
    {             
        HAL_GetTime(&TimeStart);
        
        vDrvTTDLumaSum(stInfo.u4DesWidth*stInfo.u4DesHeight, (UINT8 *)stInfo.u4DesAddrL, (UINT8 *)stInfo.u4DesAddrR);
        
        HAL_GetTime(&TimeNow);
        HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
        LOG(4, "LumaSum time  %d.%06d sec.\n", TimeDelta.u4Seconds, TimeDelta.u4Micros);       
    }
}

void vDrvTTDSet3DDepthCtrl(UINT8 u1OnOff)
{    
#if defined(CC_MT5396)    
    MJC_WRITE_FLD(MJC_TTD_REG_00, u1OnOff, TTD_LSV_EN);
#else 
    vIO32WriteFldAlign(SW_TTD_REG_00, u1OnOff, SW_LSV_EN);
#endif   

    LOG(3, "Set LSV enable  %d\n", u1OnOff);       
}

UINT8 u1DrvTTDGet3DDepthCtrl(void)
{   
#if defined(CC_MT5396)    
    return MJC_READ_FLD(MJC_TTD_REG_00, TTD_LSV_EN);
#else
    return IO32ReadFldAlign(SW_TTD_REG_00, SW_LSV_EN); 
#endif  
}

