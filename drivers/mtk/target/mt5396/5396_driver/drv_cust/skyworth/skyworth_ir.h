#ifndef MTK_IR_H
#define MTK_IR_H

#include "u_irrc_btn_def.h"
#include "skyworth/mtk_ir_nec_tbl.h"
#include "skyworth/mtk_ir_rc5_func_tbl.h"

#ifndef KEY_ARRAY
#define KEY_ARRAY
static const KEYARRAY_T _arKeyArray[] =
{
    { 0xe31c, (sizeof(_arMtkTvBigKeyMap)/sizeof(KEYUNIT_T)), _arMtkTvBigKeyMap },
    { 0x0e0e, (sizeof(_arSkyworthKeyMap)/sizeof(KEYUNIT_T)), _arSkyworthKeyMap },
};
#endif /* KEY_ARRAY */

static const WAKEUPKEYUNIT_T _arWakeupKeyMap[] =
{
    { 0x00, BTN_RED, WAKEUP_KEY_TYPE_NEC },
    { 0x4e, BTN_PRG_DOWN, WAKEUP_KEY_TYPE_NEC},
    { 0x55, BTN_PRG_UP, WAKEUP_KEY_TYPE_NEC}    
};

static const WAKEUPKEYARRAY_T _arWakeupKeyMapArray[] =
{
    { 0xe31c, (sizeof(_arWakeupKeyMap)/sizeof(WAKEUPKEYUNIT_T)), _arWakeupKeyMap }  /* Only command with system code 0 is allowed to wakeup */  
};

//----------------------------------------------------------------------------
// IRRX customization
//----------------------------------------------------------------------------
#define DEFAULT_IRRX_TYPE           (IRRX_NEC_TYPE)
#define IRRX_NEC_KEYMAP_ARRAY       (_arKeyArray)
#define IRRX_NEC_MAPARRAY_SIZE      (sizeof(_arKeyArray)/sizeof(KEYARRAY_T))
#define IRRX_RC5_KEYMAP_ARRAY       (_arWakeupKeyMapArray)
#define IRRX_RC5_KEYMAP_SIZE        (sizeof(_arWakeupKeyMapArray)/sizeof(KEYUNIT_T))

#endif /* MTK_IR_H */
