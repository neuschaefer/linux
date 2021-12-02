
#ifndef PHILIPS_LC09M_IR_MAP_H
#define PHILIPS_LC09M_IR_MAP_H

#ifndef BTN_POWER_ON
#define BTN_POWER_ON            ((UINT32)(BTN_POWER | 0x0000f005))
#endif /* BTN_POWER_ON */
#ifndef BTN_POWER_OFF
#define BTN_POWER_OFF           ((UINT32)(BTN_POWER | 0x0000f006))
#endif /* BTN_POWER_OFF */

// BTN_RED => BTN_PRG_INFO
// BTN_GREEN => BTN_FAVORITE
// BTN_YELLOW => BTN_ADD_ERASE
// BTN_BLUE => BTN_ASPECT/BTN_TIMER

#define GRP_TV1     0
#define GRP_CUSTOM  7
#define GRP_10      10  // The same as GRP_TV1 command

#if defined(IRRX_RC6_KEYMAP_FUNC)
#undef IRRX_RC6_KEYMAP_FUNC
#endif
#define IRRX_RC6_KEYMAP_FUNC (SGP_rc6_ir2crystal)

DECLARE_MAP_BEGIN(SGP_rc6_ir2crystal)
    MAP_GROUP_AS_BELOW(13)
    MAP_GROUP_AS_BELOW(18)
    MAP_GROUP_AS_BELOW(25)
    MAP_GROUP_BEGIN(GRP_TV1)
        MAP_INSERT(  0,     BTN_DIGIT_0,        "0")
        MAP_INSERT(  1,     BTN_DIGIT_1,        "1")
        MAP_INSERT(  2,     BTN_DIGIT_2,        "2")
        MAP_INSERT(  3,     BTN_DIGIT_3,        "3")
        MAP_INSERT(  4,     BTN_DIGIT_4,        "4")
        MAP_INSERT(  5,     BTN_DIGIT_5,        "5")
        MAP_INSERT(  6,     BTN_DIGIT_6,        "6")
        MAP_INSERT(  7,     BTN_DIGIT_7,        "7")
        MAP_INSERT(  8,     BTN_DIGIT_8,        "8")
        MAP_INSERT(  9,     BTN_DIGIT_9,        "9")
        MAP_INSERT( 10,     BTN_EXIT,           "back/prev ch")
        MAP_INSERT( 12,     BTN_POWER,          "power")
        MAP_INSERT( 13,     BTN_MUTE,           "mute")
        MAP_INSERT( 15,     BTN_PRG_INFO,       "Display Info +")
        MAP_INSERT( 16,     BTN_VOL_UP,         "vol +")
        MAP_INSERT( 17,     BTN_VOL_DOWN,       "vol -")
        MAP_INSERT( 32,     BTN_PRG_UP,         "ch +")
        MAP_INSERT( 33,     BTN_PRG_DOWN,       "ch -")
        MAP_INSERT( 38,     BTN_SLEEP,          "sleep")
        MAP_INSERT( 40,     BTN_FF,             "FF")
        MAP_INSERT( 43,     BTN_FR,             "FR")
        MAP_INSERT( 44,     BTN_PLAY,           "play/pause")
        MAP_INSERT( 49,     BTN_STOP,           "stop")
        MAP_INSERT( 55,     BTN_RECORD,         "RECORD")
        MAP_INSERT( 56,     BTN_INPUT_SRC,      "source")
        MAP_INSERT( 57,     BTN_INP_SRC_2,      "External 2")
        MAP_INSERT( 58,     BTN_CC,             "cc")
        MAP_INSERT( 60,     BTN_TTX,            "teletext")
        MAP_INSERT( 61,     BTN_POWER_OFF,      "power off")
        MAP_INSERT( 63,     BTN_POWER_ON,       "power on")
        MAP_INSERT( 64,     BTN_CUSTOM_6,       "custom_6/option")
        MAP_INSERT( 69,     BTN_CUSTOM_20,      "Incredible surround")
        MAP_INSERT( 70,     BTN_CC,             "Closed caption")
        MAP_INSERT( 71,     BTN_CUSTOM_8,       "Ambilight on")
        MAP_INSERT( 73,     BTN_CUSTOM_9,       "Ambilight mode")
        MAP_INSERT( 75,     BTN_SUB_TITLE,      "subtitle")
        MAP_INSERT( 78,     BTN_MTS,            "MTS: SAP on/off")
        MAP_INSERT( 79,     BTN_CUSTOM_20,      "incredible surround")
        MAP_INSERT( 84,     BTN_MENU,           "menu/exit/browse")
        MAP_INSERT( 88,     BTN_CURSOR_UP,      "cursor up")
        MAP_INSERT( 89,     BTN_CURSOR_DOWN,    "cursor down")
        MAP_INSERT( 90,     BTN_CURSOR_LEFT,    "cursor left")
        MAP_INSERT( 91,     BTN_CURSOR_RIGHT,   "cursor right")
        MAP_INSERT( 92,     BTN_SELECT,         "ok")
        MAP_INSERT( 93,     BTN_PIP_POP,        "pip pop/dual screen")
        MAP_INSERT( 97,     BTN_CUSTOM_7, "custom7/active ctrl") 
        MAP_INSERT(104,     BTN_CUSTOM_41,      "scenea (not color key)")
        MAP_INSERT(109,     BTN_RED,            "red/demo")
        MAP_INSERT(110,     BTN_GREEN,          "green")
        MAP_INSERT(111,     BTN_YELLOW,         "yellow")
        MAP_INSERT(112,     BTN_BLUE,           "blue")
        MAP_INSERT(121,     BTN_FAVORITE, "favorite")
        MAP_INSERT(127,     BTN_TIMER,          "clock")
        MAP_INSERT(143,     BTN_CUSTOM_8,       "Ambilight on")
        MAP_INSERT(144,     BTN_CUSTOM_9,       "Ambilight mode")
        MAP_INSERT(159,     BTN_TV,             "TV source hotkey")
        MAP_INSERT(192,     BTN_CUSTOM_52,      "An/Dig(bolt-on")
        MAP_INSERT(193,     BTN_CUSTOM_51,      "Digital menu(bolt-on")
        MAP_INSERT(195,     BTN_CUSTOM_10,      "battery low detection")
        MAP_INSERT(204,     BTN_EPG,            "epg/guide")
        MAP_INSERT(210,     BTN_CUSTOM_62,      "view")
        MAP_INSERT(211,     BTN_MARK,           "mark")
        MAP_INSERT(217,     BTN_DIGIT_DOT,      "Digit dot")
        MAP_INSERT(243,     BTN_P_EFFECT,       "smart picture")
        MAP_INSERT(244,     BTN_S_EFFECT,       "smart sound")
        MAP_INSERT(245,     BTN_ASPECT,         "format")
        MAP_INSERT(250,     BTN_CUSTOM_40,      "demo (not color key)")
    MAP_GROUP_END()

    // Smart keys - 3
    MAP_GROUP_AS_BELOW(12)
    MAP_GROUP_AS_BELOW(14)
    MAP_GROUP_AS_BELOW(31)
    MAP_GROUP_BEGIN(3)
        MAP_INSERT(  4,     BTN_INP_SRC_1,      "External 1")
        MAP_INSERT(  6,     BTN_INP_SRC_6,      "External 6")
        MAP_INSERT(  7,     BTN_INP_SRC_7,      "External 7")
        MAP_INSERT(  8,     BTN_INP_SRC_8,      "External 8")
        MAP_INSERT( 10,     BTN_P_EFFECT,       "smart picture")
        MAP_INSERT( 11,     BTN_S_EFFECT,       "smart sound")
        MAP_INSERT( 35,     BTN_CUSTOM_8,       "custom8/ambilight onoff")
        MAP_INSERT( 36,     BTN_CUSTOM_9,       "custom9/ambilight mode")
        MAP_INSERT( 56,     BTN_INP_SRC_3,      "External 3")
        MAP_INSERT( 57,     BTN_INP_SRC_4,      "External 4")
        MAP_INSERT( 58,     BTN_COMPOSITE_2,    "Side AV")
        MAP_INSERT( 61,     BTN_CUSTOM_11,      "Auto format key")
        MAP_INSERT( 62,     BTN_CUSTOM_13,      "Movie Expand 16:9 key")
        MAP_INSERT( 80,     BTN_CUSTOM_14,      "Movie Expand 14:9 key")
        MAP_INSERT( 82,     BTN_CUSTOM_51,      "Digital menu(bolt-on")
        MAP_INSERT( 83,     BTN_CUSTOM_15,      "16:9 mode key")
        MAP_INSERT(116,     BTN_CUSTOM_12,      "Wide screen")
        MAP_INSERT(117,     BTN_CUSTOM_16,      "4:3 mode key")
        MAP_INSERT(118,     BTN_CUSTOM_52,      "An/Dig(bolt-on")
        MAP_INSERT(119,     BTN_TV,             "TV")
        MAP_INSERT(120,     BTN_DIGIT_DOT,      "dot")
        MAP_INSERT(123,     BTN_INP_SRC_5,      "External 5")
        MAP_INSERT(125,     BTN_CUSTOM_17,      "Fullscreen key")
        MAP_INSERT(126,     BTN_CUSTOM_18,      "Superzoom key")
    MAP_GROUP_END()

    // Custom keys
    MAP_GROUP_BEGIN(GRP_CUSTOM)
        MAP_INSERT(  0,     BTN_DIGIT_0,        "0")
        MAP_INSERT(  1,     BTN_DIGIT_1,        "1")
        MAP_INSERT(  2,     BTN_DIGIT_2,        "2")
        MAP_INSERT(  3,     BTN_DIGIT_3,        "3")
        MAP_INSERT(  4,     BTN_DIGIT_4,        "4")
        MAP_INSERT(  5,     BTN_DIGIT_5,        "5")
        MAP_INSERT(  6,     BTN_DIGIT_6,        "6")
        MAP_INSERT(  7,     BTN_DIGIT_7,        "7")
        MAP_INSERT(  8,     BTN_DIGIT_8,        "8")
        MAP_INSERT(  9,     BTN_DIGIT_9,        "9")
        MAP_INSERT( 12,     BTN_POWER,          "power")
        MAP_INSERT( 13,     BTN_MUTE,           "mute")
        MAP_INSERT( 16,     BTN_VOL_UP,         "vol +")
        MAP_INSERT( 17,     BTN_VOL_DOWN,       "vol -")
        MAP_INSERT( 28,     BTN_CURSOR_UP,      "cursor up")
        MAP_INSERT( 29,     BTN_CURSOR_DOWN,    "cursor down")
        MAP_INSERT( 32,     BTN_PRG_UP,         "ch +")
        MAP_INSERT( 33,     BTN_PRG_DOWN,       "ch -")
        MAP_INSERT( 34,     BTN_PREV_PRG,       "prev ch")
        MAP_INSERT( 38,     BTN_SLEEP,          "sleep")
        MAP_INSERT( 43,     BTN_CURSOR_RIGHT,   "cursor right")
        MAP_INSERT( 44,     BTN_CURSOR_LEFT,    "cursor left")
        MAP_INSERT( 46,     BTN_CUSTOM_1,       "menu")
    MAP_GROUP_END()

    // TV1 keys: The same as GRP_TV1 command
    MAP_GROUP_BEGIN(GRP_10)
        MAP_INSERT(  0,     BTN_DIGIT_0,        "0")
        MAP_INSERT(  1,     BTN_DIGIT_1,        "1")
        MAP_INSERT(  2,     BTN_DIGIT_2,        "2")
        MAP_INSERT(  3,     BTN_DIGIT_3,        "3")
        MAP_INSERT(  4,     BTN_DIGIT_4,        "4")
        MAP_INSERT(  5,     BTN_DIGIT_5,        "5")
        MAP_INSERT(  6,     BTN_DIGIT_6,        "6")
        MAP_INSERT(  7,     BTN_DIGIT_7,        "7")
        MAP_INSERT(  8,     BTN_DIGIT_8,        "8")
        MAP_INSERT(  9,     BTN_DIGIT_9,        "9")
        MAP_INSERT( 10,     BTN_EXIT,           "back/prev ch")
        MAP_INSERT( 12,     BTN_POWER,          "power")
        MAP_INSERT( 13,     BTN_MUTE,           "mute")
        MAP_INSERT( 15,     BTN_PRG_INFO,       "Display Info +")
        MAP_INSERT( 16,     BTN_VOL_UP,         "vol +")
        MAP_INSERT( 17,     BTN_VOL_DOWN,       "vol -")
        MAP_INSERT( 32,     BTN_PRG_UP,         "ch +")
        MAP_INSERT( 33,     BTN_PRG_DOWN,       "ch -")
        MAP_INSERT( 38,     BTN_SLEEP,          "sleep")
        MAP_INSERT( 40,     BTN_FF,             "FF")
        MAP_INSERT( 43,     BTN_FR,             "FR")
        MAP_INSERT( 44,     BTN_PLAY,           "play/pause")
        MAP_INSERT( 49,     BTN_STOP,           "stop")
        MAP_INSERT( 55,     BTN_RECORD,         "RECORD")
        MAP_INSERT( 56,     BTN_INPUT_SRC,      "source")
        MAP_INSERT( 57,     BTN_INP_SRC_2,      "External 2")
        MAP_INSERT( 58,     BTN_CC,             "cc")
        MAP_INSERT( 60,     BTN_TTX,            "teletext")
        MAP_INSERT( 61,     BTN_POWER_OFF,      "power off")
        MAP_INSERT( 63,     BTN_POWER_ON,       "power on")
        MAP_INSERT( 64,     BTN_CUSTOM_6,       "custom_6/option")
        MAP_INSERT( 69,     BTN_CUSTOM_20,      "Incredible surround")
        MAP_INSERT( 70,     BTN_CC,             "Closed caption")
        MAP_INSERT( 71,     BTN_CUSTOM_8,       "Ambilight on")
        MAP_INSERT( 73,     BTN_CUSTOM_9,       "Ambilight mode")
        MAP_INSERT( 75,     BTN_SUB_TITLE,      "subtitle")
        MAP_INSERT( 78,     BTN_MTS,            "MTS: SAP on/off")
        MAP_INSERT( 79,     BTN_CUSTOM_20,      "incredible surround")
        MAP_INSERT( 84,     BTN_MENU,           "menu/exit/browse")
        MAP_INSERT( 88,     BTN_CURSOR_UP,      "cursor up")
        MAP_INSERT( 89,     BTN_CURSOR_DOWN,    "cursor down")
        MAP_INSERT( 90,     BTN_CURSOR_LEFT,    "cursor left")
        MAP_INSERT( 91,     BTN_CURSOR_RIGHT,   "cursor right")
        MAP_INSERT( 92,     BTN_SELECT,         "ok")
        MAP_INSERT( 93,     BTN_PIP_POP,        "pip pop/dual screen")
        MAP_INSERT(104,     BTN_CUSTOM_41,      "scenea (not color key)")
        MAP_INSERT(109,     BTN_RED,            "red/demo")
        MAP_INSERT(110,     BTN_GREEN,          "green")
        MAP_INSERT(111,     BTN_YELLOW,         "yellow")
        MAP_INSERT(112,     BTN_BLUE,           "blue")
        MAP_INSERT(127,     BTN_TIMER,          "clock")
        MAP_INSERT(143,     BTN_CUSTOM_8,       "Ambilight on")
        MAP_INSERT(144,     BTN_CUSTOM_9,       "Ambilight mode")
        MAP_INSERT(159,     BTN_TV,             "TV source hotkey")
        MAP_INSERT(192,     BTN_CUSTOM_52,      "An/Dig(bolt-on")
        MAP_INSERT(193,     BTN_CUSTOM_51,      "Digital menu(bolt-on")
        MAP_INSERT(195,     BTN_CUSTOM_10,      "battery low detection")
        MAP_INSERT(204,     BTN_EPG,            "epg/guide")
        MAP_INSERT(210,     BTN_CUSTOM_62,      "view")
        MAP_INSERT(211,     BTN_MARK,           "mark")
        MAP_INSERT(217,     BTN_DIGIT_DOT,      "Digit dot")
        MAP_INSERT(243,     BTN_P_EFFECT,       "smart picture")
        MAP_INSERT(244,     BTN_S_EFFECT,       "smart sound")
        MAP_INSERT(245,     BTN_ASPECT,         "format")
        MAP_INSERT(250,     BTN_CUSTOM_40,      "demo (not color key)")
    MAP_GROUP_END()
DECLARE_MAP_END()

/* VESTEL REMOTE WITH NO PROFILE USAGE.(The value readed from HW.)*/
enum ir_keys
{
    KEY_DIGIT_0     = 0,
    KEY_DIGIT_1     = 1,
    KEY_DIGIT_2     = 2,
    KEY_DIGIT_3     = 3,
    KEY_DIGIT_4     = 4,
    KEY_DIGIT_5     = 5,
    KEY_DIGIT_6     = 6,
    KEY_DIGIT_7     = 7,
    KEY_DIGIT_8     = 8,
    KEY_DIGIT_9     = 9,
    KEY_BACK        = 10,
    KEY_TVFORMAT    = 11,
    KEY_STANDBY     = 12,
    KEY_MUTE        = 13,
    KEY_PRESETS     = 14,
    KEY_LANGUAGE    = 15,
    KEY_VUP         = 16,
    KEY_VDOWN       = 17,
    KEY_HELP        = 18,
    KEY_DOWN        = 19,
    KEY_UP          = 20,
    KEY_LEFT        = 21,
    KEY_RIGHT       = 22,

    KEY_STOP        = 24,
    KEY_PLAY        = 25,

    KEY_REWIND      = 27,
    KEY_FFOWAD      = 28,  
	KEY_PREV        = 29,
	KEY_NEXT        = 30,
    KEY_SUBTITLE    = 31,
    KEY_PUP 	    = 32,
    KEY_PDOWN	    = 33,
    KEY_SWAP        = 34,
    
	KEY_EXIT        = 37,
    KEY_MYBUTTON1   = 38,
	KEY_FAV 	    = 40,
    KEY_MYBUTTON2   = 41,
    KEY_SLEEP       = 42,
	
    KEY_INTERNET    = 46,
    KEY_GUIDE       = 47,
	KEY_MENU		= 48,
	KEY_PAUSE	    = 49,
	KEY_YELLOW      = 50,
	KEY_REC 	    = 51,
	KEY_BLUE        = 52,
	KEY_OK			= 53,
    KEY_GREEN       = 54,
    KEY_RED         = 55,
	KEY_SOURCES_MENU = 56,
    KEY_MULTIMEDIA   = 57,

    KEY_TXT          = 60,
        
    KEY_INVALID      = 255,
};


#if defined(IRRX_RC5_KEYMAP_FUNC)
#undef IRRX_RC5_KEYMAP_FUNC
#endif

#define GRP_ICE_TV (1)
#define GRP_ICE_VCR (6)

#define IRRX_RC5_KEYMAP_FUNC (SGP_rc5_ir2crystal)
#if 1
DECLARE_MAP_BEGIN(SGP_rc5_ir2crystal)
    MAP_GROUP_BEGIN(GRP_ICE_TV)
        MAP_INSERT(  KEY_DIGIT_0,     BTN_DIGIT_0, "0")                          
        MAP_INSERT(  KEY_DIGIT_1,     BTN_DIGIT_1, "1")                 
        MAP_INSERT(  KEY_DIGIT_2,     BTN_DIGIT_2, "2")         
        MAP_INSERT(  KEY_DIGIT_3,     BTN_DIGIT_3, "3")                    
        MAP_INSERT(  KEY_DIGIT_4,     BTN_DIGIT_4, "4")               
        MAP_INSERT(  KEY_DIGIT_5,     BTN_DIGIT_5, "5")                     
        MAP_INSERT(  KEY_DIGIT_6,     BTN_DIGIT_6, "6")                         
        MAP_INSERT(  KEY_DIGIT_7,     BTN_DIGIT_7, "7")                 
        MAP_INSERT(  KEY_DIGIT_8,     BTN_DIGIT_8, "8")                 
        MAP_INSERT(  KEY_DIGIT_9,     BTN_DIGIT_9, "9")                
		MAP_INSERT(  KEY_BACK,	      BTN_RETURN, "BACK")				
        MAP_INSERT(  KEY_TVFORMAT,    BTN_ASPECT, "TVFORMAT")      
		MAP_INSERT(  KEY_LANGUAGE,		  BTN_MTS_AUDIO,	  "Language")    
        MAP_INSERT(  KEY_UP,          BTN_CURSOR_UP, "Up")                
        MAP_INSERT(  KEY_DOWN,        BTN_CURSOR_DOWN, "Down")                    
        MAP_INSERT(  KEY_LEFT,        BTN_CURSOR_LEFT, "Left")            
        MAP_INSERT(  KEY_RIGHT,       BTN_CURSOR_RIGHT, "Right")         
        MAP_INSERT(  KEY_OK,          BTN_SELECT, "select")          
        MAP_INSERT(  KEY_TXT,         BTN_TEXT, "Text")               
        MAP_INSERT(  KEY_RED,         BTN_RED, "Red")               
        MAP_INSERT(  KEY_GREEN,       BTN_GREEN, "Green")           
        MAP_INSERT(  KEY_YELLOW,      BTN_YELLOW, "Yellow")
        MAP_INSERT(  KEY_BLUE,        BTN_BLUE,            "Blue")
        MAP_INSERT(  KEY_STANDBY,     BTN_POWER,      "Power")
		MAP_INSERT(  KEY_MUTE,		  BTN_MUTE, "Mute") 			   
        MAP_INSERT(  KEY_VUP,         BTN_VOL_UP, "V+")             
        MAP_INSERT(  KEY_VDOWN,       BTN_VOL_DOWN, "V-")             
		MAP_INSERT(  KEY_PRESETS,	  BTN_CUSTOM_2, "PRESETS") 			
        MAP_INSERT(  KEY_PUP,         BTN_PRG_UP, "P+")             
        MAP_INSERT(  KEY_PDOWN,       BTN_PRG_DOWN, "P-")
		MAP_INSERT(  KEY_SWAP, 	      BTN_SWAP, "SWAP")
		MAP_INSERT(  KEY_EXIT, 	      BTN_EXIT, "EXIT")
		MAP_INSERT(  KEY_MYBUTTON1,	  BTN_CUSTOM_3, "MYBUTTON1")
        MAP_INSERT(  KEY_FAV,         BTN_FAVORITE,      "FAV")
		MAP_INSERT(  KEY_MYBUTTON2,   BTN_CUSTOM_4, "MYBUTTON2")		
        MAP_INSERT(  KEY_SLEEP,		  BTN_SLEEP, 	 "SLEEP")
		MAP_INSERT(  KEY_INTERNET,	  BTN_CUSTOM_1, 	 "Internet")
        MAP_INSERT(  KEY_HELP,        BTN_PRG_INFO,      "Info")
        MAP_INSERT(  KEY_MENU,        BTN_MENU,      "MENU")
        MAP_INSERT(  KEY_SUBTITLE,    BTN_SUB_TITLE, "Subtitle")    
        MAP_INSERT(  KEY_GUIDE,       BTN_EPG,      "Epg")
        MAP_INSERT(  KEY_PREV,      BTN_PREV, "Prev")               
		MAP_INSERT(  KEY_NEXT,		BTN_NEXT, "Next")				
        MAP_INSERT(  KEY_REC,       BTN_RECORD,      "Record")
        MAP_INSERT(  KEY_REWIND,    BTN_FR,      "Rewind")
        MAP_INSERT(  KEY_PLAY,      BTN_PLAY,      "PLAY")
        MAP_INSERT(  KEY_FFOWAD,    BTN_FF,      "FF")
        MAP_INSERT(  KEY_PAUSE,     BTN_PAUSE,      "PAUSE")
        MAP_INSERT(  KEY_STOP,      BTN_STOP,      "STOP")
        MAP_INSERT(  KEY_SOURCES_MENU,    BTN_INPUT_SRC,      "Sources Menu")
		MAP_INSERT(  KEY_MULTIMEDIA,	  BTN_MEM_CARD,	      "Multimedia")
    MAP_GROUP_END()

    MAP_GROUP_BEGIN(GRP_ICE_VCR)
		MAP_INSERT(  KEY_DIGIT_0,	  BTN_DIGIT_0, "0") 						 
		MAP_INSERT(  KEY_DIGIT_1,	  BTN_DIGIT_1, "1") 				
		MAP_INSERT(  KEY_DIGIT_2,	  BTN_DIGIT_2, "2") 		
		MAP_INSERT(  KEY_DIGIT_3,	  BTN_DIGIT_3, "3") 				   
		MAP_INSERT(  KEY_DIGIT_4,	  BTN_DIGIT_4, "4") 			  
		MAP_INSERT(  KEY_DIGIT_5,	  BTN_DIGIT_5, "5") 					
		MAP_INSERT(  KEY_DIGIT_6,	  BTN_DIGIT_6, "6") 						
		MAP_INSERT(  KEY_DIGIT_7,	  BTN_DIGIT_7, "7") 				
		MAP_INSERT(  KEY_DIGIT_8,	  BTN_DIGIT_8, "8") 				
		MAP_INSERT(  KEY_DIGIT_9,	  BTN_DIGIT_9, "9") 			   
		MAP_INSERT(  KEY_BACK,		  BTN_RETURN, "BACK")				
		MAP_INSERT(  KEY_TVFORMAT,	  BTN_ASPECT, "TVFORMAT")	   
		MAP_INSERT(  KEY_LANGUAGE,		  BTN_MTS_AUDIO,	  "Language")	 
		MAP_INSERT(  KEY_UP,		  BTN_CURSOR_UP, "Up")				  
		MAP_INSERT(  KEY_DOWN,		  BTN_CURSOR_DOWN, "Down")					  
		MAP_INSERT(  KEY_LEFT,		  BTN_CURSOR_LEFT, "Left")			  
		MAP_INSERT(  KEY_RIGHT, 	  BTN_CURSOR_RIGHT, "Right")		 
		MAP_INSERT(  KEY_OK,		  BTN_SELECT, "select") 		 
		MAP_INSERT(  KEY_TXT,		  BTN_TEXT, "Text") 			  
		MAP_INSERT(  KEY_RED,		  BTN_RED, "Red")				
		MAP_INSERT(  KEY_GREEN, 	  BTN_GREEN, "Green")			
		MAP_INSERT(  KEY_YELLOW,	  BTN_YELLOW, "Yellow")
		MAP_INSERT(  KEY_BLUE,		  BTN_BLUE, 		   "Blue")
		MAP_INSERT(  KEY_STANDBY,	  BTN_POWER,	  "Power")
		MAP_INSERT(  KEY_MUTE,		  BTN_MUTE, "Mute") 			   
		MAP_INSERT(  KEY_VUP,		  BTN_VOL_UP, "V+") 			
		MAP_INSERT(  KEY_VDOWN, 	  BTN_VOL_DOWN, "V-")			  
		MAP_INSERT(  KEY_PRESETS,	  BTN_CUSTOM_2, "PRESETS")			
		MAP_INSERT(  KEY_PUP,		  BTN_PRG_UP, "P+") 			
		MAP_INSERT(  KEY_PDOWN, 	  BTN_PRG_DOWN, "P-")
		MAP_INSERT(  KEY_SWAP,		  BTN_SWAP, "SWAP")
		MAP_INSERT(  KEY_EXIT,		  BTN_EXIT, "EXIT")
		MAP_INSERT(  KEY_MYBUTTON1,   BTN_CUSTOM_3, "MYBUTTON1")
		MAP_INSERT(  KEY_FAV,		  BTN_FAVORITE, 	 "FAV")
		MAP_INSERT(  KEY_MYBUTTON2,   BTN_CUSTOM_4, "MYBUTTON2")		
		MAP_INSERT(  KEY_SLEEP, 	  BTN_SLEEP,	 "SLEEP")
		MAP_INSERT(  KEY_INTERNET,	  BTN_CUSTOM_1, 	 "Internet")
		MAP_INSERT(  KEY_HELP,		  BTN_PRG_INFO, 	 "Info")
		MAP_INSERT(  KEY_MENU,		  BTN_MENU, 	 "MENU")
		MAP_INSERT(  KEY_SUBTITLE,	  BTN_SUB_TITLE, "Subtitle")	
		MAP_INSERT(  KEY_GUIDE, 	  BTN_EPG,		"Epg")
		MAP_INSERT(  KEY_PREV,		BTN_PREV, "Prev")				
		MAP_INSERT(  KEY_NEXT,		BTN_NEXT, "Next")				
		MAP_INSERT(  KEY_REC,		BTN_RECORD, 	 "Record")
		MAP_INSERT(  KEY_REWIND,	BTN_FR, 	 "Rewind")
		MAP_INSERT(  KEY_PLAY,		BTN_PLAY,	   "PLAY")
		MAP_INSERT(  KEY_FFOWAD,	BTN_FF, 	 "FF")
		MAP_INSERT(  KEY_PAUSE, 	BTN_PAUSE,		"PAUSE")
		MAP_INSERT(  KEY_STOP,		BTN_STOP,	   "STOP")
		MAP_INSERT(  KEY_SOURCES_MENU,	  BTN_INPUT_SRC,	  "Sources Menu")
		MAP_INSERT(  KEY_MULTIMEDIA,	  BTN_MEM_CARD, 	  "Multimedia")
    MAP_GROUP_END()
    
DECLARE_MAP_END()
#endif

#endif /* PHILIPS_LC09M_IR_MAP_H */
