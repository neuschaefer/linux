/*==========================================================================*/
/* Includes                  	                                            */
/*==========================================================================*/
#include "imvideo_r_glue.h"
#include "u_common.h"
#include "storage_if.h"
#include "drv_meter.h"

/*==========================================================================*/
/* LOCAL VARIABLES															*/
/*==========================================================================*/
#define NEW_VIDEO_API

#ifdef NEW_VIDEO_API
extern UINT16 bApiGetMappedContrast(void);
extern UINT16 bApiGetMappedSaturation(void);
extern void vApiGetRGBMaxHistNorm(UINT16 * dwHist);  // RGB max
#else
extern UINT8 bApiGetMappedContrast(void);
extern UINT8 bApiGetMappedSaturation(void);
extern void vApiGetRGBMaxHistNorm(UINT32 * dwHist);  // RGB max
#endif

extern UINT8 bApiGetMappedBrightness(void);

//Inputs to the Auto BL algorithm/Input to MD software control
extern UINT8 u1DrvGetLumaHist(UINT16 waHistCur[LUMA_HIST_NUM], UINT16 u2NormBase);// luma histogram
extern UINT8 u1DrvGetSatHist(UINT16 u2aHist[SAT_HIST_NUM], UINT16 u2NormBase);   // chroma histogram
extern void vDrvSetSatHistBoundary(UINT8 *u1Boundary); 
extern UINT8 bDrvGetAPL(void);                  // average video
extern void vDrvSetRGBMaxBoundary(UINT8 *u1Boundary); // RGB max boundary
//extern UINT8 bApiGetVideoDelay(void);
extern UINT8 vApiGetLightSensorADCVal(void); 

//Outputs from the Auto BL algorithm/Output from MD software to MTK
extern void vHalVideoBrightness(UINT8 bPath, UINT8 bValue); 
extern void vHalVideoContrast(UINT8 bPath, UINT16 u2Value); 
extern void vHalVideoSaturation(UINT8 bPath, UINT16 u2Value); 
extern void vApiSetDimming(UINT16 u2Freq, UINT16 u2Duty);
extern void vApiSetBooster(UINT16 wLevel); 


extern void vApiFlashPqGetReserveData(UINT8 *bData, UINT16 wLength);

/*==========================================================================*/
/* GLOBAL FUNCTIONS			 												*/
/*==========================================================================*/

// API to access PQ-extra flash data area
void MGlueVideoR_GetFlashDataPQExtra(UINT8 *Data, UINT16 Length)
{
    vApiFlashPqGetReserveData(Data, Length);
//return;
} 

// APIs to acess full range of NVM address - u8Offset (absolute NVM address)
INT32 MGlueVideoR_NVMRead(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    STORG_DEV_T *prDev;
    INT32 i4Ret;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);
    if (prDev == NULL) {
        return -1;
    }

    i4Ret = STORGSendReq(prDev, STORG_OP_READ, NULL, NULL, NULL,
                    u8Offset, u4MemLen, (void *)u4MemPtr, NULL);

    STORGCloseDev(prDev);
    return i4Ret;
}  
INT32 MGlueVideoR_NVMWrite(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    STORG_DEV_T *prDev;
    INT32 i4Ret;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);
    if (prDev == NULL) {
        return -1;
    }

    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE, NULL, NULL, NULL,
                    u8Offset, u4MemLen, (void *)u4MemPtr, NULL);

    STORGCloseDev(prDev);
    return i4Ret;
} 


// Required APIs
UINT8 MGlueVideoR_GetCurMappedBrightness(void)
{
    return bApiGetMappedBrightness();
} 
UINT8 MGlueVideoR_GetCurMappedContrast(void)
{
    return (UINT8) bApiGetMappedContrast();
}  
UINT8 MGlueVideoR_GetCurMappedSaturation(void)
{
    return (UINT8) bApiGetMappedSaturation();
}  
UINT8 MGlueVideoR_GetAverageLuma(void)
{
    return bDrvGetAPL();                  
}  


void MGlueVideoR_SetDimming(UINT16 wFreq, UINT16 wDuty)
{
    vApiSetDimming(wFreq, wDuty);
}  

void MGlueVideoR_SetBooster(UINT16 wLevel)
{
    vApiSetBooster(wLevel);
}  

void MGlueVideoR_GetChromaHist(UINT16 *wHist)
{
    u1DrvGetSatHist(*wHist, 0xFFFF);
}  

void MGlueVideoR_SetMainContrast(UINT16 u2Value)
{
    vHalVideoContrast(0, u2Value);  // main video path
}  
void MGlueVideoR_SetMainSaturation(UINT16 u2Value)
{
    vHalVideoSaturation(0, u2Value);  // main video path
}  

void MGlueVideoR_SetMainBrightness(UINT8 bValue)
{
    vHalVideoBrightness(0, bValue);  // main video path
}  

void MGlueVideoR_GetRGBMax(UINT16 *dwHist)
{
return;
}  

void MGlueVideoR_SetRGBMaxBoundary(UINT8 *u1Boundary)
{
    //vDrvSetRGBMaxBoundary(u1Boundary);  //delete by lifa cao 
    return; 
}  

UINT8 MGlueVideoR_GetLumaHistogram(UINT16 *wHist)
{
    return (u1DrvGetLumaHist(*wHist, 0xFFFF));
}  

void MGlueVideoR_SetChromaHistBoundary(UINT8 *u1Boundary)
{
    vDrvSetSatHistBoundary(u1Boundary);
} 
UINT8 MGlueVideoR_GetLightSensorADCVal(void)
{
    return (vApiGetLightSensorADCVal()); 
}  
UINT8 MGlueVideoR_GetVideoDelay(void)
{
    return (0);
}  


