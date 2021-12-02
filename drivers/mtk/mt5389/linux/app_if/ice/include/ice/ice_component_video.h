/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE Interface for control of component video output ( eg YPbPr )
 *
 */

#ifndef CABOT_ICE_ICE_COMPONENT_VIDEO_H_INCLUDED
#define CABOT_ICE_ICE_COMPONENT_VIDEO_H_INCLUDED

#include "frost/frost_basictypes.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Constrain all component video output to a maximum resolution of 520k.
 * 
 * This means that visual equivalent of not more than 520,000 pixels per 
 * frame (e.g. an image with a resolution of 960 horizontal pixels by 540
 * vertical pixels for a 16:9 aspect ratio). 
 *
 * @param force_sd #frost_true means constrain the image, #frost_false 
 * means do not constrain the image.
 *
 * @return #frost_true if the image is constrained successfully, #frost_false 
 * otherwise.
 */
frost_bool ICE_ComponentVideoForceSD(frost_bool force_sd);

/**
 * @brief Completely disable Component video output.
 * 
 * CI+ v.1.3.1 has added a new Digital Only Token for use with Content Control v2.
 * This requires ALL analog outputs to be completely disabled.
 *
 * @param force_on #frost_false means disable output, #frost_true to enable output.
 *
 * @return #frost_true if the output is disabled successfully, #frost_false 
 * otherwise.
 */
frost_bool ICE_ComponentVideoForceOnOff(frost_bool force_on);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_COMPONENT_VIDEO_H_INCLUDED*/
