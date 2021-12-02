/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_demod.h

          This file provides the demodulator control interface.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_DEMOD_H
#define SONY_DVB_DEMOD_H

#include "sony_dvb.h"
#include "sony_i2c.h"
#include "sony_dvb_t2.h"
#include "sony_dvb_t.h"
#ifdef SONY_DVB_SUPPORT_DVBC2
#include "sony_dvb_c2.h"
#endif
#include "sony_dvb_c.h"
/*------------------------------------------------------------------------------
  I2C address default (8bit form)
------------------------------------------------------------------------------*/
#define SONY_DVB_DEMOD_ADDRESS                   0xD8    /**< I2C address. */


/*------------------------------------------------------------------------------
  Recommended timeouts for wait for "lock"
------------------------------------------------------------------------------*/
#define DVB_DEMOD_DVB_UNLOCK_WAIT           100     /**< 100ms wait before reading unlock flags read. */
#define DVB_DEMOD_DVBT_WAIT_LOCK            1000    /**< DVB-T 1000ms for TPS Lock */
#define DVB_DEMOD_DVBT_WAIT_TS_LOCK         1000    /**< DVB-T 1000ms for TS (from TPS Lock) */

#define DVB_DEMOD_DVBC_WAIT_TS_LOCK         1000    /**< DVB-C 1000ms for TS Lock */
#define DVB_DEMOD_DVBC2_WAIT_TS_LOCK        5000    /**< DVB-C2 5000ms for TS Lock */

#define DVB_DEMOD_DVBT2_P1_WAIT             300     /**< DVB-T2 300ms before checking early no P1 (T2 or FEF) indication. */
#define DVB_DEMOD_DVBT2_T2_P1_WAIT          600     /**< DVB-T2 600ms before checking for P1 (T2 only) indication. */
#define DVB_DEMOD_DVBT2_WAIT_LOCK           3500    /**< DVB-T2 3500ms for demod lock indication. */
#define DVB_DEMOD_DVBT2_WAIT_TS_LOCK        1500    /**< DVB-T2 1500ms for TS lock. Assumes that demod core is locked. */

#define DVB_DEMOD_DVBC2_SLEEP_PERIOD        100     /**< DVB-C2 100ms for general sleep period. */
#define DVB_DEMOD_DVBC2_L1_WAIT             1000    /**< DVB-C2 1000ms for L1 data wait period. */
#define DVB_DEMOD_DVBC2_WAIT                2000    /**< DVB-C2 2000ms for general wait period. */
#define DVB_DEMOD_DVBC2_WAIT_LOCK           2000    /**< DVB-C2 2000ms for general wait period. */

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/

/**
 @brief The Chip ID mapping.
*/
typedef enum {
    SONY_DVB_CXD2834_IP = 0x00,         /**< FPGA based CXD2834/CXD2835/CXD2836. */
    SONY_DVB_CXD2834 = 0x25,            /**< CXD2834  DVB-T/T2/C  */
    SONY_DVB_CXD2835 = 0x28,            /**< CXD2835  DVB-C2/C */
    SONY_DVB_CXD2836 = 0x27,            /**< CXD2836  DVB-T/T2/C2/C  */
    /* Old Devices */
    SONY_DVB_CXD2817 = 0x70,            /**< CXD2817R DVB-T/C device. */
} sony_dvb_demod_chip_id_t;

/**
 @brief Software state of this driver.
*/
typedef enum {
    SONY_DVB_DEMOD_STATE_UNKNOWN,       /**< Unknown. */
    SONY_DVB_DEMOD_STATE_SHUTDOWN,      /**< Chip is in Shutdown state */
    SONY_DVB_DEMOD_STATE_SLEEP,         /**< Chip is in Sleep state */
    SONY_DVB_DEMOD_STATE_ACTIVE,        /**< Chip is in DVB-T2/T/C state */
    SONY_DVB_DEMOD_STATE_INVALID        /**< Invalid. */
} sony_dvb_demod_state_t;


/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/


/* Register values of IF frequency setting */
/* Should be set using DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBT/C/T2 macros */

/**
 @brief List of register values for IF frequency configuration.
        Used for handling tuners that output a different IF
        depending on the expected channel BW.
*/
typedef struct {
    uint32_t config_DVBT5;              /**< DVB-T 5MHz */
    uint32_t config_DVBT6;              /**< DVB-T 6MHz */
    uint32_t config_DVBT7;              /**< DVB-T 7MHz */
    uint32_t config_DVBT8;              /**< DVB-T 8MHz */
    uint32_t config_DVBT2_5;            /**< DVB-T2 5MHz */
    uint32_t config_DVBT2_6;            /**< DVB-T2 6MHz */
    uint32_t config_DVBT2_7;            /**< DVB-T2 7MHz */
    uint32_t config_DVBT2_8;            /**< DVB-T2 8MHz */
    uint32_t config_DVBC2_6;            /**< DVB-C2 6MHz */
    uint32_t config_DVBC2_8;            /**< DVB-C2 8MHz */
    uint32_t config_DVBC;               /**< DVB-C */
} sony_dvb_demod_iffreq_config_t;

/* This macro is used to calculate IF frequency setting register values.        */
/* This macro includes floating point values but will be calculated by compiler! */

/**
 @brief Calculate the demodulator IF Freq setting ::sony_dvb_demod_t::iffreq_config.
        ((IFFREQ/Sampling Freq at Down Converter DSP module) * Down converter's dynamic range + 0.5        
        
*/
#define DVB_DEMOD_MAKE_IFFREQ_CONFIG(iffreq) ((uint32_t)(((iffreq)/41.0)*16777216.0 + 0.5))

/**
 @brief Enumeration of spectrum inversion monitor values.
*/
typedef enum {
    DVB_DEMOD_SPECTRUM_NORMAL = 0,      /**< Spectrum normal sense. */
    DVB_DEMOD_SPECTRUM_INV              /**< Spectrum inverted. */
} sony_dvb_demod_spectrum_sense_t;


/**
 @brief Enumeration of supported sony tuner models.
*/
typedef enum {
    DVB_TUNER_UNKNOWN = 0,              /**< Non-Sony Tuner. */
    DVB_TUNER_ASCOT2S = 1,              /**< Sony ASCOT2S derived tuners. */
    DVB_TUNER_ASCOT2D = 2,              /**< Sony ASCOT2D derived tuners. */
} sony_dvb_tuner_model_t;

/**
 @brief The demodulator definition which allows control of the
        demodulator device through the defined set of functions.
        This portion of the driver is seperate from the tuner portion
        and so can be operated independently of the tuner.
*/
typedef struct sony_dvb_demod_t {

    /**
    @brief The demodulator crystal frequency.
    */
    sony_dvb_xtal_t xtalFreq;
    /**
    @brief The driver operating state.
    */
    sony_dvb_demod_state_t state;

    /**
    @brief The current system.
    */
    sony_dvb_system_t system;

    /**
     @brief The current bandwidth (MHz).
    */
    uint8_t bandWidth;

    /**
     @brief DVB-T I2C address (8-bit form - 8'bxxxxxxx0).
    */
    uint8_t i2cAddress;
    /**
     @brief Xtal I2C address (8-bit form - 8'bxxxxxxx0). Is fixed to Demod address + 4.
    */
    uint8_t i2cAddressXtal;

    /**
     @brief I2C API instance.
    */
    sony_i2c_t *pI2c;

    /**
     @brief IF frequency configuration. Configure prior to
            initialisation.
            Use the ::DVB_DEMOD_MAKE_IFFREQ_CONFIG macro for configuration.
    */
    sony_dvb_demod_iffreq_config_t iffreq_config;

    /**
     @brief Enable/Disable ASCOT special settings.
    */
    sony_dvb_tuner_model_t is_ascot;

    /**
     @brief RF level monitor enable/disable.
            Must be configured prior to initialisation.
            Only change this indicator during the
            SONY_DVB_DEMOD_STATE_SLEEP state.
            Use ::dvb_demod_SetConfig to configure this flag.
    */
    int enable_rflvmon;

    /* NOTE: This flag must be set before initializing */
    /**
     @brief DVB-C scan mode enable/disable.
            Only change this indicator during the
            SONY_DVB_DEMOD_STATE_SLEEP state.
            Use ::dvb_demod_SetConfig to configure this flag.
    */
    uint8_t dvbc_scanmode;

    /**
     @brief DVB-C2 scan is currently running
            1 : Scanning, 0 : Not scanning
    */
    uint8_t dvbc2_scanmode;

    /**
     @brief Auto detected chip ID at initialisation.
    */
    sony_dvb_demod_chip_id_t chipId;

    /**
     @brief The sense configured on the demodulator with
            ::dvb_demod_SetConfig.
    */
    sony_dvb_demod_spectrum_sense_t confSense;

    /**
     @brief The active IO Hi-Z setting.
            Default: RFAGC Hi-Z.
    */
    uint8_t ioHiZ;

    /**
     @brief Shared IF enable/disable flag.
            Default: Disabled (0).
            Use ::dvb_demod_SetConfig to configure this flag.
    */
    uint8_t sharedIf;

    /**
     @brief User defined data.
    */
    void *user;
} sony_dvb_demod_t;

/**
 @brief Tuning parameters used in ::sony_integration_tune.
*/
typedef struct sony_dvb_tune_params_t {

    /**
     @brief The centre frequency of the channel to tune too.
    */
    uint32_t centreFreqkHz;

    /**
     @brief The bandwidth of the channel to tune too.
    */
    uint8_t bwMHz;

    /**
     @brief The system of the channel to tune too.
    */
    sony_dvb_system_t system;
#ifdef SONY_DVB_SUPPORT_DVBT
    /**
     @brief DVBT tuning parameters which are only valid when
            system == SONY_DVB_SYSTEM_DVBT.
    */
    sony_dvbt_tune_params_t tParams;
#endif
#ifdef SONY_DVB_SUPPORT_DVBT2
    /**
     @brief DVBT2 tuning parameters which are only valid when
            system == SONY_DVB_SYSTEM_DVBT2.
    */
    sony_dvbt2_tune_params_t t2Params;
#endif
#ifdef SONY_DVB_SUPPORT_DVBC2
    /**
     @brief DVBC2 tuning parameters which are only valid when
            system == SONY_DVB_SYSTEM_DVBC2.
    */
    sony_dvbc2_tune_params_t c2Params;
#endif

} sony_dvb_tune_params_t;

/**
 @brief Declaration of the scan parameters used in ::sony_dvb_integration_Scan.
*/
typedef struct sony_dvb_scan_params_t {

    /**
     @brief The starting frequency (in kHz).
    */
    uint32_t startFrequencykHz;

    /**
     @brief The end frequency (in kHz).
    */
    uint32_t endFrequencykHz;

    /**
     @brief The step frequency (in kHz).
    */
    uint32_t stepFrequencykHz;

    /**
     @brief The tuning bandwidth to use during the scan.
    */
    uint8_t bwMHz;

    /**
     @brief The mode to scan in.
    */
    sony_dvb_system_t system;

    /**
     @brief The multiple indicator allows for multiple system (DVB-T/T2) scanning.
            The sony_dvb_scan_params_t::system shall be used as the first attempted
            system and then the alternative system.

    */
    uint8_t multiple;

    /**
     @brief User provided data.
    */
    void *user;

} sony_dvb_scan_params_t;

/**
 @brief A structure passed to the registered call-back
        on the scanning function.
*/
typedef struct sony_dvb_scan_result_t {

    /**
     @brief Pointer to the parameters passed in for the scan operation.
    */
    sony_dvb_scan_params_t *scanParams;

    /**
     @brief The last attempted tune result of the scan.
    */
    sony_result_t tuneResult;

    /**
     @brief If tuneResult == SONY_RESULT_OK, then this field contains the
            tuned frequency of the channel. Any carrier offset should be compensated
            with the ::dvb_demod_monitor_CarrierOffset function.
    */
    uint32_t centreFreqkHz;

    /**
     @brief If tuneResult == SONY_RESULT_OK, then this field contains the
            system type of the found channel. During the context of the call-back,
            the device is in the same mode as indicated here.
    */
    sony_dvb_system_t system;

    /**
     @brief If tuneResult == SONY_RESULT_OK, and the system type of the found channel is DVB-T2, 
            then this field contains the id of a plp. The callback will be invoked for each plp 
            within the signal.
    */
    uint8_t t2_plp_id;

    /**
     @brief Tune data.
    */
    sony_dvb_tune_params_t tuneData;

} sony_dvb_scan_result_t;

/**
 @brief Set up the demodulator.

        This MUST be called before calling ::sony_dvb_demod_Initialize.

 @param xtalFreq The frequency of the demod crystal.
 @param I2CAddress The demod I2C address in 8-bit form.
 @param pDemodI2c The I2C driver that the demod will use as the I2C interface.
 @param pDemod Reference to memory allocated for the demodulator instance. The create 
        function will setup this demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_Create (sony_dvb_xtal_t xtalFreq,
                                         uint8_t I2CAddress,
                                         sony_i2c_t * pDemodI2c,
                                         sony_dvb_demod_t * pDemod);

/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/

/**
 @brief Initialize the demodulator.

        From first creation or after finalize, initializes
        the demodulator and places into the "Sleep" state.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_Initialize (sony_dvb_demod_t * pDemod);

/**
 @brief Finalize the demodulator.

        The device is placed in "Shutdown" state.
        ::dvb_demod_Initialize must be called to re-initialise the
        device and driver for future acquisitions.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_Finalize (sony_dvb_demod_t * pDemod);

/**
 @brief Configure demodulator before scan, specific to the system in use.
 @param pDemod The demodulator instance.
 @param pScanParams The scan parameters.
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_ScanInitialize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

/**
 @brief Configure demodulator after scan, specific to the system in use.
 @param pDemod The demodulator instance.
 @param pScanParams The scan parameters.
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_ScanFinalize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

/**
 @brief Configure demodulator before acquisition, specific to the system in use.
 @param pDemod The demodulator instance.
 @param pTuneParams The tune parameters.
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_TuneInitialize (sony_dvb_demod_t * pDemod, sony_dvb_tune_params_t * pTuneParams);

/**
 @brief Setup demodulator for acquisition with the given parameters.

        If the demodulator/driver is asleep, then this call implicitly wakes
        the demodulator, activating it into the system requested.

        If the demodulator is active and in the same system,
        then this function only configures the device for the requested
        bandwidth (if it has changed).

        If the demodulator is active and in a different system,
        then this function will set the new system as well as the
        requested bandwidth.

        An RF tune and sony_dvb_demod_TuneEnd must then be called.

 @param pDemod The demodulator instance.
 @param system The operating system.
 @param bwMHz The expected channel bandwidth (MHz).
        - DVB-T: 6 or 7 or 8.
        - DVB-T2: 5 or 6 or 7 or 8.
        - DVB-C: Fixed for 8.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_Tune (sony_dvb_demod_t * pDemod, sony_dvb_system_t system, uint8_t bwMHz);


/**
 @brief Completes the demodulator acquisition setup.
        Must be called after sony_dvb_demod_Tune and an RF Tune.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_TuneEnd (sony_dvb_demod_t * pDemod);

/**
 @brief Check the demodulator after acquisition, specific to the system in use.
 @param pDemod The demodulator instance.
 @param pTuneParams The tune parameters.
 @param tuneResult The return code from an previous tune.
 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_TuneFinalize (sony_dvb_demod_t * pDemod, sony_dvb_tune_params_t * pTuneParams, sony_result_t tuneResult);

/**
 @brief Soft reset the demodulator.
        The soft reset will begin the devices
        acquisition process.

 @param pDemod The demod instance.

 @return SONY_RESULT_OK if successfully reset.
*/
sony_result_t sony_dvb_demod_SoftReset (sony_dvb_demod_t * pDemod);

/**
 @brief Sleep the demodulator.

 @param pDemod The demodulator instance.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_Sleep (sony_dvb_demod_t * pDemod);

/**
 @brief Configure the demodulator for blind acquisition in specified system.

 @param pDemod The demodulator instance.
 @param system The system in use.
 
 @return SONY_RESULT_OK if successful set the configuration option.
*/
sony_result_t sony_dvb_demod_EnableBlindAcquisition (sony_dvb_demod_t * pDemod, sony_dvb_system_t system);


/**
 @brief Demodulator lock indications.
*/
typedef enum {

    /**
     @brief "Lock" condition not met, yet DEMOD_LOCK_RESULT_UNLOCKED
            also not reached.
    */
    DEMOD_LOCK_RESULT_NOTDETECT,

    /**
     @brief "Lock" condition is found.
    */
    DEMOD_LOCK_RESULT_LOCKED,

    /**
     @brief Demodulator has decided that no signal
            was found or the signal was not the required system.
    */
    DEMOD_LOCK_RESULT_UNLOCKED
} sony_dvb_demod_lock_result_t;

/**
 @brief Check the demodulator lock condition (does not block or wait).

        Which state is considered as "lock" is system dependent:
        - DVB-T: TPS Lock.
        - DVB-C: TS Lock.
        - DVB-T2: OFDM core locked.

 @param pDemod The demodulator instance.
 @param pLock The lock condition.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_CheckDemodLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Check TS lock.

 @param pDemod The demodulator instance.
 @param pLock The lock condition.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_CheckTSLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Configuration options for the demodulator.

        These configuration items are generally used for static
        configuration of the system that the demodulator is operating
        in.
*/
typedef enum {
    /* DVB-T/T2 and DVB-C */

    /**
     @brief Parallel or serial TS output selection. OREG_TSIF_PARALLEL_SEL.

            Value:
            - 0: Serial output.
            - 1: Parallel output (Default).
    */
    DEMOD_CONFIG_PARALLEL_SEL,

    /**
     @brief Serial output pin of TS data. OREG_TSIF_SER_DATA_ON_MSB.

            Value:
            - 0: Output from TSDATA0
            - 1: Output from TSDATA7 (Default).
    */
    DEMOD_CONFIG_SER_DATA_ON_MSB,

    /**
     @brief Parallel/Serial output bit order on TS data. OREG_TSIF_OUTPUT_SEL_MSB.

            Value (Parallel):
            - 0: MSB TSDATA[0]
            - 1: MSB TSDATA[7] (Default).
            Value (Serial):
            - 0: LSB first
            - 1: MSB first (Default).
    */
    DEMOD_CONFIG_OUTPUT_SEL_MSB,

    /**
     @brief TS valid active level. OREG_TSIF_TSVALID_ACTIVE_HI.

            Value:
            - 0: Valid low.
            - 1: Valid high (Default).
    */
    DEMOD_CONFIG_TSVALID_ACTIVE_HI,

    /**
     @brief TS synch active level. OREG_TSIF_TSSYNC_ACTIVE_HI.

            Value:
            - 0: Valid low.
            - 1: Valid high (Default).

            @note Not available in DVB-T2 mode.
    */
    DEMOD_CONFIG_TSSYNC_ACTIVE_HI,

    /**
     @brief TS error active level. OREG_TSIF_TSERR_ACTIVE_HI.

            Value:
            - 0: Valid low.
            - 1: Valid high (Default).

            @note Not available in DVB-T2 mode.
    */
    DEMOD_CONFIG_TSERR_ACTIVE_HI,

    /**
     @brief TS clock inversion setting. OREG_TSIF_LATCH_ON_POSEDGE.

            Value:
            - 0: Falling/Negative edge.
            - 1: Rising/Positive edge (Default).
    */
    DEMOD_CONFIG_LATCH_ON_POSEDGE,

    /**
     @brief TS clock gated on valid TS data or is continuous.

            Value:
            - 0: Gated (Default)
            - 1: Continuous
    */
    DEMOD_CONFIG_TSCLK_CONT,

    /**
     @brief IFAGC sense configuration.

            Value:
            - 0: Positive IFAGC.
            - 1: Inverted IFAGC (Default)
    */
    DEMOD_CONFIG_IFAGCNEG,

     /**
     @brief Spectrum inversion tuner setting. 
            
            Value: 
            - 0: Normal (Default).
            - 1: Inverted.
    */
    DEMOD_CONFIG_SPECTRUM_INV,

    /**
     @brief RF level monitor enable/disable.
            Must be asleep for this setting to take effect on next tune/scan.

            Value:
            - 0: Disable RF level monitor.
            - 1: Enable RF level monitor (Default).
    */
    DEMOD_CONFIG_RFLVMON_ENABLE,

    /**
     @brief RF AGC PWM enable/disable.
            Must be asleep for this setting to take effect on next tune/scan.
            Please note that this will override any setting for
            GPIO1 configured with ::dvb_demod_GPIOSetting.

            Value:
            - 0: Disable RF AGC PWM output (Default).
            - 1: Enable RF AGC PWM output.
    */
    DEMOD_CONFIG_RFAGC_ENABLE,

    /**
    @brief Writes a 12-bit value to the RFAGC PWM output.
        Please note the actual PWM precision.
        - 12-bit.
        0xFFF => DVDD
        0x000 => GND
        
        This configuration is available only while the device
        is in ACTIVE mode.
    */
    DEMOD_CONFIG_RFAGC_VALUE,

    /**
     @brief OREG_TSIF_SDPR. Set the TS clock rate (data period) manually.
            *NOT recommended for normal use (testing/debug only).*
            If DEMOD_CONFIG_SCR_ENABLE = 0 (smoothing disabled), then this configuration
            will have no effect.
            If DEMOD_CONFIG_PARALLEL_SEL = 0 (serial TS), then this configuration will
            have no effect.
            If DEMOD_CONFIG_TS_AUTO_RATE_ENABLE = 1, then this configuration will have no
            effect.
            Value (DVB-T/T2):
            - Desired TS clock rate (data period) in kHz.
            Value (DVB-C):
            - 1/2 desired TS clock rate (data period) in kHz.
    */
    DEMOD_CONFIG_TSIF_SDPR,

    /**
     @brief OREG_AUTO_RATE_EN. Enable or disable the auto TS clock rate (data period).
            *NOT recommended for normal use (testing/debug only).*
            If DEMOD_CONFIG_SCR_ENABLE = 0 (smoothing disabled), then this configuration
            will have no effect.
            If DEMOD_CONFIG_PARALLEL_SEL = 0 (serial TS), then this configuration will
            have no effect.
            Value:
            - 0: Disable the TS auto rate.
                 TS clock rate = DEMOD_CONFIG_TSIF_SDPR.
            - 1: Enable the TS auto rate (Default).
                 TS clock rate is automatic.

            @note Not available in DVB-C mode.
    */
    DEMOD_CONFIG_TS_AUTO_RATE_ENABLE,

    /**
     @brief HP/LP stream selection on hierarchical transmissions.

            Value:
            - See ::sony_dvb_demod_dvbt_profile_t.
    */
    DEMOD_CONFIG_DVBT_LPSELECT,

    /**
     @brief OREG_FORCE_MODEGI (Mode/GI estimation enable (Auto or Manual)).

            Value:
            - 0: Use auto mode/guard detection (Default).
            - 1: Use manual mode/guard detection.
    */
    DEMOD_CONFIG_DVBT_FORCE_MODEGI,

    /**
     @brief OREG_MODE (For manaul Mode/GI setting)

            Value:
            - See ::sony_dvb_demod_dvbt_mode_t.
    */
    DEMOD_CONFIG_DVBT_MODE,

    /**
     @brief OREG_GI (For manual Mode/GI setting)

            Value:
            - See ::sony_dvb_demod_dvbt_guard_t.
    */
    DEMOD_CONFIG_DVBT_GI,

    /* DVB-C */

    /**
     @brief Set DVB-C scan mode.

            Value:
            - 0: Disable device DVB-C scanning mode (Default).
            - 1: Enable device DVB-C scanning mode.
    */
    DEMOD_CONFIG_DVBC_SCANMODE,

    /* DVB-T2 */


    /* DVB-C2 */

    /**
     @brief Set DVB-C2 scan mode.

            Value:
            - 0: Disable device DVB-C2 scanning mode (Default).
            - 1: Enable device DVB-C2 scanning mode.
    */
    DEMOD_CONFIG_DVBC2_SCANMODE,


    /* Extensions. */

    /**
     @brief Set Shared IF mode. Shared IF mode is available once
            demodulator is in sleep mode (::dvb_demod_Sleep).
            Please change this setting at driver initilisation.

            Value:
            - 0: Shared IF mode disabled (Default).
            - 1: Shared IF mode enabled.
    */
    DEMOD_CONFIG_SHARED_IF,
    /**
     @brief Disable/Enable TS clock during specified TS region.

            bit flags: ( can be bitwise ORed )
            - 0 : Always Active (default)
            - 1 : Disable during TS packet gap
            - 2 : Disable during TS parity
            - 4 : Disable during TS payload
            - 8 : Disable during TS header
            - 16: Disable during TS sync
    */
    DEMOD_CONFIG_TSCLK_DISABLE,

    /**
     @brief Disable/Enable TSVALID during specified TS region.

            bit flags: ( can be bitwise ORed )
            - 0 : Always Active 
            - 1 : Disable during TS packet gap (default)
            - 2 : Disable during TS parity (default)
            - 4 : Disable during TS payload
            - 8 : Disable during TS header
            - 16: Disable during TS sync
    */
    DEMOD_CONFIG_TSVALID_DISABLE,

    /**
     @brief Disable/Enable TSERR during specified TS region.

            bit flags: ( can be bitwise ORed )
            - 0 : Always Active (default)
            - 1 : Disable during TS packet gap
            - 2 : Disable during TS parity
            - 4 : Disable during TS payload
            - 8 : Disable during TS header
            - 16: Disable during TS sync
    */
    DEMOD_CONFIG_TSERR_DISABLE
} sony_dvb_demod_config_id_t;

/**
 @brief Set configuration options on the demodulator.

 @param pDemod The demodulator instance.
 @param configId The configuration ID to set. See ::sony_dvb_demod_config_id_t.
 @param value The associated value. Depends on the configId.

 @return SONY_RESULT_OK if successful set the configuration option.
*/
sony_result_t sony_dvb_demod_SetConfig (sony_dvb_demod_t * pDemod, sony_dvb_demod_config_id_t configId, int32_t value);

/**
 @brief Setup the GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 ).
 @param is_enable Set enable (1) or disable (0).
 @param is_write Write (1) or Read (0).

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_GPIOSetting (sony_dvb_demod_t * pDemod, int id, int is_enable, int is_write);

/**
 @brief Read the GPIO value.
        The GPIO should have been configured as an input (Read) GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 ).
 @param pValue The current value of the GPIO.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_GPIORead (sony_dvb_demod_t * pDemod, int id, int *pValue);

/**
 @brief Write the GPIO value.
        The GPIO should have been configured as an output (Write) GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 ).
 @param value The value to set as output.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demod_GPIOWrite (sony_dvb_demod_t * pDemod, int id, int value);

/**
 @brief Defines for demod registers.
*/
typedef struct sony_dvb_demod_reg_t {
    uint8_t bank;           /**< the bank containing the register. */
    uint8_t subAddress;     /**< the register address. */
    uint8_t value;          /**< the register value to be written/read. */
    uint8_t mask;           /**< the mask to be applied to the value. */
} sony_dvb_demod_reg_t;

/**
 @brief Function that writes an array of register values into the demodulator.

 @param pDemod The demod instance.
 @param address The I2C address that the regs reside in.
 @param regs The registers to write.
 @param regCount The number of registers in the register count array.

 @return SONY_RESULT_OK if all registers successfully written.
*/
sony_result_t sony_demod_WriteRegisters (sony_dvb_demod_t * pDemod,
                                             uint8_t address, const sony_dvb_demod_reg_t * regs, uint8_t regCount);

#endif /* SONY_DVB_DEMOD_H */
