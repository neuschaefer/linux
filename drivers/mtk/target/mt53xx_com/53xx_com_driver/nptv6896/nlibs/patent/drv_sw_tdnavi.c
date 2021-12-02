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

#define SW_NAVI_DEBUG

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

static UINT8 u1DetectedStaIn;
static UINT8 u1DetectedStaOut;        
static UINT8 u1DetectedStaMax;
static UINT8 u1DetectedCnt;



static volatile NAVI_STATE u1NaviCheckState = NAVI_WAIT;

static UINT16 u2NaviWidth;
static UINT16 u2NaviHeight;
static UINT16 u2NaviStartX;
static UINT16 u2NaviStartY;

//Detection Paramater

static UINT16 sta_li_cnt;
static UINT16 sta_li_fcr; 
static UINT16 sta_ci_cnt; 
static UINT16 sta_ci_fcr; 
static UINT16 sta_cb_cnt; 
static UINT16 sta_cb_fcr; 

static UINT16 li_cnt_buffer[NAVI_DELAY_FRAME];
static UINT16 li_fcr_buffer[NAVI_DELAY_FRAME];
static UINT16 ci_cnt_buffer[NAVI_DELAY_FRAME];
static UINT16 ci_fcr_buffer[NAVI_DELAY_FRAME];
static UINT16 cb_cnt_buffer[NAVI_DELAY_FRAME];
static UINT16 cb_fcr_buffer[NAVI_DELAY_FRAME];

static UINT8 u1BufIndex;


static UINT16 c_li_fcr_hi;
static UINT16 c_li_fcr_lo;
static UINT16 c_ci_fcr_hi;
static UINT16 c_ci_fcr_lo;
static UINT16 c_cb_fcr_hi;
static UINT16 c_cb_fcr_lo;

static UINT16 c_li_cnt_thr;
static UINT16 c_ci_cnt_thr;
static UINT16 c_cb_cnt_thr;

static UINT16 cb_th;
static UINT16 li_th;
static UINT16 ci_th;

static UINT16 c_cb_cori_th;
static UINT16 c_li_cori_th;
static UINT16 c_ci_cori_th;

#endif

void vDrvTdNaviFWDetectionInit(void)
{
#if SUPPORT_SW_NAVI

    UINT32 i;

    if(NULL==pu4NaviBuf)
    {    
        pu4NaviBuf = (UINT32 *)x_mem_alloc(FW_NAVI_WINDOW_HEIGHT*FW_NAVI_WINDOW_WIDTH);

        vIO32WriteFldAlign(TDNAVI_FW_00, 0x10  , NAVI_CNT_IN);  
        vIO32WriteFldAlign(TDNAVI_FW_00, 0x8   , NAVI_CNT_OUT);            
        vIO32WriteFldAlign(TDNAVI_FW_00, 0x20  , NAVI_CNT_MAX);  

        vIO32WriteFldAlign(TDNAVI_FW_01, 0xB8  , LI_FCT_HI_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_01, 0x68  , LI_FCT_LO_TH);            
        vIO32WriteFldAlign(TDNAVI_FW_01, 0x20  , LI_CNT_TH);         

        vIO32WriteFldAlign(TDNAVI_FW_02, 0x10  , LI_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_02, 0x10  , LI_COR_TH);            

        vIO32WriteFldAlign(TDNAVI_FW_03, 0x90  , CI_FCT_HI_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_03, 0x50  , CI_FCT_LO_TH);            
        vIO32WriteFldAlign(TDNAVI_FW_03, 0x20  , CI_CNT_TH);         

        vIO32WriteFldAlign(TDNAVI_FW_04, 0x10  , CI_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_04, 0x10  , CI_COR_TH); 

        vIO32WriteFldAlign(TDNAVI_FW_05, 0xB8  , CB_FCT_HI_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_05, 0x68  , CB_FCT_LO_TH);            
        vIO32WriteFldAlign(TDNAVI_FW_05, 0x20  , CB_CNT_TH);         

        vIO32WriteFldAlign(TDNAVI_FW_06, 0x10  , CB_TH);  
        vIO32WriteFldAlign(TDNAVI_FW_06, 0x10  , CB_COR_TH);                  
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
        li_cnt_buffer[i]=0;
        li_fcr_buffer[i]=0;
        ci_cnt_buffer[i]=0;
        ci_fcr_buffer[i]=0;
        cb_cnt_buffer[i]=0;
        cb_fcr_buffer[i]=0;
    }

    SwNaviDetectd = TD_NAVI_DETECT_2D;    
#endif    
}

TD_NAVI_DETECT vDrvTdNaviFWDetectionScore(void)
{
#if SUPPORT_SW_NAVI
    UINT32 h,w,i;
#if 0
#ifdef SW_NAVI_DEBUG
HAL_TIME_T TimeStart, TimeNow, TimeDelta;
#endif
#endif

    UINT32 data1,data2,data3,data4;
    UINT32 u4Tmp1,u4Tmp2,u4Tmp3,u4Tmp4;

    UINT32 abs_cb_diff; //Checker Board Diff ,Large means CB.
    
    UINT32 abs_ver_diff,abs_ver_diff1,abs_ver_diff2; //Vertical Diff
    UINT32 abs_hor_diff,abs_hor_diff1, abs_hor_diff2;//Horizontal Diff

    UINT32 hv_diff_min;    
    UINT32 hv_diff_max;

    UINT32 is_cb_cnt=0;
    UINT32 no_cb_cnt=0;

    UINT32 is_li_cnt=0;
    UINT32 no_li_cnt=0;

    UINT32 is_ci_cnt=0;
    UINT32 no_ci_cnt=0;    

    UINT32 li_fcr_max, ci_fcr_max, cb_fcr_max;
	UINT32 li_cnt_max, ci_cnt_max, cb_cnt_max;

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
    for(h=0;h<FW_NAVI_WINDOW_HEIGHT;h+=4)
    {
        for(w=0;w<FW_NAVI_WINDOW_WIDTH;w+=4)
        {            
            data1       = NAVI_PXL32(w,h);
            data2       = NAVI_PXL32(w,h+1);
            data3       = NAVI_PXL32(w,h+2);
            data4       = NAVI_PXL32(w,h+3);
            
            //Calculate CB Diff
            u4Tmp1 = ((data1 >>8) & 0xFF00FF)+ (data2 & 0xFF00FF) 
                   + ((data3 >>8) & 0xFF00FF) +(data4 & 0xFF00FF);
            u4Tmp1 = (u4Tmp1>>16) + (u4Tmp1 & 0xFFFF);

            u4Tmp2 = ((data1) & 0xFF00FF)+ ((data2>>8) & 0xFF00FF) 
                   + ((data3) & 0xFF00FF)+ ((data4>>8) & 0xFF00FF);            
            u4Tmp2 = (u4Tmp2>>16) + (u4Tmp2 & 0xFFFF);

            abs_cb_diff = (abs((INT32)u4Tmp1-(INT32)u4Tmp2))/2;

            //Vertical edge detection
            u4Tmp1 = ((data1>>8) & 0xFF00FF) + ((data1) & 0xFF00FF);
            u4Tmp1 = (u4Tmp1>>16) + (u4Tmp1 & 0xFFFF);
            
            u4Tmp2 = ((data2>>8) & 0xFF00FF) + ((data2) & 0xFF00FF);
            u4Tmp2 = (u4Tmp2>>16) + (u4Tmp2 & 0xFFFF);

            u4Tmp3 = ((data3>>8) & 0xFF00FF) + ((data3) & 0xFF00FF);
            u4Tmp3 = (u4Tmp3>>16) + (u4Tmp3 & 0xFFFF);
            
            u4Tmp4 = ((data4>>8) & 0xFF00FF) + ((data4) & 0xFF00FF);
            u4Tmp4 = (u4Tmp4>>16) + (u4Tmp4 & 0xFFFF);

            //[1,-1]
            abs_ver_diff1 =((abs((INT32)u4Tmp1-(INT32)u4Tmp2))
                          +2*(abs((INT32)u4Tmp2-(INT32)u4Tmp3))
                          +(abs((INT32)u4Tmp3-(INT32)u4Tmp4)))/4;
            //[1,0,-1]
            abs_ver_diff2 =((abs((INT32)u4Tmp1-(INT32)u4Tmp3))
                          +(abs((INT32)u4Tmp2-(INT32)u4Tmp4)))/2;

            
            abs_ver_diff = abs_ver_diff1 > abs_ver_diff2 ? abs_ver_diff1 : abs_ver_diff2;
                       
            //horizontal edge detection for Checkerboard
            u4Tmp1 = ((data1 >>8) & 0xFF00FF)+ ((data2 >>8) & 0xFF00FF) 
                   + ((data3 >>8) & 0xFF00FF) +((data4 >>8) & 0xFF00FF);
            
            u4Tmp2 = ((data1) & 0xFF00FF) +((data2) & 0xFF00FF) 
                   + ((data3) & 0xFF00FF) +((data4) & 0xFF00FF);            

            //[1,-1]
            abs_hor_diff1 = ((abs((INT32)(u4Tmp1&0xFFFF) - (INT32)((u4Tmp2)&0xFFFF)))            
                          + 2*(abs((INT32)((u4Tmp2>>16)&0xFFFF) - (INT32)((u4Tmp1)&0xFFFF)))
                          + (abs((INT32)((u4Tmp1>>16)&0xFFFF) - (INT32)((u4Tmp2>>16)&0xFFFF))))/4;
            //[1,0,-1]
            abs_hor_diff2 = ((abs((INT32)(u4Tmp1&0xFFFF) - (INT32)((u4Tmp1>>16)&0xFFFF))
                          + (abs((INT32)(u4Tmp2&0xFFFF) - (INT32)((u4Tmp2>>16)&0xFFFF)))))/2;

            abs_hor_diff    = abs_hor_diff1 > abs_hor_diff2 ? abs_hor_diff1 : abs_hor_diff2;

            // HV Diff
            hv_diff_min   = abs_ver_diff < abs_hor_diff ? abs_ver_diff : abs_hor_diff;
            hv_diff_max   = abs_ver_diff < abs_hor_diff ? abs_hor_diff : abs_ver_diff;
            
            //CB Check
            if((hv_diff_min >= c_cb_cori_th) || (abs_cb_diff >= c_cb_cori_th))
            {
                if(abs_cb_diff + cb_th < hv_diff_min)
                {
                    no_cb_cnt++;
                }
                else if(abs_cb_diff > hv_diff_max + cb_th)
                {
                    is_cb_cnt++;
                }
            }

            //LI Check
            if((abs_ver_diff1 >= c_li_cori_th) || (abs_ver_diff2 >= c_li_cori_th))
            {
                if(abs_ver_diff1 < abs_ver_diff2)
                {
                    no_li_cnt++;
                }  
                else if((abs_ver_diff2< (abs_ver_diff1* li_th)>>6)
                    &&!(abs_cb_diff > hv_diff_max + cb_th)//Checker Board also have saw property
                    )
                {
                    is_li_cnt++;
                }              
            }
            
            //CI Check
            if((abs_hor_diff1 >= c_ci_cori_th) || (abs_hor_diff2 >= c_ci_cori_th))
            {
                if(abs_hor_diff1 < abs_hor_diff2)
                {
                    no_ci_cnt++;
                }  
                else if((abs_hor_diff2 < (abs_hor_diff1* ci_th)>>6)
                    //&&!(abs_cb_diff > hv_diff_max + cb_th)//Checker Board also have saw property
                    )
                {
                    is_ci_cnt++;
                }              
            }            
        }
    }

#if 0
#ifdef SW_NAVI_DEBUG
    HAL_GetTime(&TimeNow);
    HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
    LOG(5, "[SW NAVI]STAGE 1 Score Cal time  %d.%06d sec.\n", TimeDelta.u4Seconds, TimeDelta.u4Micros);
#endif
#endif

    //CB Cnt
    sta_cb_cnt = is_cb_cnt + no_cb_cnt;
    cb_cnt_max = sta_cb_cnt > c_cb_cnt_thr ? sta_cb_cnt : 0;
    cb_fcr_max = cb_cnt_max!=0 ? is_cb_cnt * 0x100 /sta_cb_cnt : c_cb_fcr_lo;
    sta_cb_fcr = cb_fcr_max;

    //LI Cnt
    sta_li_cnt = is_li_cnt + no_li_cnt;
    li_cnt_max = sta_li_cnt > c_li_cnt_thr ? sta_li_cnt : 0;
    li_fcr_max = li_cnt_max !=0 ? is_li_cnt * 0x100 /sta_li_cnt : c_li_fcr_lo;    
    sta_li_fcr = li_fcr_max;

    //CI Cnt
    sta_ci_cnt = is_ci_cnt + no_ci_cnt;
    ci_cnt_max = sta_ci_cnt > c_ci_cnt_thr ? sta_ci_cnt : 0;
    ci_fcr_max = ci_cnt_max !=0  ? is_ci_cnt * 0x100 /sta_ci_cnt : c_ci_fcr_lo;    
    sta_ci_fcr = ci_fcr_max;
    
    li_cnt_buffer[u1BufIndex]=li_cnt_max;
    li_fcr_buffer[u1BufIndex]=li_fcr_max;
    ci_cnt_buffer[u1BufIndex]=ci_cnt_max;
    ci_fcr_buffer[u1BufIndex]=ci_fcr_max;
    cb_cnt_buffer[u1BufIndex]=cb_cnt_max;
    cb_fcr_buffer[u1BufIndex]=cb_fcr_max;
    
    u1BufIndex = (u1BufIndex+1)&(NAVI_DELAY_FRAME-1);
    
    for(i=0;i<NAVI_DELAY_FRAME;i++) 
    {
        if((li_cnt_max < li_cnt_buffer[i] && li_fcr_max < c_li_fcr_hi) 
            || (li_fcr_buffer[i] >= c_li_fcr_hi && li_fcr_max < li_fcr_buffer[i])) 
        {
            li_fcr_max = li_fcr_buffer[i];
            li_cnt_max = li_cnt_buffer[i];
        }
        
        if((ci_cnt_max < ci_cnt_buffer[i] && ci_fcr_max < c_ci_fcr_hi) 
            || (ci_fcr_buffer[i] >= c_ci_fcr_hi && ci_fcr_max < ci_fcr_buffer[i])) 
        {
            ci_fcr_max = ci_fcr_buffer[i];
            ci_cnt_max = ci_cnt_buffer[i];
        }
        
        if((cb_cnt_max < cb_cnt_buffer[i] && cb_fcr_max < c_cb_fcr_hi) 
            || (cb_fcr_buffer[i] >= c_cb_fcr_hi && cb_fcr_max < cb_fcr_buffer[i])) 
        {
            cb_fcr_max = cb_fcr_buffer[i];
            cb_cnt_max = cb_cnt_buffer[i];
        }
    }

    if(((li_fcr_max+c_ci_fcr_hi >=ci_fcr_max+c_li_fcr_hi)
            &&(li_fcr_max+c_cb_fcr_hi >=cb_fcr_max+c_li_fcr_hi)
            &&(li_fcr_max >= c_li_fcr_hi)
            &&(checkerboard_stg_d1==TD_NAVI_DETECT_2D))
        ||((checkerboard_stg_d1==TD_NAVI_DETECT_LI)&&(li_fcr_max>=c_li_fcr_lo)))   
    {
        checkerboard_stg_d1 = TD_NAVI_DETECT_LI;
        return checkerboard_stg_d1;
    }
    else if(((ci_fcr_max+c_li_fcr_hi>=li_fcr_max+c_ci_fcr_hi)
                &&(ci_fcr_max+c_cb_fcr_hi >=cb_fcr_max+c_ci_fcr_hi)
                &&(ci_fcr_max >= c_ci_fcr_hi)
                &&(checkerboard_stg_d1==TD_NAVI_DETECT_2D))
            ||((checkerboard_stg_d1==TD_NAVI_DETECT_CI)&&(ci_fcr_max>=c_ci_fcr_lo)))
    {
        checkerboard_stg_d1 = TD_NAVI_DETECT_CI;
        return checkerboard_stg_d1;
    }
    else if(((cb_fcr_max + c_li_fcr_hi >=li_fcr_max + c_cb_fcr_hi)
                &&(cb_fcr_max + c_ci_fcr_hi >=ci_fcr_max + c_cb_fcr_hi)
                &&(cb_fcr_max >= c_cb_fcr_hi)
                &&(checkerboard_stg_d1==TD_NAVI_DETECT_2D))
            ||((checkerboard_stg_d1==TD_NAVI_DETECT_CB)&&(cb_fcr_max>=c_cb_fcr_lo)))
    {
        checkerboard_stg_d1 = TD_NAVI_DETECT_CB;
        return checkerboard_stg_d1;
    }
    else if(((li_fcr_max < c_li_fcr_lo)
            &&(ci_fcr_max < c_ci_fcr_lo)
            &&(cb_fcr_max < c_cb_fcr_lo))
        || (checkerboard_stg_d1==TD_NAVI_DETECT_2D
            &&li_fcr_max<c_li_fcr_hi
            &&ci_fcr_max<c_ci_fcr_hi
            &&cb_fcr_max<c_cb_fcr_hi)) 
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

static inline UINT32 u4DrvTdNaviRamapLIDram(UINT32 u4Width,UINT32 u4Height,UINT32 *pu4Target,UINT32 *pLData,UINT32 *pRData)
{
    UINT32 *pu4Tar1=pu4Target;
    UINT32 *pu4L  =pLData;
    UINT32 *pu4R  =pRData;
    
    UINT32 i,j;
    if((NULL==pu4Target)
        ||(NULL==pLData)
        ||(NULL==pRData))
    {
        return SV_FAIL;
    }

    for(i=0;i<u4Height;i+=2)
    {
        for(j=0;j<u4Width;j+=4)
        {
            *pu4Tar1++ = *pu4L++;
        }        
        for(j=0;j<u4Width;j+=4)
        {
            *pu4Tar1++ = *pu4R++;
        }
    }
    return SV_SUCCESS;    
}

static inline UINT32 u4DrvTdNaviRamapDADram(UINT32 u4Width,UINT32 u4Height,UINT32 *pu4Target,UINT32 *pLData,UINT32 *pRData)
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

    for(i=0;i<u4Height;++i)
    {
        for(j=0;j<u4Width;j+=2)
        {
            *pu1Tar++ = *pu1L++;
            *pu1Tar++ = *pu1R++;                
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
                    u4RemapX = u2NaviStartX/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);
                
                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviLBuf);

                    u4RemapX = u2NaviStartX/2 + u2DrvDIGetVdpWidth(SV_VP_MAIN)/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);
                
                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviRBuf);
                    return u4DrvTdNaviRamapCBDram(FW_NAVI_WINDOW_WIDTH,FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviBuf,pu4NaviLBuf,pu4NaviRBuf);  

                case E_TD_IN_LI_P:
                    u4RemapX = u2NaviStartX/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);

                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviLBuf);

                    u4RemapX = u2NaviStartX/2 + u2DrvDIGetVdpWidth(SV_VP_MAIN)/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);

                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviRBuf);
                    return u4DrvTdNaviRamapLIDram(FW_NAVI_WINDOW_WIDTH,FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviBuf,pu4NaviLBuf,pu4NaviRBuf);  


                case E_TD_IN_DA_P:
                    u4RemapX = u2NaviStartX/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);

                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviLBuf);

                    u4RemapX = u2NaviStartX/2 + u2DrvDIGetVdpWidth(SV_VP_MAIN)/2;
                    vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT);

                    vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                        u4RemapX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH/2, FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviRBuf);
                    return u4DrvTdNaviRamapDADram(FW_NAVI_WINDOW_WIDTH,FW_NAVI_WINDOW_HEIGHT,
                        pu4NaviBuf,pu4NaviLBuf,pu4NaviRBuf);                 
                default:
                    break;                                
            }
        }
        else
        {           
            vDrvDIFlushDramForTDNAVI(SV_VP_MAIN,
                u2NaviStartX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH, FW_NAVI_WINDOW_HEIGHT);
            
            vDrvDIDumpDramForTDNAVI(SV_VP_MAIN,
                u2NaviStartX, u2NaviStartY, FW_NAVI_WINDOW_WIDTH, FW_NAVI_WINDOW_HEIGHT,
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
    TD_NAVI_DETECT preCheckerboard_stg_d1 = checkerboard_stg_d1;
        
    //Calculate Window Position   
    u2NaviWidth=u2DrvDIGetVdpWidth(SV_VP_MAIN);
    u2NaviHeight=u2DrvDIGetVdpHeight(SV_VP_MAIN);
    
    u4H = (u2NaviWidth>FW_NAVI_WINDOW_WIDTH) ? 
        (u2NaviWidth-FW_NAVI_WINDOW_WIDTH)/(NAVI_H_GRID-1) : 0;

    u4V = (u2NaviHeight>FW_NAVI_WINDOW_HEIGHT)?
        (u2NaviHeight-FW_NAVI_WINDOW_HEIGHT)/(NAVI_V_GRID-1) : 0;
        
    u2NaviStartX = (u1CurX * u4H)&(~0x3);//Align 4
    u2NaviStartY = u1CurY * u4V;
    
    u1CurX = (u1CurX + NAVI_STEP_H)%NAVI_H_GRID;
    u1CurY = (u1CurY + NAVI_STEP_V)%NAVI_V_GRID;
            
    //Detect 3D Format
    vDrvTdNaviFWDetectionScore();
    
    //Stablize Fmt Decision
    if((preCheckerboard_stg_d1 == checkerboard_stg_d1)
        &&(TD_NAVI_DETECT_UNKNOWN != checkerboard_stg_d1)
        &&(TD_NAVI_DETECT_2D != checkerboard_stg_d1))
    {
        u1DetectedCnt = MIN(u1DetectedStaMax,u1DetectedCnt + NAVI_CNT_STEP_UP);
    }
    else if(TD_NAVI_DETECT_UNKNOWN != checkerboard_stg_d1)
    {
        u1DetectedCnt = (u1DetectedCnt > u1DetectedCnt - NAVI_CNT_STEP_DN) 
            ? 0: u1DetectedCnt - NAVI_CNT_STEP_DN;
    }
    
    if(u1DetectedCnt > u1DetectedStaIn)
    {
        SwNaviDetectd = checkerboard_stg_d1;
    }
    else if(u1DetectedCnt < u1DetectedStaOut)
    {
        SwNaviDetectd = TD_NAVI_DETECT_2D;
    }      
}
#endif

void vDrvTdNaviFWDetectionUpdateStatus(void)
{
#if SUPPORT_SW_NAVI

    //Update FW Parameter
    u1DetectedStaIn =IO32ReadFldAlign(TDNAVI_FW_00, NAVI_CNT_IN);  
    u1DetectedStaOut=IO32ReadFldAlign(TDNAVI_FW_00, NAVI_CNT_OUT);            
    u1DetectedStaMax=IO32ReadFldAlign(TDNAVI_FW_00, NAVI_CNT_MAX);  

    c_li_fcr_hi =IO32ReadFldAlign(TDNAVI_FW_01, LI_FCT_HI_TH);  
    c_li_fcr_lo =IO32ReadFldAlign(TDNAVI_FW_01, LI_FCT_LO_TH); 
    c_li_cnt_thr=IO32ReadFldAlign(TDNAVI_FW_01, LI_CNT_TH); 
    li_th       =IO32ReadFldAlign(TDNAVI_FW_02, LI_TH);  
    c_li_cori_th=IO32ReadFldAlign(TDNAVI_FW_02, LI_COR_TH); 

    c_ci_fcr_hi =IO32ReadFldAlign(TDNAVI_FW_03, CI_FCT_HI_TH);  
    c_ci_fcr_lo =IO32ReadFldAlign(TDNAVI_FW_03, CI_FCT_LO_TH); 
    c_ci_cnt_thr=IO32ReadFldAlign(TDNAVI_FW_03, CI_CNT_TH); 
    ci_th       =IO32ReadFldAlign(TDNAVI_FW_04, CI_TH);  
    c_ci_cori_th=IO32ReadFldAlign(TDNAVI_FW_04, CI_COR_TH); 

    c_cb_fcr_hi =IO32ReadFldAlign(TDNAVI_FW_05, CB_FCT_HI_TH);  
    c_cb_fcr_lo =IO32ReadFldAlign(TDNAVI_FW_05, CB_FCT_LO_TH); 
    c_cb_cnt_thr=IO32ReadFldAlign(TDNAVI_FW_05, CB_CNT_TH); 
    cb_th       =IO32ReadFldAlign(TDNAVI_FW_06, CB_TH);  
    c_cb_cori_th=IO32ReadFldAlign(TDNAVI_FW_06, CB_COR_TH);         
    
    //update Status
    vIO32WriteFldAlign(TDNAVI_STATUS_00, sta_li_cnt      , LI_CNT);
    vIO32WriteFldAlign(TDNAVI_STATUS_00, sta_li_fcr      , LI_FCT);
    vIO32WriteFldAlign(TDNAVI_STATUS_01, sta_ci_cnt      , CI_CNT);
    vIO32WriteFldAlign(TDNAVI_STATUS_01, sta_ci_fcr      , CI_FCT);
    vIO32WriteFldAlign(TDNAVI_STATUS_02, sta_cb_cnt      , CB_CNT);
    vIO32WriteFldAlign(TDNAVI_STATUS_02, sta_cb_fcr      , CB_FCT);      
#endif   
}


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
       ||(fgDrvTDTVIsFix2DDisplay(SV_VP_MAIN)==SV_TRUE)
        )
    {
        SwNaviDetectd = TD_NAVI_DETECT_2D;    
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

    vDrvTdNaviFWDetectionUpdateStatus();
#endif   
}



