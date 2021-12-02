/**
 * NXP Eindhoven - Cesar 09_09_2009
 *
 * Test header file for accessing Color & Sound System Changes
 * Cabot should modify later according his interfaces.
 */

#ifndef CABOT_ICE_ICE_COLORSOUND_H_INCLUDED
#define CABOT_ICE_ICE_COLORSOUND_H_INCLUDED

//#include "tv_systemcolorsound.h"
#include "frost/frost_basictypes.h"


/**
 * @brief List of actual Tv system options
 */
typedef enum ICE_TVSystemList
{
    ice_systemcolorsound_TVSystem_BG,     /** System BG */
    ice_systemcolorsound_TVSystem_I,       /** System I */
    ice_systemcolorsound_TVSystem_DK,     /** System DK */    
    ice_systemcolorsound_TVSystem_L,     /** System L */    
    ice_systemcolorsound_TVSystem_LP,     /** System LP */    
    ice_systemcolorsound_TVSystem_M     /** System M */    
}ICE_TVSystemList;


/**
 * @brief List of actual Tv Color options
 */
typedef enum ICE_TVColorList
{
    ice_systemcolorsound_TVColor_Auto,     /** Color Auto */
    ice_systemcolorsound_TVColor_Pal,        /** Color Pal */
    ice_systemcolorsound_TVColor_Secam,  /** Color Secam*/
    ice_systemcolorsound_TVColor_NTSCM,   /** Color NTSC-M */
    ice_systemcolorsound_TVColor_NTSC443 /** Color NTSC-4.43 */
} ICE_TVColorList;


 /**
 * @brief Gets the TV's input's System 
 *
 * @param *ice_tvsystem [o] current Tv System
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_ColorSoundGetTvSystem(ICE_TVSystemList *ice_tvsystem);


/**
 * @brief Sets a TV system input's 
 *
 * @param ice_tvsystem [i] - new Tv System 
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_ColorSoundSetTvSystem(ICE_TVSystemList ice_tvsystem);

/**
 * @brief Gets the TV's input's Color System 
 *
 * @param *ice_tvcolor [o] current Tv Color System
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_ColorSoundGetColorSystem(ICE_TVColorList *ice_tvcolor);

/**
 * @brief Sets a TV Color system input's 
 *
 * @param ice_tvcolor [i] - new Tv Color System 
 *
 * @return true on success, false otherwise.
 */
frost_bool ICE_ColorSoundSetColorSystem(ICE_TVColorList ice_tvcolor);


#endif /* CABOT_ICE_ICE_COLORSOUND_H_INCLUDED */

