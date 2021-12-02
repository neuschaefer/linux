
#ifndef WAFFLE_BUTTON_H
#define WAFFLE_BUTTON_H

static MULTI_KEY_T _arWistronMultiKeyList[] = {
    { BTN_FACTORY_MODE_1,   (DKBIT_VOL_DOWN | DKBIT_INPUT_SRC) },
    { BTN_FUNCTION_1,       (DKBIT_VOL_DOWN | DKBIT_POWER) },
    { BTN_NONE,             (DKBIT_NONE) },
};

static SGL_SERVO_T _arWistronSingleServoKey[] = {
    // Servo,       Top,    Btm,    Key
    {   SERVO(7),   56,    48,    DKBIT_MENU      },
    {   SERVO(7),   46,    37,    DKBIT_INPUT_SRC },
    {   SERVO(7),   35,    26,    DKBIT_VOL_UP    },
    {   SERVO(7),   24,    16,    DKBIT_VOL_DOWN  },
    {   SERVO(7),   14,     6,    DKBIT_PRG_UP    },
    {   SERVO(7),    4,     0,    DKBIT_PRG_DOWN  },
    {   0,          0,      0,    BTN_NONE        }
};

static SGL_GPIO_T _arWistronSingleGpioKey[] = {
    // GPIO,       Btm,    Key
    {   OPCTRL(11), 0,    DKBIT_POWER },
    {   0,         0,    DKBIT_NONE  },
};

#define SINGLE_GPIO_KEY_LIST		(_arWistronSingleGpioKey)

#define SINGLE_SERVO_KEY_LIST       (_arWistronSingleServoKey)
#define MULTI_KEY_LIST              (_arWistronMultiKeyList)

#endif /* WAFFLE_BUTTON_H */
