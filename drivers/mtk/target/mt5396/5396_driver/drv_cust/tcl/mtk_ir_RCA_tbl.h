
#ifndef MTK_IR_NEC_H
#define MTK_IR_NEC_H

// The MTK TV controller IRRX key map
static const KEYUNIT_T _arMtkTvKeyMap[] =
{
    { 0x00, BTN_GREEN },            /* 0x00, Green Key */
    { 0x01, BTN_YELLOW },           /* 0x01, Yellow Key */
//    { 0x02, BTN_SMARTPIC },         /* 0x02, Smart PIC */
    { 0x03, BTN_DIGIT_0 },          /* 0x03, Num0 */
    { 0x04, BTN_DIGIT_DOT },        /* 0x04, Blank button */
    { 0x05, BTN_TTX },              /* 0x05, TT/TV */
    { 0x06, BTN_MENU },             /* 0x06, Menu */
    { 0x07, BTN_MUTE },             /* 0x07, Mute */
    { 0x08, BTN_DIGIT_8 },          /* 0x08, Num8 */
    { 0x09, BTN_DIGIT_9 },          /* 0x09, Num9 */
    { 0x0a, BTN_ASPECT },           /* 0x0a, Active Control */
    { 0x0b, BTN_DIGIT_7 },          /* 0x0b, Num7 */
    { 0x0c, BTN_DIGIT_5 },          /* 0x0c, Num5 */
    { 0x0d, BTN_DIGIT_6 },          /* 0x0d, Num6 */
    { 0x0e, BTN_POWER },            /* 0x0e, Power */
    { 0x0f, BTN_DIGIT_4 },          /* 0x0f, Num4 */
    { 0x10, BTN_DIGIT_2 },          /* 0x10, Num2 */
    { 0x11, BTN_DIGIT_3 },          /* 0x11, Num3 */
    { 0x12, BTN_POP },              /* 0x12, POP */
    { 0x13, BTN_DIGIT_1 },          /* 0x13, Num1 */
    { 0x14, BTN_RED },              /* 0x14, Red Key */
    { 0x15, BTN_CC },               /* 0x15, CC */
    { 0x16, BTN_PIP_POS },          /* 0x16, PIP POS */
    { 0x17, BTN_TV },               /* 0x17, Color System */
    { 0x18, BTN_PREV_PRG },         /* 0x18, Alternate Channel */
    { 0x19, BTN_FAV_CH },           /* 0x19, Favorite Channel */
    { 0x1a, BTN_FAVORITE },         /* 0x1a, Favorite */
//    { 0x1b, BTN_CAPTURE },          /* 0x1b, Capture */
    { 0x1c, BTN_CURSOR_LEFT },      /* 0x1c, Left Key */
    { 0x1d, BTN_CURSOR_DOWN },      /* 0x1d, Down Key */
    { 0x1e, BTN_INPUT_SRC },        /* 0x1e, Input */
    { 0x1f, BTN_PRG_INFO },         /* 0x1f, OSD */
    { 0x40, BTN_SLEEP },            /* 0x40, Sleep */
    { 0x41, BTN_ZOOM },             /* 0x41, Zoom */
    { 0x42, BTN_BLUE },             /* 0x42, Blue Key */
    { 0x43, BTN_EXIT },             /* 0x43, CE */
    { 0x44, BTN_CURSOR_UP },        /* 0x44, Up Key */
    { 0x45, BTN_VOL_DOWN },         /* 0x45, Volume Down */
    { 0x47, BTN_INDEX },            /* 0x47, Index */
    { 0x48, BTN_CURSOR_RIGHT },     /* 0x48, Right Key */
    { 0x49, BTN_VOL_UP },           /* 0x49, Volume Up */
    { 0x4b, BTN_EPG },              /* 0x4b, Clock */
    { 0x4c, BTN_TIMER },            /* 0x4c, Timer */
    { 0x4d, BTN_PRG_DOWN },         /* 0x4d, Channel Down */
    { 0x4f, BTN_MEM_CARD },         /* 0x4f, Update */
    { 0x50, BTN_ADD_ERASE },        /* 0x50, ADD/Erase */
    { 0x51, BTN_PRG_UP },           /* 0x51, Channel Up */
    { 0x54, BTN_FREEZE },           /* 0x54, Freeze */
//    { 0x55, BTN_VCHIP  },           /* 0x55, V-Chip */
    { 0x58, BTN_PIP_SIZE },         /* 0x58, PIC SIZE */
    { 0x59, BTN_S_EFFECT },         /* 0x59, Sound Effect */
    { 0x5c, BTN_SELECT },           /* 0x5c, Enter Key */
    { 0x5d, BTN_MTS }               /* 0x5d, MTS/Lang */
};

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

#ifdef SUPPORT_RC_RCA 
static const KEYUNIT_T _arRCATvKeyMap[] =
{
	{ 0x0B, BTN_SELECT }, /* OK */
	{ 0x0F, BTN_CUSTOM_2}, /*Map to ECO*/
	{ 0x13, BTN_CUSTOM_1}, /*Map to Option*/
	{ 0x17, BTN_GREEN }, 
	{ 0x18, BTN_IR_MOUSE_MOVE},  
	{ 0x1B, BTN_YELLOW },  
	{ 0x27, BTN_BLUE },
	{ 0x34, BTN_IR_MOUSE_MOVE}, 
	{ 0x51, BTN_MEM_CARD},//BTN_DGT_MEDIA},	
	{ 0x55, BTN_IR_MOUSE_MOVE},       	
    { 0x56, BTN_IR_MOUSE_MOVE},           	
    { 0x57, BTN_IR_MOUSE_MOVE},            
    { 0x58, BTN_IR_MOUSE_MOVE},
	{ 0x5C, BTN_INPUT_SRC},     
	{ 0x5F, BTN_ROOT_MENU},  //T-LINK //pwh change  
	{ 0x62, BTN_CUSTOM_10},
	{ 0x7F, BTN_SUB_TITLE},/*Map to subtitle*/
	{ 0x87, BTN_IR_MOUSE_MOVE},
	{ 0x9e, BTN_CH_LIST },      
    { 0xA6, BTN_CURSOR_UP },
	{ 0xA7, BTN_CURSOR_DOWN },
	{ 0xA8, BTN_CURSOR_RIGHT },
	{ 0xA9, BTN_CURSOR_LEFT },
	{ 0xC0, BTN_MUTE },
	{ 0xC3, BTN_PRG_INFO }, /* info */ 		
	{ 0xC5 , BTN_CUSTOM_3}, /*Map to TV*/		
	{ 0xC6, BTN_DIGIT_9 },
	{ 0xC7, BTN_DIGIT_8 },
	{ 0xC8, BTN_DIGIT_7 },
	{ 0xC9, BTN_DIGIT_6 },
	{ 0xCA, BTN_DIGIT_5 },
	{ 0xCB, BTN_DIGIT_4 },
	{ 0xCC, BTN_DIGIT_3 },
	{ 0xCD, BTN_DIGIT_2 },
	{ 0xCE, BTN_DIGIT_1 },
	{ 0xCF, BTN_DIGIT_0 },
	{ 0xD0, BTN_VOL_DOWN },
	{ 0xD1, BTN_VOL_UP },
	{ 0xD2, BTN_PRG_UP },
	{ 0xD3, BTN_PRG_DOWN },
	{ 0xD5, BTN_POWER },
    { 0xD8, BTN_PREV_PRG}, /*Map to RETURN */	
//	{ 0xDD, BTN_TTX_STOP },
//	{ 0xDF, BTN_TTX_HELP },
	{ 0xE0, BTN_STOP },
	{ 0xE1, BTN_FR }, /* Map to Text,function is for teletext */  
	{ 0xE2, BTN_PREV },
	{ 0xE3, BTN_NEXT },
	{ 0xE5, BTN_EPG },//guide
	{ 0xE6, BTN_PAUSE },
	{ 0xE8, BTN_RECORD},
//	{ 0xE9, BTN_TTX_ZOOM}, //BTN_TTX_ZOOM },
	{ 0xEA, BTN_PLAY},
//	{ 0xED, BTN_PRESETS },	
	{ 0xF7, BTN_MENU }, 
    { 0xF9, BTN_EXIT },
	{ 0xFA, BTN_IR_MOUSE_MOVE}, 
	{ 0xFD, BTN_FAVORITE }, /* Fav, Sub. Code */
	{ 0xFF, BTN_RED },	             
};

#endif


#endif /* MTK_IR_NEC_H */

