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
#include "drv_noisemeter.h"
#include "drv_nr.h"
#include "drv_meter.h"
#include "hw_nr.h"
#include "hw_sw.h"

/** static functions
 */
 
static UINT16 vDrvNMGetHistoMoPxlCnt(void);
static UINT16 vDrvNMGetHistoTotalPxlCnt(void);
static UINT32 vDrvNMGetMinDS(void);
static UINT32 vDrvNMGetMaxDS(void);
static UINT32 vDrvNMGetAvgDS(void);
static UINT32 vDrvNMGetDSRatio(void);
static UINT32 vDrvNMGetNDSRatio(void);
static UINT8 vDrvNMIsStatusStable(void);
static void vDrvNMSWRegInit(void);
static void vDrvNMUpdateInfo(void);
static void vDrvNMStatusPolling(void);
static void vDrvNMPrintStatus(void);
static void vDrvNMPrintStatusHeader(void);
static UINT32 vDrvNMLUTMapping(NM_LINEAR_LUT* lut,UINT32 input);
static UINT32 vDrvNMNoiseLevelMapping(UINT32 noiseValue);

/** Global variables
 */

UINT8 whiteSceneFlag = SV_FALSE;
UINT16 sceneID = 0;
NR_NOISE_METER_INFO nmInfo;
UINT32 NMLoopNumber = 0; 
UINT32 NMLoopCounter = 0;
UINT32 NMIntervalNumber = 0;
UINT32 NMIntervalCounter = 0;
UINT8 fgNMStatusPolling = SV_FALSE;
UINT16 NMMaxPixelCount = 0;
NM_LINEAR_LUT MoPxlRatioLUT = {2,20,1000,100};
NM_LINEAR_LUT DSRatioLUT = {5,100,1000,100};
NM_LINEAR_LUT APLWLUT = {16,64,5,1};
UINT16 NoiseLevelLUT[] = {100,500,1000,2000,5000,9000,12000,65535};
UINT8 NMNormBase = 10;

/** Function implementation
 */
void vDrvNMInit(UINT8 fgNM,UINT32 loopNumber,UINT32 intervalNumber)
{
    //1. print header
    if(fgNMStatusPolling == SV_FALSE && fgNM == SV_TRUE)
    {
        vDrvNMPrintStatusHeader();
    }
    //2. initialize variable
	fgNMStatusPolling = fgNM;
	NMLoopNumber = loopNumber;
	NMLoopCounter = 0;
	NMIntervalNumber = intervalNumber;
	NMIntervalCounter = 0;
	sceneID = 0;

	//3. SW register init
	vDrvNMSWRegInit();
}

static void vDrvNMSWRegInit(void)
{
    //init noise level LUT
    vIO32WriteFldAlign(NM_LUT_03,NoiseLevelLUT[7],NM_NL7_UPBOUND);         
    vIO32WriteFldAlign(NM_LUT_03,NoiseLevelLUT[6],NM_NL6_UPBOUND);
    vIO32WriteFldAlign(NM_LUT_02,NoiseLevelLUT[5],NM_NL5_UPBOUND);         
    vIO32WriteFldAlign(NM_LUT_02,NoiseLevelLUT[4],NM_NL4_UPBOUND);
    vIO32WriteFldAlign(NM_LUT_01,NoiseLevelLUT[3],NM_NL3_UPBOUND);         
    vIO32WriteFldAlign(NM_LUT_01,NoiseLevelLUT[2],NM_NL2_UPBOUND);
    vIO32WriteFldAlign(NM_LUT_00,NoiseLevelLUT[1],NM_NL1_UPBOUND);         
    vIO32WriteFldAlign(NM_LUT_00,NoiseLevelLUT[0],NM_NL0_UPBOUND);
    //init MoPxl confidence LUT
    vIO32WriteFldAlign(NM_LUT_04,MoPxlRatioLUT.xTurn1,NM_MOPLX_LUT_XTURN1);   
    vIO32WriteFldAlign(NM_LUT_04,MoPxlRatioLUT.xTurn2,NM_MOPLX_LUT_XTURN2);                      
    vIO32WriteFldAlign(NM_LUT_05,MoPxlRatioLUT.yStart,NM_MOPLX_LUT_YSTART);   
    vIO32WriteFldAlign(NM_LUT_05,MoPxlRatioLUT.yEnd,NM_MOPLX_LUT_YEND);                          
    //init DSRatio confidence LUT
    vIO32WriteFldAlign(NM_LUT_06,DSRatioLUT.xTurn1,NM_DSR_LUT_XTURN1);   
    vIO32WriteFldAlign(NM_LUT_06,DSRatioLUT.xTurn2,NM_DSR_LUT_XTURN2);                          
    vIO32WriteFldAlign(NM_LUT_07,DSRatioLUT.yStart,NM_DSR_LUT_YSTART);   
    vIO32WriteFldAlign(NM_LUT_07,DSRatioLUT.yEnd,NM_DSR_LUT_YEND);
    //init APLW LUT
    vIO32WriteFldAlign(NM_LUT_08,APLWLUT.xTurn1,NM_APLW_LUT_XTURN1);   
    vIO32WriteFldAlign(NM_LUT_08,APLWLUT.xTurn2,NM_APLW_LUT_XTURN2);                          
    vIO32WriteFldAlign(NM_LUT_09,APLWLUT.yStart,NM_APLW_LUT_YSTART);   
    vIO32WriteFldAlign(NM_LUT_09,APLWLUT.yEnd,NM_APLW_LUT_YEND);    
    //init Status
    vIO32WriteFldAlign(NM_STATUS_00,0,NM_NOISE_LEVEL);
    vIO32WriteFldAlign(NM_STATUS_00,0,NM_FINAL_CONFIDENCE);
    vIO32WriteFldAlign(NM_STATUS_01,0,NM_MOPXL_CONFIDENCE); 
    vIO32WriteFldAlign(NM_STATUS_01,0,NM_DSRATIO_CONFIDENCE);                        
    vIO32WriteFldAlign(NM_STATUS_03,0,NM_DSSTABLE);
    vIO32WriteFldAlign(NM_STATUS_03,0,NM_MOPXLSTABLE);    
    vIO32WriteFldAlign(NM_STATUS_05,0,NM_NDSRATIO);        
    vIO32WriteFldAlign(NM_STATUS_05,0,NM_APLW);   
    vIO32WriteFldAlign(NM_CFG_00,NMNormBase,NM_DS_NORM_BASE);       
    vIO32WriteFldAlign(NM_CFG_00,1,NM_DS_NORM_EN);   
}

static void vDrvNMUpdateNMResult(void)
{
    //1. Noise level
    vIO32WriteFldAlign(NM_STATUS_00,nmInfo.nmResult.u2NoiseLevel*10,NM_NOISE_LEVEL);         
    //2. final confidence
    vIO32WriteFldAlign(NM_STATUS_00,nmInfo.nmResult.u2FinalConfidence,NM_FINAL_CONFIDENCE);
    //3. confidence by MPR (Motion Pixel Ratio)
    vIO32WriteFldAlign(NM_STATUS_01,nmInfo.nmMPR.u2MoPxlConfidence,NM_MOPXL_CONFIDENCE);         
    //4. confidence by DSR (DS Ratio)
    vIO32WriteFldAlign(NM_STATUS_01,nmInfo.nmDSR.u2DSRatioConfidence,NM_DSRATIO_CONFIDENCE);    
    //5. Flag to check if NM is stable
    vIO32WriteFldAlign(NM_STATUS_03,nmInfo.nmMPR.fgPixelCountStable,NM_MOPXLSTABLE);    
    vIO32WriteFldAlign(NM_STATUS_03,nmInfo.nmDSR.fgDSStable,NM_DSSTABLE);
    //6. MPR
    vIO32WriteFldAlign(NM_STATUS_03,nmInfo.nmMPR.bMotionPixelRatio,NM_MOPXLRATIO);    
    //7. DSR
    vIO32WriteFldAlign(NM_STATUS_03,nmInfo.nmDSR.u2DSRatio,NM_DSRATIO);
    //8. APLW
    vIO32WriteFldAlign(NM_STATUS_05,nmInfo.bAPLW,NM_APLW);
    //9. NDSR
    vIO32WriteFldAlign(NM_STATUS_05,nmInfo.nmDSR.u2NDSRatio,NM_NDSRATIO);
}

static void vDrvNMUpdateAllLUT(void)
{
    //update Noise Level LUT
    NoiseLevelLUT[7] = IO32ReadFldAlign(NM_LUT_03,NM_NL7_UPBOUND);         
    NoiseLevelLUT[6] = IO32ReadFldAlign(NM_LUT_03,NM_NL6_UPBOUND);
    NoiseLevelLUT[5] = IO32ReadFldAlign(NM_LUT_02,NM_NL5_UPBOUND);         
    NoiseLevelLUT[4] = IO32ReadFldAlign(NM_LUT_02,NM_NL4_UPBOUND);
    NoiseLevelLUT[3] = IO32ReadFldAlign(NM_LUT_01,NM_NL3_UPBOUND);         
    NoiseLevelLUT[2] = IO32ReadFldAlign(NM_LUT_01,NM_NL2_UPBOUND);
    NoiseLevelLUT[1] = IO32ReadFldAlign(NM_LUT_00,NM_NL1_UPBOUND);         
    NoiseLevelLUT[0] = IO32ReadFldAlign(NM_LUT_00,NM_NL0_UPBOUND);
    //update MoPxl confidence LUT
    MoPxlRatioLUT.xTurn1= IO32ReadFldAlign(NM_LUT_04,NM_MOPLX_LUT_XTURN1);
    MoPxlRatioLUT.xTurn2= IO32ReadFldAlign(NM_LUT_04,NM_MOPLX_LUT_XTURN2);
    MoPxlRatioLUT.yStart= IO32ReadFldAlign(NM_LUT_05,NM_MOPLX_LUT_YSTART);
    MoPxlRatioLUT.yEnd= IO32ReadFldAlign(NM_LUT_05,NM_MOPLX_LUT_YEND);
    //update DSRatio confidence LUT
    DSRatioLUT.xTurn1 = IO32ReadFldAlign(NM_LUT_06,NM_DSR_LUT_XTURN1);
    DSRatioLUT.xTurn2 = IO32ReadFldAlign(NM_LUT_06,NM_DSR_LUT_XTURN2);
    DSRatioLUT.yStart = IO32ReadFldAlign(NM_LUT_07,NM_DSR_LUT_YSTART);
    DSRatioLUT.yEnd = IO32ReadFldAlign(NM_LUT_07,NM_DSR_LUT_YEND);
    //update APLW LUT
    APLWLUT.xTurn1 = IO32ReadFldAlign(NM_LUT_08,NM_APLW_LUT_XTURN1);
    APLWLUT.xTurn2 = IO32ReadFldAlign(NM_LUT_08,NM_APLW_LUT_XTURN2);
    APLWLUT.yStart = IO32ReadFldAlign(NM_LUT_09,NM_APLW_LUT_YSTART);
    APLWLUT.yEnd = IO32ReadFldAlign(NM_LUT_09,NM_APLW_LUT_YEND);    

    //update Normalized base
    NMNormBase = IO32ReadFldAlign(NM_CFG_00,NM_DS_NORM_BASE);
}

void vDrvNMSetBlockSize(UINT16 width, UINT16 height)
{
	NMMaxPixelCount = (UINT16)(((UINT32)width * (UINT32)height) /16);	
}

static UINT16 vDrvNMGetHistoMoPxlCnt(void)
{
	UINT8 index = 0;
	UINT16 ret = 0;

	for(index = 0; index < 6; index++)
	{
		ret += nmInfo.u2GlobalHistogram[index];
	}

	for(index = 10; index < 16; index ++)
	{
		ret += nmInfo.u2GlobalHistogram[index];
	}

	return ret;
}

static UINT16 vDrvNMGetHistoTotalPxlCnt(void)
{
	UINT8 index = 0;
	UINT16 ret = 0;

	for(index = 0; index < 16; index++)
	{
		ret += nmInfo.u2GlobalHistogram[index];
	}
	
	return ret;
}

static UINT32 vDrvNMGetMinDS(void)
{
    UINT8 index = 0;
    UINT32 ret = 0xFFFFFFFF;
	
    for(index = 0; index < 16; index++)
    {
		if(nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum < ret)
        {
            ret = nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum;
        }
    }
    
    return ret;
}

static UINT32 vDrvNMGetMaxDS(void)
{
	UINT32 ret = 0x0;
    UINT8 index = 0;
    
	for(index = 0; index < 16; index++)
	{
		if(nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum > ret)
		{
            ret = nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum;
		}
	}

	return ret;
}

static UINT32 vDrvNMGetAvgDS(void)
{
	UINT32 ret = 0;
	UINT8 index = 0;	

	for(index = 0; index < 16; index++)
    {
        ret += nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum;
	}

	ret = ret / 16;
	
	return ret;
}

static UINT32 vDrvNMGetNDSRatio(void)
{
	UINT32 ret = 0;
	ret = nmInfo.nmDSR.u2DSRatio * nmInfo.bAPLW;
	
    return ret;
}

static UINT32 vDrvNMGetDSRatio(void)
{
	UINT32 ret = 0;
	UINT8 apl;
	ret = (nmInfo.nmDSR.u4MaxDS + 500) / (nmInfo.nmDSR.u4MinDS + 500);	
	apl = bDrvGetAPL();
	if(apl <=16)
	{
	    apl = 17;   //to avoid divide by 0
	}
	
    //when DSR == 1, treat it as a special case

	if(ret == 1)
	{
	    ret = 0;
	}
	else if(ret == 2)
	{
	    ret = 1;
	}

	//Normalize DSR
	/*
	if(IO32ReadFldAlign(NM_CFG_00,NM_DS_NORM_EN) == 1)
	{	
	    ret = (ret * (235-16))/ (bDrvGetAPL() - 16);
	}
	*/
	return ret;
}

static void vDrvNMUpdateInfo(void)
{
	UINT8 index = 0;
	UINT16 moPxlCnt = 0;
	UINT16 totalPxlCnt = 0;
	
	//1. get register value
	nmInfo.u2NoiseValue = RegReadFldAlign(NR_3DNR9B, NEW_METER_NOISE_VALUE);
	for(index = 0; index < 16; index++)
	{
		if(index % 2 == 0)
		{
			nmInfo.u2GlobalHistogram[index] = RegReadFldAlign(NR_3DNR90+(index/2)*0x4, NEW_METER_DIFF_AREA1);
		}
		else
		{
			nmInfo.u2GlobalHistogram[index] = RegReadFldAlign(NR_3DNR90+(index/2)*0x4, NEW_METER_DIFF_AREA2);
		}
		nmInfo.NMLocalAreaInfo[index].u4CurSum = RegReadFldAlign(NR_3DNRC0+index*0x10, AREA00_CSUM);
		nmInfo.NMLocalAreaInfo[index].u4PreSum = RegReadFldAlign(NR_3DNRC1+index*0x10, AREA00_PSUM);
		nmInfo.NMLocalAreaInfo[index].u4AbsDiffSum = RegReadFldAlign(NR_3DNRC2+index*0x10, AREA00_DA);
		nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum = RegReadFldAlign(NR_3DNRC3+index*0x10, AREA00_DS);
		if(NMMaxPixelCount !=0)
		{
		    nmInfo.NMLocalAreaInfo[index].u4BlockAPL = nmInfo.NMLocalAreaInfo[index].u4CurSum / NMMaxPixelCount;
		}
		//Normalize DS by Block APL
		if(IO32ReadFldAlign(NM_CFG_00,NM_DS_NORM_EN) == 1)
		{
		    if(nmInfo.NMLocalAreaInfo[index].u4BlockAPL > 64)
		    {
		        nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum = ((nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum * (940-64))+ NMNormBase) / 
		                                                        ((nmInfo.NMLocalAreaInfo[index].u4BlockAPL - 64)+ NMNormBase);
		    }
		}
	}

	//2. get motion pixel cnt
	moPxlCnt = vDrvNMGetHistoMoPxlCnt();
	//3. get total pixel cnt
	totalPxlCnt = vDrvNMGetHistoTotalPxlCnt();	
	//4. determine stable flag
	if(totalPxlCnt >= (UINT16)((UINT32)NMMaxPixelCount * 9 /10))
	{
		nmInfo.nmMPR.fgPixelCountStable = SV_TRUE;
	}
	else
	{
		nmInfo.nmMPR.fgPixelCountStable = SV_FALSE;
	}
	
	//5. Get DS Info
	nmInfo.nmDSR.u4MinDS = vDrvNMGetMinDS();
	nmInfo.nmDSR.u4MaxDS = vDrvNMGetMaxDS();
	nmInfo.nmDSR.u4AvgDS = vDrvNMGetAvgDS();
    //6. determine DS stable flag
    /*
	if(nmInfo.nmDSR.u4MaxDS > 5000000)
	{
	    nmInfo.nmDSR.fgDSStable = SV_FALSE;
	}
	else
	{
	    nmInfo.nmDSR.fgDSStable = SV_TRUE;
	}
	*/
    //make fgDSStable always true
    nmInfo.nmDSR.fgDSStable = SV_TRUE;

	//when stable, update NM result by searching LUT
	if(vDrvNMIsStatusStable() == SV_TRUE)
	{
    	//calcluate Motion pixel ratio
    	nmInfo.nmMPR.bMotionPixelRatio = (totalPxlCnt == 0)? 0 : ((moPxlCnt * 1000) / totalPxlCnt) ; 	    	
    	//calculate DS ratio
    	nmInfo.nmDSR.u2DSRatio = vDrvNMGetDSRatio();
    	//calculate APLW
    	nmInfo.bAPLW = vDrvNMLUTMapping(&APLWLUT,bDrvGetAPL());
    	//calculate NDS ratio
    	nmInfo.nmDSR.u2NDSRatio = vDrvNMGetNDSRatio();
    	//MPR confidence
	    nmInfo.nmMPR.u2MoPxlConfidence = vDrvNMLUTMapping(&MoPxlRatioLUT, nmInfo.nmMPR.bMotionPixelRatio);
    	//DSR confidence
        nmInfo.nmDSR.u2DSRatioConfidence = vDrvNMLUTMapping(&DSRatioLUT, nmInfo.nmDSR.u2NDSRatio);
	    //Get the final result & normalize confidence to 0~100
	    nmInfo.nmResult.u2FinalConfidence = (UINT16)(((UINT32)nmInfo.nmMPR.u2MoPxlConfidence * (UINT32)nmInfo.nmDSR.u2DSRatioConfidence) / 10000);
	    nmInfo.nmResult.u2NoiseLevel = vDrvNMNoiseLevelMapping(nmInfo.u2NoiseValue);
	    
	    if(nmInfo.nmResult.u2NoiseLevel == 7)
        {
            nmInfo.nmResult.u2FinalConfidence = nmInfo.nmResult.u2FinalConfidence / 10; //confidence * 0.1
	    }
	}
	
    vDrvNMUpdateNMResult();	

    //when trigger bit changed, update all LUT table parameter
	if(nmInfo.fgTriggerStatus != IO32ReadFldAlign(NM_CFG_00, NM_TRIGGER))
	{
	    nmInfo.fgTriggerStatus = IO32ReadFldAlign(NM_CFG_00, NM_TRIGGER);
	    vDrvNMUpdateAllLUT();
	}
}

NR_NOISE_METER_RESULT* vDrvNMGetResult(void)
{    
    return &(nmInfo.nmResult);
}

static UINT8 vDrvNMIsStatusStable(void)
{
    UINT8 ret = SV_FALSE;
    if(nmInfo.nmMPR.fgPixelCountStable == SV_TRUE && nmInfo.nmDSR.fgDSStable == SV_TRUE)
    {
        ret = SV_TRUE;
    }

    return ret;
}

static void vDrvNMStatusPolling(void)
{
	if(NMLoopNumber ==0 && fgNMStatusPolling == SV_TRUE) //always print
	{
        NMIntervalCounter++;
		if(NMIntervalCounter >= NMIntervalNumber)        
		{
			vDrvNMPrintStatus();
			NMIntervalCounter = 0;	//reset counter
		}		
	}
	else	// print "NMLoopNumber" times
	{
		if((NMLoopCounter < NMLoopNumber) & fgNMStatusPolling == SV_TRUE)
		{
	        NMIntervalCounter++;
			if(NMIntervalCounter >= NMIntervalNumber)        
			{
				vDrvNMPrintStatus();
				NMIntervalCounter = 0;	//reset counter
				NMLoopCounter++;
			}
		}
		else
		{
			fgNMStatusPolling = SV_FALSE;
		}
	}
}

#if(NR_NOISE_METER_LOG_LEVEL==0)
static void vDrvNMPrintStatus(void)
{
	Printf("Noise Value: %10d\n", nmInfo.u2NoiseValue);
}
#elif(NR_NOISE_METER_LOG_LEVEL==2)
static void vDrvNMPrintStatus(void)
{
}
#else
static void vDrvNMPrintStatus(void)
{
	UINT8 apl = bDrvGetAPL();
	UINT8 index = 0;
	
	if(apl > 220 && (whiteSceneFlag==SV_FALSE))
	{
		whiteSceneFlag = SV_TRUE;
		vRegWriteFldAlign(NR_3DNR80, 1, C_CLEAR);
	}

	if(apl <= 220 && whiteSceneFlag == SV_TRUE)
	{
		sceneID++;
		whiteSceneFlag = SV_FALSE;		
		vRegWriteFldAlign(NR_3DNR80, 0, C_CLEAR);		
	}

	if(apl>220)
	{
		return;
	}

	if(vDrvNMIsStatusStable() == SV_TRUE)
	{
//	    Printf("X,");
		return;
	}
	else
	{
//	    Printf("O,");
	}
	
	Printf("%d,%d,%d,", sceneID, apl, nmInfo.u2NoiseValue);
	for(index=0;index < 16; index++)
	{
		Printf("%d,",nmInfo.u2GlobalHistogram[index]);
	}	

	for(index=0;index < 16; index++)
	{
		Printf("%d,",nmInfo.NMLocalAreaInfo[index].u4SquareDiffSum);
	}

	Printf("%d,",nmInfo.nmMPR.bMotionPixelRatio);

	Printf("%d,%d,%d,%d,",nmInfo.nmDSR.u4MinDS,
	                      nmInfo.nmDSR.u4MaxDS,
	                      nmInfo.nmDSR.u2DSRatio,
	                      nmInfo.nmDSR.u4AvgDS);

	Printf("%d,%d,%d,%d,",nmInfo.nmResult.u2NoiseLevel,
	                      nmInfo.nmResult.u2FinalConfidence, 
	                      nmInfo.nmMPR.u2MoPxlConfidence, 
	                      nmInfo.nmDSR..u2DSRatioConfidence);
	
	Printf("\n");	
}
#endif

static void vDrvNMPrintStatusHeader(void)
{
	UINT8 index;
	Printf("\n");
	Printf("Scene ID, APL, Noise Value,");
	for(index=0;index < 16; index++)
	{
		Printf("Bin%d,",index);
	}	

	for(index=0;index < 16; index++)
	{
		Printf("DS%d,",index);
	}
	
	Printf("MoPxlRatio,");	
	Printf("MinDS,MaxDS,DSRatio,AvgDS,");		
	Printf("Noise Level,Final Confidence,MoPxl Confidence,DSRatio Confidence,");
	Printf("\n");		
}

static UINT32 vDrvNMLUTMapping(NM_LINEAR_LUT* lut,UINT32 input)
{
    UINT32 ret = 0;
    //error handle
    if(lut->xTurn2 <= lut->xTurn1)
    {
        Printf("LUT x-axis config error Turn2(%d)<=Turn1(%d)\n",lut->xTurn2,lut->xTurn1);
        ret = 0xffffffff;
        return ret;
    }
    
    if(input<=lut->xTurn1)
    {
        ret = lut->yStart;
    }
    else if(input > lut->xTurn2)
    {
        ret = lut->yEnd;
    }
    else // when xTurn1< input <= xTurn2
    {
        //do interpolation
        if(lut->yEnd >= lut->yStart)   
        {
            ret = (((lut->yEnd - lut->yStart) * (input - lut->xTurn1)) / (lut->xTurn2 - lut->xTurn1)) + lut->yStart;
        }
        else
        {
            ret = ((lut->yStart - lut->yEnd) * (lut->xTurn2 - input)) / (lut->xTurn2 - lut->xTurn1) + lut->yEnd;
        }
    }

    return ret;
}

static UINT32 vDrvNMNoiseLevelMapping(UINT32 noiseValue)
{
    UINT32 ret = 0;
    UINT8 max = sizeof(NoiseLevelLUT)/sizeof(UINT16);
    UINT8 i = 0;
    for(i=0; i<max; i++)
    {
        if(noiseValue < NoiseLevelLUT[i])
        {
            ret = i;
            break;
        }
    }
    return ret;    
}

void vDrvNMLUTMappingUT(void)
{
    UINT16 i=0;
    Printf("==========MoPxlRatioLUT Unit Test==========\n");
    for(i = 0;i<MoPxlRatioLUT.xTurn2+10;i++)
    {
        Printf("(in,out)=(%d,%d)\n",i,vDrvNMLUTMapping(&MoPxlRatioLUT, i));
    }
    Printf("==========DSRatioLUT Unit Test==========\n");
    for(i=0;i<DSRatioLUT.xTurn2+10;i++)
    {
        Printf("(in,out)=(%d,%d)\n",i,vDrvNMLUTMapping(&DSRatioLUT, i));
    }
    Printf("==========APLWLUT Unit Test==========\n");    
    for(i=0;i<APLWLUT.xTurn2+10;i++)
    {
        Printf("(in,out)=(%d,%d)\n",i,vDrvNMLUTMapping(&APLWLUT, i));
    }    
    Printf("==========NoiseLevelLUT Unit Test==========\n");        
    for(i=0;i<8;i++)
    {
        Printf("(%d,%d)\n",i,NoiseLevelLUT[i]);
    }
    
    NR_NOISE_METER_RESULT* result;
    result = vDrvNMGetResult();

    Printf("Level(%d) Confidence(%d)\n",result->u2NoiseLevel, result->u2FinalConfidence);
/*
    for(i=0;i<65535;i++)
    {
        Printf("(NV,NL)=(%d,%d)\n",i,vDrvNMNoiseLevelMapping(i));
    }
*/    
}

void vDrvNMProc(void)
{
    //update NM status
	vDrvNMUpdateInfo();
    //Print NM statistic log
	vDrvNMStatusPolling();
}

