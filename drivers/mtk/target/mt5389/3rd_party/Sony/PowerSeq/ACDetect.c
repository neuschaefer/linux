
#include "x_os.h"
#include "x_bim.h"
#include "x_debug.h"
#include "x_hal_arm.h"
#include "x_timer.h"
//#include "x_assert.h"

#include "ostg_if.h"
#include "rtc_if.h"
#include "pdwnc_if.h"
#include "eeprom_if.h"
#include "PowerSeq.h"
#include "CustomCtrl.h"

//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------
extern void vDrvLVDSOff(void);
extern void vDrvTurnOffPanel(void);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _bfgACDetectGo = FALSE;
static BOOL _bfgEnableShutdownState = FALSE;
static UINT32 _u4ACDetectDelay = 0;
static UINT32 _u4ACDetectRetryLimit = 0;
static HANDLE_T _hACDetectHandle = NULL;
static HANDLE_T _hACDetectSema = NULL;
static HANDLE_T _hACDetectMsgQue = NULL;
static HANDLE_T _hACDetectStateHandle = NULL;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
static void CustomACDetectStateHandle(void *pvArg)
{
    UINT16 u2MsgIdx;
    UINT32 u4Msg;

    UNUSED(pvArg);
    while (1)
    {
        SIZE_T zMsgSize = sizeof(UINT32);
        INT32  i4MsgRet;
        UINT8  u1ShutdownState = 0;
     
        i4MsgRet = x_msg_q_receive(&u2MsgIdx, &u4Msg, &zMsgSize,
                                   &_hACDetectMsgQue, 1, X_MSGQ_OPTION_WAIT);
        if (i4MsgRet == OSR_OK)
        {
            UNUSED(EEPROM_Read(CUSTOM_SHUTDOWN_OCCUR_ADDR, 
                               (UINT32)(VOID *)&u1ShutdownState, sizeof(UINT8)));
            
            switch (u4Msg)
            {
            case CUSTOM_POWER_SEQ_SET_SHUTDOWN_STATE:
#ifdef CUSTOM_ALL_AC_DET_LOG
                PRINT_TIMESTAMP_DBG_MSG("[AC Detect]",
                                    "Read shutdown status = %d\n", u1ShutdownState);
#endif /* CUSTOM_ALL_AC_DET_LOG */

                // Set AC_DET error into EEPROM
                u1ShutdownState |= CUSTOM_SHUTDOWN_AC_DET;
                UNUSED(EEPROM_Write(CUSTOM_SHUTDOWN_OCCUR_ADDR, 
                                    (UINT32)(VOID *)&u1ShutdownState,
                                    sizeof(UINT8)));
                
#ifdef CUSTOM_ALL_AC_DET_LOG
                PRINT_TIMESTAMP_MSG("[AC Detect]",
                                    "Write CUSTOM_SHUTDOWN_AC_DET = 1!\n");
#endif /* CUSTOM_ALL_AC_DET_LOG */
                break;

            case CUSTOM_POWER_SEQ_CLEAR_SHUTDOWN_STATE:
#ifdef CUSTOM_ALL_AC_DET_LOG
                PRINT_TIMESTAMP_DBG_MSG("[AC Detect]",
                                    "Read shutdown status = %d\n", u1ShutdownState);
#endif /* CUSTOM_ALL_AC_DET_LOG */

                // Clear AC_DET error into EEPROM
                u1ShutdownState &= ~CUSTOM_SHUTDOWN_AC_DET;
                UNUSED(EEPROM_Write(CUSTOM_SHUTDOWN_OCCUR_ADDR, 
                                    (UINT32)(VOID *)&u1ShutdownState,
                                    sizeof(UINT8)));
                break;
            }
        }
    }
}

static void CustomACDetectShutdown(void)
{
    UINT64 u8Time;
    CRIT_STATE_T rCritState;

#ifdef CUSTOM_ALL_AC_DET_LOG
    PRINT_TIMESTAMP_MSG("[AC Detect]", "AC_DET error !\n");
#endif /* CUSTOM_ALL_AC_DET_LOG */

    RTC_Init();
    rCritState = x_crit_start();
    
#ifdef CUSTOM_ALL_AC_DET_LOG
    PRINT_TIMESTAMP_MSG("[AC Detect]", "LVDS_OUTPUT = 0\n");
#endif /* CUSTOM_ALL_AC_DET_LOG */
    vDrvLVDSOff();
            
    x_thread_delay(CustomQueryPowerSequenceDelay(10));

    vDrvTurnOffPanel(); // Panel off
    
#ifdef CUSTOM_ALL_AC_DET_LOG
    PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "BL_ON = %d\n",
                            (INT32)!DRVCUST_PanelGet(ePanelPowerOnVal));
#endif /* CUSTOM_ALL_AC_DET_LOG */
            
#ifdef CC_WAFFLE_PA_ANALOG_ONLY_RC_SUPPORT
    INT32 i4Value = 0;
    UNUSED(GPIO_Output(SIGNAL_X_U_RST, &i4Value));
    PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "X_U_RST = %d\n", i4Value);
#endif /* CC_WAFFLE_PA_ANALOG_ONLY_RC_SUPPORT */
            
    // Turn off all Interrupt.
    BIM_WRITE32(REG_RW_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_RW_IRQEN, 0);
    BIM_WRITE32(REG_RW_FIQCL, 0xffffffff);
    BIM_WRITE32(REG_RW_FIQEN, 0);

    if (DRVCUST_InitGet(ePdwncPolarity))
    {
        vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_PDWN_POL);
    }
    else
    {
        vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_PDWN_POL);
    }

    // Disable the watchdog
    vIO32Write4B(PDWNC_WDTCTL, 0);
                
    RTC_GetTimeDate(&u8Time);
    u8Time += 3;
    RTC_SetAlertTime(u8Time);
        
    // Enable RTC wakeup            
    vIO32WriteFldAlign(PDWNC_WAKEN, 0x1, FLD_RTC_WAKEN);

    // Control LED
    CustomLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
    CustomLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
    CustomLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
    PRINT_TIMESTAMP_MSG("[AC Detect]", "Set Standy LED !\n");
    x_crit_end(rCritState);
            
    PRINT_TIMESTAMP_MSG("[AC Detect]", "Standby !\n");
    vIO32Write4B(PDWNC_PDCODE, PWRDWN_CODE1);
    vIO32Write4B(PDWNC_PDCODE, PWRDWN_CODE2);
    return;
}

static void CustomACDetectClearShutdownState(HANDLE_T hHandle, VOID* pTag)
{
    UNUSED(hHandle);
    UNUSED(pTag);
    
    // Clear AC_DET error into EEPROM
    UINT32 u4Value = CUSTOM_POWER_SEQ_CLEAR_SHUTDOWN_STATE;
    UNUSED(x_msg_q_send(_hACDetectMsgQue, &u4Value, sizeof(UINT32), 1));
    _bfgEnableShutdownState = FALSE;
}

static void CustomACDetectBackNormal(HANDLE_T hHandle, VOID* pTag)
{
    BOOL bPowerOnIsDone;
    
    UNUSED(hHandle);
    UNUSED(pTag);
    
    // get the power sequence status
    bPowerOnIsDone = CustomGetPowerSeqStatus();
    
    // Clear AC_DET error into EEPROM
    CustomACDetectClearShutdownState(NULL, NULL);
    
    if (bPowerOnIsDone)
    {
        // Turn on backlight
        PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "LVDS_PWR_ON = %d\n",
                                DRVCUST_PanelGet(eBackLightOnVal));
        vDrvTurnOnBacklt();
        
        // Start Self Diagnostic
        PRINT_TIMESTAMP_MSG("[AC Detect]", "Start Self Diagnostic\n");
        SELF_DIAG_RESUME();
    }
}

static void CustomACDetectHandle(void *pvArg)
{
    INT32  i4Value = 0;
    UINT8  u1ShutdownState = 0;
    UINT32 u4Counter = 0;
    BOOL bGoBackNormal = FALSE;
    HANDLE_T hACDetectTimer = NULL;

    // Create a 200-ms timer to turn on backlight.
    UNUSED(x_timer_create(&hACDetectTimer));

    // Clear AC_DET error into EEPROM
    UNUSED(EEPROM_Read(CUSTOM_SHUTDOWN_OCCUR_ADDR, 
                       (UINT32)(VOID *)&u1ShutdownState, sizeof(UINT8)));
    u1ShutdownState &= ~CUSTOM_SHUTDOWN_AC_DET;
    UNUSED(EEPROM_Write(CUSTOM_SHUTDOWN_OCCUR_ADDR, 
                        (UINT32)(VOID *)&u1ShutdownState, sizeof(UINT8)));
    _bfgEnableShutdownState = FALSE;

    UNUSED(pvArg);
    while (_bfgACDetectGo)
    {
        // Detect AC drop
        i4Value = GPIO_Input(SIGNAL_AC_DET);
        if (i4Value == AC_DETECT_ERROR)
        {
            PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "AC_DET = %d\n", i4Value);
            
            if (bGoBackNormal)
            {
                // If the AC_DET go back the normal state before,
                // it must stop the 200-ms timer and re-start error detection
                UNUSED(x_timer_stop(hACDetectTimer));
                bGoBackNormal = FALSE;
            }
            
            if (u4Counter == 0)
            {
                // Pause SelfDiag
#ifdef CUSTOM_ALL_AC_DET_LOG
                PRINT_TIMESTAMP_MSG("[AC Detect]", "Stop Self Diagnostic\n");
#endif /* CUSTOM_ALL_AC_DET_LOG */
                SELF_DIAG_PAUSE();
                
                // backlight power -> lvds sugnal -> lvds power
#ifdef CUSTOM_ALL_AC_DET_LOG
                PRINT_TIMESTAMP_DBG_MSG("[AC Detect]", "BL_ON = %d\n",
                                        (INT32)!DRVCUST_PanelGet(eBackLightOnVal));
#endif /* CUSTOM_ALL_AC_DET_LOG */
                vDrvTurnOffBacklt(); // Backlight off

                // Update shutdown status into EEPROM 
                if (!_bfgEnableShutdownState)
                {
                    UINT32 u4Value = CUSTOM_POWER_SEQ_SET_SHUTDOWN_STATE;
                    UNUSED(x_msg_q_send(_hACDetectMsgQue, &u4Value,
                                        sizeof(UINT32), 1));
                    _bfgEnableShutdownState = TRUE;
                }
            }

            if (++u4Counter == _u4ACDetectRetryLimit)
            {
                // If "AC_DET == AC_DETECT_ERROR" continues 3 times.
                // Go to panel off
                CustomACDetectShutdown();
            }
        }
        else
        {
            // Wait 200 ms to check if AC power is always on the normal state
            if (u4Counter > 0)
            {
                PRINT_TIMESTAMP_MSG("[AC Detect]", "AC_DET back to the normal state \n");
                UNUSED(x_timer_start(hACDetectTimer, CustomQueryPowerSequenceDelay(11),
                                     X_TIMER_FLAG_ONCE, CustomACDetectBackNormal,
                                     NULL));
                bGoBackNormal = TRUE;
            }

            u4Counter = 0;
        }
        
        x_thread_delay(_u4ACDetectDelay);
    }

    VERIFY(x_sema_unlock(_hACDetectSema) == OSR_OK);
    
    UNUSED(x_timer_delete(hACDetectTimer));

    x_thread_exit();
}

void CustomStartACDetect(void)
{
    UINT8  u1Val = 0;
    UINT16 u2Val = 0;
    UINT32 u4Priority = DRVCUST_OptGet(eReleaseWatchdogPriority);

    // Get detect interval & retry limit
    GET_POWER_SEQ_DELAY(POWER_SEQ_AC_DET_TIME_ADDR, _u4ACDetectDelay, 
                        u2Val, sizeof(UINT16), POWER_SEQ_AC_DET_TIME_DEFAULT);

    GET_POWER_SEQ_DELAY(POWER_SEQ_AC_DET_RETRY_ADDR, _u4ACDetectRetryLimit, 
                        u2Val, sizeof(UINT16), POWER_SEQ_AC_DET_RETRY_LIMIT);
    
    // Clear Shutdown state in EEPROM
    GET_POWER_SEQ_DELAY(CUSTOM_SHUTDOWN_OCCUR_ADDR, u1Val, u2Val, 
                        sizeof(UINT8), CUSTOM_SHUTDOWN_STATE_DEFAULT);
    
    _bfgACDetectGo = TRUE;
    
    if (_hACDetectMsgQue == NULL)
    {
        VERIFY(x_msg_q_create(&_hACDetectMsgQue, "ACDetectMsgQue", 
                              sizeof(UINT32), 8) == OSR_OK);
        
        // Create the AC detect message handle task
        if (x_thread_create(&_hACDetectStateHandle, "ACDetectState",
                            0x800, u4Priority, CustomACDetectStateHandle,
                            0, NULL) != OSR_OK)
        {
            LOG(3, "\nError: Create AC Detect state handle thread fail\n");
            x_msg_q_delete(_hACDetectMsgQue);
            _hACDetectMsgQue = NULL;
        }
    }
    
    // Create the AC detection handle semaphore
    VERIFY(x_sema_create(&_hACDetectSema, X_SEMA_TYPE_BINARY,
                         X_SEMA_STATE_LOCK) == OSR_OK);
    
    // Create the AC detect handle task
    if (x_thread_create(&_hACDetectHandle, "ACDetectHandle", 0x800,
                        u4Priority, CustomACDetectHandle, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create AC Detect thread fail\n");
        x_sema_delete(_hACDetectSema);
        _hACDetectSema = NULL;
        _bfgACDetectGo = FALSE;
    }
}

void CustomStopACDetect(void)
{
    // Wait AC_DET thread exit
    if (_bfgACDetectGo == TRUE)
    {
        _bfgACDetectGo = FALSE;
        VERIFY(x_sema_lock(_hACDetectSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        
        x_sema_delete(_hACDetectSema);
        _hACDetectSema = NULL;
        _hACDetectHandle = NULL;
    }
}


