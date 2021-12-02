
#ifndef CUSTOM_PDWNC_H
#define CUSTOM_PDWNC_H

#include "x_bim.h"
#include "x_hal_arm.h"

#ifdef CC_MTK_LOADER
#include "loader_if.h"
#endif /* CC_MTK_LOADER */

//---------------------------------------------------------------------------
// Customer function definitions
//---------------------------------------------------------------------------
#ifdef CC_MTK_LOADER
#ifndef CC_UBOOT
#define LOADER_ENV_INIT_FUNC                    (LOADER_Custom_EnvInitFunc)
#endif
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#ifndef CC_PSEUDO_WATCHDOG_ASSERT
#define CC_PSEUDO_WATCHDOG_ASSERT               0
#endif /* CC_PSEUDO_WATCHDOG_ASSERT */

#define WATCHDOG_SEC_CNT(x)                     (0x0fffffffU - (x * 0x300000))

#define CUSTOM_BLINK_TIME                       (25)    /* 25 x 20ms = 500ms*/
#define CUSTOM_BLINK_INVERAL                    (150)   /* 150 x 20ms = 3000ms*/

#define CUSTOM_USB_UPGRADE_BLINK_PERIOD         1000    // 1000ms
#if 0
#ifdef WAFFLE_EU_PAD
#define CUSTOM_USB_UPGRADE_BLINK_PERIOD         1000    // 1000ms
#else
#define CUSTOM_USB_UPGRADE_BLINK_PERIOD         500    // 500ms
#endif
#endif

// Multi-key definition
#define CUSTOM_uP_CMD_GET_IR_MULTIKEY_WAKEUP    0x40
#define CUSTOM_UNKNOWN_MODE                     0
#define CUSTOM_SERVICE_MODE                     1
#define CUSTOM_SELF_DIAGNOSTIC_MODE             2
#define CUSTOM_ACTIVE_HOTEL_MODE                3
#define CUSTOM_EXIT_HOTEL_MODE                  4

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
#ifdef CC_MTK_LOADER
extern void LOADER_Custom_EnvInitFunc(void *prLdrData, UINT32 fgValidFlag);
#endif /* CC_MTK_LOADER */

void PDWNC_Custom_Init(void);
void PDWNC_Custom_WatchDogInit(void);
void PDWNC_Custom_ErrorHandleInit(void);
void PDWNC_Custom_ReadWakeupReason(UINT32* pu4Reason);
void PDWNC_Custom_SetupPowerDown(UINT32 u4PowerDownEvent, UINT32 *pu4PowerDownEn);
void PDWNC_Custom_ErrorCode(UINT8 u1ErrorCode);
void PDWNC_Custom_GetCurrentState(UINT32 *pu4CurrentState);
void PDWNC_Custom_DetermineNextState(UINT32 u4CurrentState, UINT32 u4PowerEvent,
                                     UINT32 *pu4NextState, UINT32 *pfgEnterStandby,
                                     UINT32 *pu4PowerDownEnable);
void PDWNC_Custom_EnterNextState(UINT32 u4NextState);
INT32 PDWNC_Custom_Set8032uPLedBlink(UINT32 u4BlinkTimes);
UINT32 PDWNC_Custom_GetStatus(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size);
UINT32 PDWNC_Custom_LEDSet(UINT32 u4Addr, VOID* pInfo, UINT32 u4Size);

UINT32 _PDWNC_ReadWakeupStatus(void);
UINT32 _PDWNC_ReadWatchDogStatus(void);
void LOADER_Custom_TimerOn(void);
void LOADER_Custom_TimerOff(void);


#ifdef CC_UBOOT
#include "x_pdwnc.h"
#include "pdwnc_if.h"

static PDWNC_FuncTbl rPDWNC_EXT_FuncTbl = 
{
    NULL,                  // pfnInit
    NULL,        // pfnPowerDown
    NULL,      // pfnReadWakeupReason
    NULL,          // pfnWatchDogInit;
    NULL,       // pfnErrorHandleInit
    NULL,             // pfnWriteErrorCode
    NULL,                               // pfnReadErrorCode
    NULL,                               // pfnGetCurrentState
    NULL,                               // pfnDetermineNextState
    NULL,                               // pfnEnterNextState
    NULL,     // pfnSet8032uPLedBlink
    NULL,                               // pfnCustomRead
    NULL                                // pfnCustomWrite
};

PDWNC_FuncTbl* PDWNC_EXT_GetFunc(void)
{
 return (&rPDWNC_EXT_FuncTbl);
}
#endif

#endif /* CUSTOM_PDWNC_H*/

