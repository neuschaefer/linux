#ifndef _SRC_METER_R_GLUE_H
#define _SRC_METER_R_GLUE_H

/*==========================================================================*/
/*     I N C L U D E S                                                      */
/*==========================================================================*/

//#include "../mcommon/u_common.h"
#include "u_common.h"
#include "hw_nr.h"
#include "general.h"
#include "vdec_if.h"
#include "pe_table.h"
#include "hw_tvd.h"

/*==========================================================================*/
/* Functions Prototypes     			                                    */
/*==========================================================================*/
/**Requested API**/
/**
	\page getflash BYTE IMsrcmeterR_GetSrcAutoMeterFlashData(BYTE *bData, WORD wLength)
	This function reads all Video Source Quality Auto Indicator related Flash data.
	\param bData, BYTE Pointer type
	- the pointer to store the Flash data
	\param wLength, WORD type
	- the length of the Flash data to be gotten
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterR_GetSrcAutoMeterFlashData(BYTE *bData, WORD wLength);

/**
	\page reade2prom BYTE IMsrcmeterR_ReadSrcAutoMeterEepromBYTE(WORD wAddr)
	This function read one Byte of NVM data which is related to Video Source Quality Auto Indicator
	\param wAddr, WORD type
	- the address of the NVM data to be read
	\return
		- The read NVM data
	\remark
*/
BYTE IMsrcmeterR_ReadSrcAutoMeterEepromBYTE(WORD wAddr);

/**
	\page writee2prom BYTE IMsrcmeterR_WriteSrcAutoMeterEepromBYTE(WORD wAddr, BYTE bData)
	This function write one Byte of NVM data which is related to Video Source Quality Auto Indicator to the given address
	\param wAddr, WORD type
	- the address of the NVM data to be written
	\param bData, BYTE type
	- the data to be written to the NVM
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterR_WriteSrcAutoMeterEepromBYTE(WORD wAddr, BYTE bData);

/**
	\page sharp BYTE IMsrcmeterR_GetUISharpness(void)
	This function is to get UI sharpness value
	\return
		- UI sharpness value, range is from 0 to 100
	\remark
	
	This function is OK,return value is UI sharpness value, range is from 0 to 100
*/
BYTE IMsrcmeterR_GetUISharpness(void);

/**
	\page setmeter BYTE IMsrcmeterR_SetSrcAutoMeter(BYTE bMeter)
	This function sets Video Source Quality Auto Indicator value which is calculated by Philips algorithm
	\param[in] bMeter, BYTE type
	- the calculated meter value whose range is 0 to 255 and will be mapped to the decimal 0 to 100 for OSD indication
	\return
		- TRUE for success
		- FALSE for failed
	\remark

	This function is OK,return value Range is 0~100
*/
BYTE IMsrcmeterR_SetSrcAutoMeter(BYTE bMeter);

/**
	\page rfagc BYTE IMsrcmeterR_GetRFAGC(void)
	This function gets the RF AGC value from video front-end
	\return
		- The RF AGC value whose range is 0 to 255
	\remark
*/
BYTE IMsrcmeterR_GetRFAGC(void);

/**
	\page syncnoise BYTE IMsrcmeterR_GetVideoSynchNoise(void)
	This function gets video synch based noise meter value
	\return
		- The video synch based noise meter value whose range is 0 to 255
	\remark
	
	This function is OK,return value Range is 0~255(8bits)
*/
BYTE IMsrcmeterR_GetVideoSynchNoise(void);

/**
	\page contentnoise WORD IMsrcmeterR_GetVideoContentNoise(void)
	This function is to get video content noise meter value
	\return
		- The video content noise meter value whose range is 0 to 65535
	\remark
	
	This function is OK, return value Range is 0~65535(16bits)
*/
WORD IMsrcmeterR_GetVideoContentNoise(void);

/**
	\page resolution videoInfo_t IMsrcmeterR_GetVideoResolution(void)
	This function is to get video source resolution information for horizontal, vertical, temporal and the progressive or interlace
	\return
		- Video information in videoInfo_t struct type.
	\remark
		- <B>Note: for details of struct videoInfo_t struct, please refer to the struct definition</B>

	This function is OK,return value is:HTotal, VTotal, Height, Width, Framerate, P/I
*/
videoInfo_t IMsrcmeterR_GetVideoResolution(void);

/**
	\page dtvber BYTE IMsrcmeterR_GetDTVBER(void)
	This function is to get bit error rate value from digital channel decoder
	\return
		- BER value whose range is 0 to 255
	\remark
*/
BYTE IMsrcmeterR_GetDTVBER(void);

/**
	\page codeinfo videoCodingInfo_t IMsrcmeterR_GetVideoCodingInfo(void)
	This function is to get the video source coding information.
	\return
		- Video coding information in videoCodingInfo_t struct
	\remark
		- <B>Note: please refer to videoCodingInfo_t for the details definition of the struct members </B>

	This function is TBD, now return value is videoCodingInfo_t
	bCodingType:
		0:  MPV		MPEG1/2
		1:  MP4		MPEG4 part2
		2:  H264		MPEG4 Part10 (H.264)/MPEG4 MVC (3D)
		3:  WMV		WMV:(VC-1)
		4:  H264VER	
		5:  MJPEG	Motion Jpeg
		6:  RV		RealMedia RM or RMVB
		7:  AVS,
		8:  Others
*/
videoCodingInfo_t IMsrcmeterR_GetVideoCodingInfo(void);

/**
	\page mpeg mpegBlock_t IMsrcmeterR_GetMPEGBlock(void)
	This function is to get video MPEG block count for both Horizontal and vertical directions
	\return
		- Video MPEG block in mpegBlock_t struct
	\remark
		- <B>Note: please refer to mpegBlock_t for the details definition of the struct members. </B>

	This function is OK,return value is HBlock 11bits, VBlock 11bits
*/
mpegBlock_t IMsrcmeterR_GetMPEGBlock(void);

/**
	\page peakfactor BYTE IMsrcmeterR_SetPeakingFactor(contributeFactors_t contributeFactors)
	This function is to set the contribution factors for peaking band 1 to band 11
	\param contributeFactors, contributeFactors_t type
	\return
		- TRUE for success
		- FALSE for failed
	\remark
		- <B>Note: please refer to contributeFactors_t for the details definition of the struct members </B>
*/
BYTE IMsrcmeterR_SetPeakingFactor(contributeFactors_t contributeFactors);


#endif /* _SRC_METER_R_GLUE_H */
