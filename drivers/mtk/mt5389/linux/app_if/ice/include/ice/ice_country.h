 /**
 * Copyright 2010 Vestel Elektronik A.S. All rights reserved.
 *
 * @file ice_country.h
 * @brief Interface to country information.
 *
 */

#ifndef CABOT_ICE_ICE_COUNTRY_H_INCLUDED
#define CABOT_ICE_ICE_COUNTRY_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration of possible country settings.
 *
 * @note Please change this enumaration if any change is applied to middleware enumaration.
 */
typedef enum ICE_CountryCode
{
    ice_country_code_none             = 0x00,
    ice_country_code_australia        = 0x01,
    ice_country_code_denmark,
    ice_country_code_finland,
    ice_country_code_france,
    ice_country_code_germany,
    ice_country_code_italy,
    ice_country_code_netherlands,
    ice_country_code_norway,
    ice_country_code_poland,
    ice_country_code_portugal,
    ice_country_code_spain,
    ice_country_code_sweden,
    ice_country_code_united_kingdom,
    ice_country_code_albania,
    ice_country_code_austria,
    ice_country_code_belgium,
    ice_country_code_bulgaria,
    ice_country_code_china,
    ice_country_code_czech_republic,
    ice_country_code_estonia,
    ice_country_code_greece,
    ice_country_code_hungary,
    ice_country_code_israel,
    ice_country_code_latvia,
    ice_country_code_lithuania,
    ice_country_code_macedonia,
    ice_country_code_romania,
    ice_country_code_russia,
    ice_country_code_serbia,
    ice_country_code_slovakia,
    ice_country_code_slovenia,
    ice_country_code_switzerland,
    ice_country_code_turkey,
    ice_country_code_croatia,
    ice_country_code_iceland,
    ice_country_code_luxembourg,
    ice_country_code_ireland,
    ice_country_code_san_marino,
    ice_country_code_new_zealand,
    ice_country_code_montenegro,
    ice_country_code_ukraine,
    ice_country_code_south_africa,
    ice_country_code_isle_of_man,
    ice_country_code_saudi_arabia,
    ice_country_code_persia,
    ice_country_code_united_arab_emirates,
    ice_country_code_kuwait,
    ice_country_code_oman,
    ice_country_code_bahrain,
    ice_country_code_qatar,
    ice_country_code_iraq,
    ice_country_code_jordan,
    ice_country_code_lebanon,
    ice_country_code_belarussia,
    ice_country_code_other_frequency_search,
    ice_country_code_other             = 0xFFFF
} ICE_CountryCode;

/**
 * @brief Set the geographical location of the tv.
 *
 * @param country Country code of tv's location
 */
void ICE_CountrySetCode(ICE_CountryCode country_code);

/**
 * @brief Get the geographical location of the tv.
 *
 */
ICE_CountryCode ICE_CountryGetCode();

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_COUNTRY_H_INCLUDED*/
