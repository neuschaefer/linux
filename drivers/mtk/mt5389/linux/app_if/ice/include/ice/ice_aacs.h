/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for the Advanced Access Control System (AACS)
 *
 * This file defines the ICE interface for the setup of the AACS.
 *
 *
 * @note this interface implies that all suitable storage that support AACS are
 * configured together (e.g. Blu-ray)
 *
 * See Advanced Access Content System (AACS) Blu-ray Disc Recordable Book Revision 0.95
 * http://www.aacsla.com
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 *
 * @note For aacs_cci_copy_one_generation it is the responsibility of the ICE implementation 
 * to determine if a copy can be made or not and set aacs_cci_no_more_copy if applicable.
 */

#ifndef CABOT_ICE_ICE_AACS_H_INCLUDED
#define CABOT_ICE_ICE_AACS_H_INCLUDED

#include "ice/ice_device_number.h"
#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The AACS Copy Control Information (CCI) values.
 */
typedef enum ICE_AACS_CCI
{
    aacs_cci_copy_control_not_asserted, /* i.e. Copy free */
    aacs_cci_no_more_copy,              /* Note: This will never be set by Eclipse */
    aacs_cci_copy_one_generation,
    aacs_cci_never_copy
} ICE_AACS_CCI;

/**
 * @brief AACS Encryption Plus Non-assertion (EPN) values.
 */
typedef enum ICE_AACS_EPN
{
    aacs_epn_asserted,
    aacs_epn_unasserted
    
} ICE_AACS_EPN;

/**
 * @brief AACS Image Contraint Token (ICT) values.
 */
typedef enum ICE_AACS_ICT
{
    aacs_ict_asserted,      /* High Definition Analog Output in the form of Constrained Image */
    aacs_ict_unasserted     /* High Definition Analog Output in High Definition Analog Form */
} ICE_AACS_ICT;

/**
 * @brief AACS Trusted Input values.
 */
typedef enum ICE_AACS_TI
{
    aacs_ti_not_trusted,
    aacs_ti_trusted
    
} ICE_AACS_TI;

/**
 * @brief AACS Digital Only Token values.
 */
typedef enum ICE_AACS_DOT
{
    aacs_dot_analog_digital_outputs,
    aacs_dot_only_digital_outputs
    
} ICE_AACS_DOT;

/**
 * @brief AACS Analog Protection System (APS) values.
 */
typedef enum ICE_AACS_APS
{
    aacs_aps_copy_free,
    aacs_aps_on_type_1_agc,
    aacs_aps_on_type_2_agc_2l_colorstripe,
    aacs_aps_on_type_3_agc_4l_colorstripe,
    aacs_aps_reserved,
    aacs_aps_reserved2,
    aacs_aps_aps2_on,
    aacs_aps_aps2_on2
} ICE_AACS_APS;

/**
 * @brief AACS settings
 */
typedef struct ICE_AACSSettings
{
    /**
     * Copy Control Information (CCI) value
     */
    ICE_AACS_CCI cci;

    /**
     * Encryption Plus Non-assertion (EPN) value
     */
    ICE_AACS_EPN epn;
    
    /**
     * Image Contraint Token (ICT) value
     */
    ICE_AACS_ICT ict;
    
    /**
     * Trusted Input (TI) value
     */
    ICE_AACS_TI ti;

    /**
     * Digital Only Token (DOT) value
     */
    ICE_AACS_DOT dot;

    /**
     * Analog Protection System (APS) value
     */
    ICE_AACS_APS apstb;
    
} ICE_AACSSettings;

/**
 * @brief Set the settings for all outputs that support AACS.
 *
 * See Advanced Access Control System (AACS) Blu-ray Disc Recordable Book Revision 0.95
 * for how these values should be used.
 *
 * @param source The source device i.e. ICE_source_tuner0 or ICE_source_tuner1
 * @param settings The AACS settings to use. If null then AACS is not required
 *
 * @return #frost_true if the AACS settings were set successfully,
 *         #frost_false otherwise.
 */
frost_bool ICE_AACSSet(ICE_DeviceSource source, const ICE_AACSSettings* settings);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef CABOT_ICE_ICE_AACS_H_INCLUDED */
