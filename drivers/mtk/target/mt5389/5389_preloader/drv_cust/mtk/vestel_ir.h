
#ifndef VESTEL_IR_H
#define VESTEL_IR_H

#include "u_irrc_btn_def.h"

#include "mtk/vestel_ir_map_multiRC.h"

//----------------------------------------------------------------------------
// RC5/6 IRRX key table
//----------------------------------------------------------------------------
#ifndef KEY_ARRAY
#define KEY_ARRAY

#ifndef NDEBUG  // Debug version, add mtk ir.
#include "mtk/mtk_ir_nec_tbl.h"

static const KEYARRAY_T _arKeyArray[] =
{
    { 0x9f20, (sizeof(_arMtkTvKeyMap)/sizeof(KEYUNIT_T)), _arMtkTvKeyMap },
    { 0xe31c, (sizeof(_arMtkTvBigKeyMap)/sizeof(KEYUNIT_T)), _arMtkTvBigKeyMap },
};

#define IRRX_NEC_KEYMAP_ARRAY       (_arKeyArray)
#define IRRX_NEC_MAPARRAY_SIZE      (sizeof(_arKeyArray)/sizeof(KEYARRAY_T))

#else // Release version.

#define IRRX_NEC_KEYMAP_ARRAY       (NULL)
#define IRRX_NEC_MAPARRAY_SIZE      (0)

#endif /* NDEBUG */

#endif /* KEY_ARRAY */

//----------------------------------------------------------------------------
// IRRX customization
//----------------------------------------------------------------------------
#define DEFAULT_IRRX_TYPE           (IRRX_RC5_TYPE)

static const WAKEUPKEYUNIT_T _arWakeupKeyMap[] =
{
    { 12, BTN_POWER, WAKEUP_KEY_TYPE_RC5_RC6},         /* 12, power button is common for RC5/RC6 wakeup */
//    { 32, BTN_PRG_UP, WAKEUP_KEY_TYPE_RC5_RC6},         /* 32, ch_up button is only for RC5 wakeup */    
//    { 33, BTN_PRG_DOWN, WAKEUP_KEY_TYPE_RC5}         /* 33, ch_down button is only for RC5 wakeup */        
};

static const WAKEUPKEYARRAY_T _arRC56KeyMapArray[] =
{
    { 0, (sizeof(_arWakeupKeyMap)/sizeof(WAKEUPKEYUNIT_T)), _arWakeupKeyMap },
    { 25, (sizeof(_arWakeupKeyMap)/sizeof(WAKEUPKEYUNIT_T)), _arWakeupKeyMap },  // MultiRC support
    { 18, (sizeof(_arWakeupKeyMap)/sizeof(WAKEUPKEYUNIT_T)), _arWakeupKeyMap }, // MultiRC support
    { 13, (sizeof(_arWakeupKeyMap)/sizeof(WAKEUPKEYUNIT_T)), _arWakeupKeyMap }  // MultiRC support
};

#define IRRX_RC5_KEYMAP_ARRAY       (_arRC56KeyMapArray)
#define IRRX_RC5_KEYMAP_SIZE        (sizeof(_arRC56KeyMapArray)/sizeof(WAKEUPKEYARRAY_T))

#endif /* VESTEL_IR_H */

