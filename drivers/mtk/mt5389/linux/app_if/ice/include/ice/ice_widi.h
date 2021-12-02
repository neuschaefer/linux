/**
 *
 * @file
 * @brief Types and functions to interface with the WiDi interface.
 */

#ifndef CABOT_ICE_ICE_WIDI_H_INCLUDED
#define CABOT_ICE_ICE_WIDI_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_WIDI

#include "widi_type.h"
#include "widi_api.h"


/// Callback function to ICE layer
/// ICE_WiDiCallback(EN_WIDI_EVENT eventID, void * param, void *pUsrParam)
///
/// ICE layer need to implement below event, please reference "Widi_type.h"
///         int enable = (*((int*)param));
///
///   3), WIDI_EVENT_FAST_CURSOR_POSITION  (WIDI_EVENT_FAST_CURSOR_POSITION, void * param, void *pUsrParam)
///         Widi_position cursor_pos = *((Widi_position*)param);
///       *. plot the cursor with the position info from WiDi library

//typedef WIDI_EVENT_CB ICE_WiDiCallback;
typedef void (* ICE_WiDiCallback)(EN_WIDI_EVENT eventID, void * param, void *pUsrParam, void* user_data);

typedef struct
{
    void * ICE_WiDi_EVENT_STATE_pUserPara;
    void * ICE_WiDi_EVENT_FAST_CURSOR_POSITION_pUserPara;
    void * ICE_WiDi_EVENT_FAST_CURSOR_ENABLED_pUserPara;
    void * ICE_WiDi_userdata;
} ICE_WiDi_Callback_UserPara;


frost_bool ICE_WiDiInit(ICE_WiDiCallback cb_func,ICE_WiDi_Callback_UserPara *cb_func_userpara, void* userdata);

#else

#include "ice_widi_types.h"

typedef void (* ICE_WiDiCallback)(EN_WIDI_EVENT eventID, void * param, void *pUsrParam, void* user_data);

typedef struct
{
    void * ICE_WIDI_EVENT_STATE_UserPara;
    void * ICE_WIDI_EVENT_FAST_CURSOR_POSITION_UserPara;
    void * ICE_WIDI_EVENT_FAST_CURSOR_ENABLED_UserPara;
    void * ICE_WiDi_userdata;
}ICE_WiDi_Callback_UserPara;

static ICE_WiDiCallback Widi_Callback;
static ICE_WiDi_Callback_UserPara *Widi_Callback_User_Para;

frost_bool ICE_WiDiInit(ICE_WiDiCallback cb_func, ICE_WiDi_Callback_UserPara *cb_func_userpara, void* userdata);

#endif

frost_bool ICE_WiDiStart(void);

frost_bool ICE_WiDiStop(void);

frost_bool ICE_WiDiHDCPInit(void);

frost_bool ICE_WiDiSetHDCPKey(void);

#ifdef __cplusplus
}
#endif

#endif /* defined CABOT_ICE_ICE_WIDI_H_INCLUDED */
