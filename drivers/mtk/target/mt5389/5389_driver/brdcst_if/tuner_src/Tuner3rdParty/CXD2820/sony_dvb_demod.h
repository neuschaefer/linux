/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2009 Sony Corporation.

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

#include <sony_dvb.h>
#include <sony_dvb_i2c.h>
#include <sony_dvb_t2.h>

// MTK include files
#include "PD_Def.h"  

/*------------------------------------------------------------------------------
  I2C address default (8bit form)
------------------------------------------------------------------------------*/
#define DVB_DEMOD_DVBT_ADDRESS              0xD8    /**< DVB-T I2C address. */
#define DVB_DEMOD_DVBT2_ADDRESS             0xD8    /**< DVB-T2 I2C address. Shared with DVB-T I2C address space. */
#define DVB_DEMOD_DVBC_ADDRESS              0xDC    /**< DVB-C I2C address. */

/*------------------------------------------------------------------------------
  Recommended timeouts for wait for "lock"
------------------------------------------------------------------------------*/
#define DVB_DEMOD_DVBT_EARLY_UNLOCK_WAIT    80      /**< DVBT 80ms wait before reading unlock flags read. */
#define DVB_DEMOD_DVBT_WAIT_LOCK            1000    /**< DVB-T 1000ms for TPS Lock */
#define DVB_DEMOD_DVBT_WAIT_TS_LOCK         1000    /**< DVB-T 1000ms for TS (from TPS Lock) */

#define DVB_DEMOD_DVBC_WAIT_TS_LOCK         1000    /**< DVB-C 1000ms for TS Lock */

#define DVB_DEMOD_DVBT2_P1_WAIT             300     /**< DVB-T2 300ms before checking early no P1 (T2 or FEF) indication. */
#define DVB_DEMOD_DVBT2_T2_P1_WAIT          600     /**< DVB-T2 600ms before checking for P1 (T2 only) indication. */
#define DVB_DEMOD_DVBT2_WAIT_LOCK           5000    /**< DVB-T2 5000ms for demod lock indication. */
#define DVB_DEMOD_DVBT2_WAIT_TS_LOCK        5000    /**< DVB-T2 5000ms for TS lock. Assumes that demod core is locked. */

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/

/**
 @brief The Chip ID mapping.
*/
typedef enum {
    SONY_DVB_CXD2820_IP     = 0x00,     /**< FPGA based CXD2820. */
    SONY_DVB_CXD2817        = 0x70,     /**< CXD2817R DVB-T/C device. */
    SONY_DVB_CXD2820_TS     = 0x80,     /**< CXD2820 DVB-T/T2/C TS - Test Sample. */
    SONY_DVB_CXD2820_ES1_0  = 0xE0,     /**< CXD2820 DVB-T/T2/C ES1.0 - Engineering Sample 1.0. */
    SONY_DVB_CXD2820_ES1_1  = 0xE1      /**< CXD2820 DVB-T/T2/C ES1.1 - Engineering Sample 1.1. */
} sony_dvb_demod_chip_id_t ;

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

/* DVB-T definitions */

/**
 @brief DVB-T constellation.
*/
typedef enum {
    DVBT_CONSTELLATION_QPSK,            /**< QPSK */
    DVBT_CONSTELLATION_16QAM,           /**< 16-QAM */
    DVBT_CONSTELLATION_64QAM,           /**< 64-QAM */
    DVBT_CONSTELLATION_RESERVED_3       /**< Reserved. */
} sony_dvb_demod_dvbt_constellation_t;

/**
 @brief DVB-T hierarchy.
*/
typedef enum {
    DVBT_HIERARCHY_NON,                 /**< Non-hierarchical service. */
    DVBT_HIERARCHY_1,                   /**< a = 1. */
    DVBT_HIERARCHY_2,                   /**< a = 2. */
    DVBT_HIERARCHY_4                    /**< a = 4. */
} sony_dvb_demod_dvbt_hierarchy_t;

/**
 @brief DVB-T code rate.
*/
typedef enum {
    DVBT_CODERATE_1_2,                  /**< 1/2 */
    DVBT_CODERATE_2_3,                  /**< 2/3 */
    DVBT_CODERATE_3_4,                  /**< 3/4 */
    DVBT_CODERATE_5_6,                  /**< 5/6 */
    DVBT_CODERATE_7_8,                  /**< 7/8 */
    DVBT_CODERATE_RESERVED_5,           /**< CR reserved 5 */
    DVBT_CODERATE_RESERVED_6,           /**< CR reserved 6 */
    DVBT_CODERATE_RESERVED_7            /**< CR reserved 7 */
} sony_dvb_demod_dvbt_coderate_t;

/**
 @brief DVB-T guard interval.
*/
typedef enum {
    DVBT_GUARD_1_32,                    /**< 1/32 */
    DVBT_GUARD_1_16,                    /**< 1/16 */
    DVBT_GUARD_1_8,                     /**< 1/8  */
    DVBT_GUARD_1_4                      /**< 1/4  */
} sony_dvb_demod_dvbt_guard_t;

/**
 @brief DVB-T mode.
*/
typedef enum {
    DVBT_MODE_2K,                       /**< 2k mode */
    DVBT_MODE_8K,                       /**< 8k mode */
    DVBT_MODE_RESERVED_2,               /**< Mode Reserved 2 */
    DVBT_MODE_RESERVED_3                /**< Mode Reserved 3 */
} sony_dvb_demod_dvbt_mode_t;

/**
 @brief DVB-T HP/LP profile.
*/
typedef enum {
    DVBT_PROFILE_HP = 0,                /**< HP */
    DVBT_PROFILE_LP                     /**< LP */
} sony_dvb_demod_dvbt_profile_t;

/* DVB-C definitions */

/**
 @brief DVB-C constellation.
*/
typedef enum {
    DVBC_CONSTELLATION_16QAM,           /**< 16-QAM */
    DVBC_CONSTELLATION_32QAM,           /**< 32-QAM */
    DVBC_CONSTELLATION_64QAM,           /**< 64-QAM */
    DVBC_CONSTELLATION_128QAM,          /**< 128-QAM */
    DVBC_CONSTELLATION_256QAM           /**< 256-QAM */
} sony_dvb_demod_dvbc_constellation_t;

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/

/* These macro is used to calculate IF frequency setting register values.        */
/* This macro includes floating point values but will be calculated by compiler! */

/**
 @brief Create the DVB-T fields for ::sony_dvb_demod_t::iffreq_config.
*/
//#define DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBT(iffreq) ((uint32)(((iffreq)/41)*16777216 + 0.5))
//yuan mark for linux build fail!!!!! 20110518
/**
 @brief Create the DVB-T2 fields for ::sony_dvb_demod_t::iffreq_config.
*/
//#define DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBT2(iffreq) ((uint32)(((iffreq)/41)*16777216 + 0.5))
//yuan mark for linux build fail!!!!! 20110518

/**
 @brief Create the DVB-C fields for ::sony_dvb_demod_t::iffreq_config.
        OREG_ITB_DWNCONVER_FRQENCY = - (IF freq[MHz] / 41[MHz] * 2^14) = - X 
        2's complement of X (14bit) = 2^14 - (IF freq[MHz] / 41[MHz] * 2^14)
        = 2^14 * (1.0 - IF freq[MHz] / 41[MHz]) 
*/
//#define DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBC(iffreq) ((uint16)(16384*(1-(iffreq)/41) + 0.5) & 0x3FFF)
//yuan mark for linux build fail!!!!! 20110518

/**
 @brief The DVB-T2 tuning information.
*/
typedef struct sony_dvb_demod_t2_preset_t {

    /**
     @brief The pre-amble mixed indicator (from the S2). 
            0: Not mixed. 1: Mixed. 
    */
    uint8 mixed;

    /**
     @brief S1 signalling. SISO/MISO indication. 0: SISO, 1: MISO. 
     */
    sony_dvb_dvbt2_s1_t s1;

    /**
     @brief The FFT mode of transmission (from the S2).
    */
    sony_dvb_dvbt2_mode_t mode;

    /**
     @brief The guard interval used for the super-frame. 
    */
    sony_dvb_dvbt2_guard_t gi;

    /**
     @brief The pilot pattern for the OFDM symbols in this frame.
    */
    sony_dvb_dvbt2_pp_t pp;

    /**
     @brief BW extension indicator (8K/16K/32K only).
    */
    uint8 bwExt;

    /**
     @brief The PAPR indicator. 
    */
    sony_dvb_dvbt2_papr_t papr;

    /**
     @brief Number of OFDM symbols per T2-frame. 
    */
    uint16 numSymbols;
} sony_dvb_demod_t2_preset_t;

/* Register values of IF frequency setting */
/* Should be set using DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBT/C/T2 macros */

/**
 @brief List of register values for IF frequency configuration.
        Used for handling tuners that output a different IF 
        depending on the expected channel BW.
*/
typedef struct {
    uint32 config_DVBT6;              /**< DVB-T 6MHz */
    uint32 config_DVBT7;              /**< DVB-T 7MHz */
    uint32 config_DVBT8;              /**< DVB-T 8MHz */
    uint32 config_DVBT2_5;            /**< DVB-T2 5MHz */
    uint32 config_DVBT2_6;            /**< DVB-T2 6MHz */
    uint32 config_DVBT2_7;            /**< DVB-T2 7MHz */
    uint32 config_DVBT2_8;            /**< DVB-T2 8MHz */
    uint16 config_DVBC;               /**< DVB-C */
} sony_dvb_demod_iffreq_config_t;

/**
 @brief Debugging structure definition for lock
        detection.
*/
typedef struct {
    uint8 IREG_SEQ_OSTATE;            /**< Debugging. */   
    uint8 IREG_ARGD_STATE;            /**< Debugging. */  
    uint8 IREG_COSNE_COK;             /**< Debugging. */  
} sony_dvb_demod_DVBT_lock_info_t;

/**
 @brief Enumeration of spectrum inversion monitor values.
*/
typedef enum {
    DVB_DEMOD_SPECTRUM_NORMAL = 0,      /**< Spectrum normal sense. */
    DVB_DEMOD_SPECTRUM_INV              /**< Spectrum inverted. */    
} sony_dvb_demod_spectrum_sense_t ;

/**
 @brief The demodulator definition which allows control of the 
        demodulator device through the defined set of functions.
        This portion of the driver is seperate from the tuner portion 
        and so can be operated independently of the tuner.
*/
typedef struct sony_dvb_demod_t {

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
    uint8 bandWidth;

    /**
     @brief DVB-T I2C address (8-bit form - 8'bxxxxxxx0).
    */
    uint8 i2cAddressT;

    /**
     @brief DVB-T2 I2C address (8-bit form - 8'bxxxxxxx0).
    */
    uint8 i2cAddressT2;

    /**
     @brief DVB-C I2C address (8-bit form - 8'bxxxxxxx0).
    */
    uint8 i2cAddressC;

    /**
     @brief I2C API instance.
    */
    sony_dvb_i2c_t *pI2c;

    /**
     @brief IF frequency configuration. Configure prior to 
            initialisation. 
            Use the ::DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBT,
            ::DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBC,
            ::DVB_DEMOD_MAKE_IFFREQ_CONFIG_DVBT2 macros for configuration.
    */
    sony_dvb_demod_iffreq_config_t iffreq_config;

    /**
     @brief Enable/Disable ASCOT special settings.
    */
    int is_ascot;

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
     @brief DVB-T scan mode enable/disable.  
            Only change this indicator during the 
            SONY_DVB_DEMOD_STATE_SLEEP state.
            Use ::dvb_demod_SetConfig to configure this flag.
    */
    int dvbt_scannmode;

    /**
     @brief Debugging structure which holds the results of the 
            last acquisition.
    */
    sony_dvb_demod_DVBT_lock_info_t dvbt_lock_info;

    
    /**
     @brief Auto detected chip ID at initialisation.
    */
    sony_dvb_demod_chip_id_t chipId ;

    /**
     @brief The sense configured on the demodulator with 
            ::dvb_demod_SetConfig.
    */
    sony_dvb_demod_spectrum_sense_t confSense ;

    /**
     @brief The active IO Hi-Z setting. 
            Default: RFAGC Hi-Z.
    */
    uint8 ioHiZ ;

    /**
     @brief User defined data.
    */
    void *user;
} sony_dvb_demod_t;


/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/

/**
 @brief Initialize the demodulator.
        
        From first creation or after finalize, initializes 
        the demodulator and places into the "Sleep" state.

 @param pDemod The demodulator instance.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_Initialize (sony_dvb_demod_t * pDemod);

/**
 @brief Finalize the demodulator.

        The device is placed in "Shutdown" state.
        ::dvb_demod_Initialize must be called to re-initialise the
        device and driver for future acquisitions.

 @param pDemod The demodulator instance.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_Finalize (sony_dvb_demod_t * pDemod);

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

 @param pDemod The demodulator instance.
 @param system The operating system.
 @param bwMHz The expected channel bandwidth (MHz).
        - DVB-T: 6 or 7 or 8.
        - DVB-T2: 5 or 6 or 7 or 8.
        - DVB-C: Fixed for 8.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_Tune (sony_dvb_demod_t * pDemod, 
                                  sony_dvb_system_t system, 
                                  uint8 bwMHz);

/**
 @brief Sleep the demodulator.

 @param pDemod The demodulator instance.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_Sleep (sony_dvb_demod_t * pDemod);

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

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_CheckDemodLock (sony_dvb_demod_t * pDemod, 
                                            sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Check TS lock.

 @param pDemod The demodulator instance.
 @param pLock The lock condition.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_CheckTSLock (sony_dvb_demod_t * pDemod,
                                         sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Configuration options for the demodulator.

        These configuration items are generally used for static
        configuration of the system that the demodulator is operating
        in.
*/
typedef enum {
    /* DVB-T and DVB-C */

    /**
     @brief Parallel or serial TS output selection. OREG_TSIF_PARALLEL_SEL.
            
            Value: 
            - 0: Serial output.
            - 1: Parallel output (Default).
    */
    DEMOD_CONFIG_PARALLEL_SEL,     //0

    /**
     @brief Serial output bit order on TS data. OREG_TSIF_SER_DATA_ON_MSB.
            
            Value:
            - 0: Output from TSDATA0
            - 1: Output from TSDATA7 (Default).
    */
    DEMOD_CONFIG_SER_DATA_ON_MSB,   // 1

    /**
     @brief Parallel output bit order on TS data. OREG_TSIF_OUTPUT_SEL_MSB.
            
            Value: 
            - 0: MSB TSDATA[0] 
            - 1: MSB TSDATA[7] (Default).
    */
    DEMOD_CONFIG_OUTPUT_SEL_MSB,   // 2

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
    DEMOD_CONFIG_TSCLK_CONT,    // 7

    /**
     @brief IFAGC sense configuration. OCTL_IFAGCNEG / OREG_INVERTXGC.
            
            Value:
            - 0: Positive IFAGC.
            - 1: Inverted IFAGC (Default)
    */
    DEMOD_CONFIG_IFAGCNEG,

    /**
     @brief Spectrum inversion default setting. 
            DVB-C: OREG_INV_SPECTRUM. (Bank 0x00)
            DVB-T: OREG_DNCNV_SRVS. (Bank 0x00)
            DVB-T2: OREG_DNCNV_SRVS. (Bank 0x20)

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
     @brief OREG_TSIF_SCR_ENABLE (TS smoothing ON/OFF. OFF is not recommended!)
            
            Value: 
            - 0: Disable TS smoothing buffer.
            - 1: Enable TS smoothing buffer (Default).

            @note Not available in DVB-T2 mode.
    */
    DEMOD_CONFIG_SCR_ENABLE, 

    /**
     @brief TS clock. OREG_TSIF_SDPR (TSCLK[MHz] = 82[MHz] * 256 / OREG_TSIF_SDPR)
            
            \verbatim
            SCLK[MHz] = 82[MHz] * 256 / OREG_TSIF_SDPR
            Default SCLK: 82[MHz], OREG_TSIF_SDPR = 256.
            
            Where:
            OREG_TSIF_SDPR = value.
            \endverbatim
    */
    DEMOD_CONFIG_TSIF_SDPR,

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

    /**
     @brief Set DVB-T scan mode. 
            
            Value: 
            - 0: Disable driver DVB-T scanning mode (Default).
            - 1: Enable driver DVB-T scanning mode.
    */
    DEMOD_CONFIG_DVBT_SCANMODE,
    
    /* DVB-C */

    /**
     @brief Set DVB-C scan mode.
            
            Value: 
            - 0: Disable device DVB-C scanning mode (Default).
            - 1: Enable device DVB-C scanning mode.
    */
    DEMOD_CONFIG_DVBC_SCANMODE         
} sony_dvb_demod_config_id_t;

/**
 @brief Set configuration options on the demodulator.

 @param pDemod The demodulator instance.
 @param configId The configuration ID to set. See ::sony_dvb_demod_config_id_t.
 @param value The associated value. Depends on the configId.

 @return SONY_DVB_OK if successful set the configuration option.
*/
sony_dvb_result_t dvb_demod_SetConfig (sony_dvb_demod_t * pDemod, 
                                       sony_dvb_demod_config_id_t configId, 
                                       int32 value);

/**
 @brief Setup the GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 (CXD2820 only)).
 @param is_enable Set enable (1) or disable (0). 
 @param is_write Write (1) or Read (0).

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_GPIOSetting (sony_dvb_demod_t * pDemod, 
                                         int id, 
                                         int is_enable, 
                                         int is_write);

/**
 @brief Read the GPIO value. 
        The GPIO should have been configured as an input (Read) GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 (CXD2820 only)).
 @param pValue The current value of the GPIO.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_GPIORead (sony_dvb_demod_t * pDemod, 
                                      int id, 
                                      int *pValue);

/**
 @brief Write the GPIO value. 
        The GPIO should have been configured as an output (Write) GPIO.

 @param pDemod The demodulator instance.
 @param id GPIO number (0 or 1 or 2 (CXD2820 only)).
 @param value The value to set as output. 

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_GPIOWrite (sony_dvb_demod_t * pDemod, 
                                       int id, 
                                       int value);

/**
 @brief Soft reset the demodulator.
        The soft reset will begin the devices
        acquisition process.

 @param pDemod The demod instance.

 @return SONY_DVB_OK if successfully reset.
*/
sony_dvb_result_t dvb_demod_SoftReset (sony_dvb_demod_t * pDemod);

/**
 @brief Check if a P1 symbol (T2 or FEF) has been detected since 
        last soft reset.

 @param pDemod The demodulator instance.
 @param pLock The lock indication.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_t2_CheckP1Lock (sony_dvb_demod_t * pDemod, 
                                            sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Check if a T2 P1 symbol (T2 only) has been detected since 
        last soft reset.

 @param pDemod The demodulator instance.
 @param pLock The lock indication.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_t2_CheckT2P1Lock (sony_dvb_demod_t * pDemod, 
                                              sony_dvb_demod_lock_result_t * pLock);

/**
 @brief Setup the PLP configuration of the demodulator. Selecting both the device 
        PLP operation (automatic/manual PLP select) and the PLP to be selected
        in manual PLP mode.

 @param pDemod The demod instance.
 @param autoPLP The auto PLP setting.
        - 0x00: Fully automatic. The first PLP found during acquisition will be output.
        - 0x01: The data PLP ID set by plpId will be output.
               If the PLP with the ID is not found, then a PLP error is indicated
               (::dvb_demod_monitorT2_DataPLPError) but the 
               demod will still output the first found data PLP Id.
 @param plpId The PLP Id to set.

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_t2_SetPLPConfig (sony_dvb_demod_t * pDemod, 
                                             uint8 autoPLP,
                                             uint8 plpId);

/**
 @brief Optimises the stream processor TS output settings 
        according to the active PLP and DVB-T2 system.
        TS lock is required for this function, if TS is not locked 
        then SONY_DVB_ERROR_HW_STATE is returned.
 
 @param pDemod The demod instance.

 @return SONY_DVB_OK if successful and TS output settings optimised.
         Returns SONY_DVB_ERROR_HW_STATE if TS is not locked.
*/
sony_dvb_result_t dvb_demod_t2_OptimiseTSOutput (sony_dvb_demod_t * pDemod);

/**
 @brief Set the DVB-T2 expected spectrum sense. 

       @note Do not use. Internal use only.

 @param pDemod The demod instance.
 @param sense The new spectrum sense.

 @return SONY_DVB_OK if successful and sense has been set.
*/
sony_dvb_result_t dvb_demod_t2_SetSpectrumSense (sony_dvb_demod_t * pDemod, 
                                                 sony_dvb_demod_spectrum_sense_t sense);

/**
 @brief Writes a 12-bit value to the RFAGC PWM output. 
        Please note the actual PWM precision is system dependent:
        - DVB-T/T2:12-bit. 
        - DVB-C: 10-bit.
        
        This interface is available only while the device
        is in ACTIVE mode.

 @param pDemod The demod instance.
 @param val The value to write (12-bit). Unitless.
            0xFFF => DVDD
            0x000 => GND

 @return SONY_DVB_OK if successful.
*/
sony_dvb_result_t dvb_demod_RFAGCWrite (sony_dvb_demod_t* pDemod, uint16 val);

#endif /* SONY_DVB_DEMOD_H */
