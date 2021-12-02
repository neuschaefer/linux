/*------------------------------------------------------------------------------

 <dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
#ifndef SONY_DVBC2_DEMOD_H
#define SONY_DVBC2_DEMOD_H

#include "sony_dvb.h"
#include "sony_dvb_demod.h"

#ifndef SONY_DVB_SUPPORT_DVBC2

#define sony_dvb_demodC2_InitScanSeq(pDemod, pScanParams, scanSeq)      SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_InitTuneSeq(pDemod, pTuneParams, tuneSeq)      SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_ScanSeq(pDemod, scanSeq)                       SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_TuneSeq(pDemod, tuneSeq)                       SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_SetConfig(pDemod, configId, value)             SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_SetPLPConfig(autoPLP, plpId)                   SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_CheckTSLock(pDemod, pLock)                     SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_BandwidthChanged(pDemod)                       SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_Sleep(pDemod)                                  SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_TuneInitialize(pDemod, pTuneParams)            SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_Tune(pDemod)                                   SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_ScanInitialize(pDemod, pScanParams)            SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_ScanFinalize(pDemod, pScanParams)              SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_TuneFinalize(pDemod, pTuneParams,tuneResult)   SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_CheckDemodLock(pDemod, pLock)                  SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_EnableBlindAcquisition(pDemod)                 SONY_RESULT_ERROR_NOSUPPORT

#define sony_dvb_demodC2_CheckBandwidth(bandWidth)                  SONY_RESULT_ERROR_NOSUPPORT       

#else

/*-----------------------------------------------------------------------------
    Defines
-----------------------------------------------------------------------------*/
#define SONY_DVBC2_DS_NORMAL                    0x00        /**< Normal data slice */
#define SONY_DVBC2_DS_DEPENDANT_STATIC          0x01        /**< Dependant static data slice */

#define SONY_DVBC2_SEQ_RUN                      0           /**< DVBC2 tune and scan routine is running */
#define SONY_DVBC2_SEQ_COMPLETE                 1           /**< DVBC2 tune and scan routine is complete */

#define SONY_DVBC2_NO_ACTION                    0           /**< No tune or callback requests to the integration layer */

#define SONY_DVBC2_TUNE_REQUIRED                1           /**< RF tune requested by the scan or tune sequences to the integration layer */
#define SONY_DVBC2_CALLBACK_REQUIRED            1           /**< Callback requested for the current scan result, read and actioned by the integration layer */

#define SONY_DVBC2_PS_SIZE                      23          /**< Number of bytes for a single preset */

#define SONY_DVBC2_NUM_REGS_PER_NOTCH           5           /**< Number of bytes of data for each individual notch */
#define SONY_DVBC2_FILTER1                      0           /**< Co & Adjecsant channel Supression filter 1 */
#define SONY_DVBC2_FILTER2                      1           /**< Co & Adjecsant channel Supression filter 2 */
#define SONY_DVBC2_FILTER3                      2           /**< Co & Adjecsant channel Supression filter 3 */
#define SONY_DVBC2_FILTER4                      3           /**< Co & Adjecsant channel Supression filter 4 */

#define SONY_DVBC2_TU_6MHZ                      (256 * 7)   /* ((4096 / 48) * 7) NOTE: 4096/48 = 256/3. Use SONY_DVBC2_TU_6MHZ_FACTOR along with this quantity (seperated to minimise rounding */
#define SONY_DVBC2_TU_6MHZ_FACTOR               3
#define SONY_DVBC2_TU_8MHZ                      (64 * 7)    /* ((4096 / 64) * 7) */
#define SONY_DVBC2_KHZ_FACTOR                   1000        /* Factor to scale to KHz */

/*-----------------------------------------------------------------------------
    Enumerations
-----------------------------------------------------------------------------*/
/** @brief DVB-C2 Tune Sequence states. */
typedef enum {
    SONY_DVBC2_SCAN_STAGE_1,            /**< Calls DemodSetting1 : Requests RF tune */
    SONY_DVBC2_SCAN_STAGE_2,            /**< Calls CheckDemodLock : Requests Sleep */
    SONY_DVBC2_SCAN_STAGE_3,            /**< Calls Change1stTrial : Requests N/A */
    SONY_DVBC2_SCAN_STAGE_4,            /**< Calls waitL1DataState : Requests Sleep and callback */
    SONY_DVBC2_SCAN_STAGE_5,            /**< Calls GetDataSliceIDList : Requests N/A */
    SONY_DVBC2_SCAN_STAGE_6,            /**< Calls SetNextDS : Requests N/A */
    SONY_DVBC2_SCAN_STAGE_7,            /**< Calls waitL1DataState : Requests Sleep and callback */
    SONY_DVBC2_SCAN_STAGE_8,            /**< Calls GetPLPIDList : Requests N/A */
    SONY_DVBC2_SCAN_STAGE_9,            /**< Calls CalculateSystemTuningFrequency : Requests N/A Does not return to Integration layer */
    SONY_DVBC2_SCAN_TUNE_NEXT,          /**< Calls SetPLPID if same DS : Requests  demod and RF tune if new DS */
    SONY_DVBC2_SCAN_FINALIZE            /**< Calls N/A : Requests N/A */
} sony_dvbc2_scan_seq_state_t;


/** @brief DVB-C2 Tune Sequence states. */
typedef enum {
    SONY_DVBC2_TUNE_INITIALIZE = 0,     /**< Calls ReleasePreset : Requests Tune RF */
    SONY_DVBC2_TUNE_NON_DSDS_STAGE_1,   /**< Calls DemodSetting2 : Requests N/A */
    SONY_DVBC2_TUNE_DSDS_STAGE_1,       /**< Calls DemodSetting3 : Requests N/A */
    SONY_DVBC2_TUNE_DSDS_STAGE_2,       /**< Calls PresetReady1State : Requests Sleep */
    SONY_DVBC2_TUNE_DSDS_STAGE_3,       /**< Calls Change1stTrial, CalculateCentreFrequency, DemodSetting4 : Requests N/A */
    SONY_DVBC2_TUNE_DSDS_STAGE_4,       /**< Calls PresetReady2State : Requests Sleep */
    SONY_DVBC2_TUNE_DSDS_STAGE_5,       /**< Calls Change1stTrial, CalculatePresetValue : Requests Tune RF */
    SONY_DVBC2_TUNE_DSDS_STAGE_6,       /**< Calls DemodPreset : Requests N/A */
    SONY_DVBC2_TUNE_WAIT_LOCK,          /**< Calls CheckDemodLock [scan], CheckTSLock [tune] : Requests Sleep */
    SONY_DVBC2_TUNE_CHANGE_FIRST_TRIAL, /**< Calls Change1stTrial : Requests N/A */
    SONY_DVBC2_TUNE_FINALIZE            /**< Calls N/A : Requests N/A */
} sony_dvbc2_tune_seq_state_t;         

/*-----------------------------------------------------------------------------
    Structs
-----------------------------------------------------------------------------*/
/**
 @brief DVB-C2 Scan Sequence strcuture.
*/
typedef struct sony_dvbc2_scan_seq_t {
    uint8_t complete;                               /**< Sequencer complete flag. */
    sony_dvbc2_scan_seq_state_t state;              /**< Sequence state. Internal use only. */
    uint8_t tuneRequired;                           /**< RF tune required flag */
    uint8_t callbackRequired;                       /**< A callback is required */
    sony_result_t loopResult;                       /**< Result code for the last wait loop */
    sony_dvb_tune_params_t *tunerParams;            /**< Tune parameters for next found data slice */
    uint8_t ds_tune_index;                          /**< Current data slice index in ds array */
    uint8_t plp_tune_index;                         /**< Current PLP index in the ds[].dataPLPIDs array */
    uint32_t centerFreqKHz;                         /**< Current scan centre frequency kHz */

    /* Scan data storage */
    sony_dvbc2_dataSlice_t ds[C2_DS_MAX];           /**< DSLICE ID information */
    uint8_t num_ds;                                 /**< Number of dataslices */
    uint8_t current_ds_index;                       /**< Current data slice index in the ds array */
    sony_dvbc2_notch_data_t notch[C2_NUM_NOTCH_MAX];/**< Notch data */
    uint8_t num_notch;                              /**< Number of notches */
    sony_dvbc2_stf_data_t stf;                      /**< System tuning frequency data */
    uint32_t sf;                                    /**< Start freqency */
    uint8_t gi;                                     /**< Guard Interval */
    uint8_t bnw;                                    /**< Notch width */
    uint16_t dx;                                    /**<  */
    uint32_t scanResumeFrequency;                   /**< Next frequency for the scan to use instead of the specified step */
    uint32_t system_tuning_frequency;               /**< System tuning frequency kHz */

    uint32_t sleepTime;                             /**< Sleep duration requested from demod layer to integration */
    uint32_t currentTime;                           /**< Current time elapsed from sequence start */
    uint32_t loopStartTime;                         /**< Time that the current loop operation started */
} sony_dvbc2_scan_seq_t;

/**
 @brief DVB-C2 Tune Sequence strcuture.
*/
typedef struct sony_dvbc2_tune_seq_t {
    uint8_t complete;                               /**< Sequencer complete flag. */
    sony_dvbc2_tune_seq_state_t state;              /**< Sequence state. Internal use only. */
    uint8_t tuneRequired;                           /**< RF tune required flag */
    uint32_t rfFrequency;                           /**< RF frequency to be tuned to */
    uint8_t bandwidthMHz;                           /**< Bandwidth of RF to tune to */
    sony_result_t lockResult;                       /**< Lock result from the wait TS lock function */

    /* Tuning Algorithm */
    sony_dvbc2_tune_params_t *pTuneData;            /**< Tune parameters */
    uint8_t filterSelect;                           /**< Index of filter range to use */
    uint16_t cas_dagc_target;                       /**<  */
    uint32_t crcg2_bn_freq_ofst;                    /**<  */
    uint8_t trcg_nominalrate[5];                    /**<  */
    uint8_t ps[SONY_DVBC2_PS_SIZE];                 /**< Preset data */
    uint32_t center_freq;                           /**< 2nd RF frequency to be tuned to for Dependant Static DS scenario */

    uint32_t sleepTime;                             /**< Sleep duration requested from demod layer to integration */
    uint32_t currentTime;                           /**< Current time elapsed from sequence start */
    uint32_t loopStartTime;                         /**< Time that the current loop operation started */
} sony_dvbc2_tune_seq_t;

/*-----------------------------------------------------------------------------
    Function Prototypes
-----------------------------------------------------------------------------*/

/**
 @brief     Initialize the sony_dvbc2_scan_seq_t structure 

 @param     pDemod        The demod instance.
 @param     pScanParams    Scan parameters structure, populated by the calling application.
 @param     scanSeq        Internal structure used to store scan parameters and result data.

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_InitScanSeq (sony_dvb_demod_t        * pDemod, 
                                            sony_dvb_scan_params_t    * pScanParams,
                                            sony_dvbc2_scan_seq_t    * scanSeq);

/**
 @brief     Initialize the sony_dvbc2_tune_seq_t structure 

 @param     pDemod        The demod instance.
 @param     pTuneParams    Tune parameters structure, populated by the calling application.
 @param     tuneSeq        Internal structure used to store tune parameters and result data.

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_InitTuneSeq (sony_dvb_demod_t        * pDemod, 
                                            sony_dvb_tune_params_t    * pTuneParams,
                                            sony_dvbc2_tune_seq_t    * tuneSeq);

/**
 @brief     State machine based function used to step through the scan process for an
            individual C2 system.  This function is cranked by the Scan function
            in sony_dvb_integration.c.

 @param     pDemod        The demod instance.
 @param     scanSeq        Internal structure used to store scan parameters and result data.

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_ScanSeq (sony_dvb_demod_t * pDemod, sony_dvbc2_scan_seq_t * scanSeq);

/**
 @brief     State machine based function used to step through the tune process for a
            specified C2 channel.  This function is cranked by the Tune function in sony_dvb_integration.c.

 @param     pDemod        The demod instance.
 @param     scanSeq        Internal structure used to store scan parameters and result data.

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_TuneSeq (sony_dvb_demod_t * pDemod, sony_dvbc2_tune_seq_t * tuneSeq);

/**
 @brief     Set the C2 demodulator for configuration items


 @param     pDemod        The demod instance.
 @param     configId    The configuration Id.
 @param     value        The value of the configuration item.

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_SetConfig (sony_dvb_demod_t * pDemod, sony_dvb_demod_config_id_t configId, int32_t value);

/**
 @brief     Setup the PLP configuration of the demodulator. Selecting both the device
            PLP operation (automatic/manual PLP select) and the PLP to be selected
            in manual PLP mode.

 @param     pDemod        The demod instance.
 @param     autoPLP        The auto PLP setting.
                        - DVB_PLP_AUTO_SELECT   (0x00): Fully automatic. The first PLP found during acquisition will be output.
                        - DVB_PLP_MANUAL_SELECT (0x01): The data PLP ID set by plpId will be output.
                        If the PLP with the ID is not found, then a PLP error is indicated
                        (::dvb_demod_monitorC2_DataPLPError) but the
                        demod will still output the first found data PLP Id.
 @param     plpId The PLP Id to set.

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_SetPLPConfig (sony_dvb_demod_t * pDemod, uint8_t autoPLP, uint8_t plpId);

/**
 @brief     Check DVB-C2 TS Lock. Check TS lock state of demod. (no wait)

 @param     pDemod        Instance of demod control struct
 @param     pLock        TS lock state

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_CheckTSLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);

/**
 @brief     Modifies the current bandwidth settings in the demodulator

 @param     pDemod        Instance of demod control struct

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_BandwidthChanged (sony_dvb_demod_t * pDemod);

/**
 @brief     Put the demodulator into sleep mode.

 @param     pDemod        Instance of demod control struct
 
 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_Sleep (sony_dvb_demod_t * pDemod);

/**
 @brief     Sets up the demodulator ready for a tune

 @param     pDemod        Instance of demod control struct
 @param     pTuneParams    Tune parameters structure, populated by the calling application.
 
 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_TuneInitialize (sony_dvb_demod_t * pDemod, sony_dvb_tune_params_t * pTuneParams);

/**
 @brief     Configures the demodulator to move from sleep state to active

 @param     pDemod            Instance of demod control struct
 
 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_Tune (    sony_dvb_demod_t    * pDemod);

/**
 @brief     Sets up the demodulator ready for a spectrum scan

 @param     pDemod        Instance of demod control struct
 @param     pScanParams    Scan parameters structure, populated by the calling application.
 
 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_ScanInitialize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

/**
 @brief     Configure any specific settings or registers required to finalise the scan process

 @param     pDemod        Instance of demod control struct
 @param     pScanParams    Scan parameters structure, populated by the calling application.
 
 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_ScanFinalize (sony_dvb_demod_t * pDemod, sony_dvb_scan_params_t * pScanParams);

/**
 @brief     Configure any specific settings or registers required to finalise the tune process

 @param     pDemod        Instance of demod control struct
 @param     pScanParams    Tune parameters structure, populated by the calling application.
 @param     tuneResult    Sony result code for the current tune process
 
 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_TuneFinalize (sony_dvb_demod_t * pDemod, sony_dvb_tune_params_t * pTuneParams, sony_result_t tuneResult);

/**
 @brief     Check DVB-C2 demod Lock. Check lock state of demod. (no wait)

 @param     pDemod        Instance of demod control struct
 @param     pLock        Demod lock state

 @return    SONY_RESULT_OK if successful.
*/
sony_result_t sony_dvb_demodC2_CheckDemodLock (sony_dvb_demod_t * pDemod, sony_dvb_demod_lock_result_t * pLock);
sony_result_t sony_dvb_demodC2_CheckBandwidth(uint8_t bandWidth);

#endif /*SONY_DVB_SUPPORT_DVBC2 */
#endif /* SONY_DVBC2_DEMOD_H */
