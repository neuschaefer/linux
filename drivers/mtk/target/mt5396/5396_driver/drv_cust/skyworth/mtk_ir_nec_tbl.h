
#ifndef MTK_IR_NEC_H
#define MTK_IR_NEC_H

#define BTN_FAC BTN_CUSTOM_2
#define BTN_BARCODE BTN_CUSTOM_4
#define BTN_RESET BTN_CUSTOM_5
#define BTN_AGINGMODE BTN_CUSTOM_6
#define BTN_ADC BTN_CUSTOM_7
#define BTN_BUS BTN_CUSTOM_8
#define BTN_OUTSET BTN_CUSTOM_9
#define BTN_SOURCE_DOWN BTN_CUSTOM_12
#define BTN_SOURCE_UP BTN_CUSTOM_13
#define BTN_DREAMPANEL BTN_CUSTOM_14  //eva 100825

#define BTN_SCAN_DOWN BTN_CUSTOM_15//eva add 100830
#define BTN_SCAN_UP BTN_CUSTOM_16
#define BTN_WB_PAGE BTN_CUSTOM_17
#define BTN_NETWOEK BTN_CUSTOM_18
#define BTN_UPLAYER BTN_CUSTOM_19
#define BTN_COOCAA BTN_CUSTOM_20
#define BTN_HDMI3 BTN_CUSTOM_21
#define BTN_HDMI2 BTN_CUSTOM_22
#define BTN_HDMI1 BTN_CUSTOM_23
#define BTN_ENTER_VGA BTN_CUSTOM_24
#define BTN_YPBPR2 BTN_CUSTOM_25
#define BTN_YPBPR1 BTN_CUSTOM_26
#define BTN_S1 BTN_CUSTOM_27
#define BTN_AV3 BTN_CUSTOM_28
#define BTN_AV2 BTN_CUSTOM_29
#define BTN_AGC BTN_CUSTOM_30
#define BTN_AV1 BTN_CUSTOM_31

// The PIONEER TV controller IRRX key map
#if 0//LX_suning_110314 added from kk 110311 add 
static const KEYUNIT_T _arPioneerKeyMap[] =
{
    { 0x00, BTN_POWER },
    { 0x02, BTN_FR },
    { 0x03, BTN_DIGIT_0 },
    { 0x04, BTN_DIGIT_1 },
    { 0x05, BTN_DIGIT_2 },
    { 0x07, BTN_DIGIT_4 },
    { 0x08, BTN_DIGIT_5 },
    { 0x09, BTN_DIGIT_6 },
    { 0x0a, BTN_DIGIT_7 },
    { 0x0b, BTN_DIGIT_8 },
    { 0x0c, BTN_DIGIT_9 },
    { 0x0d, BTN_CUSTOM_11 },
    { 0x0e, BTN_STOP},
    { 0x0f, BTN_PREV_PRG},
    { 0x10, BTN_INPUT_SRC },
    { 0x11, BTN_AV1 },
    { 0x12, BTN_PREV },
    { 0x13, BTN_NEXT },
    { 0x14, BTN_PRG_DOWN},
    { 0x15, BTN_VOL_DOWN },
    { 0x16, BTN_PLAY_PAUSE},
    { 0x17, BTN_FF },
    { 0x18, BTN_MENU },
    { 0x19, BTN_VOL_UP},
    { 0x1b, BTN_SLEEP},
    { 0x1c, BTN_CURSOR_LEFT},
    { 0x1d, BTN_CURSOR_DOWN},
    { 0x42, BTN_EXIT },
    { 0x43, BTN_HDMI1},
    { 0x44, BTN_CURSOR_UP},
    { 0x45, BTN_VGA},
    { 0x47, BTN_MUTE },
    { 0x48, BTN_CURSOR_RIGHT},
    { 0x49, BTN_TV},
    { 0x4f, BTN_FREEZE},
    { 0x51, BTN_SELECT},
    { 0x53, BTN_P_EFFECT },
    { 0x54, BTN_PRG_INFO },
    { 0x55, BTN_S_EFFECT },
    { 0x56, BTN_YPBPR1},
    { 0x58, BTN_ZOOM }, 
    { 0x59, BTN_MEM_CARD }, 
    { 0x5c, BTN_PRG_UP }, 
    { 0x5f, BTN_DIGIT_3 },
    
};
#endif
// The MTK TV controller IRRX key map
static const KEYUNIT_T _arMtkTvBigKeyMap[] =
{
    { 0x00, BTN_RED },
    { 0x01, BTN_GREEN },
    { 0x02, BTN_POWER },
    { 0x03, BTN_YELLOW },
    { 0x04, BTN_ZOOM },
    { 0x05, BTN_BLUE },
    { 0x06, BTN_TIMER },
    { 0x07, BTN_MEM_CARD },
    { 0x08, BTN_DIGIT_3 },
    { 0x09, BTN_DIGIT_2 },
    { 0x0a, BTN_DIGIT_1 },
    { 0x0b, BTN_SELECT },       // ENTER
    { 0x0c, BTN_DIGIT_9 },
    { 0x0d, BTN_DIGIT_8 },
    { 0x0e, BTN_DIGIT_7 },
    { 0x0f, BTN_FAV_CH },       // STOP, FAV_CH
    { 0x10, BTN_DIGIT_6 },
    { 0x11, BTN_DIGIT_5 },
    { 0x12, BTN_DIGIT_4 },
    { 0x13, BTN_CURSOR_DOWN },
    { 0x14, BTN_MENU },
    { 0x15, BTN_DIGIT_0 },
    { 0x16, BTN_FREEZE },
    { 0x17, BTN_RECORD },       // RECORD, CAPTURE
    { 0x18, BTN_PRG_INFO },     // INFO
    { 0x19, BTN_EPG },          // GUIDE
    { 0x1a, BTN_EXIT },
    { 0x1b, BTN_PREV },         // DVD previous, REVEAL
    { 0x1c, BTN_CURSOR_RIGHT },
    { 0x1d, BTN_CURSOR_UP },
    { 0x1e, BTN_CURSOR_LEFT },
    { 0x1f, BTN_P_EFFECT },     // P.EFFECT
    { 0x40, BTN_S_EFFECT },     // S.EFFECT
    { 0x42, BTN_PIP_SIZE },
    { 0x43, BTN_MTS },          // MTS/Audio
    { 0x44, BTN_REPEAT },       // DVD REPEAT, WIDE MODE
    { 0x45, BTN_SUB_TITLE },    // SUB_TITLE
    { 0x46, BTN_TITLE_PBC },    // DVD TITLE/PBC, MIX
    { 0x47, BTN_SWAP },
    { 0x48, BTN_FAVORITE },     // DVD PLAY_PAUSE, Favorite
    { 0x49, BTN_ADD_ERASE },    // DVD Eject, ADD_ERASE
    { 0x4a, BTN_NEXT },         // DVD next, SIZE
    { 0x4c, BTN_VOL_UP },
    { 0x4d, BTN_FR },           // DVD backward, TTX
    { 0x4e, BTN_PRG_DOWN },
    { 0x50, BTN_FF },           // DVD forward, SUBPAGE
    { 0x54, BTN_VOL_DOWN },
    { 0x55, BTN_PRG_UP },
    { 0x56, BTN_MUTE },
    { 0x57, BTN_PREV_PRG },
    { 0x58, BTN_PIP_POP },      // PIP/POP
    { 0x59, BTN_INPUT_SRC },    // Source
    { 0x5a, BTN_DIGIT_DOT },
    { 0x5b, BTN_ROOT_MENU },    // DVD Root menu, INDEX
    { 0x5c, BTN_PIP_POS },      // PIP POS
    { 0x5d, BTN_ASPECT },
    { 0x5e, BTN_CC },
    { 0x5f, BTN_SLEEP },
};


// The MTK TV controller IRRX key map
static const KEYUNIT_T _arSkyworthKeyMap[] =
{
    { 0x00, BTN_DIGIT_0 },        	/* 0x00, Num0 */
    { 0x01, BTN_DIGIT_1 },        	/* 0x01, Num1 */
    { 0x02, BTN_DIGIT_2 },         /* 0x02, Num2 */
    { 0x03, BTN_DIGIT_3 },       	/* 0x03, Num3 */
    { 0x04, BTN_DIGIT_4 },         /* 0x04, Num4 */
    { 0x05, BTN_DIGIT_5 },         /* 0x05, Num5 */
    { 0x06, BTN_DIGIT_6},          /* 0x06, Num6 */
    { 0x07, BTN_DIGIT_7},         	/* 0x07, Num7 */
    { 0x08, BTN_DIGIT_8 },         /* 0x08, Num8 */
    { 0x09, BTN_DIGIT_9 },         /* 0x09, Num9 */
    { 0x0a, BTN_CUSTOM_11},           /* 0x0a, Active Control */
    { 0x0b, BTN_PREV_PRG },          	/* 0x0b, Num7 */
    { 0x0c, BTN_POWER},           /* 0x0c, Num5 */
    { 0x0d, BTN_MUTE },           	/* 0x0d, Mute */
    { 0x0e, BTN_NONE },            /* 0x0e, None */
    { 0x0f, BTN_INPUT_SRC},   	/* 0x0f, Input Source */
 
    { 0x10, BTN_NONE },           	/* 0x10, None */
    { 0x11, BTN_MENU },           	/* 0x11, Menu */
    { 0x12, BTN_PRG_UP},          	/* 0x12, Channel Up */
    { 0x13, BTN_PRG_DOWN},     	/* 0x13, Channel Down */
    { 0x14, BTN_VOL_UP},         	/* 0x14, Volume Up */
    { 0x15, BTN_VOL_DOWN},   	/* 0x15, Volume Down */
    { 0x16, BTN_PRG_INFO},     	/* 0x16, OSD display */
    { 0x17, BTN_P_EFFECT},     	/* 0x17, Picture effect */
    { 0x18, BTN_S_EFFECT},   	/* 0x18, Sound effect */
    { 0x19, BTN_NONE},            	/* 0x19, None */
    { 0x1a, BTN_FREEZE},          	/* 0x1a, Freeze */
    { 0x1b, BTN_NONE},           	/* 0x1b, None */
    { 0x1c, BTN_CUSTOM_1},   		/* 0x1c, VII */ //should be modified, no function
    { 0x1d, BTN_NONE},        	/* 0x1d, None */
    { 0x1e, BTN_NONE},         	/* 0x1e, None */
    { 0x1f, BTN_NONE},          	/* 0x1f, None */

    { 0x20, BTN_SCAN_DOWN},
    { 0x21, BTN_SCAN_UP},
    { 0x22, BTN_BARCODE},             	/* 0x22, barcode *///MTK_SZ_BAR_CODE
    { 0x28, BTN_WB_PAGE},
    { 0x29, BTN_DREAMPANEL},         /*0X29 dreampanel*/

    { 0x2A, BTN_NETWOEK},
    { 0x2B, BTN_UPLAYER},
    { 0x2C, BTN_COOCAA},
    { 0x2D, BTN_HDMI3},
    { 0x2E, BTN_HDMI2},
    { 0x2F, BTN_HDMI1},
    { 0x30, BTN_ENTER_VGA},
    { 0x31, BTN_YPBPR2},
    { 0x32, BTN_YPBPR1},
    { 0x33, BTN_S1},
    { 0x34, BTN_AV3},
    { 0x35, BTN_AV2},
    { 0x36, BTN_AGC},
    { 0x37, BTN_AV1},           
    { 0x38, BTN_ADC},                     /*0X39,ADC *///eva 100811
    { 0x39, BTN_AGINGMODE},          /*0X39,agingmode */
    { 0x3A, BTN_BUS},                     /*0X3A, bus */
    { 0x3B, BTN_OUTSET},                /*0X3b,outset */
    { 0x3C, BTN_SOURCE_DOWN},      /*0X3C,source down */
    { 0x3D, BTN_SOURCE_UP},           /*0X3D,source up */
    { 0x3e, BTN_RESET}, 
    { 0x3f, BTN_FAC},             	/* 0x3f, fac *///MTK_SKY_FACTORY_KEY_FAC//eva v28 100519
        
    { 0x40, BTN_FR},             	/* 0x40, Sleep */
    { 0x41, BTN_FF},              	/* 0x41, Zoom */
    { 0x42, BTN_CURSOR_UP},   	/* 0x42, Navi Up */
    { 0x43, BTN_CURSOR_DOWN}, 	/* 0x43, Navi Down */
    { 0x44, BTN_CURSOR_LEFT},        /* 0x44, Navi Left */
    { 0x45, BTN_CURSOR_RIGHT},  	/* 0x45, Navi right */
    { 0x46, BTN_SELECT},  		/* 0x45, "ENTER" confirm */
    { 0x47, BTN_PREV},             	/* 0x47, Previous */
    { 0x48, BTN_NEXT},       	/* 0x48, Next */
    { 0x49, BTN_FAVORITE},   	/* 0x49, Play & pause *///BTN_FAVORITE,BTN_PLAY_PAUSE
    { 0x4a, BTN_FAV_CH},   		/* 0x49, Stop *///BTN_FAV_CH,BTN_STOP
    { 0x4b, BTN_EJECT},          	/* 0x4b, Record */
    { 0x4c, BTN_NONE},             /* 0x4c, None */
    { 0x4d, BTN_MEM_CARD},    	/* 0x4d, CooCaa *///Multimedia
    { 0x4f, BTN_ASPECT},          /* 0x4f, Display Mode */
        
    { 0x50, BTN_NONE},         	/* 0x50, ADD/Erase */
    { 0x51, BTN_DISPLAY},       	/* 0x51, Screen Display *///should be modified
    { 0x53, BTN_CUSTOM_10},         /*Uplayer\TV */   
    { 0x54, BTN_ECHO_MINUS},         /*Org voice */      
    { 0x55, BTN_VRMT},            	/* 0x55, Reserve */
    { 0x56, BTN_KARAOKE},            	/* 0x55, Diange */  
    { 0x57, BTN_SURROUND},           	/* 0x54, VOICE */    
    { 0x58, BTN_PROGRAM},          	/* 0x58, Function */
    { 0x59, BTN_DIGEST},          	/* 0x59, Pingfen */
    { 0x5a, BTN_TITLE_PBC},          	/* 0x5a, yidian gequ*/ 
    { 0x5b, BTN_EXIT},          	/* 0x5b, Return/Exit */
    { 0x5c, BTN_RANDOM},            	/* 0x5c, Favorite */
    { 0x5d, BTN_ANGLE},          	/* 0x5d, None */
    { 0x5e, BTN_ECHO_PLUS},          	/* 0x5e, youxian  */  
    { 0x5f, BTN_HOME_DLIST},          	/* 0x5f, shanchu  */  
    { 0x60, BTN_EPG },              /* 0x4b, Clock */
    { 0x61, BTN_CLEAR},              /* 0x4b, Clock */
    
};
#endif /* MTK_IR_NEC_H */

