/**
 * Copyright 2008 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface to retrieve CEC data. 
 *
 */

#ifndef CABOT_ICE_ICE_CEC_IDS_H_INCLUDED
#define CABOT_ICE_ICE_CEC_IDS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * High-Definition Multimedia Specification Version 1.3a
 * CEC 15 Message Descriptions
 */
typedef enum ICE_CecOpcode
{
    ICE_CecOpcode_ActiveSource           = 0x82,
    ICE_CecOpcode_InactiveSource         = 0x9D,
    ICE_CecOpcode_ImageViewOn            = 0x04,
    ICE_CecOpcode_TextViewOn             = 0x0D,
    ICE_CecOpcode_Standby                = 0x36,
    ICE_CecOpcode_RoutingChange          = 0x80, 
    ICE_CecOpcode_RoutingInfomation      = 0x81,
    ICE_CecOpcode_RequestActiveSource    = 0x85,
    ICE_CecOpcode_SetStreamPath          = 0x86,
    ICE_CecOpcode_CecVersion             = 0x9E,
    ICE_CecOpcode_GetCecVersion          = 0x9F,
    ICE_CecOpcode_ReportPhysicalAddr     = 0x84,
    ICE_CecOpcode_GivePhysicalAddr       = 0x83,
    ICE_CecOpcode_GetMenuLanguage        = 0x91,
    ICE_CecOpcode_SetMenuLanguage        = 0x32,
    ICE_CecOpcode_DeckControl            = 0x42,    
    ICE_CecOpcode_Play                   = 0x41,
    ICE_CecOpcode_DeckStatus             = 0x1B,
    ICE_CecOpcode_GetDeckStatus          = 0x1A,
    ICE_CecOpcode_TuneStepIncrement      = 0x05,
    ICE_CecOpcode_TuneStepDecrement      = 0x06,
    ICE_CecOpcode_TunerStatus            = 0x07,
    ICE_CecOpcode_GetTunerStatus         = 0x08,
    ICE_CecOpcode_SelectAnalogueService  = 0x92,
    ICE_CecOpcode_SelectDigitalService   = 0x93,
    ICE_CecOpcode_DeviceVendorId         = 0x87,
    ICE_CecOpcode_GetDeviceVendorId      = 0x8C,
    ICE_CecOpcode_VendorCommand          = 0x89,
    ICE_CecOpcode_VendorCommandWithId    = 0xA0,
    ICE_CecOpcode_VendorRemoteButtonDown = 0x8A,
    ICE_CecOpcode_VendorRemoteButtonUp   = 0x8B,
    ICE_CecOpcode_SetOSDString           = 0x64,
    ICE_CecOpcode_GetOSDName             = 0x46,
    ICE_CecOpcode_SetOSDName             = 0x47,
    ICE_CecOpcode_Abort                  = 0xFF,
    ICE_CecOpcode_FeatureAbort           = 0x00,
    ICE_CecOpcode_GivePowerStatus        = 0x8F,
    ICE_CecOpcode_ReportPowerStatus      = 0x90,
    ICE_CecOpcode_UserControlPressed     = 0x44,
    ICE_CecOpcode_UserControlReleased    = 0x45,
    ICE_CecOpcode_GiveAudioStatus        = 0x71,
    ICE_CecOpcode_ReportAudioStatus      = 0x7A,    
    ICE_CecOpcode_GiveAudioModeStatus    = 0x7D,
    ICE_CecOpcode_SetSystemAudioMode     = 0x72, 
    ICE_CecOpcode_SystemAudioModeRequest = 0x70,
    ICE_CecOpcode_SystemAudioModeStatus  = 0x7E,
    ICE_CecOpcode_InitiateARC            = 0xC0,
    ICE_CecOpcode_ReportARCInitiated     = 0xC1,
    ICE_CecOpcode_ReportARCTerminated    = 0xC2,
    ICE_CecOpcode_RequestARCInitiation   = 0xC3,
    ICE_CecOpcode_RequestARCTermination  = 0xC4,
    ICE_CecOpcode_TerminateARC           = 0xC5
} ICE_CecOpcode;

/**
 * High-Definition Multimedia Specification Version 1.3a
 * CEC 27 User Control Codes.
 */
typedef enum ICE_CecUserCode
{
    ICE_CecUserCode_Select = 0,
    ICE_CecUserCode_Up,
    ICE_CecUserCode_Down,
    ICE_CecUserCode_Left,
    ICE_CecUserCode_Right,
    ICE_CecUserCode_RightUp,
    ICE_CecUserCode_RightDown,
    ICE_CecUserCode_LeftUp,
    ICE_CecUserCode_LeftDown,
    ICE_CecUserCode_RootMenu,
    ICE_CecUserCode_SetupMenu,
    ICE_CecUserCode_ContentsMenu,
    ICE_CecUserCode_FavoriteMenu,
    ICE_CecUserCode_Exit,

    ICE_CecUserCode_Digit0 = 32,
    ICE_CecUserCode_Digit1,
    ICE_CecUserCode_Digit2,
    ICE_CecUserCode_Digit3,
    ICE_CecUserCode_Digit4,
    ICE_CecUserCode_Digit5,
    ICE_CecUserCode_Digit6,
    ICE_CecUserCode_Digit7,
    ICE_CecUserCode_Digit8,
    ICE_CecUserCode_Digit9,
    ICE_CecUserCode_Dot,
    ICE_CecUserCode_Enter,
    ICE_CecUserCode_Clear,

    ICE_CecUserCode_NextFavorite = 47,
    ICE_CecUserCode_ChannelUp,
    ICE_CecUserCode_ChannelDown,
    ICE_CecUserCode_PreviousChannel,

    ICE_CecUserCode_SoundSelect,
    ICE_CecUserCode_InputSelect,
    ICE_CecUserCode_DisplayInfo,
    ICE_CecUserCode_Help,
    ICE_CecUserCode_PageUp,
    ICE_CecUserCode_PageDown,

    ICE_CecUserCode_Power = 64,
    ICE_CecUserCode_VolumeUp,
    ICE_CecUserCode_VolumeDown,
    ICE_CecUserCode_Mute,
    ICE_CecUserCode_Play,
    ICE_CecUserCode_Stop,
    ICE_CecUserCode_Pause,
    ICE_CecUserCode_Record,
    ICE_CecUserCode_Rewind,
    ICE_CecUserCode_FastForward,
    ICE_CecUserCode_Eject,
    ICE_CecUserCode_Forward,
    ICE_CecUserCode_Backward,
    ICE_CecUserCode_StopRecord,
    ICE_CecUserCode_PauseRecord,

    ICE_CecUserCode_Angle = 80,
    ICE_CecUserCode_SubPicture,
    ICE_CecUserCode_VideoOnDemand,
    ICE_CecUserCode_EPG,
    ICE_CecUserCode_TimerProgramming,
    ICE_CecUserCode_InitialConfiguration,

    ICE_CecUserCode_PlayFunction = 96,
    ICE_CecUserCode_PausePlayFunction,
    ICE_CecUserCode_RecordFunction,
    ICE_CecUserCode_PauseRecordFunction,
    ICE_CecUserCode_StopFunction,
    ICE_CecUserCode_MuteFunction,
    ICE_CecUserCode_RestoreVolumneFunction,
    ICE_CecUserCode_TuneFunction,
    ICE_CecUserCode_SelectMediaFunction,
    ICE_CecUserCode_SelectAVInputFunction,
    ICE_CecUserCode_selectAudioFunction,
    ICE_CecUserCode_PowerToggleFunction,
    ICE_CecUserCode_PowerOffFunction,
    ICE_CecUserCode_PowerOnFunction,

    ICE_CecUserCode_Blue = 0x71,
    ICE_CecUserCode_Red,
    ICE_CecUserCode_Green,
    ICE_CecUserCode_Yellow,
    ICE_CecUserCode_F5,   
    ICE_CecUserCode_Data,
    ICE_CecUserCode_LoeweCECZoomKey = 0xDC,
}  ICE_CEC_User_code;


/**
 * High-Definition Multimedia Specification Version 1.3a
 * CEC 10.2 Logical Addresses.
 */
typedef enum ICE_CecLogical
{
    ICE_CecLogical_Television,
    ICE_CecLogical_Recorder1,
    ICE_CecLogical_Recorder2,
    ICE_CecLogical_Tuner1,
    ICE_CecLogical_Player1,
    ICE_CecLogical_Audio,
    ICE_CecLogical_Tuner2,
    ICE_CecLogical_Tuner3,
    ICE_CecLogical_Player2,
    ICE_CecLogical_Recorder3,
    ICE_CecLogical_Tuner4,
    ICE_CecLogical_Player3,
    ICE_CecLogical_Reserved1,
    ICE_CecLogical_Reserved2,
    ICE_CecLogical_FreeUse,
    ICE_CecLogical_Broadcast
} ICE_CecLogical;
    
typedef enum ICE_CecVersion
{
    ICE_CecVersion_Version11,
    ICE_CecVersion_Version12,
    ICE_CecVersion_Version12a,
    ICE_CecVersion_Version13, //reserved
    ICE_CecVersion_Version13a, //¡°Version 1.3a¡± 0x04
    ICE_CecVersion_Version14a //¡°Version 1.4 or Version 1.4a¡±  0x05
} ICE_CecVersion;

typedef enum ICE_CecAbortReason
{
    ICE_CecUnrecognizedopcode,
    ICE_CecNotCorrectMode,
    ICE_CecCannotProvideSource,
    ICE_CecInvalidOperand,
    ICE_CecRefused
}ICE_CecAbortReason;

typedef enum ICE_CecPowerStatus
{
    ICE_CecStatusOn,
    ICE_CecStatusStandby,
    ICE_CecStatusTransitionToOn,
    ICE_CecStatusTransitionToStandby,
    ICE_CecStatusUninitialized = -1
}ICE_CecPowerStatus;

typedef enum ICE_CecDeviceType
{
    ICE_CecTV,
    ICE_CecRecordingDevice,
    ICE_CecReserved,
    ICE_CecTuner,
    ICE_CecPlaybackDevice,
    ICE_CecAudioSystem
}ICE_CecDeviceType;

#ifdef __cplusplus
}
#endif

#endif /*CABOT_ICE_ICE_CEC_IDS_H_INCLUDED*/

