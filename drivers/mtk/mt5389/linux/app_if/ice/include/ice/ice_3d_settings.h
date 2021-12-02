
#ifndef CABOT_ICE_ICE_3D_SETTINGS_H_INCLUDED
#define CABOT_ICE_ICE_3D_SETTINGS_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_video_types.h"
#include "config/include_project_median.h"
#include "config/include_project_phoenix.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ICE_2D_Only_Mode
{
    ICE_2D_Only_Mode_Off = 0,
    ICE_2D_Only_Mode_Left,
    ICE_2D_Only_Mode_Right,
    ICE_2D_Only_Mode_invalid

} ICE_2D_Only_Mode;

typedef enum ICE_3D_Video_Scaling_Modes
{
    ICE_3D_Scaling_Off = 0,
    ICE_3D_Scaling_Left,
    ICE_3D_Scaling_Right,
    ICE_3D_Scaling_Top,
    ICE_3D_Scaling_Bottom,
    ICE_3D_Scaling_Invalid
} ICE_3D_Video_Scaling_Modes;

typedef enum ICE_3D_Virtual_3D_Mode
{   
    ICE_3D_Virtual_3D_Mode_Off = 0,
    ICE_3D_Virtual_3D_Mode_Low,
    ICE_3D_Virtual_3D_Mode_Medium,
    ICE_3D_Virtual_3D_Mode_High,
    ICE_3D_Virtual_3D_Mode_invalid

}ICE_3D_Virtual_3D_Mode;

typedef enum ICE_3D_Mode
{
    ICE_3D_Mode_Off,
    ICE_3D_Mode_Auto,
    ICE_3D_Mode_SBS,
    ICE_3D_Mode_TB,
    ICE_3D_Mode_Virtual,
    ICE_3D_Mode_FramePacked,
    ICE_3D_Mode_invalid
}ICE_3D_Mode;

typedef enum ICE_DVB_3D_Format
{
    ICE_DVB_3D_Format_None,
    ICE_DVB_3D_Format_SBS,
    ICE_DVB_3D_Format_TB
}ICE_DVB_3D_Format;

typedef enum ICE_3D_Extented_Status
{
    ICE_3D_ES_Pix_LVDS_Stable_Set,
    ICE_3D_ES_VideoPositionUpdateFor3D,
    ICE_3D_ES_IsPortalMode,
    ICE_3D_ES_IsFlashAppStarted,
    ICE_3D_ES_ForceVideoPosition,
    ICE_3D_ES_Count
}ICE_3D_Extented_Status;

#define DEPTH_REG_LEVEL_1     2//minus
#define DEPTH_REG_LEVEL_2     4//minus
#define DEPTH_REG_LEVEL_3     6//minus
#define DEPTH_REG_LEVEL_4     7//minus
#define DEPTH_REG_LEVEL_5     8//minus
#define DEPTH_REG_LEVEL_OFF 0


#define EFFECT_MODE_LOW 0
#define EFFECT_MODE_MED 1
#define EFFECT_MODE_HIGH 2


#define EFFECT_REG_LOW 1
#define EFFECT_REG_MED 2
#define EFFECT_REG_HIGH 4

#define DEPTH_POSITIVE 0
#define DEPTH_NEGATIVE 1

#define FPGA_MODE_3D_SBS  0x01
#define FPGA_MODE_3D_TB    0x03
#define FPGA_MODE_2D          0x00

typedef enum ICE_3D_Vsif3DModes
{
    ICE_3D_Vsif_SBS,   // Side-by-Side
    ICE_3D_Vsif_TB,    // Tob Bottom
    ICE_3D_Vsif_FP,    // Frame Packed
    ICE_3D_Vsif_NA     // Not Available
} ICE_3D_Vsif3DModes;


typedef enum ICE_3D_Allow3DSources
{
    ALLOW3D_SRC_HDMI,
    ALLOW3D_SRC_MBVID,
    ALLOW3D_SRC_MBPIC,
    ALLOW3D_SRC_DTV,
    ALLOW3D_SRC_ATV,
#if !(defined(INCLUDE_PROJECT_MEDIAN) || defined(INCLUDE_PROJECT_PHOENIX))
    ALLOW3D_SRC_PORTAL,
    ALLOW3D_SRC_FLASH,
#endif
    ALLOW3D_SRC_OTHER
}ICE_3D_Allow3DSources;

typedef enum ICE_3D_Allow3DResolutions
{
    ALLOW3D_RES_1080p24FP,
    ALLOW3D_RES_720p50FP,
    ALLOW3D_RES_720p60FP,
    ALLOW3D_RES_1080pFC,
    ALLOW3D_RES_720pFC,
    ALLOW3D_RES_1080iFC,
    ALLOW3D_RES_Other
}ICE_3D_Allow3DResolutions;


frost_bool ICE_set3DMode(frost_int mode);

frost_bool  ICE_set2DOnlyMode(frost_uint8 mode);

frost_uint8 ICE_get2DOnlyMode(void);

frost_bool ICE_set3DAutoViewEnabled(frost_bool  enable);

frost_bool ICE_is3DSignalDetected(void);

ICE_DVB_3D_Format ICE_getDvb3DFormat(void);

frost_bool ICE_is3DModeDetected(void);

frost_bool ICE_getPanel3DStatus(void);

frost_bool ICE_is3DAutoViewEnabled(void);

frost_bool ICE_is3DTBAvailable(void);

frost_bool ICE_is3DSBSAvailable(void);

frost_uint8  ICE_get3DSignalStatus(void);

/*return true if panel was set to 3D*/
frost_bool ICE_getPanel3DStatus(void);

frost_bool ICE_Virtual3DEnabled(frost_bool enable);

frost_bool ICE_setVirtual3DEffectMode(frost_uint8 mode);

frost_uint8 ICE_getVirtual3DMode(void);

frost_uint8 ICE_getPanel3DMode(void);

frost_bool  ICE_setPanel3DMode(frost_int mode);

frost_bool ICE_set3DStatusMessageDisplayed(frost_bool enable);

frost_bool ICE_is3DStatusMessageDisplayed(void);

frost_uint8 ICE_get3DPanelType(void);

frost_int ICE_get3DExtendedStatus(ICE_3D_Extented_Status Status, frost_bool ResetStatus);

frost_bool ICE_set3DExtendedStatus(ICE_3D_Extented_Status Status, frost_int NewValue);

ICE_3D_Video_Scaling_Modes ICE_get3DScalingMode(void);

ICE_3D_Vsif3DModes ICE_getVsif3DMode(void);

void ICE_update3DImageDisplay(void);

frost_bool ICE_is3DAllowed(void);

frost_bool ICE_isVirtual3DSupported(void);

ICE_VideoTVZoomMode ICE_get3DZoomMode(void);

frost_bool ICE_isMFCAvailable(void);

frost_bool ICE_set3DDepthSense(frost_int8 level);

frost_bool ICE_setLeftRightSwap(frost_bool invert);

frost_bool ICE_getLeftRightSwap(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_3D_SETTINGS_H_INCLUDED*/


