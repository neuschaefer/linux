/**
 * Copyright 2011 Vestel Electronics.  All rights reserved.
 *
 * @file
 * @brief Vestel's portable interface to a flash engine.
 */

#ifndef ICE_ICE_FLASH_ENGINE_H_INCLUDED
#define ICE_ICE_FLASH_ENGINE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"
#include "ice/ice_keypress_id.h"

/**
 * Enum of possible flash engine error codes
 */
typedef enum ICE_Flash_Engine_Error
{
    ICE_Flash_Engine_No_Error = 0,        /**< Operation completed successfully. */
    ICE_Flash_Engine_Generic_Error        
} ICE_Flash_Engine_Error;

/**
 * @brief Initialise the ICE flash engine.
 *
 * @return ICE_Flash_Engine_No_Error if initialisation is successful or if the flash engine is already initialised,
 * else appropriate ICE_Flash_Engine_Error code to indicate the initialisation problem.
 */
ICE_Flash_Engine_Error ICE_FlashEngineInit();

/**
 * @brief Terminate the ICE flash engine.
 *
 * @return ICE_Flash_Engine_No_Error if termination is successful or if the flash engine is already terminated,
 * else appropriate ICE_Flash_Engine_Error code to indicate the termination problem.
 */
ICE_Flash_Engine_Error ICE_FlashEngineTerm();

/**
 * @brief Force flash engine to load flash application located in applicationPath.
 *
 * @param applicationPath  Specifies the the location of flash application to be loaded.
 * @return ICE_Flash_Engine_No_Error if load is successfull,
 * else appropriate ICE_Flash_Engine_Error code to indicate the loading problem.
 */
ICE_Flash_Engine_Error ICE_FlashEngineLoadApp(const char* applicationPath, const char* flashvars);

/**
 * @brief Force flash engine to destroy flash application if there is any loaded application.
 *
 * @return ICE_Flash_Engine_No_Error if destroy is successfull,
 * else appropriate ICE_Flash_Engine_Error code to indicate the destroying problem.
 */
ICE_Flash_Engine_Error ICE_FlashEngineDestroyApp();

/**
 * @brief Returns the flash engine running status.
 *
 * @return frost_true if flash engine is running (indicates if there is a flash application running),
 * else frost_false.
 */
frost_bool ICE_FlashEngineIsRunning();

/**
 * @brief Send an event key to be handled by flash engine.
 *
 * @param key  Key value that needs to be handled by flash engine.
 * @return frost_true if key is handled by flash engine,
 * else frost_false.
 */
frost_bool ICE_FlashEnginePostKeyEvent(ICE_KeypressId key);

#ifdef __cplusplus
}
#endif

#endif /* ICE_ICE_FLASH_ENGINE_H_INCLUDED*/

