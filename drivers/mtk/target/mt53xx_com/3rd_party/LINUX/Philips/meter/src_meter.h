/*****************************************************************************************************
 \file src_meter.h
 \brief This is the header file to declare the struct and APIs of Video
 Source Quality Auto Indicator software module.
******************************************************************************************************/

/** \mainpage Video Source Quality Auto Indicator Software Design
	This module defines the Video Source Quality Auto Indicator algorithm which is used by Philips to dynamically indicate different video signal quality.

	This design include the following parts
	- \subpage intro "Introduction"
	- \subpage srcmeter "Video Source Quality Auto Indicator Algorithm Design"
	- \subpage api "Application Program Interfaces"
*/

/** \page intro Introduction
	\section purpose Purpose
	This document describes the details design of Video Source Quality Auto Indicator algorithm running on MTK platform (MT5395, MT5366 and MT5365).
	\section audience Audience
	This document is intented for software designer, software architect, and software project leader.
	\section abbre Abbreviation
	<I>TBD</I>
	\section scope Scope
	This document is restricted to MTK platform (including MT5395, MT5366 and MT5365).
	\section ref Reference
		- [1]<I>TBD</I>
		- [2]<I>TBD</I>, Li Chengyue
		- [3]<I>TBD</I>, Li Rui
*/

/** \page srcmeter Video Source Quality Auto Indicator Algorithm Design
	\section overview Software Design Overview
	\image html APIOverview.jpg
	\image rtf APIOverview.jpg
	\section algo Video Source Quality Auto Indicator Algorithm
	The details of the Video Source Quality Auto Indicator Algorithm is described in reference [1], [2] and [3]. 
	\section naming Naming Convention
		- The naming convention of Philips IP implementation should follow Philips IP component model. The API can be mapped/glued to the MTK API(s)
		- APIs which are interfacing with external software should start with prefix 'I'
		- APIs which are provided for external software should be included in header file named : IMxxx_p.h
		- APIs which are required from external software (to support) should be included in header file named : IMxxx_r.h
*/

/** \page api Application Program Interfaces
	Following APIs are exported to be used by MTK platform to run the Video Signal Quality Auto Indicator Algorithm
	- \subpage init "BYTE IMsrcmeterP_InitSrcAutoMeter(void)"
	- \subpage feature "BYTE IMsrcmeterP_SetSrcAutoMeterFeatureBYTEs(BYTE bFeatureBYTE1, BYTE bFeatureBYTE2)"
	- \subpage vsync "BYTE IMsrcmeterP_UpdateSrcAutoMeterVSync(void)"
	- \subpage source "BYTE IMsrcmeterP_SetCurrentSource(BYTE bSource)"
	- \subpage run "BYTE IMsrcmeterP_RunSrcAutoMeter(void)"
	- \subpage pql "BYTE IMsrcmeterP_EnablePQL2(BOOL bOnOff)"
	- \subpage enable "BYTE IMsrcmeterP_EnableSrcAutoMeter( BYTE bCondition)"
	
	Following APIs are provied by MTK to be call by Video Signal Source Quality Auto Indicator
	- \subpage getflash "BYTE IMsrcmeterR_GetSrcAutoMeterFlashData(BYTE *bData, WORD wLength)"
	- \subpage reade2prom "BYTE IMsrcmeterR_ReadSrcAutoMeterEepromBYTE(WORD wAddr)"
	- \subpage writee2prom "BYTE IMsrcmeterR_WriteSrcAutoMeterEepromBYTE(WORD wAddr, BYTE bData)"
	- \subpage sharp "BYTE IMsrcmeterR_GetUISharpness(void)"
	- \subpage setmeter "BYTE IMsrcmeterR_SetSrcAutoMeter(BYTE bMeter)"
	- \subpage rfagc "BYTE IMsrcmeterR_GetRFAGC(void)"
	- \subpage syncnoise "BYTE IMsrcmeterR_GetVideoSynchNoise(void)"
	- \subpage contentnoise "WORD IMsrcmeterR_GetVideoContentNoise(void)"
	- \subpage resolution "videoInfo_t IMsrcmeterR_GetVideoResolution(void)"
	- \subpage dtvber "BYTE IMsrcmeterR_GetDTVBER(void)"
	- \subpage codeinfo "videoCodingInfo_t IMsrcmeterR_GetVideoCodingInfo(void)"
	- \subpage mpeg "mpegBlock_t IMsrcmeterR_GetMPEGBlock(void)"
	- \subpage peakfactor "BYTE IMsrcmeterR_SetPeakingFactor(contributeFactors_t contributeFactors)"

*/

/*
	Revision History
	V0.1	3rd August 2010		LI Rui
	Initial work - dummy library
	V0.2	25th August 2010	LI Rui
	Add first draft codes to main function based on Chara's first input
*/

#ifndef _SRC_METER_H
#define _SRC_METER_H

/*****************************************************************************************************
			include
******************************************************************************************************/
/*#include <math.h>
#include <stdlib.h>
#include <stdio.h>*/
#include "com_type.h"
#include "x_typedef.h"

/*****************************************************************************************************
			struct
******************************************************************************************************/
/**\struct videoInfo_t 
	\brief the definition of the videoInfo_t struct 
*/
typedef struct{
	WORD wResH; ///<Number of pixels horizontally per line of video source per field/frame (e.g. 1920)
	WORD wResV; ///<Number of lines of video source per field/frame. (e.g. 1080)
	WORD wHeight;
	WORD wLength;
	BYTE bResT;///<Number of field/fram per second (e.g. 24)
	BYTE bPI;///<Video source format (1: progressive; 0: interlaced)
}videoInfo_t;

/** \struct videoCodingInfo_t
	\brief The definition of videoCodingInfo_t struct
*/
typedef struct{
	/** bCodingType member, BYTE type
		- 0:analog video source
		- 1:MPEG1
		- 2:MPEG2
		- 3:MPEG4 part2
		- 4.MPEG4 Part10 (H.264)
		- 5.MPEG4 MVC (3D)
		- 6:WMV:(VC-1)
		- 7:RealMedia RM or RMVB
		- 8:Jpeg
		- 9:Motion Jpeg
		- A:others
	*/
	BYTE bCodingType;
	WORD wBitRate; ///<in Kbit/second, if bitrate is more than 25.5Mbits/sec, output 0xFFFF
}videoCodingInfo_t;

/** \struct mpegBlock_t
	\brief The definition of mpegBlock_t struct
*/
typedef struct{
	WORD wMpegH;///<MPEG block count in horizontal direction
	WORD wMpegV;///<MPEG block count in vertial direction
}mpegBlock_t;

/** \struct contributeFactors_t
	\brief The definition of contributeFactors_t
	
	The members of contributeFactors_t struct are mapped to cotnribution factors for peaking Band 1 to Band 11, whose range are 0 to 255 respectively.
	Final peaking applied in band "i" is peaking from PQ settings x Fi/255
*/
typedef struct{
	BYTE bFactor1;
	BYTE bFactor2;
	BYTE bFactor3;
	BYTE bFactor4;
	BYTE bFactor5;
	BYTE bFactor6;
	BYTE bFactor7;
	BYTE bFactor8;
	BYTE bFactor9;
	BYTE bFactor10;
	BYTE bFactor11;
}contributeFactors_t;


/****************************************************************************************************
			API
*****************************************************************************************************/
/**Provided API**/
/**
	\page init BYTE IMsrcmeterP_InitSrcAutoMeter(void)
	This function initializes registers and data of Video Source Quality Auto Indicator module
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterP_InitSrcAutoMeter(void);

/**
	\page feature BYTE IMsrcmeterP_SetSrcAutoMeterFeatureBYTEs(BYTE bFeatureBYTE1, BYTE bFeatureBYTE2)
	This function to set the feature BYTEs of Video Source Quality Auto Indicator module
	\param[in] bFeatureBYTE1, BYTE type
	- <I>refer to Requirement Spec</I>
	\param[in] bFeatureBYTE2, BYTE type
	- <I>refer to Requirement Spec</I>
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterP_SetSrcAutoMeterFeatureBYTEs(BYTE bFeatureBYTE1, BYTE bFeatureBYTE2);

/**
	\page vsync BYTE IMsrcmeterP_UpdateSrcAutoMeterVSync(void)
	This function update signal quality and sharpness values in v-sync interrupt service routine
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterP_UpdateSrcAutoMeterVSync(void);

/**
	\page source BYTE IMsrcmeterP_SetCurrentSource(BYTE bSource)
	By calling this function, MTK software can provide the current source information to Philips Video Signal Source Quality Auto Indicator module
	\param[in] bSource, BYTE type
	- 0: Analog TV (Terrestrial and Cable) 
	- 1: Digital TV-SD (Terrestrial and Cable) 
	- 2: Digital TV-HD (Terrestrial and Cable) 
	- 3: CVBS/SVHS (NTSC/PAL)
	- 4: YPbPr SD 480i/p, 576i/p 
	- 5: YPbPr HD 1080i/p, 720p 
	- 6: HDMI SD 480i/p, 576i/p 
	- 7: HDMI HD 1080i/p, 720p 
	- 8: PC Digital (HDMI PC) 
	- 9: USB Video Player SD 
	- A: USB Video Player HD
	- B: USB Photo
	- C: Net TV
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterP_SetCurrentSource(BYTE bSource);

/**
	\page run BYTE IMsrcmeterP_RunSrcAutoMeter(void)
	This is the main body of Video Source Quality Auto Indicator algorithm, it is called called every 20ms by MTK software
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterP_RunSrcAutoMeter(void);

/**
	\page pql BYTE IMsrcmeterP_EnablePQL2(BOOL bOnOff)
	This function is to enable or disable PQL2 function
	\param[in] bOnOff, Boolean type
	- ON: UI selection is On, PQL2 information is successfully received through HDMI CEC
	- OFF: UI selection is OFF or PQL2 information is not successfully received
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterP_EnablePQL2(BOOL bOnOff);

/**
	\page enable BYTE IMsrcmeterP_EnableSrcAutoMeter( BYTE bCondition)
	This function is to enable Video Source Quality Auto Indicator algorithm
	\param[in] bCondtion, BYTE type
	- 0: Video Source Quality Auto Meter agorithm is disabled
	- 1: It is in indication mode only
	- 2: It is under full feature mode
	\return
		- TRUE for success
		- FALSE for failed
	\remark
*/
BYTE IMsrcmeterP_EnableSrcAutoMeter( BYTE bCondition);

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
*/
BYTE IMsrcmeterR_GetVideoSynchNoise(void);

/**
	\page contentnoise WORD IMsrcmeterR_GetVideoContentNoise(void)
	This function is to get video content noise meter value
	\return
		- The video content noise meter value whose range is 0 to 65535
	\remark
*/
WORD IMsrcmeterR_GetVideoContentNoise(void);

/**
	\page resolution videoInfo_t IMsrcmeterR_GetVideoResolution(void)
	This function is to get video source resolution information for horizontal, vertical, temporal and the progressive or interlace
	\return
		- Video information in videoInfo_t struct type.
	\remark
		- <B>Note: for details of struct videoInfo_t struct, please refer to the struct definition</B>
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
*/
videoCodingInfo_t IMsrcmeterR_GetVideoCodingInfo(void);

/**
	\page mpeg mpegBlock_t IMsrcmeterR_GetMPEGBlock(void)
	This function is to get video MPEG block count for both Horizontal and vertical directions
	\return
		- Video MPEG block in mpegBlock_t struct
	\remark
		- <B>Note: please refer to mpegBlock_t for the details definition of the struct members. </B>
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

#endif //_SRC_METER_H
