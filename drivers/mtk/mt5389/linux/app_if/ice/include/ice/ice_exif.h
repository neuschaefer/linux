/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for file playback types
 *
 * This file defines the ICE structures used by the ice_file_playback interface
 *
 */

#ifndef CABOT_ICE_ICE_EXIF_H_INCLUDED
#define CABOT_ICE_ICE_EXIF_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"

/**
 * @brief Enumaration of exif details orientation types
 */
enum
{
    firstRTop_firstCLeftSide = 1,
    firstRTop_firstCRightSide,
    firstRBottom_firstCRightSide,
    firstRBottom_firstCLeftSide,
    firstRLeftSide_firstCTop,
    firstRRightSide_firstCTop,
    firstRRightSide_firstCBottom,
    firstRLeftSide_firstCBottom
};

/**
 * @brief Structure of exif information passed
 */
typedef struct ICE_MediaPlayerEXIFDetails
{
    frost_uint ice_resolution_x;
    frost_uint ice_resolution_y;
    char ice_date[11];
    char ice_time[11];
    frost_uint ice_orientation;
} ICE_MediaPlayerEXIFDetails;




#ifdef __cplusplus
}
#endif


#endif /* CABOT_ICE_ICE_EXIF_H_INCLUDED */
