#ifndef _VIDEO_DISPLAY_RTC_DDI_H_
#define _VIDEO_DISPLAY_RTC_DDI_H_

#define PARAM_NAME_OD_COUNT	12
#define OD_LUT_SIZE	17
#define OD_CON_FACT	4
#define OD_PARAM_INDEX(F) (((UINT32)&((NEXUS_DisplayRtcTuningSettings*)0)->F)/sizeof(UINT32))

typedef UINT32 (*OD_LUT_LINE_T)[OD_LUT_SIZE];
typedef UINT32 (*OD_LUT_LINE_T2)[OD_LUT_SIZE*2];

typedef enum _OD_PARAM_E_{
     OD_PARAM_rtcEnable,
     OD_PARAM_edgeCntl,
     OD_PARAM_saturationThresholdHigh,
     OD_PARAM_saturationThresholdLow,
     OD_PARAM_whiteRatioThreshold,
     OD_PARAM_whiteAbsoluteHigh,
     OD_PARAM_whiteAbsoluteLow,
     OD_PARAM_lutDataFormat,
     OD_PARAM_lutCh0,
     OD_PARAM_lutCh1,
     OD_PARAM_lutCh2,
     OD_PARAM_outputSel,
     OD_PARAM_MAX
} OD_PARAM_E;

extern void VIDEO_RTC_DEBUG_test(void);
extern char const * param_name_OD_str[PARAM_NAME_OD_COUNT];
extern void VIDEO_RTC_writePara(OD_PARAM_E paramName, UINT32 *data);
extern void VIDEO_RTC_readPara(OD_PARAM_E paramName, UINT32 *data);
#endif
