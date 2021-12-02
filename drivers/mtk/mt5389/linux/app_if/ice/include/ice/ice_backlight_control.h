
/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief ICE interface for backlight control. 
 
 * This file defines the ICE interfaces to control backlight settings of a TV panel.
 *
 */

#ifndef CABOT_ICE_ICE_BACKLIGHT_CONTROL_H_INCLUDED
#define CABOT_ICE_ICE_BACKLIGHT_CONTROL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"

/**
 * @brief Enable / disable backlight of a TV panel.
 * 
 * @param enable Enable backlight if #frost_true, disable if #frost_false.
 * 
 * @return #frost_true on success, #frost_false otherwise. 
 */
frost_bool ICE_BacklightEnable(frost_bool enable);

#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_BACKLIGHT_CONTROL_H_INCLUDED */

