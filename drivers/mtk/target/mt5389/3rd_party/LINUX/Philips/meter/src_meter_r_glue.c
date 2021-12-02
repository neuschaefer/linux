/*==========================================================================*/
/* Includes                  	                                            */
/*==========================================================================*/
#include "src_meter.h"
#include "com_type.h"
#include "src_meter_r_glue.h"
//#include "pi_demod.h"
#include "vdec_if.h"

/*==========================================================================*/
/* LOCAL VARIABLES															*/
/*==========================================================================*/

/*==========================================================================*/
/* GLOBAL FUNCTIONS			 												*/
/*==========================================================================*/

mpegBlock_t IMsrcmeterR_GetMPEGBlock(void)
{
	mpegBlock_t MPEGBlock;
	MPEGBlock.wMpegH = RegReadFldAlign(NR_2DNR8F, R_HLINECNT);
	MPEGBlock.wMpegV = RegReadFldAlign(NR_2DNR8F, R_VLINECNT);

	return MPEGBlock;
}

WORD IMsrcmeterR_GetVideoContentNoise(void)
{
	WORD wContentNoise;
	wContentNoise = RegReadFldAlign(NR_3DNR9B, NEW_METER_NOISE_VALUE);

	return wContentNoise;
}

BYTE IMsrcmeterR_GetRFAGC(void)
{
	UINT16 bAGC;
	bAGC = 0;//DVBT_GetIfAgcVol(pMTPdCtx->pDemodCtx); //TBD

	return bAGC;
}

BYTE IMsrcmeterR_GetVideoSynchNoise(void)
{
	BYTE bNoiseLevel;
	bNoiseLevel = bHwTvdNRLevel();

	return bNoiseLevel;
}

BYTE IMsrcmeterR_SetSrcAutoMeter(BYTE bMeter)
{
	BYTE bMidValue;
	
	if((bMeter > 255) || (bMeter < 0))
		{
		return 0;
		}
	else
		{
		bMidValue = (bMeter/0xFF)*100;
		}
	
	return bMidValue;
}

videoInfo_t IMsrcmeterR_GetVideoResolution(void)
{
	videoInfo_t VIDEOINFO;
	VIDEOINFO.wResH = wDrvVideoGetHTotal(SV_VP_MAIN);
	VIDEOINFO.wResV = wDrvVideoGetVTotal(SV_VP_MAIN);
	VIDEOINFO.wLength= wDrvVideoInputWidth(SV_VP_MAIN);
	VIDEOINFO.wHeight= wDrvVideoInputHeight(SV_VP_MAIN);
	VIDEOINFO.bResT= bDrvVideoGetRefreshRate(SV_VP_MAIN);
	VIDEOINFO.bPI= bDrvVideoIsSrcInterlace(SV_VP_MAIN);

	return VIDEOINFO;
}

BYTE IMsrcmeterR_GetDTVBER(void)
{
	BYTE bBER;
	bBER = 0;//(UINT8)DVBT_GetPostVBer(pMTPdCtx->pDemodCtx); // return value = PostVBer * 10^5;

	return bBER;
}

videoCodingInfo_t IMsrcmeterR_GetVideoCodingInfo(void)
{
	VDEC_HDR_INFO_T rHdrInfo;
	videoCodingInfo_t VIDEOCODINGINFO;
	VIDEOCODINGINFO.bCodingType = (BYTE)rHdrInfo.eCodecType;
	VIDEOCODINGINFO.wBitRate = (WORD)VDEC_GetAvgPicSize(0);;

	return VIDEOCODINGINFO;
}

BYTE IMsrcmeterR_GetUISharpness(void)
{
	BYTE bSHARPUIVALUE;
	UINT16 i2Cur;
	
	// Get UI Setting
    PE_GetArg(SV_VP_MAIN, PE_ARG_SHARPNESS, &i2Cur);    
	bSHARPUIVALUE = (BYTE)i2Cur;

	return bSHARPUIVALUE;
}

BYTE IMsrcmeterR_GetSrcAutoMeterFlashData(BYTE *bData, WORD wLength)
{
return;
}

BYTE IMsrcmeterR_ReadSrcAutoMeterEepromBYTE(WORD wAddr)
{
return;
}

BYTE IMsrcmeterR_WriteSrcAutoMeterEepromBYTE(WORD wAddr, BYTE bData)
{
return;
}

BYTE IMsrcmeterR_SetPeakingFactor(contributeFactors_t contributeFactors)
{
return;
}

