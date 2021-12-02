
#ifndef CABOT_PROFILE_HELPER_PROFILEHELPER_H_INCLUDED
#define CABOT_PROFILE_HELPER_PROFILEHELPER_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#define SPEAKER_POWER_STRING_LENGTH        30


typedef enum
{
    PrescaleSource_DTV,
    PrescaleSource_ATV,    
    PrescaleSource_HDMI,
    PrescaleSource_AV,    
    PrescaleSource_SCART1,
    PrescaleSource_PC,
    PrescaleSource_USB,
    PrescaleSource_SCART2,
    PrescaleSource_DVD

}PrescaleSource;

typedef enum
{
    SpeakerPower_8_Watt,
    SpeakerPower_6_Watt,
    SpeakerPower_2_5_Watt
}SpeakerPower;

typedef enum
{
    No_Item,
    Logo_Item,
    Led_Item,
    Logo_Led_Item
}LedType;

typedef enum
{
    Board_MB80,
    Board_MB81,
    Board_MB82,
    Board_none = -1
}BoardName;

typedef struct
{
    unsigned int Foh;
    unsigned int Fol;
    unsigned int Gain;
    unsigned int Q;
}PeqParams;

typedef struct
{
    unsigned int SpeakerSize;
    unsigned int TruBassControl;
    unsigned int DefinitionControl;
    unsigned int FocusControl;
    unsigned int InputGain;
    unsigned int SurroundLevel;
}SRSParams;

typedef enum
{
    Disabled,
    Basic,
    Premium
}InOutTvType;

typedef enum
{
    Encrypt_None,
    Encrypt_All,
    Encrypt_Auto
}RecordingEncryptionMode;

typedef enum
{
    Custom_Key,
    Mac_Key    
}RecordingEncryptionType;

typedef enum
{
    Customer_None,
    Customer_Hitachi
}CustomerID;

typedef enum
{
    Disable,
    Vestel,
    MMH
}PortalMode;

typedef enum
{
    ONE128_MMAP,
    TWO128_MMAP,
    ONE256_MMAP,
    ONE128_ONE256_MMAP,
    TWO256_MMAP
}MemoryMapType;

typedef enum
{
    GlassType3D_Accupix,
    GlassType3D_Elentec,
    GlassType3D_Other
}GlassType3D;


typedef enum
{
    PanelType3D_ShutterGlass,
    PanelType3D_PatternRetarder,
    PanelType3D_Cinema
}PanelType3D;

int Profile_GetSwProfileVersion(char * sw_profile_version);
int Profile_GetLanProfileVersion(char * lang_profile_version);
int Profile_GetHwProfileVersion(char * hw_profile_version);

/**
    *@brief Retreives the connection speed test url
    *@param speed_test_url Url of speed test file
    *@param size Size of the buffer
*/
void Profile_GetSpeedTestUrl(char *speed_test_url, int size);//[d]:added
void Profile_GetMainSpeakerPower(char * speaker_string);
int Profile_GetMainPrescale(PrescaleSource input_source);
int Profile_GetLineoutPrescale(PrescaleSource input_source);
int Profile_GetSifoutPrescale(PrescaleSource input_source);
int Profile_GetSpdifPrescale(PrescaleSource input_source);
int Profile_GetHpPrescale(PrescaleSource input_source);
int Profile_GetTIPrescale(void);
int Profile_GetLineoutHPPrescale(PrescaleSource input_source);
int Profile_GetTISubwPrescale(void);

int Profile_GetClipLevel(PrescaleSource input_source,unsigned char avc_on);
int Profile_GetClipLevelHPLineout(PrescaleSource input_source,unsigned char avc_on);
int Profile_GetDRCThreshold(PrescaleSource input_source);
int Profile_GetTIClipLevel(unsigned char avc_on);
int Profile_GetTISubwClipLevel(unsigned char avc_on);
int Profile_IsAudioLineoutHeadPhone(void);

int Profile_IsApplicationVersionActive(void);

void Profile_GetPanelName(char * panel_name, int length);
int Profile_GetPanelType(void);
int Profile_GetNumberOfUSB();
int Profile_GetUSBPowerOff();


void Profile_GetPanelUpTimings(const char * panel_name,int * LvdsBacklightMiliSec, int * PowerUpLvdsMicroSec);
void Profile_GetPanelDownTimings(const char * panel_name,int * BacklightLvdsMiliSec, int * LvdsPowerDownMicroSec);
int Profile_IsTestToolEnabled(void);
int Profile_IsRemoteControlTypePVR(void);
int Profile_IsRemoteControlTypeRF(void);
int Profile_RfRemoteDiscoveryLqiThreshold(void);
void Profile_GetRemoteControllerType(char * rc_type, int length);
unsigned char Profile_GetRemoteControllerCode(const char * rc_type, const char * rc_key);
int Profile_CheckRemoteControlSignalFormat(const char * signal_format);
int Profile_GetSourceSelection(const char* source_name);
int Profile_GetTunerSourceSelectionEnabled();
int Profile_IsDebugPrintRemoteControlEnabled(void);
void Profile_GetSurroundType(char * surround_type);
void Profile_GetSurroundModeText(char * surround_mode_text);
int Profile_IsHotelModeEnabled(void);
int Profile_GetMovieSenseMode(void);
void Profile_GetMovieSenseString(char * movie_sense_name);
int Profile_FixedVolumeThroughLineout(void);
int Profile_IsMovieSenseEnabled(void);

/**
 * @brief when pixellence is seperate item in menu we need to have default pixellence mode
 * . This function returns default pixellence mode for this case such as off/low/mid/high
 */
int Profile_GetPixellenceMode(void);
/**
 * @brief 
 * 0: Pixellence disabled in menu and application
 * 1: Pixellence is a "Picture Mode" like Dynamic, Cinema, Natural etc.
 * 2: Pixellence is seperate menu item which can be off/on; mid/low/high etc.
 */
int Profile_IsPixellenceAvailable(void);
int Profile_GetPixellenceWorkingMode(void);
void Profile_GetPixellenceString(char * pixellence_name);
int Profile_IsPixellenceControlInvisible(void);
int Profile_LoadFRCProfile(void);
int Profile_IsFRCAvailable(void);
int Profile_GetStartupVolumeLevel(void);
int Profile_IsDynamicMenuEnabled(void);
int Profile_IsAutoZoomModeEnabled(void);
int Profile_IsPanoramicZoomModeEnabled(void);
int Profile_IsApsSortingEnabled(void);
int Profile_isEPGAvailable(void);
int Profile_IsToshibaPatentProtectionEnabled(void);
int Profile_isEPGExtendedInfoEnabled(void);
void Profile_GetTunerType(char * tuner_type);
int Profile_IsMediaBrowserEnabled(void);
int Profile_IsCECEnabled(void);
int Profile_IsMediaBrowserDRMRegistrationEnabled(void);
int Profile_IsMediaBrowserDivxEnabled(void);
int Profile_IsMediaBrowserDisableBannerInLoop(void);
int Profile_IsMediaBrowserDivxPlusHDEnabled(void);
int Profile_IsMediaBrowserWmaEnabled(void);
int Profile_IsMediaBrowserWmvEnabled(void);
int Profile_IsMediaBrowserFlashSWFSupportEnabled(void);
int Profile_IsMediaBrowserVideoFileExtEnabled(const char* ext);
int Profile_IsMediaBrowserAudioFileExtEnabled(const char* ext);
int Profile_IsMediaBrowserPhotoFileExtEnabled(const char* ext);
int Profile_IsMediaBrowserSubtitleFileExtEnabled(const char* ext);
int Profile_IsMediaBrowserRecordFileExtEnabled(const char* ext);

int Profile_GetMediaBrowserVideoFileExt(int index, char* ext);
int Profile_GetMediaBrowserAudioFileExt(int index, char* ext);
int Profile_GetMediaBrowserPhotoFileExt(int index, char* ext);
int Profile_GetMediaBrowserSubtitleFileExt(int index, char* ext);
int Profile_GetMediaBrowserRecordFileExt(int index, char* ext);

int Profile_IsMediaBrowserStageCraftAutoTest(void);
int Profile_GetAutoTVOFFMode(void);
int Profile_IsPCStandbyEnabled(void);
int Profile_IsPCStandbyOutEnabled(void);
int Profile_GetHDMINumber(void);
int Profile_IsMediaBrowserXVidEnabled(void);
int Profile_IsMediaBrowserMenuEnabled(void);
void Profile_GetPanelPquFileName(const char * panel_name, char * pqu_file, int length);
void Profile_GetPanelPnsFileName(const char * panel_name, char * pns_file, int length);
void Profile_GetPanelPqsFileName(const char * panel_name, char * pqs_file, int length);
void Profile_GetPanelPqfFileName(const char * panel_name, char * pqf_file, int length);
void Profile_GetPanelCdFileName(const char * panel_name, char * pqu_file, int length);
void Profile_GetAudioAqsFileName(char * aqs_file, int length);
/**
 * @brief Audio delay value which will be applied according to external 
 * back-end video processor (such as FRC/MFC) state
 *
 * @param is_high true: get higher audio delay value for higher video latency
 *                  false: get smaller audio delay value for less video latency
 *
 */
int Profile_GetAudioDelay(int is_high);
void Profile_GetHdmiEdidFileName(char * hdmi_file, int length, int hdmi_no);
int Profile_EDIDPhysicalAddressPosition(void);
int Profile_IsOverrideSwVersionEnabled(void);
void Profile_GetSwVersionNumber(char * ver_no);
void Profile_GetSwVersionPostfix(char * postfix);
int Profile_GetModelId(void);
void Profile_GetDcfName(char * dcf_name);
void Profile_GetOui(char * oui);
void Profile_GetWebServerType(char * server_type,int no);
void Profile_GetWebDownloadAddress(char * web_adress, int no);
void Profile_GetWebUserName(char * user_name,int no);
void Profile_GetWebUserPassword(char * password,int no);
void Profile_GetProductID(char * product_id);
void Profile_GetProductID2(char * product_id);
int Profile_GetCountry(void);
int Profile_GetLanguage(void);
int Profile_GetTesttoolSocketSupportEnabled(void);
int Profile_GetAnalogSupportEnabled(void);
int Profile_GetCableSupportEnabled(void);
int Profile_GetSatelliteSupportEnabled(void);
int Profile_GetInstallationBeepSupportEnabled(void); 
int Profile_GetIPTVSupportEnabled(void);
int Profile_GetHotelTVSupportEnabled(void);
int Profile_GetMotorOptionSupportforSatelliteEnabled(void);
int Profile_IsOnlyAnalogTV(void);
int Profile_GetSearchType();
int Profile_GetNoSignalTimeoutEnabled(void);
int Profile_GetActiveAntennaEnabled(void);
int Profile_IsLcnEnabled(void);
int Profile_IsStbySearchEnabled(void);
int Profile_TravelTVEnabled(void);
int Profile_GetDefaultStdbySearch(void);
int Profile_GetEpgType(void);
int Profile_GetEqualizerBandValue(unsigned int mode, const char* band);
int Profile_IsEqualizerPeqEnabled(void);
void Profile_GetEqualizerPeqValues(unsigned int mode,unsigned int band, PeqParams* peq_params);
int Profile_IsDVDExist(void);
void Profile_GetExt1Name(char * ext1_name);
void Profile_GetExt1RgbName(char * ext1rgb_name);
void Profile_GetExt2Name(char * ext2_name);
void Profile_GetExt2RgbName(char * ext2rgb_name);
void Profile_GetExt1SName(char * ext1s_name);
void Profile_GetExt2SName(char * ext2s_name);
void Profile_GetFavName(char * fav_name);
void Profile_GetSvhsName(char * svhs_name);
void Profile_GetHdmi1Name(char * hdmi1_name);
void Profile_GetHdmi2Name(char * hdmi2_name);
void Profile_GetHdmi3Name(char * hdmi3_name);
void Profile_GetHdmi4Name(char * hdmi4_name);
void Profile_GetYpbprName(char * ypbpr_name);
void Profile_GetVgaName(char * vga_name);
int Profile_GetBluRayName(char * bluray_name);
int Profile_GetDVDName(char * dvd_name);
void Profile_GetWiDiName(char * widi_name);
void Profile_GetAPMName(char * apm_name);
void Profile_GetBacklightTrickModeTimeout(int *enter_timeout, int *exit_timeout);
void Profile_GetBacklightTrickModeThreshold(int *enter_threshold);
int Profile_IsBacklightTrickModeContinuous(void);
int Profile_IsBacklightTrickModeEnabled(void);
int Profile_UseDevHdcpKeys(void);
int Profile_GetAnalogRFNoiseReductionMode(void);
int Profile_GetDigitalRFSDNoiseReductionMode(void);
int Profile_GetDigitalRFHDNoiseReductionMode(void);
int Profile_GetFAVNoiseReductionMode(void);
int Profile_GetHDMINoiseReductionMode(void);
int Profile_GetPCNoiseReductionMode(void);
int Profile_GetScartCVBSNoiseReductionMode(void);
int Profile_GetScartRGBNoiseReductionMode(void);
int Profile_GetSVideoNoiseReductionMode(void);
int Profile_GetYPbPrNoiseReductionMode(void);
void Profile_GetLocalKeyThreshold(const char * key,int * low, int * high);
void Profile_GetLocalKeyType(char * keytype);
int Profile_IsCommonVolControlEnabled(void);
int Profile_IsInOutTVEnabled(void);
int Profile_IsIlluminatedLogoEnabled(void);
int Profile_IsBacklightEcoModeEnabled(void);
int Profile_GetBacklightDefaultMode(void);
int Profile_IsRS232Enabled(void);
int Profile_isERPAlgoEnabled(void);
int Profile_GetPowerUpMode(void);
int Profile_IsHDMIAutoSwitchEnabled(void);
int Profile_GetBlueBackgroundFactoryValue(void);
void Profile_GetLocalKeySensing(const char * key,int * sensing);
void Profile_GetLocalKeySensingParameter(const char * key,int * sensing);
int Profile_GetDefaultPictureMode(char *ext_type);
int Profile_GetDefaultZoomMode(void);
int Profile_IsYPbPrViaVgaEnabled(void);
int Profile_IsMpeg4Enabled();
int Profile_IsCiPlusEnabled(void);
int Profile_IsShortProtectionEnabled(void);
int Profile_IsPvrReadyEnabled(void);
int Profile_IsTwinTunerEnabled(void);
int Profile_IsDlnaDmpEnabled(void);
int Profile_IsDlnaDmrEnabled(void);
int Profile_IsIsbEnabled(void);
int Profile_IsTextTransparencyPatentEnabled(void);
int Profile_IsPictureWizardEnabled(void);
int Profile_IsPin8StandbyOutEnabled(void);
int Profile_GetMenuTimeoutDefaultMode(void);
int Profile_GetColorEnhancementState(const char * picture_mode);
int Profile_GetBrightnessDefault(const char * video_source, const char * picture_mode);
int Profile_GetColourDefault(const char * video_source, const char * picture_mode);
int Profile_GetContrastDefault(const char * video_source, const char * picture_mode);
int Profile_GetSharpnessDefault(const char * video_source, const char * picture_mode);
int Profile_GetHueDefault(const char * video_source, const char * picture_mode);
int Profile_GetColourTempDefault(const char * video_source, const char * picture_mode);
int Profile_IsBacklightControlEnabled(void);
int Profile_IsBacklightDimmingModeEnabled(void);
int Profile_IsPowerSaveModeEnabled(void);
int Profile_GetBacklightDimmingDefaultMode(void);
int Profile_GetPowerSaveModeDefault(void);
int Profile_GetSurroundModeControlEnabled(void);
int Profile_GetSurroundModeDefaultValue(void);
int Profile_GetSPDIFEnabled(void);
int Profile_GetHDMITrueBlackDefaultValue(void);
int Profile_IsDynamicContrastEnabled(void);
int Profile_GetDynamicContrastDefault(const char * video_source, const char * picture_mode);
int Profile_GetFilmModeDefault(void);
int Profile_IsSkinToneEnabled(void);
int Profile_GetSkinToneDefault(const char * video_source, const char * picture_mode);
int Profile_GetRGBGainRedDefault(const char * video_source, const char * picture_mode);
int Profile_GetRGBGainGreenDefault(const char * video_source, const char * picture_mode);
int Profile_GetRGBGainBlueDefault(const char * video_source, const char * picture_mode);
int Profile_IsSportItemForPictureModeEnabled(void);
int Profile_IsMB91Enabled(void);
int Profile_IsMB91_3Enabled(void);
void Profile_GetBoardName(char * board_name, int length);
int Profile_IsQuickStandbyControlEnabled(void);
int Profile_GetQuickStandbyDefaultValue(void);
int Profile_GetRecordingEncryptionMode(void);
int Profile_GetRecordingEncryptionType(void);
int Profile_IsPVRPartitionFormatTypeCheckEnabled(void);
int Profile_IsWiFiEnabled(void);
int Profile_OnlyWiFiEnabled(void);
int Profile_IsUsbNetEnabled();
int Profile_IsHbbTvEnabled(void);
int Profile_GetPortalMode(void);
int Profile_IsPortalMenuEnabled(void);
int Profile_IsIPlayerEnabled(void);
void Profile_GetSRSValues(SRSParams* SRS_params);
int Profile_IsSRSEnabled(void);
void Profile_GetCustomerIplayerCertificate(char *);
void Profile_GetCustomerName(char*);
int Profile_IsSatcoDXEnabled(void);
int Profile_IsOneColourDVBSubtitleEnabled(void);
int Profile_IsFixColorPositionDVBSubtitleEnabled(void);
int Profile_GetSonyDemodSupportEnabled(void);
int Profile_GetFmRadioCountry(void);
int Profile_GetDsmartSupportEnabled(void);
int Profile_IsMhpEnabled(void);
void Profile_DhcpClientId(char * dhcp_client_id);
unsigned int Profile_GetDivxModelID(void);
int Profile_IsWinkAnimationEnabled(void);
int Profile_GetFeatureOsdMode(void);
int Profile_GetFeatureLogoType(void);

/* Mode definitions
(1) All elements shift to left and goes off-screen from the left,
(2) Only the rightmost element cycles, hence none of the first 9 elements goes
off-screen ever,
(3) In a hybrid mode, where the leftmost elements goes on screen as much as
possible.
*/
int Profile_GetFeatureOsdHtmlMode(void);
int Profile_IsFeatureOsdHtmlModeTitlesOn(void);
int Profile_IsHomeStoreModeEnabled(void);
int Profile_GetBootUpPictureMode(void);
int Profile_GetDigiturkSupportEnabled(void);
int Profile_GetFransatSupportEnabled(void);
int Profile_GetLanguageSelection(const char* lang_code);
int Profile_IsTVBannerTransparencyEnabled(void);
int Profile_IsUSBCloningEnabled(void);
void Profile_GetLedType(char * ledtype);
int Profile_GetLedColor(void);
int Profile_GetMemoryMapType(void);
int Profile_IsWelcomeScreenNeeded(void);
int Profile_IsSaorviewSupportEnabled(void);
int Profile_GetAstraHDSupportEnabled(void);
int Profile_GetOrfSupportEnabled(void);
int Profile_GetGCCCountryEnabled(void);
int Profile_GetMybutton1YoutubeEnabled(void);
int Profile_GetConnectivityEnabled(void);
int Profile_Is3DEnabled(void);
int Profile_IsOpenBrowserEnabled(void);
int Profile_IsRemoteControlType3D(void);
int Profile_GetBootupImageTimeoutValueStart(void);
int Profile_IsBootupImageEnabledStart(void);
int Profile_GetBootupImageTimeoutValueEnd(void);
int Profile_IsBootupImageEnabledEnd(void);
int Profile_IsBootLogoEnabled(void);
int Profile_GetLogoType3D(void);
int Profile_IsBoxerHDEnabled(void);
int Profile_IsCableHdReadyEnabled(void);
int Profile_IsCanalDigitalEnabled(void);
int Profile_IsCanalReadyEnabled(void);
int Profile_IsComHemEnabled(void);
int Profile_IsDigitalTickEnabled(void);
int Profile_IsDgtviGoldEnabled(void);
int Profile_IsDgtviSilverEnabled(void);
int Profile_IsFreeviewHDEnabled(void);
int Profile_IsFreeviewEnabled(void);
int Profile_IsRiksTvEnabled(void);
int Profile_IsSaorviewEnabled(void);
int Profile_IsStofaEnabled(void);
int Profile_IsTntHdEnabled(void);
int Profile_IsUpcEnabled(void);
int Profile_IsYouseeEnabled(void);
int Profile_IsZiggoEnabled(void);
int Profile_IsDvbt2Enabled(void);
int Profile_IsTntsatEnabled(void);


int Profile_GetDefault3DMode(void);
int Profile_GetPanelType3D(void);
int Profile_GetGlassType3D(void);
int Profile_IsVideoWallEnabled(void);
int Profile_GetUseProductionCredentialsEnabled(void);
int Profile_GetBoardConfig(void);
int Profile_isDVDScartOutEnabled(void);
int Profile_EnableAudioAtScartout(void);
int Profile_IsTtxSubPageAutoUpdateEnabled(void);
int Profile_IsSmoothStreamingEnabled(void);
int Profile_IsDiscretixDrmEnabled(void);
int Profile_IsVoltageDropDetectEnabled(void);
int Profile_GetLineOutDBLimit(void);
int Profile_IsUSBStreamPlayerSupportEnabled(void);
int Profile_isDvdEmcModificationsEnabled(void);
int Profile_isDvdStandbyEnabled(void);
int Profile_IsEnlargeWidthTo720Enabled(void);
int Profile_IsInternalSubwooferEnabled(void);
int Profile_IsPictureOffModeEnabled(void);
int Profile_IsAmbilightSensorEnabled(void);
int Profile_IsDeleteAndMoveUpEnabled(void);
int Profile_IsFTPUpgradeEnabled(void);
int Profile_IsCopyNeverEnabled(void);

/**
 * @brief Get the virtual remote controller type string from the HW profile
 */
void Profile_GetVirtualRemoteControllerType(char * rc_type, int length);

/**
 * @brief Check whether the virtual remote support is enabled in the profile
 * @return non-zero if the support is enabled, zero is not enabled
 */
int Profile_GetVirtualRemoteSupportEnabled(void);

/**
 * TODO: Document this
 */
unsigned int Profile_GetVirtualRemoteControllerCode(const char * rc_type, const char * rc_key);


int Profile_IsDigitalIBEnabled(void);

int Profile_IsCompleteEpgEnabled(void);
int Profile_IsDVBSubtitlesEnabled(void);
int Profile_IsFollowTvEnabled(void);
int Profile_IsSkypeEnabled(void);
int Profile_IsSDTServiceAdditionAllowed(void);
int Profile_IsISBProtocolEnabled(void);
int Profile_IsUartRxEnabled(void);
int Profile_IsRobotesterEnabled(void);
int Profile_IsConditionalAccessEnabled(void);

#ifdef __cplusplus
}
#endif

#endif //CABOT_PROFILE_HELPER_PROFILEHELPER_H_INCLUDED

