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
 * $RCSfile: drv_mjc_int.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/

#include "x_assert.h"
#include "hw_tdnavi.h"

#include "drv_tdtv_feature.h"
#include "drv_tdnavi.h"

#include "vdo_misc.h"
#include "sv_const.h"
#include "vdp_if.h"

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
#include "x_os.h"
#include "drv_di.h"

EXTERN void HalFlushInvalidateDCache(void);

//#define SW_NAVI_DEBUG
#define NAVI_REDUCED_CALC (1)

#ifndef abs
#define abs(x)                  (((x) >= 0) ? (x) : -(x))
#endif

#define NAVI_H_GRID (5)
#define NAVI_V_GRID (5)
#define NAVI_STEP (23)

#define NAVI_DELAY_FRAME (8) // This parameter should be power of 2

#define NAVI_STEP_H (NAVI_STEP%NAVI_H_GRID)
#define NAVI_STEP_V (NAVI_STEP/NAVI_V_GRID)

#define NAVI_CNT_STEP_UP 1
#define NAVI_CNT_STEP_DN 4

UINT32 *pu4NaviBuf=NULL;
UINT32 *pu4NaviLBuf=NULL; 
UINT32 *pu4NaviRBuf=NULL;  


#define NAVI_PXL(x,y) (*(UINT8 *)((UINT8*)(((UINT8 *)pu4NaviBuf) +(((INT32)(y))*FW_NAVI_WINDOW_WIDTH) + ((INT32)x))))
#define NAVI_PXL32(x,y) (*(UINT32 *)((UINT32*)(((UINT8 *)pu4NaviBuf) +(((INT32)(y))*FW_NAVI_WINDOW_WIDTH) + ((INT32)x))))

typedef enum
{
    NAVI_WAIT,
    NAVI_COPY,
    NAVI_CHK,
}NAVI_STATE;

static TD_NAVI_DETECT checkerboard_stg_d1 = TD_NAVI_DETECT_2D;
static TD_NAVI_DETECT SwNaviDetectd = TD_NAVI_DETECT_2D;

#if SUPPORT_SW_NAVI

static UINT8 cb_score_buffer[8];
static UINT32 cb_complex_buffer[8];
static UINT8 u1DetectedCnt;

static UINT32 u4NaviCBDiff;
static UINT32 u4NaviHvDiff;

static UINT8 cbBufIndex;

static volatile NAVI_STATE u1NaviCheckState = NAVI_WAIT;

UINT32 u4NaviWidth;
UINT32 u4NaviHeight;
UINT32 u4NaviStartX;
UINT32 u4NaviStartY;

//Detection Paramater
UINT32 c_cb_edge_th;
UINT32 c_cb_cori_th;
UINT32 c_cb_complex_th;

UINT32 c_cb_score_hi;
UINT32 c_cb_score_lo;

UINT32 c_cb_cnt_in;
UINT32 c_cb_cnt_out;
UINT32 c_cb_cnt_max;

#endif

void vDrvTdNaviFWDetectionInit(void)
{
#if SUPPORT_SW_NAVI

    UINT32 i;

    if(NULL==pu4NaviBuf)
    {    
        pu4NaviBuf = (UINT32 *)x_mem_alloc(FW_NAVI_WINDOW_HEIGHT*FW_NAVI_WINDOW_WIDTH);

        vIO32WriteFldAlign(TDNAVI_FW_00, 0x5   , NAVI_EDGE_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_00, 0x2   , NAVI_DIFF_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_00,0x200  , NAVI_CMPLX_TH);  

        vIO32WriteFldAlign(TDNAVI_FW_01, 0x60  , NAVI_RATIO_LO);  
        vIO32WriteFldAlign(TDNAVI_FW_01, 0xA0  , NAVI_RATIO_HI); 

        vIO32WriteFldAlign(TDNAVI_FW_02, 0x10  , NAVI_CNT_IN);  
        vIO32WriteFldAlign(TDNAVI_FW_02, 0x8   , NAVI_CNT_OUT);            
        vIO32WriteFldAlign(TDNAVI_FW_02, 0x20  , NAVI_CNT_MAX);                    
    }

    if(NULL==pu4NaviLBuf)
    {   
        pu4NaviLBuf = (UINT32 *)x_mem_alloc(FW_NAVI_WINDOW_HEIGHT*FW_NAVI_WINDOW_WIDTH/2);
    }

    if(NULL==pu4NaviRBuf)
    {   
        pu4NaviRBuf = (UINT32 *)x_mem_alloc(FW_NAVI_WINDOW_HEIGHT*FW_NAVI_WINDOW_WIDTH/2);
    }
       
    for(i=0;i<8;i++) 
    {
        cb_score_buffer[i] = 0;
        cb_complex_buffer[i] = 0;
    }

    SwNaviDetectd = TD_NAVI_DETECT_2D;    
#endif    
}

TD_NAVI_DETECT vDrvTdNaviFWDetectionCBScore(void)
{
#if SUPPORT_SW_NAVI
    UINT32 h,w,i;
#if 0
#ifdef SW_NAVI_DEBUG
HAL_TIME_T TimeStart, TimeNow, TimeDelta;
#endif
#endif

#if NAVI_REDUCED_CALC 
    UINT32 data1,data2;
    UINT32 u4Tmp1,u4Tmp2;
#else

    //pixal index
    //     1 2 3     delay 1 line
    //     6 7 8     current line

	INT32 pix_1, pix_2, pix_3;//Should be signed for diff calculation
    INT32 pix_6, pix_7, pix_8;//Should be signed for diff calculation
#endif

	UINT32 abs_hor_edge_diff1, abs_hor_edge_diff2, abs_hor_edge_diff_min;
	UINT32 abs_checkb_diff, abs_ver_diff, abs_hor_diff1, abs_hor_diff2, abs_hor_diff;
	UINT32 hv_diff_min;

    UINT32 checkb_diff_sum=0;
	UINT32 hv_diff_sum=0;

	UINT32 cb_complex;
	UINT32 cb_complex_out;
    
    //Score
	UINT32 cb_score;
	UINT32 cb_score_out;

#if 0
#ifdef SW_NAVI_DEBUG
    HAL_GetTime(&TimeStart);                                
#endif
#endif

    if(NULL==pu4NaviBuf)
    {
        return TD_NAVI_DETECT_2D;        
    }
    //detection window
#if NAVI_REDUCED_CALC    
    for(h=0;h<FW_NAVI_WINDOW_HEIGHT;h+=2)
    {
        for(w=0;w<FW_NAVI_WINDOW_WIDTH;w+=4)
        {            
            data1       = NAVI_PXL32(w,h);
            data2       = NAVI_PXL32(w,h+1);
                
            //Calculate CB Diff
            u4Tmp1 = ((data1 >>8) & 0xFF00FF)+ (data2 & 0xFF00FF);
            u4Tmp1 = (u4Tmp1>>16) + (u4Tmp1 & 0xFFFF);

            u4Tmp2 = ((data1) & 0xFF00FF)+ ((data2>>8) & 0xFF00FF);
            u4Tmp2 = (u4Tmp2>>16) + (u4Tmp2 & 0xFFFF);

            abs_checkb_diff = (abs((INT32)u4Tmp1-(INT32)u4Tmp2));

            //Vertical edge detection
            u4Tmp1 = ((data1>>8) & 0xFF00FF) + ((data1) & 0xFF00FF);
            u4Tmp1 = (u4Tmp1>>16) + (u4Tmp1 & 0xFFFF);
            
            u4Tmp2 = ((data2>>8) & 0xFF00FF) + ((data2) & 0xFF00FF);
            u4Tmp2 = (u4Tmp2>>16) + (u4Tmp2 & 0xFFFF);

            abs_ver_diff = (abs((INT32)u4Tmp1-(INT32)u4Tmp2));
            
            //horizontal edge detection for Checkerboard
            u4Tmp1 = ((data1 >>8) & 0xFF00FF)+ ((data2 >>8) & 0xFF00FF);
            u4Tmp2 = ((data1) & 0xFF00FF)+ ((data2) & 0xFF00FF);            

            abs_hor_diff1 = (abs((INT32)(u4Tmp1&0xFFFF) - (INT32)((u4Tmp1>>16)&0xFFFF))
                + abs((INT32)(u4Tmp2&0xFFFF) - (INT32)((u4Tmp2>>16)&0xFFFF)));

            abs_hor_diff2 = (abs((INT32)(u4Tmp1&0xFFFF) - (INT32)((u4Tmp2)&0xFFFF))
                + abs((INT32)((u4Tmp1>>16)&0xFFFF) - (INT32)((u4Tmp2>>16)&0xFFFF)));

            abs_hor_diff    = abs_hor_diff1 > abs_hor_diff2 ? abs_hor_diff1 : abs_hor_diff2;
            hv_diff_min     = abs_ver_diff < abs_hor_diff ? abs_ver_diff : abs_hor_diff;
            
            //Edge Detection
            abs_hor_edge_diff1      = (abs(((INT32)(data1&0xFF))-((INT32)((data1>>16)&0xFF)))
                + abs(((INT32)((data1>>8)&0xFF))-((INT32)((data1>>24)&0xFF))))/2;

            abs_hor_edge_diff2      = (abs(((INT32)(data2&0xFF))-((INT32)((data2>>16)&0xFF)))
                + abs(((INT32)((data2>>8)&0xFF))-((INT32)((data2>>24)&0xFF))))/2;
            
            abs_hor_edge_diff_min   = abs_hor_edge_diff1 < abs_hor_edge_diff2 ? abs_hor_edge_diff1 : abs_hor_edge_diff2;

            //summation
            if((abs_hor_edge_diff_min < c_cb_edge_th )&& (hv_diff_min >= c_cb_cori_th || abs_checkb_diff >= c_cb_cori_th)) 
            {
                checkb_diff_sum  += abs_checkb_diff>>1; //Prevent Overflow
                hv_diff_sum      += hv_diff_min>>1;     //Prevent Overflow
            }
        }
    }
#else
    for(h=1;h<FW_NAVI_WINDOW_HEIGHT;h++)
    {
        for(w=1;w<FW_NAVI_WINDOW_WIDTH-1;w++)
        {
            pix_6       = NAVI_PXL(w-1,h);
            pix_7       = NAVI_PXL(w  ,h);
            pix_8       = NAVI_PXL(w+1,h);
            
            pix_1       = NAVI_PXL(w-1,h-1);
            pix_2       = NAVI_PXL(w  ,h-1);
            pix_3       = NAVI_PXL(w+1,h-1);

            //horizontal edge detection for Checkerboard
            abs_hor_edge_diff1      = abs(pix_6 - pix_8);
            abs_hor_edge_diff2      = abs(pix_1 - pix_3);
            abs_hor_edge_diff_min   = abs_hor_edge_diff1 < abs_hor_edge_diff2 ? abs_hor_edge_diff1 : abs_hor_edge_diff2;

            //checkerboard diff
            abs_checkb_diff = abs(((pix_1 + pix_7*2 + pix_3)-(pix_6+ pix_2*2 + pix_8)+2)/4);
            abs_ver_diff    = abs(((pix_1 + pix_2*2 + pix_3)-(pix_6+ pix_7*2 + pix_8)+2)/4);
            abs_hor_diff1   = abs(((pix_1 + pix_6)-(pix_3 + pix_8)+1)/2);
            abs_hor_diff2   = (abs(((pix_2 + pix_7)-(pix_3 + pix_8)+1)/2)+abs(((pix_2 + pix_7)-(pix_1 + pix_6)+1)/2)+1)/2;
            abs_hor_diff    = abs_hor_diff1 > abs_hor_diff2 ? abs_hor_diff1 : abs_hor_diff2;

            hv_diff_min     = abs_ver_diff < abs_hor_diff ? abs_ver_diff : abs_hor_diff;

            //summation
            if((abs_hor_edge_diff_min < c_cb_edge_th )&& (hv_diff_min >= c_cb_cori_th || abs_checkb_diff >= c_cb_cori_th)) {
                checkb_diff_sum  = checkb_diff_sum + abs_checkb_diff;
                hv_diff_sum      = hv_diff_sum     + hv_diff_min;
            }
        }
    }
#endif    

#if 0
#ifdef SW_NAVI_DEBUG
    HAL_GetTime(&TimeNow);
    HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
    LOG(5, "[SW NAVI]STAGE 1 Score Cal time  %d.%06d sec.\n", TimeDelta.u4Seconds, TimeDelta.u4Micros);
#endif
#endif

    u4NaviCBDiff = checkb_diff_sum;
    u4NaviHvDiff = hv_diff_sum;

    //Calculate Current Score
    cb_complex = (checkb_diff_sum + hv_diff_sum);
    cb_score = (0 == cb_complex )? 0x80 : (checkb_diff_sum * 0xFF) / cb_complex;
    
    //Score IIR
    cb_score_out = cb_complex > c_cb_complex_th ? cb_score : 0x80;
    cb_complex_out = cb_complex > c_cb_complex_th ? cb_complex : 0;
    
    for(i=0;i<NAVI_DELAY_FRAME;i++) {
        if((cb_complex_out < cb_complex_buffer[i] && cb_score_out < c_cb_score_hi) || 
           (cb_score_buffer[i] > c_cb_score_hi && cb_score_out < cb_score_buffer[i])) {
            cb_score_out = cb_score_buffer[i];
            cb_complex_out = cb_complex_buffer[i];
        }
    }

    //Update IIR
    cb_score_buffer[cbBufIndex] = cb_complex > c_cb_complex_th ? cb_score : 0x80;
    cb_complex_buffer[cbBufIndex] = cb_complex > c_cb_complex_th ? cb_complex : 0;
    cbBufIndex = (cbBufIndex+1)&(NAVI_DELAY_FRAME-1);

	//Make Detection Decision
	if(cb_score_out > c_cb_score_hi || ((checkerboard_stg_d1==TD_NAVI_DETECT_CB)&&(cb_score_out>=c_cb_score_lo)))
	{	
        checkerboard_stg_d1 = TD_NAVI_DETECT_CB;
        return checkerboard_stg_d1;
	}
	else if(cb_score_out < c_cb_score_lo || ((checkerboard_stg_d1==TD_NAVI_DETECT_2D)&&(cb_score_out<=c_cb_score_hi)))
	{
        checkerboard_stg_d1 = TD_NAVI_DETECT_2D;
        return checkerboard_stg_d1;        
	}
	else
	{
        checkerboard_stg_d1 = TD_NAVI_DETECT_UNKNOWN;
        return checkerboard_stg_d1;        
	}
#else
    return checkerboard_stg_d1;
#endif
}

TD_NAVI_DETECT eDrvSwNaviDetectedFmtQuery(void)
{
    return SwNaviDetectd;
}

#if SUPPORT_SW_NAVI
static inline UINT32 u4DrvTdNaviRamapCBDram(UINT32 u4Width,UINT32 u4Height,UINT32 *pu4Target,UINT32 *pLData,UINT32 *pRData)
{
    UINT8 *pu1Tar=(UINT8 *)pu4Target;
    UINT8 *pu1L  =(UINT8 *)pLData;
    UINT8 *pu1R  =(UINT8 *)pRData;
    
    UINT32 i,j;
    if((NULL==pu4Target)
        ||(NULL==pLData)
        ||(NULL==pRData))
    {
        return SV_FAIL;
    }

    for(i=0;i<u4Height;i+=2)
    {
        for(j=0;j<u4Width;j+=2)
        {
            *pu1Tar++ = *pu1L++;
            *pu1Tar++ = *pu1R++;                
        }        
        for(j=0;j<u4Width;j+=2)
        {
            *pu1Tar++ = *pu1R++;                            
            *pu1Tar++ = *pu1L++;
        }       
    }
    return SV_SUCCESS;    
}

static inline UINT32 u4DrvTdNaviDramDumpProc(void)
{
#if SUPPORT_SW_NAVI
    UINT32 u4DQC=u4DrvTDTVDQCModeQuery();
    UINT32 u4RemapX;
    if(NULL==pu4NaviBuf)
    {
        return SV_FAIL;
    }
    else
    {          
        if(E_TD_OUT_3D_SBS==TD_DQC_OUT(u4DQC))
        {
            switch(TD_DQC_IN(u4DQC))
            {
                case E_TD_IN_CB_P:
                    u4RemapX = u4NaviStartX/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u4NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);
                
                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u4NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviLBuf);

                    u4RemapX = u4NaviStartX/2 + u2DrvDIGetVdpWidth(SV_VP_MAIN)/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u4NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);
                
                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u4NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviRBuf);
                    return u4DrvTdNaviRamapCBDram(FW_NAVI_WINDOW_WIDTH,FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviBuf,pu4NaviLBuf,pu4NaviRBuf);          
                default:
                    break;                                
            }
        }
        else
        {            
            vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                u4NaviStartX, u4NaviStartY, FW_NAVI_WINDOW_WIDTH, FW_NAVI_WINDOW_HEIGHT);
            
            vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                u4NaviStartX, u4NaviStartY, FW_NAVI_WINDOW_WIDTH, FW_NAVI_WINDOW_HEIGHT,
                pu4NaviBuf);
            
            return SV_SUCCESS;          
        }            
    }

    return SV_FAIL;                
#endif   
}

static inline void vDrvTdNaviAnalyzeProc(void)
{
    UINT32 u4H,u4V;
    static UINT8 u1CurX=0,u1CurY=0;

    //Update FW Parameter
    c_cb_edge_th=IO32ReadFldAlign(TDNAVI_FW_00, NAVI_EDGE_TH);  
    c_cb_cori_th=IO32ReadFldAlign(TDNAVI_FW_00, NAVI_DIFF_TH);  
    c_cb_complex_th=IO32ReadFldAlign(TDNAVI_FW_00, NAVI_CMPLX_TH);  
    
    c_cb_score_lo=IO32ReadFldAlign(TDNAVI_FW_01, NAVI_RATIO_LO);  
    c_cb_score_hi=IO32ReadFldAlign(TDNAVI_FW_01, NAVI_RATIO_HI);            
    
    c_cb_cnt_in=IO32ReadFldAlign(TDNAVI_FW_02, NAVI_CNT_IN);  
    c_cb_cnt_out=IO32ReadFldAlign(TDNAVI_FW_02, NAVI_CNT_OUT);            
    c_cb_cnt_max=IO32ReadFldAlign(TDNAVI_FW_02, NAVI_CNT_MAX);  
    
    
    //Calculate Window Position   
    u4NaviWidth=u2DrvDIGetVdpWidth(SV_VP_MAIN);
    u4NaviHeight=u2DrvDIGetVdpHeight(SV_VP_MAIN);
    
    u4H = (u4NaviWidth-FW_NAVI_WINDOW_WIDTH)/(NAVI_H_GRID-1);
    u4V = (u4NaviHeight-FW_NAVI_WINDOW_HEIGHT)/(NAVI_V_GRID-1);
        
    u4NaviStartX = (u1CurX * u4H)&(~0x3);//Align 4
    u4NaviStartY = u1CurY * u4V;
    
    u1CurX = (u1CurX + NAVI_STEP_H)%NAVI_H_GRID;
    u1CurY = (u1CurY + NAVI_STEP_V)%NAVI_V_GRID;
            
    //Detect 3D Format
    vDrvTdNaviFWDetectionCBScore();
    
    //Stablize Fmt Decision
    if(TD_NAVI_DETECT_CB==checkerboard_stg_d1)
    {
        u1DetectedCnt = MIN(c_cb_cnt_max,u1DetectedCnt + NAVI_CNT_STEP_UP);
    }
    else if(TD_NAVI_DETECT_2D==checkerboard_stg_d1)
    {
        u1DetectedCnt = (u1DetectedCnt > u1DetectedCnt - NAVI_CNT_STEP_DN) 
            ? 0: u1DetectedCnt - NAVI_CNT_STEP_DN;
    }
    
    if(u1DetectedCnt > c_cb_cnt_in)
    {
        SwNaviDetectd = TD_NAVI_DETECT_CB;
    }
    else if(u1DetectedCnt < c_cb_cnt_out)
    {
        SwNaviDetectd = TD_NAVI_DETECT_2D;
    }      
}
#endif

void vDrvTdNaviFWDetectionProc(void)
{
#if SUPPORT_SW_NAVI
    
    #ifdef SW_NAVI_DEBUG
    HAL_TIME_T TimeStart, TimeNow, TimeDelta;
    #endif
    
    //Temporally Put Here to Check Checker board Detection
    if((bDrvVideoSignalStatus(SV_VP_MAIN)!=SV_VDO_STABLE)
       ||(u4DrvTDTVGetTag3D(SV_VP_MAIN)!=TDTV_TAG3D_2D)
       ||(bDrvVideoIsSrcInterlace(SV_VP_MAIN))
       ||(!fgDrvDIIsMIBDramAvaliable(SV_VP_MAIN))
       ||(eDrvTDTVTDNavimModeQuery()==E_TDTV_UI_NAVI_MANAUL)
        )
    {
        u1NaviCheckState=NAVI_WAIT;
    }
    else if(NAVI_WAIT==u1NaviCheckState)
    {            
        vDrvTdNaviFWDetectionInit();                       
        u1NaviCheckState=NAVI_COPY;
    }

    if(NAVI_COPY==u1NaviCheckState)
    {
        #ifdef SW_NAVI_DEBUG
        HAL_GetTime(&TimeStart);                                        
        #endif
                      
        if(SV_SUCCESS==u4DrvTdNaviDramDumpProc())
        {
            u1NaviCheckState = NAVI_CHK;            
        }
        
        #ifdef SW_NAVI_DEBUG
        HAL_GetTime(&TimeNow);
        HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
        LOG(5, "[SW NAVI]Dump time  %d.%06d sec.\n", TimeDelta.u4Seconds, TimeDelta.u4Micros);
        #endif        
    }    
    else if(NAVI_CHK==u1NaviCheckState)
    {    
        #ifdef SW_NAVI_DEBUG
        HAL_GetTime(&TimeStart);                                
        #endif

        vDrvTdNaviAnalyzeProc();
        u1NaviCheckState = NAVI_COPY;                    

        #ifdef SW_NAVI_DEBUG
        HAL_GetTime(&TimeNow);
        HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
        LOG(5, "[SW NAVI]Score Cal time  %d.%06d sec.\n", TimeDelta.u4Seconds, TimeDelta.u4Micros);
        #endif        
    }

    vIO32WriteFldAlign(TDNAVI_STATUS_00, SwNaviDetectd      , NAVI_DETECTED);
    vIO32WriteFldAlign(TDNAVI_STATUS_01, u4NaviHvDiff       , NAVI_HVDIFF); 
    vIO32WriteFldAlign(TDNAVI_STATUS_02, u4NaviCBDiff       , NAVI_CBDIFF);           
#endif   
}



