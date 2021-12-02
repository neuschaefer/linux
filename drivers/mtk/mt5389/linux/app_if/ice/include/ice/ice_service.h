/**
 * Copyright 2002-9 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot portable interface to TV service menu functionality.
 *
 */
#ifndef CABOT_ICE_ICE_SERVICE_H_INCLUDED
#define CABOT_ICE_ICE_SERVICE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "config/include_iplayer.h"
#include "config/include_project_median.h"
#include "config/include_project_phoenix.h"

#include "frost/frost_basictypes.h"


/**
 * @brief ADC Calibration Sources. 
 */
typedef enum
{
    ADC_calibration_source_EXT1 = 0,
    ADC_calibration_source_YPbPr,
    ADC_calibration_source_VGA,
    ADC_calibration_source_number,
}ADCCalibrationSource;

/**
 * @brief ADC Calibration Types. 
 */
typedef enum
{
    ADC_calibration_type_R_gain = 0,
    ADC_calibration_type_G_gain,
    ADC_calibration_type_B_gain,
    ADC_calibration_type_R_offset,
    ADC_calibration_type_G_offset,
    ADC_calibration_type_B_offset,
    ADC_calibration_type_number,
}ADCCalibrationType;

/**
 * @brief White Balance Calibration Types. 
 */
typedef enum
{
    WhiteBalance_type_R_gain = 0,
    WhiteBalance_type_G_gain,
    WhiteBalance_type_B_gain,
    WhiteBalance_type_R_offset,
    WhiteBalance_type_G_offset,
    WhiteBalance_type_B_offset,
    WhiteBalance_type_number,
}WhiteBalanceType;


typedef enum
{
    Logo_Disable = 0,
    Logo_TrueScan,
    Logo_TrueScene,    
    Logo_NaturalMotion,
    Logo_Xmotion,
    Logo_MotionScan,
    Logo_MovieSense,
    Logo_SMR600Hz,
    Logo_SMR400Hz,
    Logo_SMR200Hz,
    Logo_SMR100Hz,
    Logo_RMR600Hz,
    Logo_RMR400Hz,
    Logo_RMR200Hz,
    
}FeaturedLogoType;

/**
 * @brief Get Default Tuner AGC level for given index
 *
 * @param index  to get related AGC level value
 * @return default AGC level value defined for given index.
 *
 * @note The AGC value should be mapped according to the platform.
 */
frost_uint32 ICE_ServiceGetDefaultTunerAgcLevel(frost_uint32 index);

/**
 * @brief Set Tuner AGC value between 0-63 for negative modulation
 *
 * @return frost_true for success, frost_false otherwise.
 *
 * @note The AGC value should be mapped according to the platform.
 */
frost_bool ICE_ServiceSetTunerAgcNegativeModulation(frost_uint32 value);

/**
 * @brief Execute ADC calibration algorithm and store results to be stored in NVRAM
 *
 * @return frost_true for success, frost_false otherwise.
 *
 * @note value should be 0 for now (TBD)
 */
frost_bool ICE_ServiceExecuteADCCalibration(frost_uint32 value);

/**
 * @brief Set Tuner AGC value between 0-63 for positive modulation
 *
 * @return frost_true for success, frost_false otherwise.
 *
 * @note The AGC value should be mapped according to the platform.
 */
frost_bool ICE_ServiceSetTunerAgcPositiveModulation(frost_uint32 value);

/**
 * @brief Set function for general usage service item. For test purposes,
 *
 * item can be linked to any function. The value can be between 0-255
 *
 * @return frost_true for success, frost_false otherwise.
 *
 */
frost_bool ICE_ServiceSetTestItem(frost_uint32 value);

/**
 * @brief ADC Auto Calibration function. Starts calibration when executed.
 *
 * After execution, Red, Green and Blue gain values and offset values should be ready
 *
 * for reading with another function.
 *
 * @param source The source which calibration function will be executed for. 
 *
 * One of the sources in the enum ADCCalibrationSource. 
 *
 * @return frost_true for success, frost_false otherwise.
 *
 */
frost_bool ICE_ServiceStartADCCalibration(ADCCalibrationSource source);

/**
 * @brief ADC Set value function. 
 *
 * @param type The type of ADC value which will be set. One of the types in the enum ADCCalibrationType. 
 *
 * @param source The source which calibration value will be set for. One of the sources in the enum ADCCalibrationSource. 
 *
 * @param value The new value of the ADCCalibrationType. Between 0 and 255 in decimal. 
 *
 * @return frost_true for success, frost_false otherwise.
 *
 */
frost_bool ICE_ServiceSetADCCalibration(ADCCalibrationSource source, ADCCalibrationType type, frost_uint32 value);

/**
 * @brief ADC Get value function. 
 *
 * @param type The type of ADC value which is read. One of the types in the enum ADCCalibrationType.
 *
 * @param source The source which calibration value will be read. One of the sources in the enum ADCCalibrationSource. 
 *
 * @return The value of the given ADCCalibrationSource and ADCCalibrationType. Between 0 and 255 in decimal. 
 *
 */
frost_uint32 ICE_ServiceGetADCCalibration(ADCCalibrationSource source, ADCCalibrationType type);

/**
 * @brief White Balance set value function.
 *
 * @param colortemp The colour temperature value.

 * @param type The type of White Balance value which will be set. One of the types in the enum WhiteBalanceType.
 *
 * @param value The new value of the WhiteBalanceType.
 *
 *@param test_command define if it is called as a test command or not
 *
 * @return frost_true for success, frost_false otherwise.
 *
 */
frost_bool ICE_ServiceSetWhiteBalance(int colortemp, WhiteBalanceType type, frost_uint32 value, frost_bool test_command);

/**
 * @brief White Balance get value function.
 *
 * @param colortemp The colour temperature value.
 *
 * @param type The type of White Balance value which is read. One of the types in the enum WhiteBalanceType.
 *
 * @return The value of the given WhiteBalanceType.
 *
 */
frost_uint32 ICE_ServiceGetWhiteBalance(int colortemp, WhiteBalanceType type);

/**
 * @brief Spread spectrum registers set function. 
 *
 * @param property The related register of spread spectrum. 0 = Range, 1 = Scale, 2 = Step.
 *
 * @param value The register value to be set. 
 *
 * @return frost_true for success, frost_false otherwise.
 *
 */
frost_bool ICE_ServiceSetSpreadSpectrum(frost_uint32 property,frost_uint32 value);


/**
 * @brief Get Default Spread Spectrum register level for given index.
 *
 * @param index to get related register level.
 * @return default spread spectrum register level defined for given index.
 *
 */
frost_uint32 ICE_ServiceGetDefaultSpreadSpectrumValue(frost_uint32 index);


/**
 * @brief Reads the PQ and AQ file names in the file system to the corresponding pointers
 *
 * @return frost_true for success, frost_false otherwise.
 *
 */

#if (defined(INCLUDE_PROJECT_MEDIAN) || defined(INCLUDE_PROJECT_PHOENIX))
frost_bool ICE_ServiceGetPQandAQFileNames(char * svn_version, char * pqu_name, char * pns_name, char * pqs_name,
                                            char * pqf_name, char * aqs_name, char * ptf_name);
#else
frost_bool ICE_ServiceGetPQandAQFileNames(char * svn_version, char * pqu_name, char * pns_name, char * pqs_name,
                                            char * pqf_name, char * aqs_name, char * peq_name, char * edid_name, 
                                            char * pns_file_revision, char * pqs_file_revision, char * peqs_file_revision);

#endif

#ifdef INCLUDE_IPLAYER
/**
 * @brief Reads the customer .p12 certificate file info. It is used for Iplayer useragent settings
 * and service menu
 *
 * @return frost_true for success, frost_false otherwise.
 *
 */
frost_bool ICE_ServiceGetIplayerCustomerInfo(char * customer_info);

#endif

/**
 * @brief Reads SatcoDX file name. It is used for service menu.
 *
 */
void ICE_ServiceGetSatcoDXInfo(char * sdx_file_info);

/**
 * @brief to get panel depended featured osd logo type from panel file
 *
 */
FeaturedLogoType ICE_ServiceGetFeaturedLogo(void);


#ifdef __cplusplus
}
#endif

#endif /*CABOT_ICE_ICE_SERVICE_H_INCLUDED*/

