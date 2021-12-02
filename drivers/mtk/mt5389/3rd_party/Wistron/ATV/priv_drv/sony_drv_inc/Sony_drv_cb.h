/*!
 * \file Sony_drv_cb.h
 *
 *  - Sony driver callback header for Rafflesia Sony Library
 *
 *
 * SONY TVEG
 *
 * Copyright (c) SONY Corporation
 *
 * Author : Ramesh.Silvarajo@ap.sony.com : 23 July 2009 : Created
 *
 */
 
#ifndef _SONY_DRV_CB_H_
#define _SONY_DRV_CB_H_

/*** Include files ******************************************************************/
#include "u_common.h"

/*** Constant definitions ***********************************************************/

/* When using a slot in callback func set array, update the index here */
enum{
CB_DRV_DBC_PARAM = 0,            
CB_DRV_DBC4PS_ENABLE,        
CB_DRV_DBC_ENABLE,          
CB_DRV_NO_SIG_GAIN,
CB_DRV_SET_DIMMING,
CB_DRV_GAM_OSD_CONTROL,
CB_DRV_GAM_PATGEN_GAM,
CB_DRV_GAM_PATGEN_SCE,
CB_DRV_GAM_LOADTBL,
CB_DRV_WB_GAINOFST,
CB_DRV_IP_GET_NOISE,
CB_DRV_IP_GET_SIGNAL_STATUS,
CB_DRV_IP_IS_CVBS,
CB_DRV_SIF_READ,
CB_DRV_SIF_WRITE,
CB_DRV_IS_RGB_SENSOR,
CB_DRV_BL_PWM_SWITCH,
CB_DRV_BL_24P_STATUS,
CB_DRV_SET_AGING_BIT,
CB_DRV_X_MEM_ALLOC,
CB_DRV_X_MEM_CALLOC,
CB_DRV_X_MEM_REALLOC,
CB_DRV_X_MEM_FREE,
CB_DRV_LNA_GET_NOISE_LVL,
CB_DRV_LNA_GET_IFAGC,
CB_DRV_LNA_SET_LNA,
CB_DRV_DBL_PARAM,
CB_DRV_ATD_GETREG,
CB_DRV_ATD_SETAGCPARA,
CB_DRV_IP_IS_CHANCHG,
CB_DRV_TUNER_DEVDATA_R,
CB_DRV_TUNER_DEVDATA_W,
CB_DRV_TUNER_GET_IP_STATUS,
CB_DRV_IS_PC_HDMI_PC,

CB_DRV_FUNC_COUNT
};
        
#define EXT_PWM                     0
#define MTK_PWM                     1
#define Y_HISTGRAM_POINT_NUM        32
#define C_HISTGRAM_POINT_NUM        8

/*** Type definitions ***************************************************************/

typedef void (* cbSonyDrv)  (void* pvArg);

typedef struct _SONY_DRV_CB_ST
{
	cbSonyDrv  cbDrv[CB_DRV_FUNC_COUNT];
} SONY_DRV_CB_ST;

/* cbSony_AutoBacklightGetDbcParam */
typedef struct _ST_PARAM_DBC_MAIN
{
	UINT16  aplm;               
	BOOL    scene_change_det;
	UINT16  wHist32[Y_HISTGRAM_POINT_NUM];
	UINT8   luma_max;
	UINT16  cHist8[C_HISTGRAM_POINT_NUM];
} ST_PARAM_DBC_MAIN;

typedef struct _ST_PARAM_DIMMER
{
    UINT16  u2_pwm_Freq;
    UINT16  u2_pwm_Duty;
} ST_PARAM_DIMMER;

/* cbSony_GammaSetPattern */
typedef struct _ST_PARAM_GAM_PTG
{
	BOOL		onOff;		/* gamma patgen state */
	UINT16		wGammaR;	/* R gamma */
	UINT16		wGammaG;	/* G gamma */
	UINT16		wGammaB;	/* B gamma */
} ST_PARAM_GAM_PTG;

/* cbSony_GammaSetSCEPattern */
typedef struct _ST_PARAM_SCE_PTG
{
	BOOL		onOff;		/* wb patgen state */
	BOOL		sceProc;	/* sce process bypass, 0:normal, 1:bypass */
	UINT16		data; 		/* Y data level */
} ST_PARAM_SCE_PTG;


typedef struct _ST_PARAM_WB_T
{
	UINT16 wGainR, wGainG, wGainB;
	UINT16 wOfstR, wOfstG, wOfstB;
} ST_PARAM_WB_T;

typedef struct _ST_PARAM_SIF_T
{
	UINT16 u2ClkDiv;
	UINT8 u1DevAddr;
	UINT8 u1WordAddr;
	UINT8 *pu1Buf;
	UINT16 u2ByteCnt;
	UINT16 u2Ret;
} ST_PARAM_SIF_T;

typedef struct _PARAM_X_MEM_MGMT
{
  VOID*   pv_mem_addr;
  VOID*   pv_mem_block;
  SIZE_T  z_size;
  UINT32  ui4_num_element;
} PARAM_X_MEM_MGMT;

typedef struct _ST_PARAM_SetLNA
{
    BOOL  fgLNA;
    BOOL  SetSuccess;
} ST_PARAM_SetLNA;

typedef struct _ST_PARAM_DBL_CTRL
{
    UINT8   bl_top;
    UINT8   bl_mid;
    UINT8   bl_bottom;
    UINT16  bl_pwm_frq;
    UINT16  bl_24p_pwm_frq;
    UINT8   bl_mid_knee;
    UINT8   bl_setuden;
    UINT8   bl_psh;
    UINT8   bl_cri_onoff;
    UINT8   bl_cri_pwm_frq;
    UINT8   bl_cri_duty;
    UINT8   bl_max_duty;
    UINT8   bl_min_duty;
    UINT8   d_dimmer_th_en;
    UINT8   d_dimmer_th_h;
    UINT8   d_dimmer_th_l;
    
    UINT8   bl_temp_th_low;
    UINT8   bl_temp_th_high;
    UINT8   temp_bl_limit;
    UINT8   bl_temp_cntth;
    UINT8   bl_temp_inc;
    UINT8   temp_bl_bottom;
    UINT8   opt_bl_deadb;
    UINT8   opt_bl_deadb_cin;
    UINT8   bl_cri_temp_l;
    UINT8   bl_cri_temp_h;
} ST_PARAM_DBL_CTRL;
typedef struct _ST_PARAM_ATD_GETSETREG
{
    UINT16  RegAddr;
    UINT8*  pData;
    UINT8   ByteCnt;
    BOOL   Success;
} ST_PARAM_ATD_GETSETREG;
typedef struct _ST_SONY_TUNER_DATA
{
    UINT8   TU_DATA;
    UINT16  value;
    BOOL    success;
} ST_SONY_TUNER_DATA;

/*** Global functions ***************************************************************/

extern INT8 sonyDrvRegisterCallback(SONY_DRV_CB_ST* callback_funcs);
#ifdef SONY_PRIV_DRV_INC_USE /* Begin - Put all function prototype for Sony private file usage */
extern void vAutoBacklightGetDbcParam(ST_PARAM_DBC_MAIN* param);
extern void vAutoBacklightGetDbc4psEnable(BOOL* dbc4psEnable);
extern void vAutoBacklightGetDbcEnable(BOOL* dbcEnable);
extern void vAutoBacklightGetNoSigGain(BOOL* noSigGain);
extern void vAutoBacklightSetDimming(ST_PARAM_DIMMER* param);

extern void vGammaOSDControl(BOOL* param);
extern void vGammaSetPattern(ST_PARAM_GAM_PTG* param);
extern void vGammaSetSCEPattern(ST_PARAM_SCE_PTG* param);
extern void vGammaLoadTable(VOID* param);
extern void vWBSetGainOfst(ST_PARAM_WB_T* param);

extern void vIPGetNoise (UINT8* noise_lvl);
extern void vIPGetSignalStatus (BOOL* sig_status);
extern void vIPIsCVBS (BOOL* param);

extern void vIsRgbSensor (UINT32* param);
extern UINT16 vSIFWrite(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
                        UINT8 *pu1Buf, UINT16 u2ByteCnt);
                        
extern UINT16 vSIFRead(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
                        UINT8 *pu1Buf, UINT16 u2ByteCnt);
                        
extern void vAutoBacklightPwmSwitch(INT32 u1_switch);

extern void vAutoBacklightGet24P(BOOL* param);

extern VOID* sony_x_mem_alloc(SIZE_T  z_size);
extern VOID* sony_x_mem_calloc(UINT32  ui4_num_element, SIZE_T  z_size_element);
extern VOID* sony_x_mem_realloc(VOID*  pv_mem_block, SIZE_T z_new_size);
extern VOID sony_x_mem_free(VOID*  pv_mem_block);
extern UINT32 vSigBstrGetNoiseLevel(void);
extern UINT16 vSigBstrGetIFAGC(void);
extern BOOL vSigBstrSetLNA(BOOL setLNA);
extern void vAutoBacklightGetDblParam(ST_PARAM_DBL_CTRL* param);
extern void vSonyTunerGetReg(ST_PARAM_ATD_GETSETREG* param);
extern void vSonyTunerSetAGCReg(ST_PARAM_ATD_GETSETREG* param);
extern void vIPIsChannelChange(BOOL* param);
extern void vSonyTunerGetDevData(void* pvArg);
extern void vSonyTunerSetDevData(void* pvArg);
extern void vSonyTunerGetIPStatus(void* pvArg);
extern void vSonyIsPcHdmiPc(BOOL* pvArg);
#endif /* End - Put all function prototype for Sony private file usage */
#endif
