/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Allow the implementation to provide an I-frame to be shown
 *        while a UK MHEG5-only service is loading if required.
 */

#ifndef CABOT_ICE_ICE_SPLASH_IFRAME_H_INCLUDED
#define CABOT_ICE_ICE_SPLASH_IFRAME_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice_video_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Get details of any I-frame which is to be displayed while an
 *        MHEG-5 service without default audio or video is being loaded.
 *
 * @param iframe_data   Output parameter that should be set to the start
 *                      address of the I-frame data.
 * @param iframe_length Output parameter that should be set to the length
 *                      of the I-frame data.
 * @param iframe_type   Output parameter that should be set to either
 *                      ICE_VideoStreamType_mpeg2 or ICE_VideoStreamType_h264.
 *
 * This function will be called when a service is selected AND:
 *    -# Cabot's Mercator MHEG-5 engine is integrated with Eclipse;
 *    -# the current country has been set to country_code_united_kingdom;
 *    -# the selected service has MHEG-5 data components;
 *    -# the selected service has no default audio or video components.
 *
 * @note  UK content authors prefer implementations NOT to display splash
 *        I-frames while MHEG-5 applications load.  This API should return
 *        frost_false to achieve this.
 *
 * @return #frost_true if the parameters have been set correctly and a splash
 *         I-frame is to be shown.  #frost_false on failure or if no I-frame
 *         is to be shown (preferred).
 */
frost_bool ICE_GetDataServiceSplashIframe(frost_uint8 **iframe_data,
                                          frost_uint *iframe_length,
                                          ICE_VideoStreamType *iframe_type);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SPLASH_IFRAME_H_INCLUDED*/
