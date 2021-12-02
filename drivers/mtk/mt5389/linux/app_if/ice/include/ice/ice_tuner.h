/**
 * Copyright 2002, 2003 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot's interface from the Eclipse system to tuner drivers.
 */

#ifndef CABOT_ICE_ICE_TUNER_H_INCLUDED
#define CABOT_ICE_ICE_TUNER_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"
#include "config/use_sat_tuner.h"

#ifdef __cplusplus
extern "C" {
#endif


/************** General typedefs and parameters common to DVB-S, DVB-C and DVB-T ****/

/**
 * @brief A typedef for a transport stream handle.
 *
 * For UHF DVB-T these might be in the range 21-69.
 * For DVB-S, these may start from 1000 to allow combined DVB-T and DVB-S
 * solutions
 */
typedef frost_uint ICE_ChannelHandle; /**< a handle for a transport stream  */

/**
 * @brief A constant to use when a channel handle is not set.
 */
#define ICE_CHANNEL_NOT_DEFINED 0


/**
 * @brief A typedef for frequencies.
 *
 * These are all in kHz regardless of network or resolution of the source.
 */
typedef frost_ulong ICE_Frequency; /**< in kHz */

/**
 * @brief A constant for a non-existent frequency.
 *
 * This may be used where the frequency has not been set e.g. for a
 * non-existent device.
 */
#define ICE_FREQUENCY_NOT_DEFINED 0

/**
 * @brief Enumeration of possible modulation types which determines automatic
 *        analogue full search will be done for only BG/DK/I channels or for both BG/DK/I and L/Lp
 */
typedef enum ICE_ModulationType
{
    ICE_negative_modulation = 0, /** Search only for BG/DK/I channels */
    ICE_positive_modulation = 1  /** Search for both BG/DK/I and L/Lp channels */
} ICE_ModulationType;

/**
 * @brief Enumeration of possible Constellation (modulation) settings for the
 *        required demodulation scheme.
 *
 * @note See ETSI EN 300 468 V1.9.1 (2009-03), tables 34, 40 and 45.
 *
 * @note Not all modulations are valid for all networks.
 */
typedef enum ICE_Constellation
{
    ICE_constellation_not_defined, /**< DVB-C                    */
    ICE_constellation_none, /**< DVB-C                    */    
    ICE_constellation_qpsk,        /**<       DVB-S DVB-S2 DVB-T */
    ICE_constellation_8psk,        /**<       DVB-S DVB-S2       */
    ICE_constellation_16qam,       /**< DVB-C DVB-S        DVB-T */
    ICE_constellation_32qam,       /**< DVB-C                    */
    ICE_constellation_64qam,       /**< DVB-C              DVB-T */
    ICE_constellation_128qam,      /**< DVB-C                    */
    ICE_constellation_256qam,      /**< DVB-C                    */
    ICE_constellation_reserved,    /**< DVB-C              DVB-T */
    ICE_constellation_16_64qam,    /**< Automatically check both 16 and 64 QAM */
    ICE_constellation_auto         /**<       DVB-S DVB-S2       */
} ICE_Constellation;

/**
 * @brief Enumeration of possible Code Rate settings for both the High
 *        Priority and Low Priority stream code rates.
 *
 * @note See ETSI EN 300 468 V1.9.1 (2009-03), tables 35 and 47.
 *
 * @note Not all convolution code rates apply for all modulation schemes.
 * These may be used in an #ICE_CodeRateSet or independently.
 *
 * @note #ICE_code_rate_none is ambiguous and deprecated.
 * Use #ICE_code_rate_not_defined or #ICE_code_rate_no_conversion as appropriate.
 *
 * @note Code rate 6/7 only applies for DSS.
 */
typedef enum ICE_CodeRate
{
    ICE_code_rate_none = 0, /**< do not use */

    ICE_code_rate_not_defined = 0x0000, /**< there is no code rate  */
    ICE_code_rate_1_4 = 0x0001,  /**< code rate 1/4 */
    ICE_code_rate_1_3 = 0x0002,  /**< code rate 1/3 */
    ICE_code_rate_2_5 = 0x0004,  /**< code rate 2/5 */
    ICE_code_rate_1_2 = 0x0008,  /**< code rate 1/2 */
    ICE_code_rate_3_5 = 0x0010,  /**< code rate 3/5 */
    ICE_code_rate_2_3 = 0x0020,  /**< code rate 2/3 */
    ICE_code_rate_3_4 = 0x0040,  /**< code rate 3/4 */
    ICE_code_rate_4_5 = 0x0080,  /**< code rate 4/5 */
    ICE_code_rate_5_6 = 0x0100,  /**< code rate 5/6 */
    ICE_code_rate_7_8 = 0x0200,  /**< code rate 7/8 */
    ICE_code_rate_8_9 = 0x0400,  /**< code rate 8/9 */
    ICE_code_rate_9_10 = 0x0800,  /**< code rate 9/10 */
    ICE_code_rate_no_conversion = 0x1000, /**< code rate 1/1 */
    ICE_code_rate_reserved = 0x2000 /**< the code rate could be signalled as
                                       this */
} ICE_CodeRate;

/**
 * @brief A set of #ICE_CodeRate values
 *
 * For specifying a subset of code rates that may be used,
 * e.g. to improve lock time or weak signal performance
 *
 */
typedef frost_uint ICE_CodeRateSet;


/**
 * @brief Enumeration of possible delivery network types.
 *
 *  Used primarily for identifying general tuner capabilities and
 *  may be used in an #ICE_NetworkSet.
 */
typedef enum ICE_Network
{
    ICE_network_none        = 0x00,    /**< No network */
    ICE_network_dvbt1       = 0x01,    /**< DVB-T (Terrestrial) */
    ICE_network_dvbs1       = 0x02,    /**< DVB-S (Satellite)   */
    ICE_network_dvbc        = 0x04,    /**< DVB-C (Cable) */
    ICE_network_analog      = 0x08,    /**< Analog */
    ICE_network_dvbt2       = 0x10,    /**< DVB-T2 (Terrestrial) */
    ICE_network_dvbs2       = 0x20,    /**< DVB-S2 (Satellite)   */
    ICE_network_dvbt        = 0x11,    /**< DVB-T, DVB-T2 (Terrestrial) */
    ICE_network_dvbs        = 0x22,    /**< DVB-S, DVB-S2 (Satellite)   */
    ICE_network_ip          = 0x40,    /**< IP network   */
    ICE_network_fm          = 0x80     /**< Fm Radio    */
} ICE_Network;

/**
 * @brief A set of #ICE_Network.
 *
 * Used primarily for identifying general tuner capabilities.
 * The #ICE_NetworkSet exists because  a tuner can conceivably handle
 * more than one network type. For example one tuner driver uses
 * the tuning frequency to determine if it is terrestrial or satellite.
 *
 */
typedef frost_uint ICE_NetworkSet;

/**
 * @brief Enumeration of possible tuner types.
 */
typedef enum ICE_TunerType
{
    /**
     * DVB-T tuner.
     */
    ICE_tuner_terrestrial,

    /**
     * Fm tuner.
     */
    ICE_tuner_fm,

    /**
     * General tuner.
     * The functionality will depend on which #ICE_Network
     * are supported by the instance of general tuner.
     */
    ICE_tuner_general

} ICE_TunerType;

/**
 * @brief Tuner Identification.
 *
 * A tuner is identified by its device type (is it DVB-T or
 * multi-network), and its logical device number in that class of
 * devices, based at zero.
 */
typedef struct ICE_TunerId
{
    /**
     * Type of tuner (such as DVB-T, or General (DVB-S, DVB-C and/or DVB-T
     * multi-network tuner).
     */
    ICE_TunerType tuner_type;

    /**
     *  The capabilities of the tuner, i.e. which network(s) are supported
     */
    ICE_NetworkSet tuner_networks;

    /**
     * Logical device number of the tuner.
     */
    ICE_DeviceNumber device_number;

} ICE_TunerId;


/**
 * @brief Enumeration of possible (coax) voltage values.
 *
 * @note This is a voltage, if any, to be injected onto the cable by the front
 *       end.
 */
typedef enum ICE_Voltage
{
    ICE_voltage_off = 0,       /**< No voltage */
    ICE_voltage_unchanged = 1, /**< Means do not change the present voltage (if any). */
    ICE_voltage_5 = 5,         /**< DVB-T front ends may inject 5V for a powered antenna. */
    ICE_voltage_12 = 12,       /**< Some DVB-T. DVB-S will move to this. */
    ICE_voltage_13 = 13,       /**< DVB-S LNB vertical voltage. */
    ICE_voltage_13v5 = 14,     /**< A higher vertical voltage to compensate for loss. */
    ICE_voltage_18 = 18,       /**< DVB-S LNB horizontal voltage. */
    ICE_voltage_18v5 = 19      /**< A higher horizontal voltage to compensate for loss. */

} ICE_Voltage;

/**
 * @brief Enumeration of lnb types.
 *
 * @note This is an lnb type,
 *
 */
typedef enum ICE_Lnb
{
    ICE_lnb_unicable = 0,
    ICE_lnb_universal,
    ICE_lnb_mdu1,
    ICE_lnb_mdu2,
    ICE_lnb_mdu3,
    ICE_lnb_mdu4,
    ICE_lnb_mdu5
} ICE_Lnb;


/************** DVB-T Specific typedefs and parameters ***************/

/**
 * @brief Enumeration of possible DVB-T Hierarchy settings for the
 *        required demodulation scheme.
 *
 * @note See ETSI EN 300 468 V1.5.1 (2003-01), table 42.
 */
typedef enum ICE_Hierarchy
{
    ICE_hierarchy_none,     /**< non-hierarchical */
    ICE_hierarchy_alpha1,   /**< */
    ICE_hierarchy_alpha2,   /**< */
    ICE_hierarchy_alpha4,   /**< */
    ICE_hierarchy_auto    /**< */

} ICE_Hierarchy;


/**
 * @brief Enumeration of possible DVB-T Priority settings for the
 *        required demodulation scheme.
 *
 * @note See ETSI EN 300 468 V1.7.1 (2005-12), table 44.
 */
typedef enum ICE_Priority
{
    ICE_priority_low,     /**< */
    ICE_priority_high   /**< */

} ICE_Priority;


/**
 * @brief Enumeration of possible DVB-T Guard Interval settings for the
 *        required demodulation scheme.
 *
 * @note See ETSI EN 300 468 V1.5.1 (2003-01), table 44.
 */
typedef enum ICE_GuardInterval
{
    ICE_guard_interval_1_32,  /**< */
    ICE_guard_interval_1_16,  /**< */
    ICE_guard_interval_1_8,   /**< */
    ICE_guard_interval_1_4,   /**< */

    ICE_guard_interval_1_128,   /**< DVB-T2 */
    ICE_guard_interval_19_128,  /**< DVB-T2 */
    ICE_guard_interval_19_256   /**< DVB-T2 */

} ICE_GuardInterval;


/**
 * @brief Enumeration of possible DVB-T Transmission Mode settings for the
 *        required demodulation scheme.
 *
 * @note See ETSI EN 300 468 V1.5.1 (2003-01), table 45.
 */
typedef enum ICE_TransmissionMode
{
    ICE_transmission_mode_2k,   /**< FFT size is 2k*/
    ICE_transmission_mode_8k,   /**< FFT size is 8k*/
    ICE_transmission_mode_4k,   /**< FFT size is 4k (DVB-T2) */
    ICE_transmission_mode_1k,   /**< FFT size is 1k (DVB-T2) */
    ICE_transmission_mode_16k,  /**< FFT size is 16k (DVB-T2) */
    ICE_transmission_mode_32k   /**< FFT size is 32k (DVB-T2) */

} ICE_TransmissionMode;


/**
 * @brief Enumeration of possible DVB-T Transmission Bandwidth settings for
 *        the required demodulation scheme.
 *
 * @note See ETSI EN 300 468 V1.5.1 (2003-01), table 40.
 */
typedef enum ICE_BandwidthMhz
{
    ICE_bandwidth_6mhz, /**< */
    ICE_bandwidth_7mhz, /**< */
    ICE_bandwidth_8mhz,  /**< */
    ICE_bandwidth_7_8mhz,   /* Try 7MHz bandwidth first, if no success then try 8MHz */
    ICE_bandwidth_17mhz    /**< */

} ICE_BandwidthMhz;

/***************     DVB-T2 specific typedefs and parameters      *************/
/**
 * @brief Enumeration of the preamble values.
 */
typedef enum ICE_PreambleMix
{
    ICE_preamble_mixed,
    ICE_preamble_not_mixed

} ICE_PreambleMix;

/**
 * @brief Enumeration of the S1 Signalling values.
 */
typedef enum ICE_S1Signalling
{
    ICE_s1_signalling_unknown,
    ICE_s1_signalling_siso,
    ICE_s1_signalling_miso,
    ICE_s1_signalling_non_dvbt2

} ICE_S1Signalling;

/**
 * @brief Enumeration of the different pilot patterns.
 */
typedef enum ICE_L1PrePilotPattern
{
    ICE_l1_pre_pilot_unknown,
    ICE_l1_pre_pilot_pattern1,
    ICE_l1_pre_pilot_pattern2,
    ICE_l1_pre_pilot_pattern3,
    ICE_l1_pre_pilot_pattern4,
    ICE_l1_pre_pilot_pattern5,
    ICE_l1_pre_pilot_pattern6,
    ICE_l1_pre_pilot_pattern7,
    ICE_l1_pre_pilot_pattern8

} ICE_L1PrePilotPattern;

/**
 * @brief Enumeration of the different peak average power ratios.
 */
typedef enum ICE_L1PrePeakAveragePowerRatio
{
    ICE_l1_pre_peak_average_power_ratio_unknown,
    ICE_l1_pre_peak_average_power_ratio_none,
    ICE_l1_pre_peak_average_power_ratio_ace,
    ICE_l1_pre_peak_average_power_ratio_tr,
    ICE_l1_pre_peak_average_power_ratio_tr_and_ace

} ICE_L1PrePeakAveragePowerRatio;


/*************** DVB-S and DVB-C specific typedefs and parameters *******/

/**
 * @brief Type used to specify symbol rate.
 *
 * This is an integer value in kS/s.  For example, 27.45 MSymbols/s is stored
 * as 27450 decimal.
 *
 * @note See ETSI EN 300 468 V1.5.1 (2003-01), table 36 (part).
 */
typedef frost_ulong ICE_SymbolRate;

/**
 * @brief A constant for an undefined symbol rate.
 *
 * This may be used where the symbol rate has not been set.
 */
#define ICE_SYMBOLRATE_NOT_DEFINED 0


/*************** DVB-S specific typedefs and parameters ****************/

/**
 * @brief Enumeration of possible Channel Polarisations.
 *
 * @note See ETSI EN 300 468 V1.9.1 (2009-03), table 37.
 */
typedef enum ICE_Polarisation
{
    ICE_polarisation_H,         /**< linear horizontal*/
    ICE_polarisation_V,         /**< linear vertical*/
    ICE_polarisation_circular_left,  /**< maps to horizontal */
    ICE_polarisation_circular_right, /**< maps to vertical */
    ICE_polarisation_not_defined     /**< used as a default when polarisation
                                          not known */

} ICE_Polarisation;


/**
 * @brief Enumeration of possible LNB continuous tones.
 *
 * @note This continuous tone is only generated once tone signalling
 *       (DiSEqC, SA/SB tone burst) has been completed.
 */
typedef enum ICE_Tone
{
    ICE_tone_off, /**< there is no tone */
    ICE_tone_22khz  /**< there is a continuous 22kHz tone */

} ICE_Tone;


/**
 * @brief Enumeration for spectral inversion.
 *
 * Spectral inversion may occur depending on the relative frequencies of the
 * local oscillator, mixer input and mixer output frequencies.
 */
typedef enum ICE_SpectralInversion
{
    ICE_spectral_inversion_auto,/**< spectral inversion handled automatically */
    ICE_spectral_inversion_off, /**< the signal has not been spectrally inverted */
    ICE_spectral_inversion_on   /**< the signal has been spectrally inverted */

} ICE_SpectralInversion;


/**
 * @brief Indicates which band is mapped onto an intermediate frequency.
 */
typedef enum ICE_FrequencyBand
{
    ICE_band_low,
    ICE_band_high
} ICE_FrequencyBand;


/**
 * @brief DiSEqC messages do not exceed this length.
 */
#define ICE_MAX_DISEQC_MESSAGE_LENGTH 6

/**
 * @brief Specification of a single DiSEqC command.
 *
 * @note Tone-burst is signalled using a single byte.
 * - 0x00 for unmodulated tone-burst (satellite position A) or
 * - 0xFF for modulated tone-burst (satellite position B).
 */
typedef struct ICE_DiseqcCommand
{
    /**
     * DiSEqC command bytes.
     */
    frost_uint8 data[ICE_MAX_DISEQC_MESSAGE_LENGTH];

    /**
     * Number of bytes in DiSEqC command \c data.
     */
    frost_uint8 length;

    /**
     * Delay (in ms) to wait after sending the command.
     */
    frost_uint16 delay_ms;

} ICE_DiseqcCommand;


/**
 * @brief DiSEqC command linked-list node.
 */
typedef struct ICE_DiseqcCommandList
{
    /**
     * DiSEqC command for this node.
     */
    ICE_DiseqcCommand command;

    /**
     * Next command. A null pointer indicates the end of the list.
     */
    struct ICE_DiseqcCommandList * next;

} ICE_DiseqcCommandList;


/**
 * @brief Antenna is a description of the connection between the tuner
 * and and cable or antenna.
 *
 * @note There may be more than one antenna connected to each tuner, so
 * #ICE_TunerClose() followed by  #ICE_TunerOpen() is used to switch antenna.
 *
 */
typedef struct ICE_Antenna {
    /**
     * Voltage to be applied to the input cable (DVB-S, DVB-T)
     */
    ICE_Voltage antenna_voltage;

    /**
     * Method of band switching (DVB-S)
     */
    ICE_Tone antenna_continuous_tone;

    /**
     * DiSEqC command sequence (DVB-S)
     *
     * This is used to perform operations such as:
     * - configuring an LNB (band and polarisation)
     * - switching antenna
     * - controlling a positioner
     * - sending a tone-burst
     *
     * A null pointer indicates that DiSEqC is not being used.
     */
    ICE_DiseqcCommandList * diseqc_commands;

    /**
     * Frequency mapping (DVB-S)
     */
    ICE_SpectralInversion antenna_inversion;

    /**
     * Lower, or only, local oscillator frequency e.g. 9750000 kHz
     * A zero value indicates no local oscillator.(DVB-S)
     */
    ICE_Frequency antenna_lo_low;

    /**
     * MDU type local oscillator frequency e.g. 9550000 kHz
     * A zero value indicates no local oscillator.(DVB-S)
     */
    ICE_Frequency antenna_lo_medium;

    /**
     * Higher local oscillator frequency (if any) e.g. 11700000 kHz
     * A zero value indicates no local oscillator.(DVB-S)
     */
    ICE_Frequency antenna_lo_high;

    /**
     * The input frequency at which the higher or lower local oscillator is
     * chosen e.g. 11700000 kHz (DVB-S)
     */
    ICE_Frequency antenna_band;

    /**
     * User band for unicable systems.
     */
    frost_uint user_band;

} ICE_Antenna;


/**
 * @brief Transport Stream information.
 *
 * This structure provides parameters to allow demodulation of the
 * transport stream.
 */
typedef struct ICE_TransportStreamInfo
{

/*  Transport stream parameters common to all networks (DVB-T, DVB-C and DVB-S) */
    /**
     * Constellation or modulation type of the transport stream.
     */
    ICE_Constellation constellation;

    /**
     * DVB-T high-priority code rate settings for the stream,
     * or DVB-S and cable FEC_inner.
     */
    ICE_CodeRate hp_code_rate;

/* DVB-T and DVB-C network specific transport stream parameters */
    /**
     * Low-priority code rate settings for the stream,
     * or cable FEC_outer
     */
    ICE_CodeRate lp_code_rate;

/* DVB-T network specific transport stream parameters ***********/
    /**
     * Hierarchy type of the transport stream.
     */
    ICE_Hierarchy hierarchy;

    /**
     * Priority type of the transport stream.
     * @note Eclipse will ignore this field if the hierarchy field is set to
     *       ICE_hierarchy_none and set the priority field to ICE_priority_high
     */
    ICE_Priority priority;

    /**
     * The guard interval settings for the stream.
     */
    ICE_GuardInterval guard_interval;

    /**
     * The transmission mode settings for the stream.
     */
    ICE_TransmissionMode transmission_mode;

    /**
     * The Cell Identifier as per EN 300744 4.6.2.10
     */
    frost_uint cell_identifier;

/* DVB-S DVB-C network specific ***********************************/
    /**
     * The symbol rate to use for the stream.
     * If this is #ICE_SYMBOLRATE_NOT_DEFINED, then an appropriate default
     * must be used instead
     */
    ICE_SymbolRate symbol_rate;

/* DVB-S network specific *****************************************/
    /**
     * DVB-S polarisation.
     */
    ICE_Polarisation polarisation;

/* DVB-T2 network specific ****************************************/
    /**
     * DVB-T2 pre-amble mixed.
     */
    ICE_PreambleMix preamble_mix;

    /**
     * DVB-T2 S1 Signalling
     */
    ICE_S1Signalling s1_signalling;

    /**
     * DVB-T2 Pre Pilot Pattern
     */
    ICE_L1PrePilotPattern l1_pre_pilot_pattern;

    /**
     * DVB-T2 Bandwidth Extension
     */
    frost_uint8 bandwidth_extension;

    /**
     * DVB-T2 L1 Pre Peak Average Power Ratio
     */
    ICE_L1PrePeakAveragePowerRatio l1_pre_peak_average_power_ratio;

    /**
     * DVB-T2 OFDM Symbols Per Frame
     */
    frost_uint16 ofdm_symbols_per_frame;

} ICE_TransportStreamInfo;


/**
 * @brief Enumeration of possible DVB-S2 roll-off factors.
 *
 * @note See ETSI EN 300 468 V1.9.1 (2009-03), table 38.
 */
typedef enum ICE_RollOffFactor
{
    ICE_roll_off_factor_0_35, /**< alpha = 0.35 */
    ICE_roll_off_factor_0_25, /**< alpha = 0.25 */
    ICE_roll_off_factor_0_20, /**< alpha = 0.20 */
    ICE_roll_off_factor_reserved, /**< the roll-off could be signalled as this */
    ICE_roll_off_factor_not_applicable /**< used when a roll-off factor is not applicable */

} ICE_RollOffFactor;


/**
* @brief possible TV system types.
*/
typedef enum ICE_AnalogTVSystem
{
    ice_tv_system_m,
    ice_tv_system_bg,
    ice_tv_system_i,
    ice_tv_system_dk,
    ice_tv_system_l,
    ice_tv_system_lp,
    ice_tv_system_invalid
}ICE_AnalogTVSystem;

/**
* @brief Analog tuning parameters.
*/
typedef struct ICE_AnalogTuneInfo
{
    /**
    * broadcasted tv system.
    */
    ICE_AnalogTVSystem tv_system;

    /**
    * fine tune offset
    */
    frost_int fine_tune_offset;

    /**
    * to enable fine tune
    */
    frost_bool fine_tune_enabled;

} ICE_AnalogTuneInfo;

/**
 * @brief PLP_ID for DVB-T2 systems.
 */
typedef struct ICE_PhysicalLayerPipeId
{
    /**
     * Flag to indicate the presence of a PLP_ID.
     */
    frost_bool is_set;

    /**
     * The PLP_ID. If \a is_set equals #frost_false, this field is ignored.
     */
    frost_uint8 value;

} ICE_PhysicalLayerPipeId;


/**
 * @ host, request and url does not exceed this length.
 */
#define ICE_IP_MAX_LENGTH 256


/**
 * @ Enum defining supported transport protocols
**/
typedef enum ICE_IpProtocol
{
    ice_ip_protocol_udp,          /* UDP */
    ice_ip_protocol_rtp,          /* RTP */
    ice_ip_protocol_rtsp         /* RTSP */
} ICE_IpProtocol;


/**
 * @brief Url Identification.
 *
 * Url is identified by protocol type, ip adress, port number and (optional, for rtsp) request
 */
typedef struct ICE_Url
{
    /**
     * Protocol Type
     */
    ICE_IpProtocol protocol;

    /**
     *  Host IP adress
     */
    char host[ICE_IP_MAX_LENGTH];

    /**
     *  Port number
     */
    frost_int port;

    /**
     *  Request (file path/name on host)
     */
    char request[ICE_IP_MAX_LENGTH]; /* for RTSP protocol */

} ICE_Url;

/**
 * @brief ICE_FMTunerRdsInfo structure.
 *
 */
typedef struct ICE_FMTunerRdsInfo
{
    frost_bool      is_data_valid;
    frost_uint16    displayed_program_identifier;// Displayed Program Identifier
    frost_uint8     displayed_program_type;// Displayed Program Type
    char *          displayed_radio_text; // Displayed Radio Text
    char *          displayed_radio_simple_text; // Simple Displayed Radio Text
    char *          displayed_program_service_text; // Displayed Program Service text
    frost_bool      julian_day_high_bit; // Modified Julian Day high bit
    frost_uint16    julian_day_Low; // Modified Julian Day low 16 bits
    frost_uint8     hour; // Hour
    frost_uint8     minute; // Minute
    frost_int8      utc_offset; // Local Time Offset from UTC
}ICE_FMTunerRdsInfo;

typedef frost_bool (* ICE_RdsDataCallback)(void * context) ;

/**
 * @brief ICE_FmTuneInfo structure.
 *
 */
typedef struct ICE_FmTuneInfo
{
    /**
     * Callback function when rds data is ready
     */
    ICE_RdsDataCallback rds_callback;

    /**
     * Rds Data
     */
    ICE_FMTunerRdsInfo * rds_data ;

    void * context;

    frost_uint32 channel_index;

}ICE_FmTuneInfo;


/**
 * @brief Parameters passed to the tuner so that it may attempt to
 *        tune to a given frequency.
 *
 * @note Only parameters relevant to the network need be set.
 */
typedef struct ICE_TuneToParams
{
    /**
     * The channel frequency to tune to in kHz.
     * Common to DVB-S, DVB-C and DVB-T.
     */
    ICE_Frequency channel_frequency_khz;

    /**
     * The intermediate frequency in kHz that corresponds to the channel
     * frequency (DVB-S only).
     */
    ICE_Frequency intermediate_frequency_khz;

    /**
     * The frequency band to use (DVB-S).
     * Used to select between the high and low bands when tuning to a Ku band
     * transponder. For C band transponders, only low band is used.
     * This band setting was used in the calculation of the
     * intermediate frequency.
     */
    ICE_FrequencyBand frequency_band;

    /**
    * Broadcast type, analog, digital, etc.
    */
    ICE_Network network_type;

    /**
     * Ignore the <code> ts_info </code> field if this flag is set. This flag
     * will typically be set when attempting a first time tune (ie a scan
     * step), when the exact transport stream settings are not known.
     *
     * The implementation should then use default settings to attempt to find
     * a transport stream on the given frequency.
     * Common to DVB-S, DVB-C and DVB-T.
     */
    frost_bool use_defaults;

    /**
    * Tuning parameters that are specific to analog broadcast.
    */
    ICE_AnalogTuneInfo analog_tune_info;

    /**
    * Tuning parameters that are specific to fm broadcast.
    */
    ICE_FmTuneInfo fm_tune_info ;

    /**
     * The transport stream info to tune to.
     * Common to DVB-S, DVB-C and DVB-T.
     */
    ICE_TransportStreamInfo ts_info;

    /**
     * The channel bandwidth to tune to in MHz (DVB-T only).
     */
    ICE_BandwidthMhz channel_bandwidth_mhz;

    /**
     * The default symbol rate of the stream (DVB-S and DVB-C).
     * There is no single default for this value, so if
     * <code> use_defaults </code> is set, this one must be used.
     */
    ICE_SymbolRate default_symbol_rate;

    /**
     * The acceptable code rate(s) for a stream (DVB-S and DVB-C).
     * If this is set to #ICE_code_rate_not_defined, then any code rate
     * is acceptable. Otherwise the tuner will only lock if the code
     * rate is present in <code> code_rates </code>
     */
    ICE_CodeRateSet code_rates;

    /**
     * The default polarisation to use (DVB-S).
     * If <code> use_defaults </code> is set, this one must be used.
     */
    ICE_Polarisation default_polarisation;

    /**
     * Lnb type to calculate the IF frequency (DVB-S)
     */
    ICE_Lnb lnb_type;

    /**
     * Physical Layer Id (DVB-T2).
     */
    ICE_PhysicalLayerPipeId plp_id;

    /**
     * Url for virtual IP tuner
     */
    ICE_Url ip_url;

    /**
     * Set when scan is in progress
     */
    frost_bool in_scan_mode;
#ifdef USE_SAT_TUNER
    ICE_Antenna tuner_antenna;
#endif
} ICE_TuneToParams;

typedef struct ICE_TsClockParams
{
    /**
     * Parallel line clock frequency
     */
    frost_int ts_clock_frequency;

    /**
     * Indicates whether clock phase shifted 180 degree or not
     */
    frost_bool invert_clock;

}ICE_TsClockParams;

/**
 * @brief Tuner Initialisation parameters.
 */
typedef struct ICE_TunerInitParams
{
    /**
     * The ID of the tuner device.
     */
    ICE_TunerId tuner_id;

} ICE_TunerInitParams;

/**
 * @brief Tuner Open parameters.
 */
typedef struct ICE_TunerOpenParams
{
    /**
     * The ID of the tuner device.
     */
    ICE_TunerId tuner_id;

    /**
     * Antenna connection parameters, i.e which antenna should be
     * connected by the open.
     * If an antenna is to be switched from one source to another then
     * the tuner must be closed, the <code> tuner_antenna </code> changed, and
     * the tuner re-opened.
     */
    ICE_Antenna tuner_antenna;

} ICE_TunerOpenParams;


/**
 * @brief Opaque type used to identify an open connection.
 */
typedef struct ICE_TunerHandle ICE_TunerHandle;


/**
 * @brief Tuner device signal parameters.
 */
typedef struct ICE_SignalParams
{
    /**
     * A measure of the signal quality expressed as a percentage.
     * The figure shall be suitable for display on a user interface as
     * an indication of received signal quality.  Zero indicates
     * failure to get a tuner lock and 100 indicates a perfect signal.
     *
     * @note Conversion to or from the underlying measurement
     *       must be carried out by the ICE implementation.
     *
     * For both DVB-T and DVB-S this is the Bit Error Ratio
     * (BER) measured post Viterbi and pre Reed Solomon. This gives a
     * quality measure that is independent of code rate and modulation.
     * <code> signal_quality 0 </code> should represent a BER of 1E-2 or larger
     * (poor), while 100 should represent a BER of 1E-7 or smaller (very good).
     * The pseudo code for ice converting the bit error ratio to
     * <code> signal_quality </code> is:
     * <code> <br>
     * if (BER > 1e-2) BER = 1e-2; <br>
     * if (BER < 1e-7) BER = 1e-7; <br>
     * signal_quality = 20*(-log(BER) - 2) <br>
     * <br> </code>
     *
     * This means the threshold reception of 2e-4 is a
     * <code> signal_quality </code> of 34.
     */
    frost_uint signal_quality;

    /**
     * A measure of the signal strength expressed as a percentage.
     * The figure shall be suitable for display on a user interface as
     * an indication of received signal strength.  Zero indicates
     * failure to get a tuner lock and 100 indicates a perfect signal.
     *
     * @note Conversion to or from the underlying measurement
     *       must be carried out by the ICE implementation.
     *
     * For DVB-T, this strength should be in the range 45dB�V to
     * 70dB�V, the higher the value the better. Some representation of
     * strength outside this range should be allowed. Signals in the
     * range 35-45dB�V may give unreliable service. Map 75dB�V (and
     * above) onto 100 and 25dB�V (and below) onto 0. The values between
     * 0 and 100 are mapped directly with a resolution of 0.5dB�V and so
     * the value 25 represents 37.5dB�V, 50 represents 50dB�V and 75 would
     * represent 62.5dB�V. Values between 40 and 90 represent strengths
     * in the 'ideal' range of 45dB�V to 70dB�V. Values in the range 0
     * to 30 represent weak (probably unusable) strength and values in the
     * range 90 to 100 indicate a possible overload.
     *
     * For DVB-S, the exact upper and lower bounds of the measurement
     * range (mapped to 100 and 0 respectively) will depend on the
     * measurement bandwidth: full IF (950MHz to 2100MHz) would be -43dBm
     * to -3dBm, while a 33MHz measurement bandwidth would be -69dBm to
     * -19dBm. Doubling the bandwidth adds 3dBm to the upper and lower
     * limit. The resolution is 0.5dBm, but it is up to the underlying
     * ICE implementation to decide the measurement bandwidth.
     */
    frost_uint signal_strength;
    frost_uint ber_value;

} ICE_SignalParams;


/**
 * @brief Tuner error status.
 */
typedef enum ICE_TunerError
{
    /**
     * The tuner is functioning normally.
     */
    ICE_tuner_error_none,

    /**
     * Antenna overload (e.g. a short-circuit) has been detected and the
     * tuner may not be functioning correctly.
     */
    ICE_tuner_error_antenna_overload

} ICE_TunerError;


/**
 * @brief Tuner frequency range.
 */
typedef struct ICE_TunerFreqRange
{
    /**
     * The lowest frequency in the range (kHz).
     */
    ICE_Frequency min_frequency_khz;

    /**
     * The highest frequency in the range (kHz).
     */
    ICE_Frequency max_frequency_khz;

    /**
     * The RFC bandwidth in kHz.
     */
    ICE_Frequency min_step_khz;

    /**
     * The largest tuning step (kHz).
     * This is a multiple of the RFC bandwidth.
     * After the driver has been unable to lock for this many khz it needs to
     * send a no lock message to eclipse so that the user can be informed
     * of progress. This message is not acknowledged with ICE_ScanContinue as
     * the driver is assumed to be already scanning the next frequency that
     * contains a signal.
     */
    ICE_Frequency max_step_khz;

    /**
     * The polarisation to use (DVB-S).
     */
    ICE_Polarisation polarisation;

    /**
     * The number of symbol rates to use during scan (DVB-S).
     */
    frost_uint number_of_symbol_rates;

    /**
     * The symbol rates to try during scan (DVB-S).
     */
    ICE_SymbolRate *symbol_rates;

    /**
     * Modulation type determining for analogue full search standards.
     */
    ICE_ModulationType modulation_type;

} ICE_TunerFreqRange;

/**
 * @brief Tuner device information.
 */
typedef struct ICE_TunerInfo
{
    /**
     * Device number of the particular tuner.
     */
    ICE_DeviceNumber device_number;

    /**
     * Tuner type of the particular tuner.
     */
    ICE_TunerType tuner_type;

    /**
     * For tuner_type general, what network(s) is/are supported.
     */
    ICE_NetworkSet networks;

    /**
     * Number of supported frequency ranges for DVB-T/DVB-C network.
     */
    frost_uint freq_ranges_count;

    /**
     * Pointer to DVB-T/DVB-C network frequency range array.
     */
    ICE_TunerFreqRange** freq_ranges;

    /**
     * Lowest supported symbol rate for DVB-C/DVB-S network.
     */
    ICE_SymbolRate min_symbol_rate;

    /**
     * Highest supported symbol rate for DVB-C/DVB-S network.
     */
    ICE_SymbolRate max_symbol_rate;

    /**
     * Lowest supported IF frequency for DVB-S network e.g. 900000 kHz.
     */
    ICE_Frequency min_sat_if_frequency;

    /**
     * Highest supported IF frequency for DVB-S network e.g. 2150000 kHz.
     */
    ICE_Frequency max_sat_if_frequency;

    /**
     * DiSEqC 2.x reply message length (in bits) for some DVB-S network.
     * A selection_length of zero means <code>antenna_selection</code> must
     * be ignored.
     */
    frost_uint selection_length;

    /**
     * The actual DiSEqC 2.x reply message for some DVB-S network. <br>
     *
     * This is a pointer to the first of a total of
     * <code>((selection_length/8) + 1) </code> #frost_uint8 values. If the
     * first bit of the message is a one then
     * <code>((*antenna_selection) & 0x80) == 0x80 </code>
     * A bit set to one means the 22kHz tone will be applied
     * for 11 cycles while if the bit is clear then no tone will be
     * applied for 0.5mS.
     *
     */
    frost_uint8 * antenna_selection;

    /**
     * Placeholder for extended data.
     */
    void* extended;

} ICE_TunerInfo;


/**
 * @brief Enumeration of possible scan results.
 */
typedef enum ICE_ScanResult
{
    /**
     * Analogue (not digital) carrier found.
     */
    ICE_Scan_AnalogueOnly,

    /**
     * Tuner is not locked shall be used for scanning progress
     * notifications.
     */
    ICE_Scan_NotLocked,

    /**
     * Tuner is locked.
     */
    ICE_Scan_Locked,

    /**
     * The scan has reached the end of the frequency range.
     */
    ICE_Scan_Finished

} ICE_ScanResult;


/**
 * @brief Enumeration of trick modes.
 */
typedef enum ICE_TrickMode
{
    ICE_TrickMode_FF_8x, /* Fast forward 8x */
    ICE_TrickMode_FF_4x, /* Fast forward 4x */
    ICE_TrickMode_FF_2x, /* Fast forward 2x */
    ICE_TrickMode_1x,    /* Normal speed */
    ICE_TrickMode_RW_2x, /* Rewind 2x */
    ICE_TrickMode_RW_4x, /* Rewind 4x */
    ICE_TrickMode_RW_8x, /* Rewind 8x */
    ICE_TrickMode_Pause  /* Pause */

} ICE_TrickMode;


/**
 * @brief Notify the caller about frequency scan result
 *
 * This callback is used by frequency scan implementation to notify about
 * scan result.
 *
 * @param handle A pointer to the device handle returned by the
 *               #ICE_TunerOpen() call.
 * @param scan_result Scan result.
 * @param tuned_params A structure providing details of the frequency (and
 *        demodulation scheme - if tuner is locked).  If zero, then no
 *        parameters are available - use #ICE_GetSignalInfo() (but not while
 *        in the callback).
 * @param user_data A data pointer as passed in #ICE_TunerScan() call.
 */
typedef frost_bool (*ICE_ScanCallback)(ICE_TunerHandle* handle,
                                       ICE_ScanResult scan_result,
                                       const ICE_TuneToParams *tuned_params,
                                       void* user_data);



/**
 * @brief Inform lower layers for the tsid of current transponder retrieved from PAT table.
 * 
 */
void FE_ICE_DVBS2_updateTSID(int tsid);

/**
 * @brief Initialise the Tuner.
 *
 * Initialise the required tuner and its associated demodulator and any
 * hardware Interface connecting them (such as I2C).
 *
 * @param params The Initialisation parameters indicate the required tuner.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TunerInit(const ICE_TunerInitParams* params);


/**
 * @brief Open a connection to the Tuner.
 *
 * Attempts to open a connection to the tuner identified and return a handle
 * for subsequent communication with the device.
 *
 * @param  params The Open parameters indicate the required tuner.
 *
 * @return A pointer to the handle to the device on success and a
 *         NULL pointer on failure.
 */
ICE_TunerHandle* ICE_TunerOpen(const ICE_TunerOpenParams* params);


/**
 * @brief Attempt to tune to a given frequency.
 *
 * Attempts to tune to a given frequency and lock the demodulator to the
 * signal found.  This function is blocking - i.e. it does not return until
 * either the tuner has successfully locked or until no lock has been found.
 *
 * @param handle A pointer to the device handle returned by the
 *               #ICE_TunerOpen() call.
 * @param params A structure providing details of the required frequency
 *               and demodulation scheme.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TuneTo(ICE_TunerHandle* handle,
                      const ICE_TuneToParams* params);


/**
 * @brief Attempt to scan given frequency range
 *
 * Attempts to scan given frequency range and lock the demodulator
 * to the signal found. This function is non-blocking, it shall return
 * immediately.
 *
 * The scanning in progress can be aborted via #ICE_TunerScanAbort() call.
 * To notify caller about scanning process state implementation shall use
 * #ICE_ScanCallback.  Once it is executed, implementation should wait
 * for subsequent #ICE_TunerScanContinue() or #ICE_TunerScanAbort() calls.
 *
 * @param handle A pointer to the device handle returned by the
 *        #ICE_TunerOpen() call.
 * @param range A pointer to frequency range to be searched for signal lock.
 * @param callback Callback function used to notify caller of a scanning
 *        state/progress
 * @param user_data Data pointer to be passed to callback function.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note Currently not required for DVB-T platforms.  (No need for even a stub).
 */
frost_bool ICE_TunerScan(ICE_TunerHandle* handle,
                         const ICE_TunerFreqRange* range,
                         ICE_ScanCallback callback,
                         void* user_data);


/**
 * @brief Continue started scanning process.
 *
 * @note The typical scan sequence would be:                 <br>
 *       STB ---> ICE #ICE_TunerScan()                       <br>
 *       STB <--- ICE #ICE_ScanCallback()                    <br>
 *       STB ---> ICE #ICE_TunerScanContinue()               <br>
 *       ...                                                 <br>
 *       STB <--- ICE #ICE_ScanCallback( #ICE_Scan_Finished )<br>
 *       or alternatively if STB wants to abort scan process <br>
 *       STB ---> ICE #ICE_TunerScanAbort()                  <br>
 *       note #ICE_ScanCallback( #ICE_Scan_Finished ) is not expected in this case.<br>
 *
 * @param handle A pointer to the device handle returned by the
 *               #ICE_TunerOpen() call.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note Currently not required for DVB-T platforms.  (No need for even a stub).
 *
 * @note This function is not called by eclipse in response to a no lock message.
 *       The frequency sent in the no lock message is used to inform the user of
 *       the progress.
 */
frost_bool ICE_TunerScanContinue(ICE_TunerHandle* handle);


/**
 * @brief Abort a tuner scan started using #ICE_TunerScan().
 *
 * This function should cause any active call to #ICE_TunerScan() to abort.
 * The ICE_ScanCallback registered at the start of the scan should not be
 * called.
 *
 * @param handle A pointer to the device handle returned by the
 *        #ICE_TunerOpen() call.
 *
 * @return #frost_true if the scan has been aborted, #frost_false if the scan
 *         could not be aborted.
 *
 * @note Currently not required for DVB-T platforms.  (No need for even a stub).
 */
frost_bool ICE_TunerScanAbort(ICE_TunerHandle* handle);


/**
 * @brief Attempt to maintain lock to a previously locked transport stream.
 *
 * Allows the demodulator to perform any signal tracking required to
 * maintain a lock on a previously locked transport stream, and
 * provide feedback if the signal is lost.
 *
 * @param handle A pointer to the device handle returned by the
 *               #ICE_TunerOpen() call.
 *
 * @return Returns #frost_true if the lock is maintained, and #frost_false
 *         on loss of lock.
 */
frost_bool ICE_MaintainLock(ICE_TunerHandle* handle);


/**
 * @brief Return details on the current signal quality.
 *
 * Returns signal strength and bit error rate information on the
 * current signal.
 *
 * @param handle A pointer to the device handle returned by the
 *        #ICE_TunerOpen() call.
 * @param params Output parameter structure for returning the signal
 *        quality information.
 * @param tune_params Output parameter structure for returning current
 *        frequency and demodulation scheme.  The contents of this structure
 *        will be available to the UI application via the EASIControlInterface's
 *        <code>getTunerLockInfo</code> method.  This structure is optional
 *        and maybe ignored if it is null.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_GetSignalInfo(ICE_TunerHandle* handle,
                             ICE_SignalParams* params,
                             ICE_TuneToParams* tune_params);

/**
 * @brief Return number of PLPs on the current RF channel.
 *
 * @note If the implementation does not support DVB-T2, this function may be
 *       stubbed; in this case, it shall return #frost_true, with a value of 0
 *       returned in \a num_plps.
 *
 * @param handle A pointer to the device handle returned by the
 *        #ICE_TunerOpen call.
 * @param num_plps Buffer in which the number of PLPs on the current RF channel
 *                  shall be returned. If the tuner is not tuned to a
 *                  DVB-T2 multiplex, then 0 shall be returned.
 *
 * @return #frost_true on success, #frost_false on failure. If the tuner is not
            tuned to a DVB-T2 multiplex, then #frost_true shall be returned.
 */
frost_bool ICE_GetNumPLPs(ICE_TunerHandle* handle, frost_uint16 *num_plps);

/**
 * @brief Return array of PLP ids available on the current RF channel.
 *
 * @note If the implementation does not support DVB-T2, this function may be
 *       stubbed, and shall return 0.
 *
 * @note This shall only be called immediately following a call to
 *          #ICE_GetNumPLPs.
 *
 * @param handle A pointer to the device handle returned by the
 *        #ICE_TunerOpen call.
 * @param plp_ids Buffer in which the list of available PLP ids on the
 *                  current RF channel should be returned. The size of
 *                  this buffer will be indicated by \a max_plp_ids,
 *                  which is the maximum number of bytes that shall be
 *                  written to this buffer.
 * @param max_plp_ids Size of \a plp_ids array. This shall be the same as
 *                  the value last returned from #ICE_GetNumPLPs.
 *
 * @return The number of PLP ids available, i.e. the number of elements
 *          of \a plp_ids that have been filled in. On failure,
 *          this will be 0. If the tuner is not tuned to a DVB-T2
 *          multiplex, this shall be 0.
 */
frost_uint16 ICE_GetPLPIds(ICE_TunerHandle* handle,
                           frost_uint8 *plp_ids,
                           frost_uint16 max_plp_ids);

/**
 * @brief Report on any current tuner errors.
 *
 * @param handle A pointer to the device handle returned by the
 *        #ICE_TunerOpen() call.
 * @param status The nature of the error, or #ICE_tuner_error_none if the
 *        tuner is functioning normally.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note A functional implementation is required for DVB-S platforms,
 *       otherwise this function may always return frost_false.
 */
frost_bool ICE_GetTunerErrorStatus(ICE_TunerHandle* handle,
                                   ICE_TunerError * status);

/**
 * @brief Get detailed device description.
 *
 * @param handle A pointer to the device handle returned by the
 *               #ICE_TunerOpen() call.
 *
 * @param info Output parameter structure to fill with tuner info.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note Currently not required for DVB-T platforms.  (No need for even a stub).
 */
frost_bool ICE_GetTunerCapabilities(ICE_TunerHandle* handle,
                                    ICE_TunerInfo* info);


/**
 * @brief Send a DiSEqC command sequence.
 *
 * This function will be used to perform operations that are not directly
 * related to tuning, such as controlling a satellite dish positioner during
 * receiver installation.
 *
 * @param handle A pointer to the device handle returned by the
 *        #ICE_TunerOpen() call.  Signalling should occur on the cable
 *        connected to this tuner device.
 * @param commands The DiSEqC command sequence to send.
 *
 * @return #frost_true on success, #frost_false on failure.
 *
 * @note Only required for DVB-S platforms.
 * @note This may be called from a different thread than the one used for
 *       the other ICE tuner calls. This allows DiSEqC commands to be
 *       sent independently of (and potentially during) other tuning
 *       operations.
 */
frost_bool ICE_SendDiseqcCommands(ICE_TunerHandle* handle,
                                  ICE_DiseqcCommandList* commands);


/**
 * @brief Close a previously open connection with the required Tuner
 *
 * Closes a device connection to the specified Tuner device, such that
 * it becomes available for use by any other client.
 *
 * This may also be used before reopening the tuner with different
 * antenna parameters.
 *
 * @param handle A pointer to the device handle returned by the
 *               #ICE_TunerOpen() call.
 *
 * @return Returns #frost_true if the tuner was successfully closed,
 *         and #frost_false if the tuner could not be closed.
 */
frost_bool ICE_TunerClose(ICE_TunerHandle* handle);


/**
 * @brief Return a tuner device to its state before initialisation.
 *
 * This complements #ICE_TunerInit(), and would be called if the
 * <code> tuner_id </code> is no longer going to be used e.g. during shutdown.
 *
 * @param tuner_id ID of the tuner device to reset.
 *
 * @return Returns #frost_true if the tuner has been reset to its uninitialised
 *         state, or #frost_false, if it was not possible to reset the tuner
 *         to its pre #ICE_TunerInit() state
 */
frost_bool ICE_TunerReset(ICE_TunerId tuner_id);


/**
 * @brief Get the number of tuners supported by the platform.
 *
 * Eclipse will call this function during initialisation.
 *
 * @return The number of tuners supported by the platform.
 */
frost_uint ICE_GetNumberOfTuners(void);


/**
 * @brief Set voltage injected into the antenna lead.
 *
 * This functionality is optional. If #ICE_SetAntennaVoltage() returns false
 * the Eclipse will attempt to close and re-open tunner in order to change voltage.
 *
 * @param antenna_voltage the DC voltage to be injected into the coax cable.
 *
 * @return Returns #frost_true on succes, #frost_false if this function is unsupported
 *         or if an attempt to change voltage has failed.
 */
frost_bool ICE_SetAntennaVoltage(ICE_Voltage antenna_voltage);


/**
 * @brief Set the type the of network.
 *
 * This functionality is not required if the tuner only supports
 * one network type. e.g. ICE_network_dvbt
 *
 * It is use before a ICE_TunerScan and other commands to set
 * the network type.
 */
void ICE_SetTunerNetworkType(ICE_TunerHandle* handle, ICE_Network type);


/**
 * @brief Set the trick mode of the tuner.
 *
 * This functionality is only valid for ip tuning. RTSP type service running
 * requires trick mode support. Trick mode request is sent over ip sockets and
 * since eclipse uses tuner to transmit the transport stream over ip to decoder,
 * setting trick mode is also done via tuner.
 */
frost_bool ICE_SetTunerTrickMode(ICE_TunerHandle* handle, ICE_TrickMode mode);


/**
 * @brief Get the trick mode of tuner.
 *
 * This functionality is only valid for ip tuning. RTSP type service running
 * requires trick mode support. Trick mode request is sent over ip sockets and
 * since eclipse uses tuner to transmit the transport stream over ip to decoder,
 * setting trick mode is done via tuner. So the getting of trick mode is
 * also done via tuner.
 */
frost_bool ICE_GetTunerTrickMode(ICE_TunerHandle* handle, ICE_TrickMode * mode);


/**
 * @brief Set the position of service in playback.
 *
 * This functionality is only valid for ip tuning.
 */
frost_bool ICE_TunerSetPosition(ICE_TunerHandle* handle, frost_uint32 seconds);


/**
 * @brief Get the current position of service in playback.
 *
 * This functionality is only valid for ip tuning.
 */
frost_bool ICE_TunerGetPosition(ICE_TunerHandle* handle, frost_uint32 * seconds);


/**
     * @brief Inform ICE for the tsid of current transponder retrieved from PAT table.
 *
 * 
 */
 void ICE_TunerUpdateTSID(frost_int tsid);
 /*
 * @brief Cable network operator configuration.
 *
 * It is only used by #ICE_GetCableNetworkInfo
 */
typedef struct ICE_CableNetworkInfo
{
    /**
     * A pointer to a const null terminated SI string that that
     * will be displayed to identify the other settings. This
     * is expected to be the network name or cable operator.
     */
    frost_int8*         m_network_name;

    /**
     * When frost_true this will allow all services that are specified in
     * the NIT (Network Infomation Table) of this transport stream (frequency)
     * to be accepted even if they are on a different frequencies.
     */
    frost_bool          m_use_nit_service_info;

    /** #ICE_Frequency */
    ICE_Frequency       m_frequency;

    /** #ICE_SymbolRate */
    ICE_SymbolRate      m_symbol_rate;

    /** #ICE_Constellation */
    ICE_Constellation   m_constellation;

    /** #ICE_CodeRateSet */
    ICE_CodeRateSet     m_code_rate;

    /** The network identifier (0 to 8191) or -1 for all networks */
    frost_int16         m_network_id;
} ICE_CableNetworkInfo;

/**
 * @brief Cable network operator configuration.
 *
 * This is used by the Manual Cable Dialog to show a predefined list
 * of cable providers or cable network the user may choose from.
 * This function may also return 0 in which case the user will not have
 * If there is no need for a for the country.
 *
 * @param country application base enum for currently selected country.
 * @param index
 * @returns if index is valid then an pionter to a ICE_CableNetworkInfo structure otherwise zero.
 */
const ICE_CableNetworkInfo* ICE_GetCableNetworkInfo(frost_uint16 country, frost_uint16 index);

typedef enum ICE_TunerDemodStatus
{
    ICE_TunerDemodStatusLoading,
    ICE_TunerDemodStatusReady
} ICE_TunerDemodStatus;
/**
 * @brief Callback function to inform Eclipse of the tuner demod update status.
 *
 *
 * @param user_data.
 * @param status The tuner demod status.
 */
typedef void (*ICE_TunerDemodCallback)(void* user_data, ICE_TunerDemodStatus status);

/**
 * @brief Register a callback that will notify Eclipse about tuner demod status changes.
 *
 * @param callback The callback function that will be called
 *        when tuner demod status changes.
 * @param user_data Context to be maintained and passed back through
 *        callback function.
 */
void ICE_RegisterTunerDemodStatusCallback(ICE_TunerDemodCallback callback, void* user_data);
#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_TUNER_H_INCLUDED*/
