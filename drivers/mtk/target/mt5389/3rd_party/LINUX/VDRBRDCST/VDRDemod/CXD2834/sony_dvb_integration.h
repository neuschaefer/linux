/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**
 @file    sony_dvb_integration.h

          This file provides the integration layer for controlling the 
          tuner and demodulator together for acquisition algorithms and 
          state control.

*/
/*----------------------------------------------------------------------------*/
#ifndef SONY_DVB_INTEG_H_
#define SONY_DVB_INTEG_H_

#include "sony_dvb.h"
#include "sony_dvb_demod.h"
#include "sony_dvb_tuner.h"
#include "sony_i2c.h"

/**
 @brief demod driver version.
*/
extern const char *sony_demod_driver_version;

/**
 @brief demod driver build date.
*/
extern const char *sony_demod_driver_build_date;

/**
 @brief demod driver build time.
*/
extern const char *sony_demod_driver_build_time;

/*------------------------------------------------------------------------------
 Types
------------------------------------------------------------------------------*/


/**
 @brief The high level driver object.
        This is the primary interface used for controlling
        and monitoring the demod device and connected tuner.

        This object is the aggregation of the demodulator and the tuner 
        to represent a single front end system. It can be used wholly, partly or just
        as a reference for developing the application software.
*/
typedef struct sony_dvb_integration_t {
    sony_dvb_demod_t *pDemod;                           /**< Instance of the demodulator. */
    sony_dvb_tuner_t *pTuner;                           /**< The connected tuner. */
    sony_atomic_t cancel;                               /**< Cancellation indicator variable. */
    void *user;                                         /**< User data. */
} sony_dvb_integration_t;

/**
 @brief Callback function that is called for every attempted frequency during a 
        scan.
        
        - DVB-T: This callback is invoked when TPS lock is achieved. 
                 Use ::dvb_integration_WaitTSLock to wait for TS lock.
        - DVB-T2: This callback is invoked when T2 demod lock (P1/L1-pre/L1-post) is achieved.
                 Use ::dvb_integration_WaitTSLock to wait for TS lock. 
                 NOTE: The callback is invoked for each PLP within the signal.
        - DVB-C: This callback is invoked when TS lock is achieved.
        - DVB-C2: This callback is invoked when C2 demod lock is achieved.
                 Use ::dvb_integration_WaitTSLock to wait for TS lock. 
                 NOTE: The callback is invoked for each PLP in each data slice within the system.
 
 @param pDriver The driver instance.
 @param result The current scan result.

*/
typedef void (*sony_dvb_scan_callback_t) (sony_dvb_integration_t * pDriver, sony_dvb_scan_result_t * result);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

/**
 \callgraph
 @brief Construct the driver.

        This function is called by the application in order
        to setup the ::sony_dvb_demod_t structure and 
        ::sony_dvb_integration_t::pDemod member.

        This MUST be called before calling ::dvb_integration_Initialize.
        
        Notes on driver configuration:
        - By default, the demodulator uses an inverted AGC.
          If the connected tuner uses a non-inverted IFAGC, call 
          ::dvb_demod_SetConfig with ::DEMOD_CONFIG_IFAGCNEG = 0 to setup 
          the demodulator with postive IFAGC sense, after calling ::dvb_integration_Initialize.
        - By default, the driver enables the RF level monitoring on the 
          device, call ::dvb_demod_SetConfig with ::DEMOD_CONFIG_RFLVMON_ENABLE = 0
          to disable it, after calling ::dvb_integration_Initialize.
        
 @note Memory is not allocated dynamically.
 @param xtalFreq The frequency of the demod crystal.
 @param I2CAddress The demod I2C address in 8-bit form.
 @param pDemodI2c The I2C driver that the demod will use as the I2C interface.
 @param pTuner The tuner driver to use with this instance of the driver.
              The tuner I2C interface should have been setup before this call.
 @param pDemod Reference to memory allocated for the demodulator instance. The create 
        function will setup this demodulator instance also.
 @param pDriver The driver to create.
              It must be a valid pointer to allocated memory for a 
              ::sony_dvb_integration_t structure.

 @return SONY_RESULT_OK if successfully created demod driver api.
*/
sony_result_t sony_dvb_integration_Create (sony_dvb_xtal_t xtalFreq,
                                               uint8_t I2CAddress,
                                               sony_i2c_t * pDemodI2c,
                                               sony_dvb_demod_t * pDemod, sony_dvb_tuner_t * pTuner,
                                               sony_dvb_integration_t * pDriver);

/**
 \callgraph
 @brief Initialize the demodulator and tuner. 
        After this operation, the demodulator and tuner are in a low power 
        state (demod state = SONY_DVB_DEMOD_STATE_SLEEP) 
        awaiting for tune and scan commands.

 @param pDriver The driver instance.

 @return SONY_RESULT_OK if OK.
*/
sony_result_t sony_dvb_integration_Initialize (sony_dvb_integration_t * pDriver);

/**
 @brief Finalize the demodulator and tuner. The demodulator and tuner
        are in a low power state and the driver interfaces are no longer available.
        Initialize must be called in order to use the demodulator/tuner again.

 @param pDriver The driver instance.

 @return SONY_RESULT_OK if OK.
*/
sony_result_t sony_dvb_integration_Finalize (sony_dvb_integration_t * pDriver);

/**
 @brief Performs a scan over the spectrum of interest. 
        The scan can perform a multiple system scan for DVB-T and DVB-T2 channels by 
        setting the sony_dvb_scan_params_t::multiple flag (1) and setting the 
        sony_dvb_scan_params_t::system = sony_dvb_system_t::SONY_DVB_SYSTEM_DVBT.
        Similarily DVB-C and DVB-C2 multiple system scans can be initiated by
        setting the sony_dvb_scan_params_t::multiple flag (1) and setting the 
        sony_dvb_scan_params_t::system = sony_dvb_system_t::SONY_DVB_SYSTEM_DVBC.
        Blocks the calling thread while scanning. Use ::dvb_integration_Cancel to cancel 
        the operation at any time.
 
 @param pDriver The driver instance.
 @param pScanParams The scan parameters.
 @param callBack User registered call-back to receive scan progress
        information and notification of found channels. 
        The call back is called fore very attempted frequency during a  scan.

 @return SONY_RESULT_OK if scan completed successfully.
        
*/
sony_result_t sony_dvb_integration_Scan (sony_dvb_integration_t * pDriver,
                                             sony_dvb_scan_params_t * pScanParams, sony_dvb_scan_callback_t callBack);


/*------------------------------------------------------------------------------
 Functions: Algorithms
------------------------------------------------------------------------------*/

/**
 @brief Performs a blind acquisition to the channel. Blocks the calling thread
        until the acquisition is complete or has timed out (see notes for 
        what is meant by "acquisition complete"). 
        Use ::dvb_integration_Cancel to cancel the operation at any time.
        The demodulator may be asleep before this call, tune will power up the 
        demodulator into the appropriate system based on the passed parameters.

        - DVBT: Waits for TPS/TS lock, returns once TPS/TS lock is achieved.
        - DVBT2: Acquisition waits for demod lock (P1/L1-pre/L1-post).
          Once successful, the application can wait for TS lock (::dvb_integration_WaitTSLock)
          to see if the demod is able to decode the PLPs carried on the channel.
          If TS is locked, then the first data PLP in the channel is selected 
          automatically and is output by the demodulator.
          In order to determine the list of data PLPs carried, then use the function 
          ::dvb_demod_monitorT2_DataPLPs.
          Once acquired, use ::dvb_demod_monitorT2_SpectrumSense to see which sense was detected.

 @param pDriver The driver instance.
 @param system The system (DVB-T/C/T2) with which to attempt the blind acquisition.
 @param centreFreqkHz The expected centre frequency of the channel in kHz.
 @param bwMHz The expected bandwidth of the channel in MHz.
 @return SONY_RESULT_OK if successfully acquired channel.
*/
sony_result_t sony_dvb_integration_BlindTune (sony_dvb_integration_t * pDriver,
                                                  sony_dvb_system_t system, uint32_t centreFreqkHz, uint8_t bwMHz);

/**
 @brief Performs acquisition to the channel. 
        Blocks the calling thread until the TS has locked or has timed out.
        Use ::dvb_integration_Cancel to cancel the operation at any time.
 
        Notes on tuning:
        - DVBT: If using preset tuning (usePresets=1) and the mode/gi information does not match, then 
          the demodulator will FAIL the acquisition.
          If not using preset tuning (usePresets=0), then only the profile (HP/LP) is required for
          tuning. If the LP stream is not available, then the HP stream will be output.
        - DVBT2 only. 
          Only the data PLP ID is required for tuning. In this case, the device will 
          wait for a T2 P1 symbol in order to decode the 
          L1 pre signalling and then begin demodulation.
          If the data PLP ID (::sony_dvbt2_tune_params_t::dataPLPId) or 
          the associated common PLP is not found in the channel, 
          the device will always select the first found PLP and output the associated TS.
          In this case, an indicator in the ::sony_dvbt2_tune_params_t::tuneResult will 
          be set.
          
 @param pDriver The driver instance.
 @param pTuneParams The parameters required for the tune.

 @return SONY_RESULT_OK if tuned successfully to the channel.
*/
sony_result_t sony_dvb_integration_Tune (sony_dvb_integration_t * pDriver, sony_dvb_tune_params_t * pTuneParams);

/**
 @brief Place the demodulator and tuner in the lowest possible power mode.    
        Calling tune and scan functions shall wake the demodulator and tuner.

 @param pDriver The driver instance.

 @return SONY_RESULT_OK if OK.
*/
sony_result_t sony_dvb_integration_Sleep (sony_dvb_integration_t * pDriver);

/**
 @brief Wait for the demodulator to indicate TS lock.
        Timeout for function depends on the current system (DVB-C/T/T2).
        Blocks the calling thread until the TS has locked or has timed out.
        Use ::dvb_integration_Cancel to cancel the operation at any time.

 @param pDriver The driver instance.

 @return SONY_RESULT_OK if TS is locked.
*/
sony_result_t sony_dvb_integration_WaitTSLock (sony_dvb_integration_t * pDriver);

/**
 @brief Cancel any pending operations on the demod.
        Cancels ::dvb_integration_BlindTune, ::dvb_integration_Tune
                and ::dvb_integration_Scan and ::dvb_integration_WaitTSLock.

        This function is thread safe.

 @param pDriver The driver instance.

 @return SONY_RESULT_OK if able to cancel the pending operation.
*/
sony_result_t sony_dvb_integration_Cancel (sony_dvb_integration_t * pDriver);

/**
 @brief Set configuration options on the demodulator.

 @param pDriver The driver instance.
 @param configId The configuration ID to set. See ::sony_dvb_demod_config_id_t.
 @param value The associated value. Depends on the configId.

 @return SONY_RESULT_OK if successful set the configuration option.
*/
sony_result_t sony_dvb_integration_SetConfig (sony_dvb_integration_t * pDriver, sony_dvb_demod_config_id_t configId,
                                                  int32_t value);

#endif /* SONY_DVB_INTEG_H_ */
