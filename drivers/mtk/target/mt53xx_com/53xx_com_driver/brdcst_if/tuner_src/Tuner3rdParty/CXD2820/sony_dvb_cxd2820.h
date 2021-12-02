/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2009 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_cxd2820.h

          This file provides the integration layer for controlling the 
          tuner and demodulator together for acquisition algorithms and 
          state control.

*/
/*----------------------------------------------------------------------------*/
#ifndef SONY_DVB_CXD2820_H_
#define SONY_DVB_CXD2820_H_

#include <sony_dvb.h>
#include <sony_dvb_demod.h>
#include <sony_dvb_tuner.h>
#include <sony_dvb_i2c.h>
#include <sony_dvb_t2.h>

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
#define SONY_DVB_CXD2820_DVBT_ADDRESS		DVB_DEMOD_DVBT_ADDRESS  /**< CXD2820 DVB-T address. */
#define SONY_DVB_CXD2820_DVBC_ADDRESS		DVB_DEMOD_DVBC_ADDRESS  /**< CXD2820 DVB-C address. */
#define SONY_DVB_CXD2820_DVBT2_ADDRESS		DVB_DEMOD_DVBT2_ADDRESS /**< CXD2820 DVB-T2 address. */

/* Masks for the tune result for DVB-T2. */
#define DVB_DEMOD_DVBT2_TUNE_RESULT_DATA_PLP_NOT_FOUND      0x02    /**< DVBT2 Data PLP not found indicator. */

/*------------------------------------------------------------------------------
 Globals
------------------------------------------------------------------------------*/

/**
 @brief CXD2820 driver version.
*/
extern const char *sony_dvb_cxd2820_version;

/**
 @brief CXD2820 driver build date.
*/
extern const char *sony_dvb_cxd2820_build_date;

/**
 @brief CXD2820 driver build time.
*/
extern const char *sony_dvb_cxd2820_build_time;

/*------------------------------------------------------------------------------
 Types
------------------------------------------------------------------------------*/

/**
 @brief The high level driver object.
        This is the primary interface used for controlling
        and monitoring the CXD2820/CXD2817 device and connected tuner.

        This object is the aggregation of the demodulator and the tuner 
        to represent a single front end system. It can be used wholly, partly or just
        as a reference for developing the application software.
*/
typedef struct sony_dvb_cxd2820_t {
    sony_dvb_demod_t *pDemod;                           /**< Instance of the T2/T/C demodulator. */
    sony_dvb_tuner_t *pTuner;                           /**< The connected tuner. */
    sony_dvb_atomic_t cancel ;                          /**< Cancellation indicator variable. */
    uint8 t2AutoSpectrum ;                            /**< DVB-T2 automatic spectrum detection enable/disable flag. Default: Enabled. */
    void *user;                                         /**< User data. */
} sony_dvb_cxd2820_t;

/** 
 @brief Tuning parameters specific to DVB-T2.
*/
typedef struct sony_dvb_demod_t2_tune_params_t {

    /**
     @brief The data PLP ID to select in acquisition. 
    */
    uint8 dataPLPId;

    /**
     @brief (OUTPUT) After tune, this field indicates the tune result status.
            See bit mask defined in ::DVB_DEMOD_DVBT2_TUNE_RESULT_DATA_PLP_NOT_FOUND 
            for checking the result status.
    */
    uint8 tuneResult;

} sony_dvb_demod_t2_tune_params_t;

/** 
 @brief Tuning parameters specific to DVB-T.
*/
typedef struct sony_dvb_demod_t_tune_params_t {

    /**
     @brief Indicates the HP/LP profile to be selected.
    */
    sony_dvb_demod_dvbt_profile_t profile;

    /**
     @brief Indicates whether the following (mode and gi) preset 
            information should be used (1) or not (0).
            Using preset tuning can help speed up acquisition and help in difficult 
            channels.

            - 1: sony_dvb_demod_t_tune_params_t::mode and sony_dvb_demod_t_tune_params_t::gi are 
               forced in the demodulator. The mode and guard can be retrieved from 
               ::dvb_demod_monitorT_TPSInfo.
            - 0: sony_dvb_demod_t_tune_params_t::mode and sony_dvb_demod_t_tune_params_t::gi are 
               ignored.
    */
    uint8 usePresets;

    /**
     @brief If using presets, force the demodulator to acquire with the given mode.
    */
    sony_dvb_demod_dvbt_mode_t mode;

    /**
     @brief If using presets, force the demodulator to acquire with the given gi.
    */
    sony_dvb_demod_dvbt_guard_t gi;

} sony_dvb_demod_t_tune_params_t;

/** 
 @brief Tuning parameters used in ::sony_dvb_cxd2820_Tune.
*/
typedef struct sony_dvb_tune_params_t {
    /**
     @brief The centre frequency of the channel to tune too.
    */
    uint32 centreFreqkHz;

    /**
     @brief The bandwidth of the channel to tune too.
    */
    uint8 bwMHz;

    /**
     @brief The system of the channel to tune too.
    */
    sony_dvb_system_t system;

    /**
     @brief DVBT tuning parameters which are only valid when
            system == SONY_DVB_SYSTEM_DVBT.
    */
    sony_dvb_demod_t_tune_params_t tParams;

    /**
     @brief DVBT2 tuning parameters which are only valid when
            system == SONY_DVB_SYSTEM_DVBT2.
    */
    sony_dvb_demod_t2_tune_params_t t2Params;

} sony_dvb_tune_params_t;

/**
 @brief Declaration of the scan parameters used in ::sony_dvb_cxd2820_Scan.
*/
typedef struct sony_dvb_scan_params_t {

    /**
     @brief The starting frequency (in kHz).
    */
    uint32 startFrequencykHz;

    /**
     @brief The end frequency (in kHz).
    */
    uint32 endFrequencykHz;

    /**
     @brief The step frequency (in kHz).
    */
    uint32 stepFrequencykHz;

    /**
     @brief The tuning bandwidth to use during the scan. 
    */
    uint8 bwMHz;

    /**
     @brief The mode to scan in.
    */
    sony_dvb_system_t system;

    /**
     @brief The multiple indicator allows for multiple system (DVB-T/T2) scanning.
            The sony_dvb_scan_params_t::system shall be used as the first attempted
            system and then the alternative system.
            Multiple system scanning is not available on CXD2817 device.
    */
    uint8 multiple;

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
    sony_dvb_result_t tuneResult;

    /**
     @brief If tuneResult == SONY_DVB_OK, then this field contains the 
            tuned frequency of the channel. Any carrier offset should be compensated
            with the ::dvb_demod_monitor_CarrierOffset function.
    */
    uint32 centreFreqkHz;

    /**
     @brief If tuneResult == SONY_DVB_OK, then this field contains the 
            system type of the found channel. During the context of the call-back,
            the device is in the same mode as indicated here.
    */
    sony_dvb_system_t system;

} sony_dvb_scan_result_t;

/**
 @brief Callback function that is called for every attempted frequency during a 
        scan.
        
        - DVB-T: This callback is invoked when TPS lock is achieved. 
                 Use ::dvb_cxd2820_WaitTSLock to wait for TS lock.
        - DVB-T2: This callback is invoked when T2 demod lock (P1/L1-pre/L1-post) is achieved.
                 Use ::dvb_cxd2820_WaitTSLock to wait for TS lock.
        - DVB-C: This callback is invoked when TS lock is achieved.
 
 @param pDriver The driver instance.
 @param result The current scan result.

*/
typedef void (*sony_dvb_scan_callback_t) (sony_dvb_cxd2820_t * pDriver, 
                                          sony_dvb_scan_result_t * result);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

/**
 @brief Construct the driver.

        This function is called by the application in order
        to setup the ::sony_dvb_cxd2820_t structure and 
        ::sony_dvb_cxd2820_t::pDemod member.

        This MUST be called before calling ::dvb_cxd2820_Initialize.
        
        Notes on driver configuration:
        - By default, the demodulator uses an inverted AGC.
          If the connected tuner uses a non-inverted IFAGC, call 
          ::dvb_demod_SetConfig with ::DEMOD_CONFIG_IFAGCNEG = 0 to setup 
          the demodulator with postive IFAGC sense, after calling ::dvb_cxd2820_Initialize.
        - By default, the driver enables the RF level monitoring on the 
          device, call ::dvb_demod_SetConfig with ::DEMOD_CONFIG_RFLVMON_ENABLE = 0
          to disable it, after calling ::dvb_cxd2820_Initialize.
        - By default, the demodulator expects a normal spectrum sense from the tuner.
          If the connected tuner outputs an inverted spectrum, call 
          ::dvb_demod_SetConfig with ::DEMOD_CONFIG_SPECTRUM_INV = 1 to setup the demodulator
          for an inverted spectrum sense, after calling ::dvb_cxd2820_Initialize.

 @note Memory is not allocated dynamically.

 @param dvbtI2CAddress The DVBT demod I2C address in 8-bit form.
 @param dvbcI2CAddress The DVBC demod I2C address in 8-bit form.
 @param dvbt2I2CAddress The DVBT2 demod I2C address in 8-bit form.
 @param pDemodI2c The I2C driver that the demod will use as the I2C interface.
 @param pTuner The tuner driver to use with this instance of the driver.
              The tuner I2C interface should have been setup before this call.
 @param pDemod Reference to memory allocated for the demodulator instance. The create 
        function will setup this demodulator instance also.
 @param pDriver The driver to create.
              It must be a valid pointer to allocated memory for a 
              ::sony_dvb_cxd2820_t structure.

 @return SONY_DVB_OK if successfully created demod driver api.
*/
sony_dvb_result_t dvb_cxd2820_Create (uint8 dvbtI2CAddress,
                                      uint8 dvbcI2CAddress,
                                      uint8 dvbt2I2CAddress,
                                      sony_dvb_i2c_t * pDemodI2c,
                                      sony_dvb_demod_t * pDemod, 
                                      sony_dvb_tuner_t * pTuner, 
                                      sony_dvb_cxd2820_t * pDriver);

/**
 @brief Initialize the demodulator and tuner. 
        After this operation, the demodulator and tuner are in a low power 
        state (demod state = SONY_DVB_DEMOD_STATE_SLEEP) 
        awaiting for tune and scan commands.

 @param pDriver The driver instance.

 @return SONY_DVB_OK if OK.
*/
sony_dvb_result_t dvb_cxd2820_Initialize (sony_dvb_cxd2820_t * pDriver);

/**
 @brief Finalize the demodulator and tuner. The demodulator and tuner
        are in a low power state and the driver interfaces are no longer available.
        Initialize must be called in order to use the demodulator/tuner again.

 @param pDriver The driver instance.

 @return SONY_DVB_OK if OK.
*/
sony_dvb_result_t dvb_cxd2820_Finalize (sony_dvb_cxd2820_t * pDriver);

/**
 @brief Place the demodulator and tuner in the lowest possible power mode.	
        Calling tune and scan functions shall wake the demodulator and tuner.

 @param pDriver The driver instance.

 @return SONY_DVB_OK if OK.
*/
sony_dvb_result_t dvb_cxd2820_Sleep (sony_dvb_cxd2820_t * pDriver);

/*------------------------------------------------------------------------------
 Functions: Algorithms
------------------------------------------------------------------------------*/

/**
 @brief Performs a blind acquisition to the channel. Blocks the calling thread
        until the acquisition is complete or has timed out (see notes for 
        what is meant by "acquisition complete"). 
        Use ::dvb_cxd2820_Cancel to cancel the operation at any time.
        The demodulator may be asleep before this call, tune will power up the 
        demodulator into the appropriate system based on the passed parameters.

        - DVBT: Waits for TPS lock, returns once TPS lock is achieved.
          An application can use ::dvb_cxd2820_WaitTSLock if there is 
          a need to wait for TS lock.
        - DVBT2: Acquisition waits for demod lock (P1/L1-pre/L1-post).
          Once successful, the application can wait for TS lock (::dvb_cxd2820_WaitTSLock)
          to see if the demod is able to decode the PLPs carried on the channel.
          If TS is locked, then the first data PLP in the channel is selected 
          automatically and is output by the demodulator.
          In order to determine the list of data PLPs carried, then use the function 
          ::dvb_demod_monitorT2_DataPLPs.
          To enable/disable auto spectrum detection for DVB-T2, please call 
          ::dvb_cxd2820_SetT2AutoSpectrumDetection. Once acquired, use 
          ::dvb_demod_monitorT2_SpectrumSense to see which sense was detected.

 @param pDriver The driver instance.
 @param system The system (DVB-T/C/T2) with which to attempt the blind acquisition.
 @param centreFreqkHz The expected centre frequency of the channel in kHz.
 @param bwMHz The expected bandwidth of the channel in MHz.

 @return SONY_DVB_OK if successfully acquired channel.
*/
sony_dvb_result_t dvb_cxd2820_BlindTune (sony_dvb_cxd2820_t * pDriver,
                                         sony_dvb_system_t system,
                                         uint32 centreFreqkHz, 
                                         uint8 bwMHz);

/**
 @brief Performs acquisition to the channel. 
        Blocks the calling thread until the TS has locked or has timed out.
        Use ::dvb_cxd2820_Cancel to cancel the operation at any time.
 
        Notes on tuning:
        - DVBT: If using preset tuning (usePresets=1) and the mode/gi information does not match, then 
          the demodulator will FAIL the acquisition.
          If not using preset tuning (usePresets=0), then only the profile (HP/LP) is required for
          tuning. If the LP stream is not available, then the HP stream will be output.
        - DVBT2: CXD2820 only. 
          Only the data PLP ID is required for tuning. In this case, the device will 
          wait for a T2 P1 symbol in order to decode the 
          L1 pre signalling and then begin demodulation.
          If the data PLP ID (::sony_dvb_demod_t2_tune_params_t::dataPLPId) or 
          the associated common PLP is not found in the channel, 
          the device will always select the first found PLP and output the associated TS.
          In this case, an indicator in the ::sony_dvb_demod_t2_tune_params_t::tuneResult will 
          be set.
          To enable/disable auto spectrum detection for DVB-T2, please call 
          ::dvb_cxd2820_SetT2AutoSpectrumDetection. Once acquired, use 
          ::dvb_demod_monitorT2_SpectrumSense to see which sense was detected.

 @param pDriver The driver instance.
 @param pTuneParams The parameters required for the tune.

 @return SONY_DVB_OK if tuned successfully to the channel.
*/
sony_dvb_result_t dvb_cxd2820_Tune (sony_dvb_cxd2820_t * pDriver, 
                                    sony_dvb_tune_params_t * pTuneParams);

/**
 @brief Performs a scan over the spectrum of interest. 
        The scan can perform a multiple system scan for DVB-T and DVB-T2 channels by 
        setting the sony_dvb_scan_params_t::multiple flag (1) and setting the 
        sony_dvb_scan_params_t::system = sony_dvb_system_t::SONY_DVB_SYSTEM_DVBT.
        Blocks the calling thread while scanning. Use ::dvb_cxd2820_Cancel to cancel 
        the operation at any time.
        
        - DVBT2: To enable/disable auto spectrum detection for DVB-T2, please call 
        ::dvb_cxd2820_SetT2AutoSpectrumDetection. Once acquired, use 
        ::dvb_demod_monitorT2_SpectrumSense to see which sense was detected.
 
 @param pDriver The driver instance.
 @param pScanParams The scan parameters.
 @param callBack User registered call-back to receive scan progress
        information and notification of found channels. 
        The call back is called fore very attempted frequency during a  scan.

 @return SONY_DVB_OK if scan completed successfully.
        
*/
sony_dvb_result_t dvb_cxd2820_Scan (sony_dvb_cxd2820_t * pDriver,
                                    sony_dvb_scan_params_t * pScanParams, 
                                    sony_dvb_scan_callback_t callBack);

/**
 @brief Wait for the demodulator to indicate TS lock.
        Timeout for function depends on the current system (DVB-C/T/T2).
        Blocks the calling thread until the TS has locked or has timed out.
        Use ::dvb_cxd2820_Cancel to cancel the operation at any time.

 @param pDriver The driver instance.

 @return SONY_DVB_OK if TS is locked.
*/
sony_dvb_result_t dvb_cxd2820_WaitTSLock (sony_dvb_cxd2820_t * pDriver);

/**
 @brief Cancel any pending operations on the demod.
        Cancels ::dvb_cxd2820_BlindTune, ::dvb_cxd2820_Tune
                and ::dvb_cxd2820_Scan and ::dvb_cxd2820_WaitTSLock.

        This function is thread safe.

 @param pDriver The driver instance.

 @return SONY_DVB_OK if able to cancel the pending operation.
*/
sony_dvb_result_t dvb_cxd2820_Cancel (sony_dvb_cxd2820_t * pDriver);

/**
 @brief Set the DVB-T2 auto spectrum detection enable/disable.
        The DVB-T2 OFDM core of the CXD2820 does not automatically check for 
        spectrum inversion in hardware. This function enables a software detection 
        loop during acquisition. It is expected to be only used during
        scanning.
        Once tuned, use the ::dvb_demod_monitorT2_SpectrumSense to 
        monitor the found sense.

 @param pDriver The driver instance.
 @param enable Enable(1)/disable(0) the auto spectrum detection mechanism.
 
 @return SONY_DVB_OK if able to set auto spectrum detection.

*/
sony_dvb_result_t dvb_cxd2820_SetT2AutoSpectrumDetection (sony_dvb_cxd2820_t* pDriver, 
                                                          uint8 enable);

#endif /* SONY_DVB_CXD2820_H_ */
