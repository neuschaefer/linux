/*==========================================================================*/
/*
;     SOURCE_FILE:    imvideo_r.h
;     APPLICATION:    Funai2K9M
;     COMPONENT:      Video
;     VERSION:        %version: 101L_FB3 %
;     DATE:           %date_created: Thu Dec 24 11:42:04 2009 %
;     MODIFIER:       %derived_by: xuemd %
;
;     %full_filespec: imvideo_r.h,101L_FB3:incl:sgp88mtk#2 %
;
;     (C)2007 Philips Consumer Electronics - LoB Mainstream Displays
;
;     All rights are reserved. Reproduction in whole or in part is
;     prohibited without the prior written consent of the copyright
;     owner. The information presented in this document does not
;     form part of any quotation or contract, is believed to be
;     accurate and reliable and may be changed without notice.
;     No liability will be accepted by the publisher for any
;     consequence of its use. Publication thereof does not convey
;     nor imply any license under patent- or other industrial or
;     intellectual property rights.
*/
#ifndef _IMVIDEO_R_H
#define _IMVIDEO_R_H

/*==========================================================================*/
/*     Includes                                                             */
/*==========================================================================*/
//#include "../mcommon/mtypedef.h"
//#include "../mglue/mglue_api.h"

#include "mtypedef.h"
#include "imvideo_r_glue.h"



/*==========================================================================*/
/*     Typedefs, enums, defines                                             */
/*==========================================================================*/
// remove macro-map, these video drivers are ported to Philips code 
// #define Mvideo_DrvMainBrightness(bValue) vDrvMainBrightness(bValue)
// #define Mvideo_DrvMainContrast(bValue) vDrvMainContrast(bValue)
// #define Mvideo_DrvMainSaturation(bValue) vDrvMainSaturation(bValue)

#define PWM_180HZ_SIGNAL 180
#define PWM_10KHZ_SIGNAL 216 

/*==========================================================================*/
/*     Functions Prototypes                                                 */
/*==========================================================================*/
extern UINT8 bApiEepromReadByte(UINT16 wAddr) ;
extern BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData) ;
extern UINT8 bDrvANRGetLevel(void);
extern INT32 GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);

// APIs to access PQ related NVM address - refer respective header file for the offset used
#define IMvideoR_EepromReadOneByte(wAddr)  bApiEepromReadByte(wAddr)
#define IMvideoR_EepromWriteOneByte(wAddr, bData) fgApiEepromWriteByte(wAddr, bData)

// API to access PQ-extra flash data area
#define IMvideoR_GetFlashDataPQExtra(bData, wLength) MGlueVideoR_GetFlashDataPQExtra(bData, wLength)

// API to set GPIO pin
#define IMvideoR_OutputGPIO(i4GpioNum, pfgSet) GPIO_Output(i4GpioNum, pfgSet)

// APIs to acess full range of NVM address - u8Offset (absolute NVM address)
#define IMvideoR_EepromReadBytes(u8Offset, u4MemPtr, u4MemLen) MGlueVideoR_NVMRead(u8Offset, u4MemPtr, u4MemLen)
#define IMvideoR_EepromWriteBytes(u8Offset, u4MemPtr, u4MemLen) MGlueVideoR_NVMWrite(u8Offset, u4MemPtr, u4MemLen)

/************************************************************/
/****************** Required APIs ***************************/
/************************************************************/
/*==========================================================================
    FUNCTION NAME   : IMvideoR_SetMainBrightness
    DESCRIPTION     : Set Brightness
    INPUTS          : bBrightness (0x0=-127, 0x80 =x0, 0xFF = +127)
    OUTPUTS         : nil
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_SetMainBrightness(bBrightness) MGlueVideoR_SetMainBrightness(bBrightness)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_SetMainContrast
    DESCRIPTION     : Set Contrast
    INPUTS          : wContrast (0x0 =x0, 0x80 = x1, 0xFF= x1.992, 0x3FF = x7.992 )
    OUTPUTS         : nil
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_SetMainContrast(wContrast) MGlueVideoR_SetMainContrast(wContrast)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_SetMainSaturation
    DESCRIPTION     : Set Contrast
    INPUTS          : wContrast (0x0 =x0, 0x80 = x1, 0xFF= x1.992, 0x3FF = x7.992 )
    OUTPUTS         : nil
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_SetMainSaturation(wSaturation) MGlueVideoR_SetMainSaturation(wSaturation)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_SetPWMDutyCycle
    DESCRIPTION     : Set PWM duty cylce for backlight control
    INPUTS          : PWM duty cycle [0 .. 256]
    OUTPUTS         : nil
    CALLING SEQUENCE: nil
    REMARKS         : to control pin E20 - PWM1
  ==========================================================================*/
#define IMvideoR_SetPWMDutyCycle(wFreq, wDuty) MGlueVideoR_SetDimming(wFreq , wDuty)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_SetBacklightBoost
    DESCRIPTION     : Set PWM duty cycle value for backlight boost control
    INPUTS          : PWM duty cycle [0 .. 255]
    OUTPUTS         : nil
    CALLING SEQUENCE: nil
    REMARKS         : to control pin D20 - PWM2
  ==========================================================================*/
#define IMvideoR_SetBacklightBoost(wLevel) MGlueVideoR_SetBooster(wLevel)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetLumaHistogram
    DESCRIPTION     : Get Luma hist value for backlight control
    INPUTS          : nil
    OUTPUTS         : Luma Histogram (32 bins)
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_GetLumaHistogram(wHist)  MGlueVideoR_GetLumaHistogram(wHist)  

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetChromaHistogram
    DESCRIPTION     : Get Chroma hist value for backlight control
    INPUTS          : nil
    OUTPUTS         : Chroma Histogram (7 bins)
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_GetChromaHistogram(wHist) MGlueVideoR_GetChromaHist(wHist)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetRGBMax
    DESCRIPTION     : Get RGB Max hist value for backlight control
    INPUTS          : nil
    OUTPUTS         : RGB Max Histogram (16 bins)
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_GetRGBMax(dwHist) MGlueVideoR_GetRGBMax(dwHist)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_SetRGBMaxBoundary
    DESCRIPTION     : Set RGB Max Boundary value for backlight control
    INPUTS          : nil
    OUTPUTS         : RGB Max Histogram Boundary 
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_SetRGBMaxBoundary(abBoundary) MGlueVideoR_SetRGBMaxBoundary(abBoundary)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetLightSensorADCVal
    DESCRIPTION     : Read ADC value for light sensor
    INPUTS          : nil
    OUTPUTS         : light sensor ADC value ( 0 - 255)
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_GetLightSensorADCVal() MGlueVideoR_GetLightSensorADCVal()

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetAverageLuma
    DESCRIPTION     : Get APL for current display
    INPUTS          : nil
    OUTPUTS         : APL value
    CALLING SEQUENCE: nil
    REMARKS         : Average Video
  ==========================================================================*/
#define IMvideoR_GetAverageLuma() MGlueVideoR_GetAverageLuma()   

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetCurMappedBrightnessValue
    DESCRIPTION     : Get the mapped brightness value (after the OSD slider and source mapping)
    INPUTS          : nil
    OUTPUTS         : Mapped brightness value [0 .. 255]
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_GetCurMappedBrightnessValue()  MGlueVideoR_GetCurMappedBrightness()

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetCurMappedContrastValue 
    DESCRIPTION     : Get the mapped contrast value
    INPUTS          : nil
    OUTPUTS         : Mapped contrast value [0 .. 255]
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_GetCurMappedContrastValue()   MGlueVideoR_GetCurMappedContrast()

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetCurMappedColourValue 
    DESCRIPTION     : Get the mapped colour value
    INPUTS          : nil
    OUTPUTS         : Mapped colour value [0 .. 255]
    CALLING SEQUENCE: nil
    REMARKS         : nil
  ==========================================================================*/
#define IMvideoR_GetCurMappedColourValue()    MGlueVideoR_GetCurMappedSaturation() 

/*==========================================================================
    FUNCTION NAME   : IMvideoR_SetChromaHistBoundary
    DESCRIPTION     : Set the boundaries for Chroma Histogram (7 bins)
    INPUTS          : bHistBoundary 
    OUTPUTS         : nil
    CALLING SEQUENCE: nil
    REMARKS         : n.ycproc.al.chistb 
  ==========================================================================*/
#define IMvideoR_SetChromaHistBoundary(bHistBoundary) MGlueVideoR_SetChromaHistBoundary(bHistBoundary)

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetVideoDelay
    DESCRIPTION     : Get Video Delay from MTK
    INPUTS          : nil
    OUTPUTS         : nil
    CALLING SEQUENCE: nil
    REMARKS         : n.ycproc.fdl
  ==========================================================================*/
#define IMvideoR_GetVideoDelay() MGlueVideoR_GetVideoDelay()

/*==========================================================================
    FUNCTION NAME   : IMvideoR_GetNoiseMeterLevel
    DESCRIPTION     : Get the noise level based on MTK5382 noise meter
    INPUTS          : nil
    OUTPUTS         : Quantize effective noise levels [0 .. 15]
    CALLING SEQUENCE: nil
    REMARKS         : Noise Meter reading for RF only
  ==========================================================================*/
#define IMvideoR_GetNoiseMeterLevel() bDrvANRGetLevel()
/*==========================================================================*/
/*     END OF FILE                                                          */
/*==========================================================================*/
#endif // _IMVIDEO_R_H

