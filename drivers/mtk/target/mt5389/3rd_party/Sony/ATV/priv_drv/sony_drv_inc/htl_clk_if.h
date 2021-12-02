#ifndef HTL_CLK_IF_H
#define HTL_CLK_IF_H

#include "x_typedef.h"

/** HTLCLK_Init
 *  Initialize the hotel clock module.
 *  By default, the interface to hardware is disabled.
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLCLK_Init(VOID);

/** HTLCLK_GetMode
 *  Query the hotel clock module's running mode: 0 OFF, 1 ON
 *  @param  pui1Mode Would be stored the module's running mode
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLCLK_GetMode(UINT8 *pui1Mode);

/** HTLCLK_SetTime
 *  Set time into the hotel clock.
 *  @param  ui1Hour  Hour to set (0-23)
 *  @param  ui1Minutes  Minute to set (0-59)
 *  @param  ui1Seconds  Second to set (0-59)
 *  @param  ui1Weekday  Day of week to set (0-6), 0 for Sunday, while 6 for Saturday
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLCLK_SetTime(UINT8 ui1Hour, UINT8 ui1Minutes, UINT8 ui1Seconds, UINT8 ui1Weekday);

/** HTLCLK_Config
 *  Configure settings in the hotel clock
 *  @param  ui1Display  0 ON, 1 OFF
 *  @param  ui1Intensity  0 LOW, 1 HIGH
 *  @param  ui1Reset  When this is set, hotel clock will be reset. Time should be set again using HTLCLK_SetTime
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLCLK_Config(UINT8 ui1Display, UINT8 ui1Intensity, UINT8 ui1Reset);

/** HTLCLK_GetTime
 *  Get time from the hotel clock
 *  @param  pui1Hour  Buffer to be set Hour
 *  @param  pui1Minutes  Buffer to be set Minutes
 *  @param  pui1Seconds  Buffer to be set Seconds
 *  @param  pui1Weekday  Buffer to be set Day of Week
 *  @param  pui1TimeValid  Buffer to be set the value which will indicate whether the time set is valid or not
 *                         (Only use the time returned if this value is 1)
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLCLK_GetTime(UINT8 *pui1Hour, UINT8 *pui1Minutes, UINT8 *pui1Seconds, UINT8 *pui1Weekday, UINT8 *pui1TimeValid);

/** HTLCLK_GetVersion
 *  Get the hotel clock's SW version in string format 
 *  @param  pui1VersionStr  Buffer to be stored with the hotel clock's SW version in string format
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLCLK_GetVersion(CHAR *pui1VersionStr);

#endif
