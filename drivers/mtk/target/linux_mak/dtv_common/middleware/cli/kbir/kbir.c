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
/*-----------------------------------------------------------------------------
 * $RCSfile: kbir.c,v $
 * $Revision: #1 $
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 49a94e58968a451ba8a9307f6f11595c $
 *
 * Description:
 *          This file implements keyboard-to-IR key generator.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"
#include "res_mngr/drv/x_irrc_btn_def.h"
#include "io_mngr/iom.h"
#include "io_mngr/iom_key_gen.h"
#include "io_mngr/x_iom.h"
#include "cli/cli.h"
#include "cli/kbir/kbir.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define END_OF_KB_IR_KEY_MAP        ((UCHAR)    0xff)
#define KEY_DOWN_UP_ITVL            110 /* ms */

#define KBIR_KEY_NULL               ((UCHAR)    0x00)
#define KBIR_KEY_CTRL_C             ((UCHAR)    0x03)
#define KBIR_KEY_ENTER              ((UCHAR)    0x0d)
#define KBIR_KEY_ESC                ((UCHAR)    0x1b)
#define KBIR_KEY_UP                 ((UCHAR)    0x41)
#define KBIR_KEY_DOWN               ((UCHAR)    0x42)
#define KBIR_KEY_RIGHT              ((UCHAR)    0x43)
#define KBIR_KEY_LEFT               ((UCHAR)    0x44)
#define KBIR_KEY_ARROW              ((UCHAR)    0x5b)
#define KBIR_KEY_EARTHWORM          ((UCHAR)    0x7e)
#define KBIR_KEY_INSERT             ((UCHAR)    0xe0)
#define KBIR_KEY_DELETE             ((UCHAR)    0xe1)
#define KBIR_KEY_HOME               ((UCHAR)    0xe2)
#define KBIR_KEY_END                ((UCHAR)    0xe3)
#define KBIR_KEY_PAGE_UP            ((UCHAR)    0xe4)
#define KBIR_KEY_PAGE_DOWN          ((UCHAR)    0xe5)
#define KBIR_KEY_F1                 ((UCHAR)    0xf1)
#define KBIR_KEY_F2                 ((UCHAR)    0xf2)
#define KBIR_KEY_F3                 ((UCHAR)    0xf3)
#define KBIR_KEY_F4                 ((UCHAR)    0xf4)
#define KBIR_KEY_F5                 ((UCHAR)    0xf5)
#define KBIR_KEY_F6                 ((UCHAR)    0xf6)
#define KBIR_KEY_F7                 ((UCHAR)    0xf7)
#define KBIR_KEY_F8                 ((UCHAR)    0xf8)
#define KBIR_KEY_F9                 ((UCHAR)    0xf9)
#define KBIR_KEY_F10                ((UCHAR)    0xfa)
#define KBIR_KEY_F11                ((UCHAR)    0xfb)
#define KBIR_KEY_F12                ((UCHAR)    0xfc)

/* Definition of keyboard-to-IR key map table */
typedef struct _KB_IR_KEY_MAP_T
{
    UCHAR       uc_kb_code;
    UINT32      ui4_evt_code;
    CHAR*       ps_str;
}   KB_IR_KEY_MAP_T;

KB_IR_KEY_MAP_T t_kb_ir_key_map[] =
{
    {(UCHAR)'0',            BTN_DIGIT_0,        "Digit 0"           },
    {(UCHAR)'1',            BTN_DIGIT_1,        "Digit 1"           },
    {(UCHAR)'2',            BTN_DIGIT_2,        "Digit 2"           },
    {(UCHAR)'3',            BTN_DIGIT_3,        "Digit 3"           },
    {(UCHAR)'4',            BTN_DIGIT_4,        "Digit 4"           },
    {(UCHAR)'5',            BTN_DIGIT_5,        "Digit 5"           },
    {(UCHAR)'6',            BTN_DIGIT_6,        "Digit 6"           },
    {(UCHAR)'7',            BTN_DIGIT_7,        "Digit 7"           },
    {(UCHAR)'8',            BTN_DIGIT_8,        "Digit 8"           },
    {(UCHAR)'9',            BTN_DIGIT_9,        "Digit 9"           },
    {(UCHAR)'.',            BTN_DIGIT_DOT,      "Dot"               },
    {KBIR_KEY_LEFT,         BTN_CURSOR_LEFT,    "Cursor Left"       },
    {KBIR_KEY_RIGHT,        BTN_CURSOR_RIGHT,   "Cursor Right"      },
    {KBIR_KEY_UP,           BTN_CURSOR_UP,      "Cursor Up"         },
    {KBIR_KEY_DOWN,         BTN_CURSOR_DOWN,    "Cursor Down"       },
    {KBIR_KEY_ENTER,        BTN_SELECT,         "Select"            },
    {(UCHAR)'x',            BTN_EXIT,           "Exit"              },
    {KBIR_KEY_PAGE_UP,      BTN_PRG_UP,         "Program +"         },
    {KBIR_KEY_PAGE_DOWN,    BTN_PRG_DOWN,       "Program -"         },
    {(UCHAR)'v',            BTN_PREV_PRG,       "Previous Program"  },
    {(UCHAR)'l',            BTN_CH_LIST,        "Channel List"      },
    {(UCHAR)'b',            BTN_PIP,            "PIP"               },
    {(UCHAR)'z',            BTN_PIP_SIZE,       "PIP Size"          },
    {(UCHAR)'t',            BTN_PIP_POS,        "PIP Position"      },
    {(UCHAR)'o',            BTN_POP,            "POP"               },
    {(UCHAR)'f',            BTN_FAVORITE,       "Favorite"          },
    {(UCHAR)'z',            BTN_ZOOM,           "Zoom"              },
    {(UCHAR)'r',            BTN_ASPECT,         "Aspect Ratio"      },
    {(UCHAR)'s',            BTN_SWAP,           "Swap"              },
    {(UCHAR)'i',            BTN_PRG_INFO,       "Program Info"      },
    {(UCHAR)'c',            BTN_CC,             "Closed Caption"    },
    {KBIR_KEY_DELETE,       BTN_ADD_ERASE,      "Add/Erase"         },
    {(UCHAR)'d',            BTN_SLEEP,          "Sleep"             },
    {(UCHAR)'g',            BTN_TIMER,          "Timer"             },
    {KBIR_KEY_INSERT,       BTN_INPUT_SRC,      "Input Source"      },
    {KBIR_KEY_HOME,         BTN_VOL_UP,         "Volumn +"          },
    {KBIR_KEY_END,          BTN_VOL_DOWN,       "Volumn -"          },
    {(UCHAR)'u',            BTN_MUTE,           "Mute"              },
    {(UCHAR)'a',            BTN_MTS,            "MTS"               },
    {(UCHAR)'p',            BTN_POWER,          "Power"             },
    {(UCHAR)'m',            BTN_MENU,           "Menu"              },
    {(UCHAR)'e',            BTN_EPG,            "EPG"               },
    {KBIR_KEY_F1,           BTN_RED,            "Red"               },
    {KBIR_KEY_F2,           BTN_GREEN,          "Green"             },
    {KBIR_KEY_F3,           BTN_YELLOW,         "Yellow"            },
    {KBIR_KEY_F4,           BTN_BLUE,           "Blue"              },
    {KBIR_KEY_F9,           BTN_FACTORY_MODE_1, "Factory Mode 1"    },
    {KBIR_KEY_F10,          BTN_FACTORY_MODE_2, "Factory Mode 2"    },
    {KBIR_KEY_F11,          BTN_FACTORY_MODE_3, "Factory Mode 3"    },
    {KBIR_KEY_F12,          BTN_FACTORY_MODE_4, "Factory Mode 4"    },
    {END_OF_KB_IR_KEY_MAP,  0,                  NULL                }
};

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static CHAR _kbir_upper_to_lower_case(CHAR c_key);

static VOID _kbir_help(VOID);

static VOID _kbir_random(VOID);

static INT32 _kbir_key_gen(VOID);

/*-----------------------------------------------------------------------------
 * Name: _kbir_upper_to_lower_case
 *
 * Description: This API converts letter from upper to lower case.
 *
 * Inputs:  c_key       The letter to be converted.
 *
 * Outputs: -
 *
 * Returns: The result of conversion.
 ----------------------------------------------------------------------------*/
static CHAR _kbir_upper_to_lower_case(CHAR c_key)
{
    if ((c_key >= 'A') && (c_key <= 'Z'))
    {
        c_key += ('a' - 'A');
    }

    return c_key;
}

/*-----------------------------------------------------------------------------
 * Name: _kbir_help
 *
 * Description: This API shows KBIR help description.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _kbir_help(VOID)
{
    UINT8       ui1_idx = 0;
    BOOL        b_string;
    CHAR        c_kb_code[10];

    x_dbg_stmt("<< Keyboard-to-IR key map table >>\n\r");
    while ((ui1_idx < (UINT8)(sizeof(t_kb_ir_key_map)/sizeof(KB_IR_KEY_MAP_T))) &&
           (t_kb_ir_key_map[ui1_idx].uc_kb_code != END_OF_KB_IR_KEY_MAP))
    {
        b_string = TRUE;;
        switch (t_kb_ir_key_map[ui1_idx].uc_kb_code)
        {
            case KBIR_KEY_LEFT:
                x_strcpy(c_kb_code, "<-");
                break;

            case KBIR_KEY_RIGHT:
                x_strcpy(c_kb_code, "->");
                break;

            case KBIR_KEY_UP:
                x_strcpy(c_kb_code, "^|");
                break;

            case KBIR_KEY_DOWN:
                x_strcpy(c_kb_code, "|v");
                break;

            case KBIR_KEY_ENTER:
                x_strcpy(c_kb_code, "Enter");
                break;

            case KBIR_KEY_INSERT:
                x_strcpy(c_kb_code, "Insert");
                break;

            case KBIR_KEY_DELETE:
                x_strcpy(c_kb_code, "Delete");
                break;

            case KBIR_KEY_HOME:
                x_strcpy(c_kb_code, "Home");
                break;

            case KBIR_KEY_END:
                x_strcpy(c_kb_code, "End");
                break;

            case KBIR_KEY_PAGE_UP:
                x_strcpy(c_kb_code, "Page Up");
                break;

            case KBIR_KEY_PAGE_DOWN:
                x_strcpy(c_kb_code, "Page Down");
                break;

            case KBIR_KEY_F1:
                x_strcpy(c_kb_code, "F1");
                break;

            case KBIR_KEY_F2:
                x_strcpy(c_kb_code, "F2");
                break;

            case KBIR_KEY_F3:
                x_strcpy(c_kb_code, "F3");
                break;

            case KBIR_KEY_F4:
                x_strcpy(c_kb_code, "F4");
                break;

            case KBIR_KEY_F5:
                x_strcpy(c_kb_code, "F5");
                break;

            case KBIR_KEY_F6:
                x_strcpy(c_kb_code, "F6");
                break;

            case KBIR_KEY_F7:
                x_strcpy(c_kb_code, "F7");
                break;

            case KBIR_KEY_F8:
                x_strcpy(c_kb_code, "F8");
                break;

            case KBIR_KEY_F9:
                x_strcpy(c_kb_code, "F9");
                break;

            case KBIR_KEY_F10:
                x_strcpy(c_kb_code, "F10");
                break;

            case KBIR_KEY_F11:
                x_strcpy(c_kb_code, "F11");
                break;

            case KBIR_KEY_F12:
                x_strcpy(c_kb_code, "F12");
                break;

            default:
                b_string = FALSE;
                break;
        }

        if (b_string)
        {
            x_dbg_stmt("  %-12s%s (0x%08x)\n\r",
                       c_kb_code,
                       t_kb_ir_key_map[ui1_idx].ps_str,
                       t_kb_ir_key_map[ui1_idx].ui4_evt_code);
        }
        else
        {
            x_dbg_stmt("  %c           %s (0x%08x)\n\r",
                       t_kb_ir_key_map[ui1_idx].uc_kb_code,
                       t_kb_ir_key_map[ui1_idx].ps_str,
                       t_kb_ir_key_map[ui1_idx].ui4_evt_code);
        }

        ui1_idx++;
    };

    x_dbg_stmt("\n\r");
    x_dbg_stmt("  kbir [option]\n\r");
    x_dbg_stmt("    -h: show this help\n\r");
    x_dbg_stmt("    /h: show this help\n\r");
    x_dbg_stmt("\n\r");
    x_dbg_stmt("  <Keyboard-to-IR key generator>\n\r");
    x_dbg_stmt("    ?: show this help\n\r");
    x_dbg_stmt("    @: generate IR key randomly\n\r");
    x_dbg_stmt("\n\r");
}


/*-----------------------------------------------------------------------------
 * Name: _kbir_random
 *
 * Description: This API generates virtual IR keys to IO Manager randomly.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _kbir_random(VOID)
{
    UINT8   ui1_idx;
    UINT8   ui1_kbir_key_num;

    if ((ui1_kbir_key_num = (UINT8)(sizeof(t_kb_ir_key_map)/sizeof(KB_IR_KEY_MAP_T))) == 0)
    {
        return;
    }

    while (cli_get_char_timeout(50) != CLI_ASCII_KEY_CTRL_B)
    {
        ui1_idx = (UINT8)(x_os_get_sys_tick()%ui1_kbir_key_num);

        if (t_kb_ir_key_map[ui1_idx].ui4_evt_code != BTN_POWER) /* Skip BTN_POWER event code */
        {
            x_iom_send_evt(t_kb_ir_key_map[ui1_idx].ui4_evt_code, KEY_DOWN_UP_ITVL);

            x_dbg_stmt("[KBIR] %s (0x%08x)\n\r",
                       t_kb_ir_key_map[ui1_idx].ps_str,
                       t_kb_ir_key_map[ui1_idx].ui4_evt_code);

            x_thread_delay(1000);
        }
    }

    x_dbg_stmt("\n\rOperation was interrupted by the user!!!\n\r");
}


/*-----------------------------------------------------------------------------
 * Name: _kbir_key_gen
 *
 * Description: This API polls the character input, converts it to
 *              corresponding IRRC event, then dispatches it to IO Manager via
 *              message send.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: KBIRR_OK     Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _kbir_key_gen(VOID)
{
    BOOL        b_gen_key;
    UINT8       ui1_idx;
    CHAR        ac_key[10];
    CHAR        c_key;
    BOOL        b_quit = FALSE;

    while (!b_quit)
    {
        b_gen_key = TRUE;
        c_key = cli_get_char();
        c_key = _kbir_upper_to_lower_case(c_key);

        switch (c_key)
        {
            case 'q':
            case KBIR_KEY_CTRL_C:
                b_quit = TRUE;
                break;

            case '?':
                _kbir_help();
                break;

            case '@':
                _kbir_random();
                break;

            case KBIR_KEY_ESC:
                c_key = cli_get_char();
                if (c_key == (CHAR)KBIR_KEY_ARROW)
                {
                    /* Handle up, donw, left, right keys */
                    c_key = cli_get_char();
                    if ((c_key == (CHAR)KBIR_KEY_UP) ||
                        (c_key == (CHAR)KBIR_KEY_DOWN) ||
                        (c_key == (CHAR)KBIR_KEY_LEFT) ||
                        (c_key == (CHAR)KBIR_KEY_RIGHT))
                    {
                        break;
                    }
                    else
                    {
                        ui1_idx = 0;
                        ac_key[ui1_idx++] = c_key;
                    }

                    while ((c_key = cli_get_char()) != (CHAR)KBIR_KEY_EARTHWORM)
                    {
                        if (ui1_idx < 10)
                        {                            
                            ac_key[ui1_idx++] = c_key;
                        }                            
                    }

                    if (ui1_idx == 1)
                    {
                        switch (ac_key[0])
                        {
                            case '1':
                                c_key = (CHAR)KBIR_KEY_INSERT;
                                break;

                            case '2':
                                c_key = (CHAR)KBIR_KEY_HOME;
                                break;

                            case '3':
                                c_key = (CHAR)KBIR_KEY_PAGE_UP;
                                break;

                            case '4':
                                c_key = (CHAR)KBIR_KEY_DELETE;
                                break;

                            case '5':
                                c_key = (CHAR)KBIR_KEY_END;
                                break;

                            case '6':
                                c_key = (CHAR)KBIR_KEY_PAGE_DOWN;
                                break;

                            default:
                                b_gen_key = FALSE;
                        }
                    }
                    else if (ui1_idx == 2)
                    {
                        switch (ac_key[0])
                        {
                            /* Handle F1 to F12 keys */
                            case '1':
                                if (ac_key[1] == '1' || ac_key[1] == '2' || ac_key[1] == '3' ||
                                    ac_key[1] == '4' || ac_key[1] == '5')
                                {
                                    c_key = (CHAR)(ac_key[1] + KBIR_KEY_F1 - '1');
                                }
                                else if (ac_key[1] == '7' || ac_key[1] == '8' || ac_key[1] == '9')
                                {
                                    c_key = (CHAR)(ac_key[1] + KBIR_KEY_F6 - '7');
                                }
                                else
                                {
                                    b_gen_key = FALSE;
                                }
                                break;

                            case '2':
                                if (ac_key[1] == '0' || ac_key[1] == '1')
                                {
                                    c_key = (CHAR)(ac_key[1] + KBIR_KEY_F9 - '0');
                                }
                                else if (ac_key[1] == '3' || ac_key[1] == '4')
                                {
                                    c_key = (CHAR)(ac_key[1] + KBIR_KEY_F11 - '3');
                                }
                                else
                                {
                                    b_gen_key = FALSE;
                                }
                                break;

                            default:
                                b_gen_key = FALSE;
                                break;
                        }
                    }
                    else
                    {
                        b_gen_key = FALSE;
                    }
                }
                else
                {
                    b_gen_key = FALSE;
                }
                break;

            default:
                break;
        }

        if (b_gen_key == FALSE)
        {
            continue;
        }

        /* Traverse the keyboard-to-ir map table */
        ui1_idx = 0;
        while ((ui1_idx < (UINT8)(sizeof(t_kb_ir_key_map)/sizeof(KB_IR_KEY_MAP_T))) &&
               (t_kb_ir_key_map[ui1_idx].uc_kb_code != END_OF_KB_IR_KEY_MAP))
        {
            if ((UCHAR)c_key == t_kb_ir_key_map[ui1_idx].uc_kb_code)
            {
                /* Send IRRC event to IO Manager */
                x_iom_send_evt(t_kb_ir_key_map[ui1_idx].ui4_evt_code, KEY_DOWN_UP_ITVL);

                x_dbg_stmt("[KBIR] %s (0x%08x)\n\r",
                           t_kb_ir_key_map[ui1_idx].ps_str,
                           t_kb_ir_key_map[ui1_idx].ui4_evt_code);
                break;
            }
            ui1_idx++;
        };
    } /* while (!b_quit) */

    return KBIRR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: kbir_main
 *
 * Description: This is KBIR main function.
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
INT32 kbir_main(INT32        i4_argc,
                const CHAR** pps_argv)
{
    if (i4_argc == 1)
    {
        /* Enter keyboard-to-IR key generator loop */
        x_dbg_stmt("Starting keyboard-to-IR key generator...\n\r");
        x_dbg_stmt("Press 'q' key to exit!\n\r\n\r");
        _kbir_key_gen();
    }
    else if ((x_strcmp("-h", pps_argv[1]) == 0) ||
             (x_strcmp("/h", pps_argv[1]) == 0))
    {
        _kbir_help();
    }

    return 0;
}

