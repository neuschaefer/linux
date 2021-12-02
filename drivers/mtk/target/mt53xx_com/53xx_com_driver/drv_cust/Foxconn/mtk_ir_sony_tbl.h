/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

#ifndef MTK_IR_SONY_TBL_H
#define MTK_IR_SONY_TBL_H

/* TV-1, 12bits command */
static const KEYUNIT_T _arSonyKeyMap01[] =
{
    { 0x00, BTN_DIGIT_1      },  // #1,   1
    { 0x01, BTN_DIGIT_2      },  // #2,   2
    { 0x02, BTN_DIGIT_3      },  // #3,   3
    { 0x03, BTN_DIGIT_4      },  // #4,   4
    { 0x04, BTN_DIGIT_5      },  // #5,   5
    { 0x05, BTN_DIGIT_6      },  // #6,   6
    { 0x06, BTN_DIGIT_7      },  // #7,   7
    { 0x07, BTN_DIGIT_8      },  // #8,   8
    { 0x08, BTN_DIGIT_9      },  // #9,   9
    { 0x09, BTN_DIGIT_0      },  // #10,  10
    { 0x0b, BTN_ENTER        },  // #12,  ENTER
    { 0x0e, BTN_CUSTOM_63        },  // /* CH-15 / 10KEY (Guide/Gemstar Key)) */
    { 0x10, BTN_PRG_UP       },  // #17,  CH-UP                        
    { 0x11, BTN_PRG_DOWN     },  // #18,  CH-DOWN                             
    { 0x12, BTN_VOL_UP       },  // #19,  VOLUME-UP                        
    { 0x13, BTN_VOL_DOWN     },  // #20,  VOLUME-DOWN                          
    { 0x14, BTN_MUTE         },  // #21,  MUTE                      
    { 0x15, BTN_POWER        },  // #22,  Power
    { 0x16, BTN_CUSTOM_35    },  // #23,  Reset
    { 0x17, BTN_MTS          },  // #24,  MTS
    { 0x18, BTN_CUSTOM_1     },  // #25,  PICTURE-UP
    { 0x19, BTN_CUSTOM_2     },  // #26,  PICTURE-DOWN
    { 0x1a, BTN_CUSTOM_3     },  // #27,  COLOR-VIVID
    { 0x1b, BTN_CUSTOM_4     },  // #28,  COLOR-PALE
    { 0x1e, BTN_CUSTOM_5     },  // #31,  BRIGHTNESS-BRIGHT
    { 0x1f, BTN_CUSTOM_6     },  // #32,  BRIGHTNESS-DARK
    { 0x20, BTN_CUSTOM_8     },  // #33,  HUE-PURPLISH
    { 0x21, BTN_CUSTOM_7     },  // #34,  HUE-GREENISH
    { 0x22, BTN_CUSTOM_9     },  // #35,  SHARPNESS-SHARP
    { 0x23, BTN_CUSTOM_10    },  // #36,  SHARPNESS-SOFT
    { 0x24, BTN_CUSTOM_11    },  // #37,  VIDEO-RESET (External Input -> TV)
    { 0x25, BTN_INPUT_SRC    },  // #38,  VIDEO-SELECT (Input Select)
    { 0x26, BTN_CUSTOM_12    },  // #39,  BALANCE-LEFT
    { 0x27, BTN_CUSTOM_13    },  // #40,  BALANCE-RIGHT
    { 0x28, BTN_S_EFFECT     },  // #41,  SOUND (Sound mode)
    { 0x2e, BTN_POWER_ON     },  // #47,  Power on
    { 0x2f, BTN_POWER_OFF    },  // #48,  Power off  
    { 0x33, BTN_CURSOR_RIGHT },  // #52,  Cursor Right                   
    { 0x34, BTN_CURSOR_LEFT  },  // #51,  Cursor Left   
    { 0x36, BTN_SLEEP        },  // #55,  SLEEP   
    { 0x3a, BTN_PRG_INFO     },  // #59,  DISPLAY
    { 0x3b, BTN_PREV_PRG     },  // #60,  JUMP 
    { 0x40, BTN_CUSTOM_14    },  // #65,  VIDEO-1 (Direct AV1)
    { 0x41, BTN_CUSTOM_15    },  // #66,  VIDEO-2 (Direct AV2)
    { 0x42, BTN_CUSTOM_16    },  // #67,  VIDEO-3 (Direct Component1)
    { 0x43, BTN_CUSTOM_17    },  // #68,  ANALOG RGB (Direct PC)
    { 0x47, BTN_CUSTOM_18    },  // #72,  VIDEO-4 (Direct Component2)
    { 0x48, BTN_CUSTOM_19    },  // #73,  VIDEO-5 (Direct HDMI1)
    { 0x49, BTN_CUSTOM_20    },  // #74,  VIDEO-6 (Direct HDMI2)
    { 0x5b, BTN_PIP          },  // #92,  PIP
    { 0x60, BTN_MENU         },  // #97,  MENU  
    { 0x63, BTN_CUSTOM_60    },  // #100,    
    { 0x64, BTN_P_EFFECT     },  // #101, VIDEO PALETTE (Picture Mode)
    { 0x65, BTN_SELECT       },  // #102, SELECT  
    { 0x70, BTN_CUSTOM_21    },  // #113, TREBLE-UP        
    { 0x71, BTN_CUSTOM_22    },  // #114, TREBLE-DOWN  
    { 0x72, BTN_CUSTOM_23    },  // #115, BASS-UP
    { 0x73, BTN_CUSTOM_24    },  // #116, BASS-DOWN 
    { 0x74, BTN_CURSOR_UP    },  // #117, ANALOG-UP (Cursor Up)
    { 0x75, BTN_CURSOR_DOWN  },  // #118, ANALOG-DOWN (Cursor Down)
    { 0x7f, BTN_CUSTOM_36    },  // #128, Test 
};

/* TV adapter, 12bits command */
static const KEYUNIT_T _arSonyKeyMap03[] =
{
    { 0x4C, BTN_RED          },
    { 0x4D, BTN_GREEN        },
    { 0x4E, BTN_YELLOW       },
    { 0x4F, BTN_BLUE         }
};

/* AV Master, 15bits command */
static const KEYUNIT_T _arSonyKeyMap04[] =
{
    { 0x37, BTN_POWER_OFF    },  // #56,  SYSTEM OFF (Power off)
};

/* TV1G, 15bits command */
static const KEYUNIT_T _arSonyKeyMap1A[] =
{
    { 0x58,   BTN_CUSTOM_34  },  // #89,  Sync Menu
    { 0x59,   BTN_CUSTOM_25  },  // #90,  HDMI ONLY SELECT (Toggle HDMI)
    { 0x5a,   BTN_CUSTOM_26  },  // #91,  HDMI1 INPUT SELECT (Direct HDMI1)
    { 0x5b,   BTN_CUSTOM_27  },  // #92,  HDMI2 INPUT SELECT (Direct HDMi2)
    { 0x5c,   BTN_CUSTOM_56  },  // #93,  HDMI3 INPUT SELECT (Direct HDMI3)
    { 0x68,   BTN_CUSTOM_57  },  // #105, Info
    { 0x69,   BTN_CUSTOM_49  },  // #106, Next favorite channel
    { 0x6a,   BTN_CUSTOM_52  },  // #107, On demand
    { 0x6c,   BTN_CUSTOM_53  },  // #109, DMX menu
    { 0x6e,   BTN_CUSTOM_59  },  // #111, Guide
    { 0x78,   BTN_CUSTOM_61  },  // #111, SCENE ON/OFF
};

/* TV-1E, 15bits command */
static const KEYUNIT_T _arSonyKeyMapA4[] =
{
    { 0x10, BTN_CC           },  // #17,  CLOSED CAPTION MODE
    { 0x12, BTN_CUSTOM_30    },  // #19,  Componentl only seelct (Toggle Component)                                 // 
    { 0x16, BTN_CUSTOM_29    },  // #23,  VIDEO ONLY SELECT (Toggle AV)
    { 0x17, BTN_CUSTOM_30    },  // #24,  Componentl only seelct (Toggle Component)
    { 0x36, BTN_CUSTOM_31    },  // #55,  HD1 (Direct Component1)
    { 0x37, BTN_COMPONENT_2    },  // #56,  HD2 (Direct Component2)
    { 0x3d, BTN_ASPECT       },  // #62,  Wide Mode
    { 0x5B, BTN_EPG          },  // #92,  
    { 0x77, BTN_PIP          },  // #120,    
};

/* TV-1EE, 15bits command */
static const KEYUNIT_T _arSonyKeyMap97[] =
{
    { 0x18, BTN_STOP         },  // #25,  Stop
    { 0x19, BTN_PLAY_PAUSE   },  // #26,  Play Pause 
    { 0x1a, BTN_PLAY         },  // #27,  Play 
    { 0x1b, BTN_FR           },  // #28,  Quick-Return 
    { 0x1c, BTN_FF           },  // #29,  Fast-Forward 
    { 0x1d, BTN_DIGIT_DOT    },  // #30,  DOT
    { 0x20, BTN_CUSTOM_58    },  // #33,  REC
    { 0x23, BTN_EXIT         },  // #36,  RETURN
    { 0x24, BTN_BLUE         },  // #37,  Blue
    { 0x25, BTN_RED          },  // #38,  Red
    { 0x26, BTN_GREEN        },  // #39,  Green
    { 0x27, BTN_YELLOW       },  // #40,  Yellow
    { 0x28, BTN_CUSTOM_62       },  // /* Caption */
    { 0x36, BTN_TOOLS        },  // #55,  TOOLS
    { 0x3C, BTN_SR           },  // #61,  Previous Skip
    { 0x3D, BTN_SF           },  // #62,  Forward Skip
    { 0x4a, BTN_SELECT       },  // #75,  SELECT
    { 0x4d, BTN_CURSOR_LEFT  },  // #78,  CURSOR LEFT
    { 0x4e, BTN_CURSOR_RIGHT },  // #79,  CURSOR RIGHT
    { 0x4f, BTN_CURSOR_UP    },  // #80,  CURSOR UP
    { 0x50, BTN_CURSOR_DOWN  },  // #81,  CURSOR DOWN
    { 0x6a, BTN_CUSTOM_28    },  // #107, Picture Dynamic
    { 0x7b, BTN_S_EFFECT     },  // #124, Sound mode
    { 0x7c, BTN_CUSTOM_54    },  // #125, Demo Mode On/Off

};

/* TV-1EEE, 15bits command */
static const KEYUNIT_T _arSonyKeyMap77[] =
{
    { 0x32, BTN_CUSTOM_50 },  // #51,  SEEK+
    { 0x33, BTN_CUSTOM_51 },  // #52,  SEEK-
    { 0x36, BTN_POWER_SAVING },  // #55,  Power Saving
    { 0x60, BTN_CUSTOM_33    },  // #97,  Home Theatre 
    { 0x76, BTN_FAVORITE     },  // #119, Favorite
};

#endif /* MTK_IR_SONY_TBL_H */

