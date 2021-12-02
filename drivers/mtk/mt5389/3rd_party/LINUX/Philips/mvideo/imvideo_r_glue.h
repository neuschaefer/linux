#ifndef _IMVIDEO_R_GLUE_H
#define _IMVIDEO_R_GLUE_H

/*==========================================================================*/
/*     I N C L U D E S                                                      */
/*==========================================================================*/

//#include "../mcommon/u_common.h"
#include "u_common.h"


/*==========================================================================*/
/* Functions Prototypes     			                                    */
/*==========================================================================*/
//Below are glue functions used in imvideo_r.h.

//Previously declared in "../mglue/mglue_api.h"
// API to access PQ-extra flash data area
void MGlueVideoR_GetFlashDataPQExtra(UINT8 *Data, UINT16 Length); 

// APIs to acess full range of NVM address - u8Offset (absolute NVM address)
INT32 MGlueVideoR_NVMRead(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen); 
INT32 MGlueVideoR_NVMWrite(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen); 

//Previously declared in "imvideo_r.h"
// Required APIs
UINT8 MGlueVideoR_GetCurMappedBrightness(void); 
UINT8 MGlueVideoR_GetCurMappedContrast(void); 
UINT8 MGlueVideoR_GetCurMappedSaturation(void); 
UINT8 MGlueVideoR_GetAverageLuma(void); 
void MGlueVideoR_SetDimming(UINT16 wFreq, UINT16 wDuty); 
void MGlueVideoR_SetBooster(UINT16 wLevel); 
void MGlueVideoR_GetChromaHist(UINT16 *wHist); 
void MGlueVideoR_SetMainContrast(UINT16 u2Value); 
void MGlueVideoR_SetMainSaturation(UINT16 u2Value); 
void MGlueVideoR_SetMainBrightness(UINT8 bValue); 
void MGlueVideoR_GetRGBMax(UINT16 *dwHist); 
void MGlueVideoR_SetRGBMaxBoundary(UINT8 *u1Boundary); 
UINT8 MGlueVideoR_GetLumaHistogram(UINT16 *wHist); 
void MGlueVideoR_SetChromaHistBoundary(UINT8 *u1Boundary); 
UINT8 MGlueVideoR_GetLightSensorADCVal(void); 
UINT8 MGlueVideoR_GetVideoDelay(void); 

#endif /* _IMVIDEO_R_GLUE_H */
