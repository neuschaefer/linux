/*
 * ice_utils.h
 *
 *  Created on: 06.Þub.2009
 *      Author: ergino
 */

#ifndef CABOT_ICE_ICE_UTILS_H_INCLUDED
#define CABOT_ICE_ICE_UTILS_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

frost_int stricmp_( const char *src, const char *dest );

frost_int strnicmp_( const char *src, const char *dest, frost_int length );


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_UTILS_H_INCLUDED */
