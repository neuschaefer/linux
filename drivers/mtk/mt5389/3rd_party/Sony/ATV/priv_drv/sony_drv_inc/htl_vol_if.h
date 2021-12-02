#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */

#ifndef HTL_VOL_IF_H
#define HTL_VOL_IF_H

#include "x_typedef.h"

/** HTLVOL_Init
 *  Initialize the hotel volume module.
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_Init(VOID);

/** HTLVOL_GetMode
 *  Query the hotel volume module's running mode: 0 OFF, 1 ON
 *  @param  pui1Mode Would be stored the module's running mode
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_GetMode(UINT8 *pui1Mode);

/** HTLVOL_GetInitVol
 *  Query the hotel volume module's initial volume level
 *  @param  pui1InitVol Would be stored the module's initial volume level
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_GetInitVol(UINT8 *pui1InitVol);

/** HTLVOL_SetMode
 *  Set the hotel volume module's running mode: 0 OFF, 1 ON
 *  @param  pui1Mode Would be stored the module's running mode
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_SetMode(UINT8 *pui1Mode);

/** HTLVOL_SetInitVol
 *  Set the hotel volume module's initial volume level
 *  @param  pui1InitVol Would be stored the module's initial volume level
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_SetInitVol(UINT8 *pui1InitVol);

/** HTLVOL_GetStepTimer
 *  Query the hotel volume module's step timer
 *  @param  pui1StepTimer Would be stored the module's step timer
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_GetStepTimer(UINT8 *pui1StepTimer);

/** HTLVOL_SetInitVol
 *  Set the hotel volume module's step timer
 *  @param  pui1StepTimer Would be stored the module's step timer
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_SetStepTimer(UINT8 *pui1StepTimer);

/** HTLVOL_SetVolLvl
 *  Set the hotel volume module's volume level
 *  @param  pui1VolLvl Would be stored the module's volume level
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_SetVolLvl(UINT8 *pui1VolLvl);

/** HTLVOL_GetVolLvl
 *  Get the hotel volume module's volume level
 *  @param  pui1VolLvl Would be stored the module's volume level
 *  @retval FALSE FAIL
 *          TRUE SUCCESSFUL
 */
BOOL HTLVOL_GetVolLvl(UINT8 *pui1VolLvl);

#endif /* HTL_VOL_IF_H */
#endif /* THIRD_PARTY_LIB_EMCS_ATV_ONLY */  /* Not Used in ATSC Project */

