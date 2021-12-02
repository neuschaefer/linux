/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for Digital Transmission Content Protection (DTCP)
 *
 * This file defines the ICE interface for the setup of DTCP
 *
 * @note this interface implies that all suitable outputs that support DTCP are
 * configured together (e.g. IP, FireWire, USB, Bluetooth)
 *
 * See Digital Transmission Content Protection Specification Volume 1
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 *
 * @note For dtcp_cci_copy_one_generation it is the responsibility of the ICE implementation 
 * to determine if a copy can be made or not and set dtcp_cci_no_more_copies if applicable.
 */

#ifndef CABOT_ICE_ICE_DTCP_H_INCLUDED
#define CABOT_ICE_ICE_DTCP_H_INCLUDED

#include "ice/ice_device_number.h"
#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The DTCP Copy Control Information (CCI) values.
 */
typedef enum ICE_DTCP_CCI
{
    dtcp_cci_copy_free,
    dtcp_cci_no_more_copies,        /* Note: This will never be set by Eclipse */
    dtcp_cci_copy_one_generation,
    dtcp_cci_copy_never
} ICE_DTCP_CCI;

/**
 * @brief DTCP Encryption Plus Non-assertion (EPN) values.
 */
typedef enum ICE_DTCP_EPN
{
    dtcp_epn_asserted,
    dtcp_epn_unasserted
    
} ICE_DTCP_EPN;

/**
 * @brief DTCP Image Contraint Token (ICT) values.
 */
typedef enum ICE_DTCP_ICT
{
    dtcp_ict_asserted,
    dtcp_ict_unasserted
} ICE_DTCP_ICT;

/**
 * @brief DTCP Analog Protection System (APS) values.
 */
typedef enum ICE_DTCP_APS
{
    dtcp_aps_copy_free,
    dtcp_aps_on_type_1_agc,
    dtcp_aps_on_type_2_agc_2l_colorstripe,
    dtcp_aps_on_type_3_agc_4l_colorstripe
} ICE_DTCP_APS;

/**
 * @brief DTCP settings
 */
typedef struct ICE_DTCPSettings
{
    /**
     * Copy Control Information (CCI) value
     */
    ICE_DTCP_CCI cci;

    /**
     * Encryption Plus Non-assertion (EPN) value
     */
    ICE_DTCP_EPN epn;
    
    /**
     * Image Contraint Token (ICT) value
     */
    ICE_DTCP_ICT ict;
    
    /**
     * Analog Protection System (APS) value
     */
    ICE_DTCP_APS aps;
    
} ICE_DTCPSettings;

/**
 * @brief Set the settings for all outputs that support DTCP.
 *
 * See Digital Transmission Content Protection Specification Volume 1 for how
 * these values should be used.
 *
 * @param source The source device i.e. ICE_source_tuner0 or ICE_source_tuner1
 * @param settings The DTCP settings to use. If null then DTCP is not required
 *
 * @return #frost_true if the DTCP settings were set successfully,
 *         #frost_false otherwise.
 */
frost_bool ICE_DTCPSet(ICE_DeviceSource source, const ICE_DTCPSettings* settings);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef CABOT_ICE_ICE_DTCP_H_INCLUDED */
