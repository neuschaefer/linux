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
 * $RCSfile: iom.c,v $
 * $Revision: #3 $
 * $Date: 2012/06/26 $
 * $Author: xiaokuan.shi $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         This is I/O Manager API source file.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"

#include "res_mngr/drv/x_irrc_btn_def.h"
#include "res_mngr/drv/x_irrc.h"
#include "res_mngr/x_rm_dev_types.h"

#include "io_mngr/_iom.h"
#include "io_mngr/iom.h"
#include "io_mngr/iom_lock.h"
#include "io_mngr/x_iom.h"
#include "io_mngr/iom_cli.h"

#define IOM_KEY_GEN_SUPPORT     1
#if IOM_KEY_GEN_SUPPORT
#include "io_mngr/iom_key_gen.h"
#endif

#include "graphic/u_gl.h"
#ifdef TIME_MEASUREMENT
#include "time_msrt/x_time_msrt.h"
#include "time_msrt/u_time_msrt_name.h"
#endif

#include <directfb.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include <stdlib.h>
#include <sys/ioctl.h>
#if !defined(ANDROID) && !defined(__UCLIBC__)
#include <stropts.h>
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define DFB_THREAD_NAME                 "dfb_thread"
#define NEC_KEY_NUM 58


#ifdef MW_AUTO_KEYBOARD_TO_IRRCCODE
#define KEYBOARD_TO_IRRC_NUM  21
#endif

#ifndef ANDROID
/* macro for a safe call to DirectFB functions */
#define DFBCHECK(x...)                                                    \
     {                                                                                 \
          err = x;                                                                 \
          if (err != DFB_OK) {                                              \
               printf("%s <%d>:\n", __FILE__, __LINE__);       \
               DirectFBErrorFatal( #x, err );                             \
          }                                                                           \
     }
#endif
/*
 * NEC command to DirectFB symbol mapping
 */
#define DFB_BTN_NONE                    ((u32)0x00000000)

/* UART settings */
#define UART_THREAD1_NAME                "uart_thread_1"
#define UART_THREAD2_NAME                "uart_thread_2"

#ifndef UART_0_DEV
#define UART_0_DEV "/dev/ttyMT3"
#endif
#ifndef UART_1_DEV
#define UART_1_DEV "/dev/ttyMT2"
#endif
#define UART_DEV_NAME(_id)  ((_id) == 1 ? UART_1_DEV : UART_0_DEV)
#define UART_GET_PORT(_id)  (((_id) == 1) ? 1 : 0)
#define UART_PORT_NUM 2
#define FACTORY_PORT_ID     7

#ifndef UART_FACTORY_CMD_INTER_BYTE_TIMEOUT
#define UART_FACTORY_CMD_INTER_BYTE_TIMEOUT     1
#endif

#ifndef UART_FACTORY_CMD_BUF_LEN
#define UART_FACTORY_CMD_BUF_LEN                255
#endif

#define NECCommand_Number       96

static const DFBInputDeviceKeySymbol _NECCommand[NECCommand_Number] =
{
    DFB_BTN_RED, DFB_BTN_GREEN, DFB_BTN_POWER, DFB_BTN_YELLOW, DFB_BTN_ZOOM, DFB_BTN_BLUE, DFB_BTN_TIMER, DFB_BTN_MEM_CARD,
    DFB_BTN_DIGIT_3, DFB_BTN_DIGIT_2, DFB_BTN_DIGIT_1, DFB_BTN_ENTER, DFB_BTN_DIGIT_9, DFB_BTN_DIGIT_8, DFB_BTN_DIGIT_7, DFB_BTN_FAV_CH,
    DFB_BTN_DIGIT_6, DFB_BTN_DIGIT_5, DFB_BTN_DIGIT_4, DFB_BTN_CURSOR_DOWN, DFB_BTN_MENU, DFB_BTN_DIGIT_0, DFB_BTN_FREEZE, DFB_BTN_RECORD,
    DFB_BTN_PRG_INFO, DFB_BTN_EPG, DFB_BTN_EXIT, DFB_BTN_PREV, DFB_BTN_CURSOR_RIGHT, DFB_BTN_CURSOR_UP, DFB_BTN_CURSOR_LEFT, DFB_BTN_P_EFFECT,
    DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE,
    DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE,
    DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE,
    DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE,
    DFB_BTN_S_EFFECT, DFB_BTN_NONE, DFB_BTN_PIP_SIZE, DFB_BTN_MTS, DFB_BTN_REPEAT, DFB_BTN_SUB_TITLE, DFB_BTN_TITLE_PBC, DFB_BTN_SWAP,
    DFB_BTN_FAVORITE, DFB_BTN_ADD_ERASE, DFB_BTN_NEXT, DFB_BTN_NONE, DFB_BTN_VOL_UP, DFB_BTN_FR, DFB_BTN_PRG_DOWN, DFB_BTN_NONE,
    DFB_BTN_FF, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_NONE, DFB_BTN_VOL_DOWN, DFB_BTN_PRG_UP, DFB_BTN_MUTE, DFB_BTN_PREV_PRG,
    DFB_BTN_PIP_POP, DFB_BTN_INPUT_SRC, DFB_BTN_DIGIT_DOT, DFB_BTN_ROOT_MENU, DFB_BTN_PIP_POS, DFB_BTN_ASPECT, DFB_BTN_CC, DFB_BTN_SLEEP
};

typedef enum {
     IOM_BUTTON_DOWN,
     IOM_BUTTON_UP    
} IOM_MANGER_TYPE;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
extern IDirectFB *          g_p_dfb;

static DEV_INFO_T           at_in_dev[IOM_MAX_DEV];
static DEV_INFO_T           at_out_dev[IOM_MAX_DEV];
static DEV_INFO_T           at_io_dev[IOM_MAX_DEV];
static BOOL                 b_iom_init = FALSE;
static BOOL                 b_in_irrc;
static UINT64               ui8_evt_src_mask;
static BOOL                 b_1st_irrc_evt = TRUE;
static UINT16               ui2_iom_dbg_level;
static THREAD_DESCR_T       t_iom_thread_descr = {DEFAULT_STACK_SIZE,
                                                  DEFAULT_PRIORITY,
                                                  DEFAULT_NUM_MSGS};
static HANDLE_T             h_dfb_inp_thread   = NULL_HANDLE;
static IDirectFBEventBuffer *keybuffer;
#ifdef IOM_DFB_USE_KEYBOARD_DEV
static IDirectFBInputDevice *inputdev;
#endif
static DFBResult            err;
static IOM_IRRC_PROTOCOL_T  e_protocol         = IOM_IRRC_PROTOCOL_NEC;
static BOOL                 ab_factory_mode[UART_PORT_NUM] = {FALSE, FALSE};
static IOM_UART_SPEED_T     e_factory_speed    = IOM_UART_SPEED_75;
#ifndef IOM_DISABLE_UART_FACTORY_MODE
static HANDLE_T             ah_uart_inp_thread[UART_PORT_NUM] = {NULL_HANDLE};
#endif

static HANDLE_T             h_iom_inp_msgq = NULL_HANDLE;
static BOOL                 b_is_key_mon_enabled = FALSE;
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
static DISABLE_EVTCODE_T  t_global_disable_keys = {0};
#endif
#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
static IOM_CUST_KEY_REP_ITVL_ITEM_T at_g_cust_key_rep_itvl[MAX_CUSTOM_KEY_REPEAT_ITVL_SETTING_NUM] = {{0}};
static UINT32                       ui4_g_cust_key_num = 0;
#endif

/* For auto sleep timer */
static HANDLE_T             h_prompt_timer = NULL_HANDLE;
static HANDLE_T             h_sleep_timer = NULL_HANDLE;
static AUTO_SLEEP_TIMER_T   t_auto_sleep_timer;
static BOOL                 b_prompt_timer_running = FALSE;
static BOOL                 b_sleep_timer_running = FALSE;

static int    TTYMT3_FD;
static struct termios      t_term_factory;
static struct termios      t_term_cli;
static BOOL      b_ttymt3_opened = FALSE;
static BOOL	     b_magic_char_set = FALSE;                                                   

static UINT32 _dfb2mw_NEC_keymaptbl[NEC_KEY_NUM][2] =
{
    {DFB_BTN_RED         , BTN_RED         },
    {DFB_BTN_GREEN       , BTN_GREEN       },
    {DFB_BTN_POWER       , BTN_POWER       },
    {DFB_BTN_YELLOW      , BTN_YELLOW      },
    {DFB_BTN_ZOOM        , BTN_ZOOM        },
    {DFB_BTN_BLUE        , BTN_BLUE        },
    {DFB_BTN_TIMER       , BTN_TIMER       },
    {DFB_BTN_MEM_CARD    , BTN_MEM_CARD    },
    {DFB_BTN_DIGIT_3     , BTN_DIGIT_3     },
    {DFB_BTN_DIGIT_2     , BTN_DIGIT_2     },
    {DFB_BTN_DIGIT_1     , BTN_DIGIT_1     },
    {DFB_BTN_ENTER       , BTN_SELECT      },
    {DFB_BTN_DIGIT_9     , BTN_DIGIT_9     },
    {DFB_BTN_DIGIT_8     , BTN_DIGIT_8     },
    {DFB_BTN_DIGIT_7     , BTN_DIGIT_7     },
    {DFB_BTN_FAV_CH      , BTN_FAV_CH      },
    {DFB_BTN_DIGIT_6     , BTN_DIGIT_6     },
    {DFB_BTN_DIGIT_5     , BTN_DIGIT_5     },
    {DFB_BTN_DIGIT_4     , BTN_DIGIT_4     },
    {DFB_BTN_CURSOR_DOWN , BTN_CURSOR_DOWN },
    {DFB_BTN_MENU        , BTN_MENU        },
    {DFB_BTN_DIGIT_0     , BTN_DIGIT_0     },
    {DFB_BTN_FREEZE      , BTN_FREEZE      },
    {DFB_BTN_RECORD      , BTN_RECORD      },
    {DFB_BTN_PRG_INFO    , BTN_PRG_INFO    },
    {DFB_BTN_EPG         , BTN_EPG         },
    {DFB_BTN_EXIT        , BTN_EXIT        },
    {DFB_BTN_PREV        , BTN_PREV        },
    {DFB_BTN_CURSOR_RIGHT, BTN_CURSOR_RIGHT},
    {DFB_BTN_CURSOR_UP   , BTN_CURSOR_UP   },
    {DFB_BTN_CURSOR_LEFT , BTN_CURSOR_LEFT },
    {DFB_BTN_P_EFFECT    , BTN_P_EFFECT    },
    {DFB_BTN_S_EFFECT    , BTN_S_EFFECT    },
    {DFB_BTN_PIP_SIZE    , BTN_PIP_SIZE    },
    {DFB_BTN_MTS         , BTN_MTS         },
    {DFB_BTN_REPEAT      , BTN_REPEAT      },
    {DFB_BTN_SUB_TITLE   , BTN_SUB_TITLE   },
    {DFB_BTN_TITLE_PBC   , BTN_TITLE_PBC   },
    {DFB_BTN_SWAP        , BTN_SWAP        },
    {DFB_BTN_FAVORITE    , BTN_FAVORITE    },
    {DFB_BTN_ADD_ERASE   , BTN_ADD_ERASE   },
    {DFB_BTN_NEXT        , BTN_NEXT        },
    {DFB_BTN_VOL_UP      , BTN_VOL_UP      },
    {DFB_BTN_FR          , BTN_FR          },
    {DFB_BTN_PRG_DOWN    , BTN_PRG_DOWN    },
    {DFB_BTN_FF          , BTN_FF          },
    {DFB_BTN_VOL_DOWN    , BTN_VOL_DOWN    },
    {DFB_BTN_PRG_UP      , BTN_PRG_UP      },
    {DFB_BTN_MUTE        , BTN_MUTE        },
    {DFB_BTN_PREV_PRG    , BTN_PREV_PRG    },
    {DFB_BTN_PIP_POP     , BTN_PIP_POP     },
    {DFB_BTN_INPUT_SRC   , BTN_INPUT_SRC   },
    {DFB_BTN_DIGIT_DOT   , BTN_DIGIT_DOT   },
    {DFB_BTN_ROOT_MENU   , BTN_ROOT_MENU   },
    {DFB_BTN_PIP_POS     , BTN_PIP_POS     },
    {DFB_BTN_ASPECT      , BTN_ASPECT      },
    {DFB_BTN_CC          , BTN_CC          },
    {DFB_BTN_SLEEP       , BTN_SLEEP       },
};

#ifdef MW_AUTO_KEYBOARD_TO_IRRCCODE


static UINT32 _keyboard_to_irrc_keymaptbl[KEYBOARD_TO_IRRC_NUM][2] =
{
    {BTN_KB_0             , BTN_DIGIT_0       },
    {BTN_KB_1             , BTN_DIGIT_1       },
    {BTN_KB_2             , BTN_DIGIT_2       },
    {BTN_KB_3             , BTN_DIGIT_3       },
    {BTN_KB_4             , BTN_DIGIT_4       },
    {BTN_KB_5             , BTN_DIGIT_5       },
    {BTN_KB_6             , BTN_DIGIT_6       },
    {BTN_KB_7             , BTN_DIGIT_7       },
    {BTN_KB_8             , BTN_DIGIT_8       },
    {BTN_KB_9             , BTN_DIGIT_9       },
    {BTN_KB_PERIOD        , BTN_DIGIT_DOT     },
    {BTN_KB_F1            , BTN_YELLOW        },
    {BTN_KB_F2            , BTN_RED           },
    {BTN_KB_F3            , BTN_GREEN         },
    {BTN_KB_F4            , BTN_BLUE          },
    {BTN_KB_CURSOR_LEFT   , BTN_CURSOR_LEFT   },
    {BTN_KB_CURSOR_RIGHT  , BTN_CURSOR_RIGHT  },
    {BTN_KB_CURSOR_UP     , BTN_CURSOR_UP     },
    {BTN_KB_CURSOR_DOWN   , BTN_CURSOR_DOWN   },
    {BTN_KB_RETURN        , BTN_SELECT        },
    {BTN_KB_ENTER         , BTN_SELECT        },
    
};

#endif


/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
extern IDirectFBWindow * gl_dfb_get_main_wnd();
extern INT32 x_gl_init(GL_COLORMODE_T e_colormode, UINT32 ui4_width, UINT32 ui4_height);

/*-----------------------------------------------------------------------------
 * Name: _dfb_process_irrc_evt
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT32 _dfb2mw_NEC_keymap(UINT32 dfbkey)
{
    UINT32 mwkey = BTN_INVALID;
    UINT32 i;

    for(i = 0; i < NEC_KEY_NUM; i++)
    {
        if (dfbkey == _dfb2mw_NEC_keymaptbl[i][0])
        {
            mwkey = _dfb2mw_NEC_keymaptbl[i][1];
            break;
        }
    }

    return mwkey;
}

/*-----------------------------------------------------------------------------
 * Name: _dfb_process_irrc_evt
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT32 _dfb2mw_kb_keymap(UINT32 dfbkey, UINT32 dfbmod)
{
    UINT32 mwkey = BTN_INVALID;
#ifdef MW_AUTO_KEYBOARD_TO_IRRCCODE
    UINT32 i;
#endif

    if (dfbkey >= DIKS_NULL && dfbkey <= DIKS_DELETE)
    {
        mwkey = KEY_GROUP_KEYBOARD | (dfbkey & 0xff);
    }
    else if (dfbkey >= DIKS_CURSOR_LEFT && dfbkey <= DIKS_PAUSE)
    {
        mwkey = KEY_GROUP_KEYBOARD | (dfbkey & 0xf) | 0x80;
    }
    else if (dfbkey >= DIKS_F1 && dfbkey <= DIKS_F12)
    {
        mwkey = KEY_GROUP_KEYBOARD | (dfbkey & 0xf) | 0x90;
    }
    else if (dfbkey >= DIKS_CAPS_LOCK && dfbkey <= DIKS_SCROLL_LOCK)
    {
        mwkey = KEY_GROUP_KEYBOARD | (dfbkey & 0xf) | 0xA0;
    }

    if (mwkey != BTN_INVALID)
    {
        if (dfbmod & DIMM_SHIFT   ) { mwkey |= BTN_KB_SHIFT   ; }
        if (dfbmod & DIMM_CONTROL ) { mwkey |= BTN_KB_CONTROL ; }
        if (dfbmod & DIMM_ALT     ) { mwkey |= BTN_KB_ALT     ; }
        if (dfbmod & DIMM_ALTGR   ) { mwkey |= BTN_KB_ALTGR   ; }
        if (dfbmod & DIMM_META    ) { mwkey |= BTN_KB_META    ; }
        if (dfbmod & DIMM_SUPER   ) { mwkey |= BTN_KB_SUPER   ; }
        if (dfbmod & DIMM_HYPER   ) { mwkey |= BTN_KB_HYPER   ; }
    }

#ifdef MW_AUTO_KEYBOARD_TO_IRRCCODE
    for(i = 0; i < KEYBOARD_TO_IRRC_NUM; i++)
    {
        if (mwkey == _keyboard_to_irrc_keymaptbl[i][0])
        {
            mwkey = _keyboard_to_irrc_keymaptbl[i][1];
            break;
        }
    }    
#endif
    return mwkey;
}

#define _MAX_FILE_NUM   3
#define _FD_VALID(_fd)  ((_fd) >= 0 && (_fd) <= 2)
#define _FD_TO_IDX(_fd) (_fd)

#ifndef IOM_DISABLE_UART_FACTORY_MODE


/*-----------------------------------------------------------------------------
 * Name: _set_non_canonical_input
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _set_non_canonical_input(UINT16 ui2_id, BOOL b_enable)
{
#if 1
 
    static struct termios  at_old_term_io[UART_PORT_NUM] = {{0}};
    int fd=0;

    ui2_id = UART_GET_PORT(ui2_id);
   
    if(ui2_id==1)
    {
        fd = open(UART_DEV_NAME(ui2_id), O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            return IOMR_DEV_OPEN_FAILED;
        }
    }
    if (b_enable)
    {
        if(ui2_id==0)
        {
            
            if(tcgetattr(TTYMT3_FD, &t_term_cli) == -1)
            {
                
                printf("{IOM}:%d ERR: factory get attribute fail, errno: %d\n", __LINE__, errno);
                return IOMR_DEV_QUERY_FAILED;
            }    
            if(tcsetattr(TTYMT3_FD, TCSAFLUSH, &t_term_factory) == -1)
            {
            
                printf("{IOM}:%d ERR: factory set attribute fail, errno: %d\n", __LINE__, errno);
                
                return IOMR_DEV_SET_FAILED;
            }
            
            if(ioctl(TTYMT3_FD,0xffff,7) < 0) 
            {    
                printf("{IOM}:%d ERR: factory mode transition fail, errno: %d\n", __LINE__, errno);
                
                return IOMR_DEV_SET_FAILED;
            }
        
        }
        else
        {
            if(tcgetattr(fd, &at_old_term_io[ui2_id]) == -1)
            {
                close(fd);
                return IOMR_DEV_GET_FAILED;
            }
            else
            {
                struct termios t_term = {0};
    
                t_term.c_cflag = at_old_term_io[ui2_id].c_cflag; /* keep baud-rate */
                t_term.c_iflag = IGNPAR;    /* raw input with parity error ignored */
                    t_term.c_oflag = 0;         /* raw output */
                t_term.c_lflag = 0;         /* non-canonical, no echo, no signal.. */
                t_term.c_cc[VTIME] = UART_FACTORY_CMD_INTER_BYTE_TIMEOUT;
                t_term.c_cc[VMIN]  = UART_FACTORY_CMD_BUF_LEN;
    
                tcdrain(fd);
                tcflush(fd, TCIOFLUSH);

                if(tcsetattr(fd, TCSANOW, &t_term) == -1)
                {
                    close(fd);
                    return IOMR_DEV_SET_FAILED;
                }
            }
        }

    }
    else
    {
        if (ui2_id == 0)
        {    
    
            if(tcsetattr(TTYMT3_FD, TCSAFLUSH, &t_term_cli) == -1)
            {

                printf("{IOM}:%d ERR: factory set attribute fail, errno: %d\n", __LINE__, errno);
                return IOMR_DEV_SET_FAILED;
            }                  
            if(ioctl(TTYMT3_FD,0xffff,0) < 0) 
            {
                printf("{IOM}:%d ERR: factory set attribute fail, errno: %d\n", __LINE__, errno);
                return IOMR_DEV_SET_FAILED;
            }

        }
        else
        {
            tcflush(fd, TCIOFLUSH);

            if(tcsetattr(fd, TCSANOW, &at_old_term_io[ui2_id]) == -1)
            {
                close(fd);
                return IOMR_DEV_SET_FAILED;
            }
    
            x_memset(&at_old_term_io[ui2_id], 0, sizeof(struct termios));
        }

    }
    if(ui2_id==1)
    {
            close(fd);
    }
#endif
    return IOMR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: iom_is_init
 *
 * Description: This API returns IO Manager initialization status.
 *
 * Inputs:  e_irrc_cond     The condition of IRRC.
 *
 * Outputs: -
 *
 * Returns: TRUE        IO Manager is initialized.
 *          FALSE       IO Manager has not been initialized.
 ----------------------------------------------------------------------------*/
BOOL iom_is_init(VOID)
{
    return (b_iom_init);
}

/*-----------------------------------------------------------------------------
 * Name: iom_get_in_dev_info_struct
 *
 * Description: This API returns the address of input device information
 *              structure.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of input device information structure.
 ----------------------------------------------------------------------------*/
DEV_INFO_T* iom_get_in_dev_info_struct(VOID)
{
    return at_in_dev;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_out_dev_info_struct
 *
 * Description: This API returns the address of output device information
 *              structure.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of output device information structure.
 ----------------------------------------------------------------------------*/
DEV_INFO_T* iom_get_out_dev_info_struct(VOID)
{
    return at_out_dev;
}

/*-----------------------------------------------------------------------------
 * Name: iom_get_io_dev_info_struct
 *
 * Description: This API returns the address of IO device information
 *              structure.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of IO device information structure.
 ----------------------------------------------------------------------------*/
DEV_INFO_T* iom_get_io_dev_info_struct(VOID)
{
    return at_io_dev;
}
    
/*-----------------------------------------------------------------------------
 * Name: iom_init
 *
 * Description: This API initializes the IO Manager.
 *
 * Inputs:  b_apdemon_irrc              Indicates if IRRC events come from
 *                                      Apdemon or not.
 *          pt_iom_thread_descr         References a thread descriptor
 *                                      structure.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_ALREADY_INIT           The IO Manager has already been
 *                                      initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_MEM_ALLOC_FAILED       Memory allocation is failed.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_init(BOOL            b_apdemon_irrc_enabled,
               THREAD_DESCR_T* pt_iom_thread_descr,
               GL_COLORMODE_T  e_colormode,
               UINT32          ui4_width,
               UINT32          ui4_height)
{
    INT32       i4_return;
    i4_return = iom_op_init(b_apdemon_irrc_enabled, pt_iom_thread_descr, e_colormode, ui4_width, ui4_height);
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_open
 *
 * Description: This API opens a device whose attribute can be input, output,
 *              or both. An application is allowed to do multiple opens to a
 *              device simultaneously.
 *
 * Inputs:  e_dev_type      The device type.
 *          ui2_id          The device id.
 *          ps_name         The device name.
 *          pv_setting      The setting to the device if required.
 *          pv_tag          Private tag value which must be returned in the
 *                          notify function. It can be set to NULL.
 *          pf_nfy          References the caller's notify function.
 *
 * Outputs: ph_dev          A handle referencing the opened device.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
  *                                 invalid data.
 ----------------------------------------------------------------------------*/
INT32 x_iom_open(IOM_DEV_TYPE_T e_dev_type,
                 UINT16         ui2_id,
                 const CHAR*    ps_name,
                 VOID*          pv_setting,
                 VOID*          pv_tag,
                 x_iom_nfy_fct  pf_nfy,
                 HANDLE_T*      ph_dev)
{
    INT32   i4_return = IOMR_OK;

    DBG_API(("{IOM} x_iom_open: dev_type=%d, id=0x%x\n\r", e_dev_type, ui2_id));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    if (ph_dev == NULL)
    {
        return IOMR_INV_ARG;
    }

    switch (e_dev_type)
    {
        case IOM_DEV_TYPE_IN_IRRC:
        case IOM_DEV_TYPE_IN_IRRK:
        case IOM_DEV_TYPE_IN_IRRM:
        case IOM_DEV_TYPE_IN_DBG:
            i4_return = iom_op_open_in(e_dev_type,
                                       ui2_id,
                                       ps_name,
                                       pv_setting,
                                       pv_tag,
                                       pf_nfy,
                                       ph_dev);
            break;

        case IOM_DEV_TYPE_OUT_IND_POWER:
        case IOM_DEV_TYPE_OUT_FPD:
        case IOM_DEV_TYPE_OUT_IRB:
            i4_return = iom_op_open_out(e_dev_type,
                                        ui2_id,
                                        ps_name,
                                        pv_setting,
                                        pv_tag,
                                        pf_nfy,
                                        ph_dev);
            break;

        case IOM_DEV_TYPE_IO_UART:
        case IOM_DEV_TYPE_IO_I2C:
            i4_return = iom_op_open_io(e_dev_type,
                                       ui2_id,
                                       ps_name,
                                       pv_setting,
                                       pv_tag,
                                       pf_nfy,
                                       ph_dev);
            break;

        default:
            i4_return = IOMR_INV_ARG;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_close
 *
 * Description: This API closes an opened device referenced by h_dev.
 *
 * Inputs:  h_dev   A handle referencing the device.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 x_iom_close(HANDLE_T h_dev)
{
    INT32           i4_return;
    HANDLE_TYPE_T   e_hdl_type;

    DBG_API(("{IOM} x_iom_close: h_dev=0x%x\n\r", h_dev));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    i4_return = x_handle_get_type(h_dev, &e_hdl_type);

    if (i4_return != HR_OK)
    {
        DBG_ERROR(("{IOM} ERR: x_handle_get_type() failed\n\r"));
        return IOMR_INV_HANDLE;
    }

    switch (e_hdl_type)
    {
        case IOM_HANDLE_TYPE_IN:
            i4_return = iom_op_close_in(h_dev);
            break;

        case IOM_HANDLE_TYPE_OUT:
            i4_return = iom_op_close_out(h_dev);
            break;

        case IOM_HANDLE_TYPE_IO:
            i4_return = iom_op_close_io(h_dev);
            break;

        default:
            i4_return = IOMR_INV_HANDLE;
    }

    return (i4_return);
}

/* In future, it should be sync with module owner, tmp duplicated one's*/
/*-----------------------------------------------------------------------------
 * Name: iom_inp_dptr_set_key_mon
 *
 * Description: This API enables or disables key monitor.
 *
 * Inputs:  b_ctrl      New status (on/off) of key monitor.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_inp_dptr_set_key_mon(BOOL b_ctrl)
{
    iom_lock_db_wr();
    b_is_key_mon_enabled = b_ctrl;
    iom_unlock_db();
}

/*-----------------------------------------------------------------------------
 * Name: iom_inp_dptr_get_evt_src_mask
 *
 * Description: TThis API gets the event source mask information.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The event source mask information. 
 ----------------------------------------------------------------------------*/
UINT64 iom_inp_dptr_get_evt_src_mask(VOID)
{
    return (ui8_evt_src_mask);
}


/*-----------------------------------------------------------------------------
 * Name: iom_inp_dptr_set_evt_src_mask
 *
 * Description: This API sets the event source mask information.
 *
 * Inputs:  pui8_src_mask       The event source mask information. 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_inp_dptr_set_evt_src_mask(UINT64* pui8_src_mask)
{
    ui8_evt_src_mask = *pui8_src_mask;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_evt_src_mask
 *
 * Description: This API gets the event source mask information. The evet
 *              filter must belong to IOM_DEV_TYPE_IN_IRRC device type. Please
 *              note that this mask is a global value controlling all clients.
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *
 * Outputs: pui8_evt_src_mask       References the event source mask 
 *                                  information.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_get_evt_src_mask(HANDLE_T h_dev,
                              UINT64*  pui8_evt_src_mask)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
        (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    *pui8_evt_src_mask = iom_inp_dptr_get_evt_src_mask();

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_set_evt_src_mask
 *
 * Description: This API sets the event source mask information. The event
 *              filter must belong to IOM_DEV_TYPE_IN_IRRC device type. Please
 *              note that this mask is a global value controlling all clients.
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *          pui8_evt_src_mask       References the event source mask 
 *                                  information.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK             Routine successful.
 *          IOMR_INV_HANDLE     The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_set_evt_src_mask(HANDLE_T h_dev,
                              UINT64*  pui8_evt_src_mask)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
        (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    iom_inp_dptr_set_evt_src_mask(pui8_evt_src_mask);

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_get_evt_grp_mask
 *
 * Description: This API gets the event group mask information from an event
 *              filter referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IN_IRRC device type.
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *
 * Outputs: pui8_evt_grp_mask       References the event group mask 
 *                                  information.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_get_evt_grp_mask(HANDLE_T h_dev,
                              UINT64*  pui8_evt_grp_mask)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
        (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    *pui8_evt_grp_mask = pt_evt_filtr->u.s_inp.t_irrc_setting.ui8_evt_grp_mask;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_set_evt_grp_mask
 *
 * Description: This API sets the event group mask information to an event
 *              filter referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IN_IRRC device type.
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *          pui8_evt_grp_mask       References the event group mask 
 *                                  information.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_EVT_GRP_MASK   Event group mask is invalid.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_set_evt_grp_mask(HANDLE_T h_dev,
                              UINT64*  pui8_evt_grp_mask)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Check arguments */
    if (*pui8_evt_grp_mask >= IOM_EVT_GRP_TO_MASK(KEY_GROUP_MAX))
    {
        return IOMR_INV_EVT_GRP_MASK;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
        (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    pt_evt_filtr->u.s_inp.t_irrc_setting.ui8_evt_grp_mask = *pui8_evt_grp_mask;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_get_rpt_evt_itvl
 *
 * Description: This API gets the repeat event interval information from an
 *              event filter referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IN_IRRC device type.
 *
 * Inputs:  h_dev               A handle referencing the event filter.
 *
 * Outputs: pt_rpt_evt_itvl     References the repeat event interval 
 *                              information.
 *
 * Returns: IOMR_OK             Routine successful.
 *          IOMR_INV_HANDLE     The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_get_rpt_evt_itvl(HANDLE_T        h_dev,
                              RPT_EVT_ITVL_T* pt_rpt_evt_itvl)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
        (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    *pt_rpt_evt_itvl = pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_set_rpt_evt_itvl
 *
 * Description: This API sets the repeat event interval information to an
 *              event filter referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IN_IRRC device type.
 *
 * Inputs:  h_dev               A handle referencing the event filter.
 *          pt_rpt_evt_itvl     References the repeat event interval 
 *                              information.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK             Routine successful.
 *          IOMR_INV_HANDLE     The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_set_rpt_evt_itvl(HANDLE_T        h_dev,
                              RPT_EVT_ITVL_T* pt_rpt_evt_itvl)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
        (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl = *pt_rpt_evt_itvl;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_set_uart_magic_char 
 *
 * Description: set magic char
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *          pui4_operation_mode     References the UART operation mode.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/

INT32 iom_op_set_uart_magic_char(HANDLE_T h_dev,
                                     MAGIC_T*  pt_magic)
{
    INT32                       i4_return;               
    EVT_FILTR_T*                pt_evt_filtr;
    UINT8                       ui1_i;
    unsigned int                au4_Magic[6];
    
    iom_lock_db_wr();

    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);
    
    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
    
    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }
                
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IO_UART)
    {
        iom_unlock_db();
        x_dbg_stmt("{IOM} ERR: device type is not IOM_DEV_TYPE_IO_UART\n\r");
        return IOMR_INV_HANDLE;
    }

    if(b_ttymt3_opened==TRUE)
    {
        au4_Magic[0] = pt_magic->ui1_magic_units_count;

        if(pt_magic->ui1_magic_units_count>5)
        {
            x_dbg_stmt("not support magic exceeding 5 units\n"); 
            iom_unlock_db();
            return IOMR_INV_ARG;
        }
        
        for(ui1_i=0;ui1_i<pt_magic->ui1_magic_units_count;ui1_i++)
        
        {
            if(pt_magic->pt_magic_unit[ui1_i].z_magic_len==1)
            {   
                au4_Magic[ui1_i+1] = (unsigned long)(pt_magic->pt_magic_unit[ui1_i].pui1_magic[0]);
            }
            else
            {
                x_dbg_stmt("temporaryly not support magic string \n"); 
                iom_unlock_db();
                return IOMR_INV_ARG;
            }
        }
        if(ioctl(TTYMT3_FD, 0xfffc, au4_Magic) < 0)   
        {  
            x_dbg_stmt("Fail to set /dev/ttyMT3 magic char\n");     
            iom_unlock_db();
            return IOMR_DEV_SET_FAILED;  
        }
        b_magic_char_set = TRUE;
        
    }

    
    iom_unlock_db();
    return IOMR_OK;

}


/*-----------------------------------------------------------------------------
 * Name: x_iom_get
 *
 * Description: This API gets some info from a device referenced by h_dev.
 *
 * Inputs:  h_dev               A handle referencing the device.
 *          e_get_type          Specifies the type of information which shall
 *                              be returned in the buffer, referenced by
 *                              argument pv_get_info.
 *          pz_get_info_len     Contains the length of the buffer which is
 *                              referenced by argument pv_get_info.
 *
 * Outputs: pv_get_info         Contains the retrieved information.
 *          pz_get_info_len     Indicates the length of the valid data in the
 *                              buffer referenced by argument pv_get_info.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          IOMR_INV_HANDLE         The handle is invalid. 
 ----------------------------------------------------------------------------*/
INT32 x_iom_get(HANDLE_T  h_dev,
                IOM_GET_T e_get_type,
                VOID*     pv_get_info,
                SIZE_T*   pz_get_info_len)
{
    INT32           i4_return;
    HANDLE_TYPE_T   e_hdl_type;

    DBG_API(("{IOM} x_iom_get: h_dev=0x%x, get_type=%d\n\r", h_dev, e_get_type));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    /* Get the handle type */
    i4_return = x_handle_get_type(h_dev, &e_hdl_type);

    if (i4_return != HR_OK)
    {
        return IOMR_INV_HANDLE;
    }

    switch (e_get_type)
    {
#if 1
        case IOM_GET_IRRC_EVT_SRC_MASK:
            if ((pv_get_info == NULL) || (*pz_get_info_len != sizeof(UINT64)))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }
            
            i4_return = iom_op_get_evt_src_mask(h_dev, (UINT64*)pv_get_info);            
            break;
                    
        case IOM_GET_IRRC_EVT_GRP_MASK:
            if ((pv_get_info == NULL) || (*pz_get_info_len != sizeof(UINT64)))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_get_evt_grp_mask(h_dev, (UINT64*)pv_get_info);
            break;

        case IOM_GET_IRRC_RPT_EVT_ITVL:
            if ((pv_get_info == NULL) || (*pz_get_info_len != sizeof(RPT_EVT_ITVL_T)))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }
            
            i4_return = iom_op_get_rpt_evt_itvl(h_dev, (RPT_EVT_ITVL_T*)pv_get_info);
            break;
#endif

        case IOM_GET_IRRC_SYS_CODE_FILTER:
            if ((pv_get_info == NULL) || (*pz_get_info_len != sizeof(IOM_SYS_CODE_FILTER_T)))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }
            
            /*i4_return = iom_op_get_sys_code_filter(h_dev, (IOM_SYS_CODE_FILTER_T*)pv_get_info);*/
            /* waiting for chunyan wang */
            break;
            
        case IOM_GET_IRRC_PROTOCOL:
            if ((pv_get_info == NULL) || (*pz_get_info_len != sizeof(IOM_IRRC_PROTOCOL_T)))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }
            
            i4_return = iom_op_get_irrc_protocol(h_dev, (IOM_IRRC_PROTOCOL_T*)pv_get_info); 
            break;
            
        case IOM_GET_UART_SETTING:
            if ((pv_get_info == NULL) || (*pz_get_info_len != sizeof(UART_SETTING_T)))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IO)
            {
                return IOMR_INV_HANDLE;
            }
            
            i4_return = iom_op_get_uart_setting(h_dev, (UART_SETTING_T*)pv_get_info);
            break;

        case IOM_GET_UART_OPERATION_MODE:
            if ((pv_get_info == NULL) || (*pz_get_info_len != sizeof(UINT32)))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IO)
            {
                return IOMR_INV_HANDLE;
            }
            
            i4_return = iom_op_get_uart_operation_mode(h_dev, (UINT32*)pv_get_info);
            break;
            
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
        case IOM_GET_IRRC_DISABLE_EVTCODE_CNT:
            if ((NULL == pv_get_info) || (*pz_get_info_len != sizeof(SIZE_T)))
            {
               return IOMR_INV_ARG;
            }

            if( IOM_HANDLE_TYPE_IN !=  e_hdl_type)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_get_irrc_disablekeys_cnt(h_dev, (SIZE_T*)pv_get_info);
            break;
            
        case IOM_GET_IRRC_DISABLE_EVTCODE_INFO:
            if ((NULL == pv_get_info) || (*pz_get_info_len != sizeof(DISABLE_EVTCODE_T)))
            {
                return IOMR_INV_ARG;
            }

            if( IOM_HANDLE_TYPE_IN !=  e_hdl_type)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_get_irrc_disablekeys_info(h_dev, (DISABLE_EVTCODE_T*)pv_get_info);
            break;
#endif
            
        default:
            i4_return = IOMR_INV_ARG;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_set
 *
 * Description: This API sets some info to a device referenced by h_dev.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          e_set_type      Specifies the type of information which is
 *                          referenced by argument pv_set_info.
 *          pv_set_info     Contains the set information.
 *          z_set_info_len  Indicates the length of the data in the buffer
 *                          referenced by argument pv_set_info.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 x_iom_set(HANDLE_T    h_dev,
                IOM_SET_T   e_set_type,
                const VOID* pv_set_info,
                SIZE_T      z_set_info_len)
{
    INT32           i4_return;
    HANDLE_TYPE_T   e_hdl_type;
    DBG_API(("{IOM} x_iom_set: h_dev=0x%x, set_type=%d\n\r", h_dev, e_set_type));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    /* Get the handle type */
    i4_return = x_handle_get_type(h_dev, &e_hdl_type);

    if (i4_return != HR_OK)
    {
        return IOMR_INV_HANDLE;
    }

    switch (e_set_type)
    {
#if 1
        case IOM_SET_IRRC_EVT_SRC_MASK:
            if (z_set_info_len != sizeof(UINT64))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_set_evt_src_mask(h_dev, (UINT64*)pv_set_info);            
            break;
                    
        case IOM_SET_IRRC_EVT_GRP_MASK:
            if (z_set_info_len != sizeof(UINT64))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_set_evt_grp_mask(h_dev, (UINT64*)pv_set_info);
            break;

        case IOM_SET_IRRC_RPT_EVT_ITVL:
            if (z_set_info_len != sizeof(RPT_EVT_ITVL_T))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_set_rpt_evt_itvl(h_dev, (RPT_EVT_ITVL_T*)pv_set_info);
            break;
#endif

        case IOM_SET_IRRC_SYS_CODE_FILTER:
            if (z_set_info_len != sizeof(IOM_SYS_CODE_FILTER_T))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }

            /*i4_return = iom_op_set_sys_code_filter(h_dev, (IOM_SYS_CODE_FILTER_T*)pv_set_info);*/
            /* waiting for chunyan wang */
            break;
            
        case IOM_SET_IRRC_PROTOCOL:
            if (z_set_info_len != sizeof(IOM_IRRC_PROTOCOL_T))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_set_irrc_protocol(h_dev, (IOM_IRRC_PROTOCOL_T*)pv_set_info);
            break;

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
        case IOM_SET_IRRC_DISABLE_EVTCODE_INFO:
            if (z_set_info_len != sizeof(DISABLE_EVTCODE_T))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IN)
            {
              return IOMR_INV_HANDLE;
            }   

            i4_return = iom_op_set_irrc_disablekeys_info(h_dev, (DISABLE_EVTCODE_T*)pv_set_info);
            break;
#endif
            
        case IOM_SET_UART_SETTING:
            if (z_set_info_len != sizeof(UART_SETTING_T))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IO)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_set_uart_setting(h_dev, (UART_SETTING_T*)pv_set_info);
            break;
            
        case IOM_SET_UART_OPERATION_MODE:
            if (z_set_info_len != sizeof(UINT32))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IO)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_set_uart_operation_mode(h_dev, (UINT32*)pv_set_info);
            break;
                        
        case IOM_SET_UART_MAGIC_CHAR:
            if (z_set_info_len != sizeof(MAGIC_T))
            {
                return IOMR_INV_ARG;
            }

            if (e_hdl_type != IOM_HANDLE_TYPE_IO)
            {
                return IOMR_INV_HANDLE;
            }

            i4_return = iom_op_set_uart_magic_char(h_dev, (MAGIC_T*)pv_set_info);
            break;    
            
        default:
            i4_return = IOMR_INV_ARG;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_output
 *
 * Description: This API sends an event to an output or IO driver.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          pv_otp_info     The output event information.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          IOMR_DEV_BUSY           Device is busy.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 x_iom_output(HANDLE_T h_dev,
                   VOID*    pv_otp_info)
{
    INT32               i4_return;
    HANDLE_TYPE_T       e_dev_type;
    DEV_INFO_T*         pt_dev = NULL; 
    EVT_FILTR_T*        pt_evt_filtr;
    INT32               fd;

    DBG_API(("{IOM} x_iom_output: h_dev=0x%x\n\r", h_dev));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    if (pv_otp_info == NULL)
    {
        return IOMR_INV_ARG;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Check the object type */
    i4_return = x_handle_get_type(h_dev, &e_dev_type);

    if ((i4_return != HR_OK) || 
        ((e_dev_type != IOM_HANDLE_TYPE_OUT) &&
         (e_dev_type != IOM_HANDLE_TYPE_IO)))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: x_handle_get_type() failed\n\r"));
        return IOMR_INV_HANDLE;
    }

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: handle_get_obj() failed\n\r"));
        return IOMR_INV_HANDLE;
    }

    /* Check if device is busy */
    if (e_dev_type == IOM_HANDLE_TYPE_OUT)
    {
        pt_dev = iom_get_out_dev_info_struct();
        pt_dev = &pt_dev[pt_evt_filtr->ui1_dev_idx];
    }    
    else if (e_dev_type == IOM_HANDLE_TYPE_IO)
    {    
        pt_dev = iom_get_io_dev_info_struct();
        pt_dev = &pt_dev[pt_evt_filtr->ui1_dev_idx];        
    } 
                        
    if (pt_dev->u.b_busy)
    {
        iom_unlock_db();
        return IOMR_DEV_BUSY;
    }
    else
    {
        pt_dev->u.b_busy = TRUE;
    }
                       
    switch (pt_dev->e_dev_type)
    {
        case IOM_DEV_TYPE_OUT_IND_POWER:
            DBG_ERROR(("{IOM} ERR: not support IOM_DEV_TYPE_OUT_IND_POWER\n\r"));
            break;

        case IOM_DEV_TYPE_OUT_FPD:
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: not support IOM_DEV_TYPE_OUT_FPD\n\r"));
            return IOMR_NOT_SUPPORT;

        case IOM_DEV_TYPE_IO_UART:
            if(UART_GET_PORT(pt_dev->ui2_id)==1)
            {
                fd = open(UART_DEV_NAME(pt_dev->ui2_id), O_RDWR|O_NOCTTY|O_NDELAY);
                if (-1 == fd)
                {
                    iom_unlock_db();
                    DBG_ERROR(("{IOM} ERR: device open fail\n\r"));
                    return IOMR_DEV_OPEN_FAILED;
                }
            
                write(fd, 
                    ((IOM_UART_T*)pv_otp_info)->pui1_data,
                    ((IOM_UART_T*)pv_otp_info)->z_len);
                close(fd);
            }
            else
            {
                write(TTYMT3_FD, 
                    ((IOM_UART_T*)pv_otp_info)->pui1_data,
                    ((IOM_UART_T*)pv_otp_info)->z_len);
            }
            break;

        default:
            iom_unlock_db();
            return IOMR_INV_ARG;
    }

    /* Notify the caller if required */
    if ((pt_dev->e_dev_type == IOM_DEV_TYPE_IO_UART) &&
        (pt_dev->u.b_busy))
    {
        (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                               IOM_NFY_COND_XMT_COMPLETE,
                               0,
                               0);
    }

    pt_dev->u.b_busy = FALSE;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_send_evt
 *
 * Description: This API is called in order to generate a pair of button-down
 *              and button-up events.
 *
 * Inputs:  ui4_evt_code    Contains the event code.
 *          ui4_itvl        Contains the interval between button-down and 
 *                          button-up events in ms.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_MSGQ_FULL          Message Queue is full.
 ----------------------------------------------------------------------------*/
INT32 x_iom_send_evt(UINT32 ui4_evt_code,
                     UINT32 ui4_itvl)
{
    DFBUserEvent   event;

    event.clazz = DFEC_USER;
    event.type    = IOM_BUTTON_DOWN;
    event.data    = (VOID *)ui4_evt_code;

    if(keybuffer == NULL)
    {
        return IOMR_NOT_INIT;
    }    
    keybuffer->PostEvent( keybuffer, DFB_EVENT(&event) );

    if (ui4_itvl > 0)
    {
        x_thread_delay(ui4_itvl);
    }        

    event.clazz = DFEC_USER;
    event.type    = IOM_BUTTON_UP;
    event.data    = (VOID *)ui4_evt_code;
    keybuffer->PostEvent( keybuffer, DFB_EVENT(&event) );


    return IOMR_OK;       
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_send_evt_single
 *
 * Description: This API is called to generate either a button-down or a
 *              button-up event.
 *
 * Inputs:  ui4_evt_code    Contains the event code.
 *          b_btn_down      Contains the interval between button-down and 
 *                          button-up events in ms.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_MSGQ_FULL          Message Queue is full.
 ----------------------------------------------------------------------------*/
INT32 x_iom_send_evt_single(UINT32 ui4_evt_code,
                            BOOL   b_btn_down)
{
    DFBUserEvent   event;

    event.clazz = DFEC_USER;
    event.type    = b_btn_down ? IOM_BUTTON_DOWN : IOM_BUTTON_UP;
    event.data    = (VOID *)ui4_evt_code;

    if(keybuffer == NULL)
    {
        return IOMR_NOT_INIT;
    }    
    keybuffer->PostEvent( keybuffer, DFB_EVENT(&event) );

    return IOMR_OK;       
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_send_raw_data
 *
 * Description: This API sends a raw data event to IO Manager.
 *
 * Inputs:  ui4_raw_data    Contains the raw data.
 *          ui4_itvl        Contains the interval between button-down and 
 *                          button-up events in ms.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_MSGQ_FULL          Message Queue is full.
 ----------------------------------------------------------------------------*/
INT32 x_iom_send_raw_data(UINT32 ui4_raw_data,
                          UINT32 ui4_itvl)
{
#if 0
    INT32       i4_return;
    INP_EVT_T   t_inp_evt;

    DBG_API(("{IOM} x_iom_send_raw_data: raw_data=0x%08x\n\r", ui4_raw_data));
    
    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }
    
    t_inp_evt.e_evt_type = INP_EVT_TYPE_IRRC;
    t_inp_evt.pv_tag = NULL;
    t_inp_evt.u.s_irrc.ui4_evt_code = BTN_INVALID;
    x_memcpy((VOID*)&t_inp_evt.u.s_irrc.t_data.aui1_data[0], (VOID*)&ui4_raw_data, 4);

    t_inp_evt.e_cond = IOM_NFY_COND_BTN_DOWN;
    i4_return = x_msg_q_send(*ph_iom_inp_msgq,
                             &t_inp_evt,
                             sizeof(INP_EVT_T),
                             IOM_MSGQ_DEFAULT_PRIORITY);
                             
    if (i4_return != OSR_OK)      
    {
        return IOMR_MSGQ_FULL;
    }                                       

    if (ui4_itvl > 0)
    {
        x_thread_delay(ui4_itvl);
    }        

    t_inp_evt.e_cond = IOM_NFY_COND_BTN_UP;
    i4_return = x_msg_q_send(*ph_iom_inp_msgq,
                             &t_inp_evt,
                             sizeof(INP_EVT_T),
                             IOM_MSGQ_DEFAULT_PRIORITY);
                 
    if (i4_return != OSR_OK)      
    {
        return IOMR_MSGQ_FULL;
    }              
 #endif   
    return IOMR_OK;       
}


/*-----------------------------------------------------------------------------
 * Name: x_iom_convert_raw_data_to_evt_code
 *
 * Description: This API converted IRRC raw data to an event code.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          pt_data         Contains the raw date to be converted.
 *
 * Outputs: pt_data         Contains the event code after conversion.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_INV_HANDLE         The handle is invalid.
 *          IOMR_DEV_SET_FAILED     Device get operation is failed. 
 ----------------------------------------------------------------------------*/
INT32 x_iom_convert_raw_data_to_evt_code(HANDLE_T                    h_dev,
                                         IOM_RAW_DATA_TO_EVT_CODE_T* pt_data)
{
    INT32           i4_return;  
    EVT_FILTR_T*    pt_evt_filtr;
    DFBInputDeviceKeySymbol e_symbol;

    DBG_API(("{IOM} x_iom_convert_raw_data_to_evt_code: h_dev=0x%x\n\r", h_dev));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    /* Unlock DB */
    iom_unlock_db();

    /* Convert raw data to event code */
    e_symbol = _NECCommand[pt_data->ui1_cmd_code];
    pt_data->ui4_evt_code = _dfb2mw_NEC_keymap(e_symbol);       

    return IOMR_OK;
}    


/*-----------------------------------------------------------------------------
 * Name: x_iom_convert_raw_bits_to_evt_code
 *
 * Description: This API converted IRRC raw bits to an event code.
 *
 * Inputs:  h_dev           A handle referencing the device.
 *          pt_data         Contains the raw bits to be converted.
 *
 * Outputs: pt_data         Contains the event code after conversion.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized. 
 *          IOMR_INV_HANDLE         The handle is invalid.
 *          IOMR_DEV_SET_FAILED     Device get operation is failed. 
 ----------------------------------------------------------------------------*/
INT32 x_iom_convert_raw_bits_to_evt_code(HANDLE_T                    h_dev,
                                         IOM_RAW_BITS_TO_EVT_CODE_T* pt_data)
{
    INT32           i4_return;
    EVT_FILTR_T*    pt_evt_filtr;
    UINT32                  ui4_command = 0;
    DFBInputDeviceKeySymbol e_symbol;

    DBG_API(("{IOM} x_iom_convert_raw_data_to_evt_code: h_dev=0x%x\n\r", h_dev));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    /* Unlock DB */
    iom_unlock_db();

    /* Convert raw bits data to event code */
    if ((pt_data->ui4_raw_bits & 0x0000FFFF) == 0xE31C) /*NEC*/
    {
        ui4_command = ((pt_data->ui4_raw_bits >> 16) & 0x00FF);
        if(ui4_command >= NECCommand_Number)
        {
            return IOMR_INV_ARG;
        }
        e_symbol = _NECCommand[ui4_command];
    }
    else if (pt_data->ui4_raw_bits == DFB_BTN_NONE) /*NONE*/
    {
        e_symbol = DIKS_NULL;
    }
    else /*other protocol*/
    {
        pt_data->ui4_evt_code = BTN_INVALID;
        return IOMR_NOT_SUPPORT;
    }

    pt_data->ui4_evt_code = _dfb2mw_NEC_keymap(e_symbol);    

    return IOMR_OK;
}    


#if !IOM_REMOVE_APDEMON_IRRC
/*-----------------------------------------------------------------------------
 * Name: x_iom_apdemon_send_evt
 *
 * Description: This API is called by Apdemon in response to received IRRC
 *              events from ATV Manager.
 *
 * Inputs:  ui4_evt_code    The event code.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_NOT_INIT           The IO Manager is not initialized.
 *          IOMR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 ----------------------------------------------------------------------------*/
INT32 x_iom_apdemon_send_evt(UINT32 ui4_evt_code)
{
#if 0
    INT32           i4_return;
    DEV_INFO_T*     pt_dev = NULL;  

    DBG_API(("{IOM} x_iom_apdemon_send_evt: evt_code=0x%08x\n\r", ui4_evt_code));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    /* Check if IRRC input is enabled */
    if (b_in_irrc == FALSE)
    {
        return IOMR_OK;
    }

    if (ui4_evt_code == 0)
    {
        return IOMR_INV_ARG;
    }

    /* Apdemon IRRC is not specified or hasn't been opened by applications yet */
    if (!b_apdemon_irrc ||
        ui1_apdemon_inp_dev_idx > IOM_MAX_DEV)
    {
        return IOMR_OK;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();

    pt_dev = iom_get_in_dev_info_struct();
    
    /* Check if a button-repeat event is received */
    if ((t_apdemon_evt.e_cond == IOM_NFY_COND_BTN_DOWN) &&
        (pt_dev[ui1_apdemon_inp_dev_idx].u.i1_btn_down_cnt > 0))
    {
        /* Stop the timer */
        i4_return = x_timer_stop(h_apdemon_timer);

        if (i4_return != OSR_OK)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: x_timer_stop() failed\n\r"));
            dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_TIMER);
        }

        if (ui4_evt_code == t_apdemon_evt.u.s_irrc.ui4_evt_code) /* Same event */
        {
            /* Restart the timer */
            i4_return = x_timer_start(h_apdemon_timer,
                                      IOM_APDEMON_IRRC_TIMEOUT,
                                      X_TIMER_FLAG_ONCE,
                                      iom_op_apdemon_timer_cb,
                                      NULL);

            if (i4_return != OSR_OK)
            {
                iom_unlock_db();
                DBG_ERROR(("{IOM} ERR: x_timer_start() failed\n\r"));
                dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_TIMER);
            }

            return IOMR_OK;
        }
        else /* New event */
        {
            /* Send the button-up of previous event to input dispatcher first */
            t_apdemon_evt.e_evt_type = INP_EVT_TYPE_IRRC;   
            t_apdemon_evt.pv_tag = &(pt_dev[ui1_apdemon_inp_dev_idx].ui4_tag);         
            t_apdemon_evt.e_cond = IOM_NFY_COND_BTN_UP;

            x_msg_q_send(*ph_iom_inp_msgq,
                         &t_apdemon_evt,
                         sizeof(INP_EVT_T),
                         IOM_MSGQ_DEFAULT_PRIORITY);
        }
    }

    /* Send received event to input dispatcher, then start a timer */
    t_apdemon_evt.e_evt_type = INP_EVT_TYPE_IRRC;
    t_apdemon_evt.pv_tag = &(pt_dev[ui1_apdemon_inp_dev_idx].ui4_tag);
    t_apdemon_evt.e_cond = IOM_NFY_COND_BTN_DOWN;
    t_apdemon_evt.u.s_irrc.ui4_evt_code = ui4_evt_code;
    x_memset(&t_inp_evt.u.s_irrc.t_data, 0 , sizeof(IRRC_DATA_T));

    x_msg_q_send(*ph_iom_inp_msgq,
                 &t_apdemon_evt,
                 sizeof(INP_EVT_T),
                 IOM_MSGQ_DEFAULT_PRIORITY);

    i4_return = x_timer_start(h_apdemon_timer,
                              IOM_APDEMON_IRRC_TIMEOUT,
                              X_TIMER_FLAG_ONCE,
                              iom_op_apdemon_timer_cb,
                              NULL);

    if (i4_return != OSR_OK)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: x_timer_start() failed\n\r"));
        dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_TIMER);
    }

    /* Unlock DB */
    iom_unlock_db();
#endif    

    return IOMR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: x_iom_set_auto_sleep_timer
 *
 * Description: This API configures the auto sleep timer which will set the
 *              system into standby mode when none of IRRC key is received  
 *              during specific period of time. The timer will be restarted if 
 *              any IRRC key is received before expiration.
 *
 * Inputs:  ui4_sleep_time      Contains the auto sleep time in seconds.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_iom_set_auto_sleep_timer(UINT32 ui4_sleep_time)
{
    AUTO_SLEEP_TIMER_T      t_auto_sleep_timer;

    t_auto_sleep_timer.ui4_prompt_timeout = 0;
    t_auto_sleep_timer.ui4_sleep_timeout = ui4_sleep_time;
    t_auto_sleep_timer.pf_nfy = NULL;
    t_auto_sleep_timer.pv_nfy_tag = NULL;

    return (iom_op_set_auto_sleep_timer(&t_auto_sleep_timer));

}


/*-----------------------------------------------------------------------------
 * Name: x_iom_set_auto_sleep_timer_ex
 *
 * Description: This API configures the auto sleep timer which will set the
 *              system into standby mode when none of IRRC key is received  
 *              during specific period of time. The timer will be restarted if 
 *              any IRRC key is received before expiration. Note auto sleep 
 *              timer contains two types, prompt and sleep timers.
 *
 * Inputs:  pt_auto_sleep_timer     References the auto sleep timer data
 *                                  structure.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_iom_set_auto_sleep_timer_ex(AUTO_SLEEP_TIMER_T* pt_auto_sleep_timer)
{
    return (iom_op_set_auto_sleep_timer(pt_auto_sleep_timer));
}

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
/*-----------------------------------------------------------------------------
 * Name: x_iom_set_global_disable_evtcode
 *
 * Description: This API set the disable event code info 
 *
 * Inputs:  pt_disable_evtcode     References the disable event code info
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain  invalid data.
 ----------------------------------------------------------------------------*/

INT32   x_iom_set_global_disable_evtcode(DISABLE_EVTCODE_T* pt_disable_evtcode)
{
   INT32   i4_return;
   
   /* Check arguments */
   if (!iom_is_init())
   {
       return IOMR_NOT_INIT;
   }  

   iom_lock_db_wr();

   i4_return = iom_op_set_global_disable_evtcode(pt_disable_evtcode);

   iom_unlock_db();
   
   return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: c_iom_get_global_disable_evtcode_cnt
 *
 * Description: This API get  the disable event code count
 *
 * Inputs:  - 
 *
 * Outputs: -
 *
 * Returns:  the count of the disable event code
 ----------------------------------------------------------------------------*/
UINT32  x_iom_get_global_disable_evtcode_cnt()
{
     DISABLE_EVTCODE_T* pt_global_disable_evtcode = NULL;
     UINT32      ui4_Cnt = 0;
        
    /* Check arguments */
    if (!iom_is_init())
    {
       return ui4_Cnt;
    }  

    iom_lock_db_rd();
    
    pt_global_disable_evtcode = iom_op_get_global_disable_evtcode();
    ui4_Cnt = pt_global_disable_evtcode->z_evtcode_cnt;

    iom_unlock_db();
    
    return ui4_Cnt;
}

/*-----------------------------------------------------------------------------
 * Name: c_iom_get_global_disable_evtcode_cnt
 *
 * Description: This API get  the disable event code count
 *
 * Inputs:  - 
 *
 * Outputs: -  pt_disable_evtcode     References the disable event code info,Please note the z_evtcode_cnt
 *                                                   must be gotten by c_iom_get_global_disable_evtcode_cnt
 *
 * Returns:  the count of the disable event code
 ----------------------------------------------------------------------------*/
INT32   x_iom_get_global_disable_evtcode(DISABLE_EVTCODE_T* pt_disable_evtcode)
{

    DISABLE_EVTCODE_T* pt_global_disable_evtcode = NULL;
        
    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }  

    if ((NULL == pt_disable_evtcode)||
        (( 0 != pt_disable_evtcode->z_evtcode_cnt) &&
         (NULL == pt_disable_evtcode->pui4_evtcode_arr)))
    {
        return IOMR_INV_ARG;
    }

    iom_lock_db_rd();

    pt_global_disable_evtcode =  iom_op_get_global_disable_evtcode();

    if (pt_disable_evtcode->z_evtcode_cnt != pt_global_disable_evtcode->z_evtcode_cnt)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_ARG;
    }

    if ( 0 != pt_disable_evtcode->z_evtcode_cnt)
    {
        x_memcpy(pt_disable_evtcode->pui4_evtcode_arr,
                 pt_global_disable_evtcode->pui4_evtcode_arr, 
                 pt_disable_evtcode->z_evtcode_cnt * sizeof(UINT32));
    }  

    pt_disable_evtcode->b_enable = pt_global_disable_evtcode->b_enable;
    pt_disable_evtcode->b_positive_filter = pt_global_disable_evtcode->b_positive_filter;

    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _iom_irrc_key_isdisabled
 *
 * Description: This API check the key is disable or not.
 *
 * Inputs:  pt_disableKeys      New status (on/off) of key monitor.
 *
 * Outputs: -
 *              
 * Returns: -
 *              TRUE:  the key is disabled
 *              FALSE: the key is not disabled
 *
 ----------------------------------------------------------------------------*/
static BOOL _iom_irrc_key_isdisabled(DISABLE_EVTCODE_T* pt_disableKeys, 
                                     UINT32 ui4_key_grp_id)
{
  
   SIZE_T  z_loop = 0;
   
   if (NULL == pt_disableKeys || 0 == pt_disableKeys->z_evtcode_cnt || !(pt_disableKeys->b_enable))
   {
     return FALSE;
   }


   for(z_loop = 0; z_loop < pt_disableKeys->z_evtcode_cnt; ++ z_loop)
   {
     if (ui4_key_grp_id == *(pt_disableKeys->pui4_evtcode_arr + z_loop))
     {
         if(pt_disableKeys->b_positive_filter)
         {
              return TRUE;
         }
         else
         {
             return FALSE;
         }
      }
   }

   /* the event doest not in the pui4)evtcode_arr */
   if(pt_disableKeys->b_positive_filter)
   {
      return FALSE;   
   }

   return TRUE;
       
}
#endif

#ifdef IOM_CUSTOM_KEY_REPEAT_INTERVAL_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: x_iom_set_custom_key_repeat_interval
 *
 * Description: This API sets the custom key repeat interval configuration.
 *
 * Inputs:  ui4_interval_unit  - Unit of first/other interval in pt_setting
 *                               in milliseconds. Should be multiple of 10.
 *          ui4_custom_key_num - Number of custom key settings in pt_setting
 *          pt_setting         - Array of custom key settings. Each item
 *                               contains (event code, first interval units,
 *                               other interval units).
 *
 * Outputs: -
 *
 * Returns: -
 *        IOMR_OK        - Routine successful.
 *        IOMR_NOT_INIT  - The IO Manager is not initialized.
 *        IOMR_INV_ARG   - One or more of the arguments contain  invalid data.
 *
 ----------------------------------------------------------------------------*/
INT32   x_iom_set_custom_key_repeat_interval(
        UINT32                          ui4_interval_unit,
        UINT32                          ui4_custom_key_num,
        IOM_CUST_KEY_REP_ITVL_ITEM_T*   pt_setting)
{
    UINT32  ui4_i;

    if (pt_setting == NULL)
    {
        if (ui4_custom_key_num != 0)
        {
            return IOMR_INV_ARG;
        }

        iom_lock_db_wr();

        x_memset(at_g_cust_key_rep_itvl, 0, sizeof(at_g_cust_key_rep_itvl));
        ui4_g_cust_key_num = 0;

        iom_unlock_db();

        return IOMR_OK;
    }

    if (ui4_interval_unit % IOM_TIMER_RESOLUTION != 0 ||
        ui4_interval_unit == 0 ||
        ui4_custom_key_num > MAX_CUSTOM_KEY_REPEAT_ITVL_SETTING_NUM)
    {
        return IOMR_INV_ARG;
    }

    iom_lock_db_wr();

    x_memset(at_g_cust_key_rep_itvl, 0, sizeof(at_g_cust_key_rep_itvl));
    ui4_interval_unit /= IOM_TIMER_RESOLUTION;

    for (ui4_i = 0; ui4_i < ui4_custom_key_num; ui4_i++)
    {
        at_g_cust_key_rep_itvl[ui4_i][0] = pt_setting[ui4_i][0];
        at_g_cust_key_rep_itvl[ui4_i][1] = pt_setting[ui4_i][1] * ui4_interval_unit;
        at_g_cust_key_rep_itvl[ui4_i][2] = pt_setting[ui4_i][2] * ui4_interval_unit;
    }

    ui4_g_cust_key_num = ui4_custom_key_num;

    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _iom_irrc_key_rep_itvl
 *
 * Description: Get key repeat interval in units of IOM_TIMER_RESOLUTION
 *              for given event filter and event code
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT16  _iom_irrc_key_rep_itvl(EVT_FILTR_T* pt_evt_filtr)
{
    if (pt_evt_filtr == NULL)
    {
        return 40;  /* default value 400 ms */
    }
    do
    {
        if (pt_evt_filtr->u.s_inp.t_irrc_setting.b_disable_custom_key_rep_itvl ||
            (pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_1st == 0 &&
             pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_other == 0) ||
            pt_evt_filtr->u.s_inp.ui4_evt_code == 0)
        {
            break;
        }
        else
        {
            UINT32  ui4_i;
            UINT32  ui4_evt_code = pt_evt_filtr->u.s_inp.ui4_evt_code;

            for (ui4_i = 0; ui4_i < ui4_g_cust_key_num; ui4_i++)
            {
                if (at_g_cust_key_rep_itvl[ui4_i][0] == ui4_evt_code)
                {
                    break;
                }
            }

            if (ui4_i >= ui4_g_cust_key_num || 
                at_g_cust_key_rep_itvl[ui4_i][0] != ui4_evt_code)
            {
                break;
            }

            switch (pt_evt_filtr->u.s_inp.e_timer)
            {
                case EVT_FILTR_TIMER_1ST_ITVL:
                    return (UINT16) at_g_cust_key_rep_itvl[ui4_i][1];

                case EVT_FILTR_TIMER_OTHER_ITVL:
                    return (UINT16) at_g_cust_key_rep_itvl[ui4_i][2];

                case EVT_FILTR_TIMER_STOP:
                default:
                    return 0;
            }
        }
    } while (0);

    switch (pt_evt_filtr->u.s_inp.e_timer)
    {
        case EVT_FILTR_TIMER_1ST_ITVL:
            return pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_1st;

        case EVT_FILTR_TIMER_OTHER_ITVL:
            return pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_other;

        case EVT_FILTR_TIMER_STOP:
        default:
            break;
    }
    return 0;
}
#else
/*-----------------------------------------------------------------------------
 * Name: _iom_irrc_key_rep_itvl
 *
 * Description: Get key repeat interval in units of IOM_TIMER_RESOLUTION
 *              for given event filter and event code
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT16  _iom_irrc_key_rep_itvl(EVT_FILTR_T* pt_evt_filtr)
{
    if (pt_evt_filtr == NULL)
    {
        return 40;  /* default value 400 ms */
    }
    switch (pt_evt_filtr->u.s_inp.e_timer)
    {
        case EVT_FILTR_TIMER_1ST_ITVL:
            return pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_1st;

        case EVT_FILTR_TIMER_OTHER_ITVL:
            return pt_evt_filtr->u.s_inp.t_irrc_setting.t_rpt_evt_itvl.ui2_other;

        case EVT_FILTR_TIMER_STOP:
        default:
            break;
    }
    return 0;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _dfb_process_irrc_evt
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID _dfb_process_irrc_evt(INP_EVT_T* pt_inp_evt)
{
    DEV_INFO_T*         pt_dev;
    EVT_FILTR_T*        pt_evt_filtr;
    IRRC_SETTING_T*     pt_irrc_setting;
    IOM_TAG             t_tag;
    UINT64              ui8_evt_grp_mask;
    UINT8               ui1_evt_filtr_cnt;
    UINT16              ui2_i;
    BOOL                b_loop_dev;

    /* Check arguments */
    if ((pt_inp_evt == NULL) ||
        (pt_inp_evt->e_evt_type != INP_EVT_TYPE_IRRC))
    {
        return;
    }
#ifdef TIME_MEASUREMENT
    if (pt_inp_evt->e_cond == IOM_NFY_COND_BTN_DOWN)
    {                        
        if (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_POWER)
        {
            TMS_END(TMS_SBY_TO_PWR);
            TMS_BEGIN(TMS_BOOT_FROM_SBY);
        }
    
        if ((pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_PRG_UP) ||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_PRG_DOWN) ||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_PREV_PRG))
        {
            TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "CHANNEL_CHG");
            TMS_BEGIN(TMS_CHL_CHE_TIME);
        }
        
        if (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_SELECT)
        {
            TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "CHANNEL_CHG");
            TMS_BEGIN_EX(TMS_FLAG_INP_CHG, TMS_INP_SRC_CHE_TIME, "INP_SRC_CHG");
            TMS_BEGIN(TMS_CHL_CHE_TIME);
            TMS_BEGIN(TMS_AUTO_CHL_SCAN_TIME);            
            TMS_BEGIN(TMS_ANA_CHL_SCAN_TIME);
            TMS_BEGIN(TMS_DIG_CHL_SCAN_TIME);
        }     
        
        if ((pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_INPUT_SRC) ||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_CURSOR_LEFT) ||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_CURSOR_RIGHT)||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_TV)||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_COMPOSITE)||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_COMPONENT)||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_SCART)||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_HDMI)||
            (pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_VGA))
        {
            TMS_BEGIN_EX(TMS_FLAG_INP_CHG, TMS_INP_SRC_CHE_TIME, "INP_SRC_CHG");
            TMS_BEGIN(TMS_INP_SRC_CHE_TIME);
        }      
    }                              
#endif

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Collect all the information we need */
    ui8_evt_grp_mask = IOM_EVT_GRP_TO_MASK(pt_inp_evt->u.s_irrc.ui4_evt_code);
    if (ui8_evt_grp_mask > IOM_EVT_GRP_TO_MASK(KEY_GROUP_MAX))
    {
        iom_unlock_db();
        return;
    }

    if (pt_inp_evt->pv_tag == NULL)
    {
        t_tag = 0;
        b_loop_dev = TRUE;
    }
    else
    {
        t_tag = *(IOM_TAG*)(pt_inp_evt->pv_tag);
        b_loop_dev = FALSE;
    }

    pt_dev = iom_get_in_dev_info_struct();

    do
    {
        pt_evt_filtr = DLIST_HEAD(&(pt_dev[t_tag].t_evt_filtr_list));
        ui1_evt_filtr_cnt = pt_dev[t_tag].ui1_evt_filtr_cnt;

        if (pt_dev[t_tag].e_dev_type != IOM_DEV_TYPE_IN_IRRC)
        {
            if (b_loop_dev)
            {
                if (pt_dev[++t_tag].e_dev_type == IOM_DEV_TYPE_NONE)
                {
                    b_loop_dev = FALSE;
                }

                continue;
            }
            else
            {
                break;
            }
        }

        /* Update button down conut */
        if (pt_inp_evt->e_cond == IOM_NFY_COND_BTN_UP)
        {
            if (pt_dev[t_tag].u.i1_btn_down_cnt > 0)
            {            
                pt_dev[t_tag].u.i1_btn_down_cnt--;
            }
            else
            {
                if (!b_1st_irrc_evt)
                {
                    dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_INV_OP);
                }                    
            }
        }
        else if (pt_inp_evt->e_cond == IOM_NFY_COND_BTN_DOWN)
        {
            if (pt_dev[t_tag].u.i1_btn_down_cnt < 255)
            {            
                pt_dev[t_tag].u.i1_btn_down_cnt++;
            }        
            else
            {
                dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_INV_OP);
            }
        }

        if (b_1st_irrc_evt)
        {
            b_1st_irrc_evt = FALSE;
        }

        if (b_is_key_mon_enabled)
        {
            x_dbg_stmt("{IOM} btn_code=0x%08x %s (ms=%d)\n\r",
                       pt_inp_evt->u.s_irrc.ui4_evt_code,
                       pt_inp_evt->e_cond == IOM_NFY_COND_BTN_DOWN ? "DOWN" : (
                               pt_inp_evt->e_cond == IOM_NFY_COND_BTN_REPEAT ?
                               "REPEAT" : "UP"),
                       x_os_get_sys_tick() * x_os_drv_get_tick_period());
        }

        /* Proceed dispatching */
        for (ui2_i = 0; ui2_i < ui1_evt_filtr_cnt; ui2_i++)
        {
            pt_irrc_setting = &(pt_evt_filtr->u.s_inp.t_irrc_setting);
                    
            switch (pt_inp_evt->e_cond)
            {
                case IOM_NFY_COND_BTN_REPEAT:
                case IOM_NFY_COND_BTN_DOWN:
                    if ((ui8_evt_grp_mask & pt_irrc_setting->ui8_evt_grp_mask) ||
                        pt_irrc_setting->b_notify_raw_data)
                    {
                    /* Check if this is a raw data notification */
                    if ((pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_INVALID) &&
                        (pt_irrc_setting->b_notify_raw_data == FALSE))
                    {
                        break;
                    }           
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
                    /* Check if the key is disabled or not */
                    if ((pt_irrc_setting->b_notify_raw_data == FALSE) &&
                        (_iom_irrc_key_isdisabled(&t_global_disable_keys, pt_inp_evt->u.s_irrc.ui4_evt_code) ||
                         (!t_global_disable_keys.b_enable &&
                          (_iom_irrc_key_isdisabled(&(pt_evt_filtr->u.s_inp.t_disable_keys), pt_inp_evt->u.s_irrc.ui4_evt_code)) )
                         ))
                    {
                        break;
                    }
#endif

                    /* Dispatch the event */
                    if (((pt_inp_evt->u.s_irrc.ui4_evt_code != BTN_INVALID) &&
                         ((IOM_EVT_SRC_TO_MASK(pt_inp_evt->u.s_irrc.ui4_evt_code) & ui8_evt_src_mask) != 0)) ||
                        (pt_irrc_setting->b_notify_raw_data))
                    {
#ifdef IOM_EXTENDABLE_RAW_DATA_SUPPORT   
                        IRRC_DATA_T*    pt_raw_data = NULL;
                                                  
                        if (pt_irrc_setting->b_notify_raw_data)
                        {
                            pt_raw_data = &pt_inp_evt->u.s_irrc.t_data;
                        }                                
                        
                        (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                               pt_inp_evt->e_cond,
                                               pt_inp_evt->u.s_irrc.ui4_evt_code,
                                               (UINT32)pt_raw_data);                                
#else
                        UINT32      ui4_raw_data = 0;
                        
                        if (pt_irrc_setting->b_notify_raw_data)
                        {
                            ui4_raw_data = GET_UINT32_FROM_PTR_BIG_END(&pt_inp_evt->u.s_irrc.t_data.aui1_data[0]);
                        }

                        (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                               pt_inp_evt->e_cond,
                                               pt_inp_evt->u.s_irrc.ui4_evt_code,
                                               ui4_raw_data);
#endif

                        pt_evt_filtr->u.s_inp.ui4_evt_code = pt_inp_evt->u.s_irrc.ui4_evt_code;
                        pt_evt_filtr->u.s_inp.t_data = pt_inp_evt->u.s_irrc.t_data;

                        if (pt_irrc_setting->t_rpt_evt_itvl.ui2_1st > IOM_ZERO_RPT_EVT_ITVL)
                        {
                            pt_evt_filtr->u.s_inp.e_timer = EVT_FILTR_TIMER_1ST_ITVL;
                        }
                        else if (pt_irrc_setting->t_rpt_evt_itvl.ui2_other > IOM_ZERO_RPT_EVT_ITVL)
                        {
                            pt_evt_filtr->u.s_inp.e_timer = EVT_FILTR_TIMER_OTHER_ITVL;
                        }
                        else
                        {
                            pt_evt_filtr->u.s_inp.e_timer = EVT_FILTR_TIMER_STOP;
                        }

                        pt_evt_filtr->u.s_inp.ui2_long_press_cnt = 0;
                        pt_evt_filtr->u.s_inp.h_timer            = (HANDLE_T) 0;
                    }                            
                    }
                    break;

                case IOM_NFY_COND_BTN_UP:
                    if ((ui8_evt_grp_mask & pt_irrc_setting->ui8_evt_grp_mask) ||
                        pt_irrc_setting->b_notify_raw_data)
                    {
                    /* Check if this is a raw data notification */
                    if ((pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_INVALID) &&
                        (pt_irrc_setting->b_notify_raw_data == FALSE))
                    {
                        break;
                    }     
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
                    /* Check if the key is disabled or not */
                    if ((pt_irrc_setting->b_notify_raw_data == FALSE) &&
                        (_iom_irrc_key_isdisabled(&t_global_disable_keys, pt_inp_evt->u.s_irrc.ui4_evt_code) ||
                         (!t_global_disable_keys.b_enable &&
                          (_iom_irrc_key_isdisabled(&(pt_evt_filtr->u.s_inp.t_disable_keys), pt_inp_evt->u.s_irrc.ui4_evt_code)) )
                         ))
                    {
                        break;
                    }
#endif
                    
                    /* Dispatch the event */
                    if (((pt_inp_evt->u.s_irrc.ui4_evt_code != BTN_INVALID) &&
                         ((IOM_EVT_SRC_TO_MASK(pt_inp_evt->u.s_irrc.ui4_evt_code) & ui8_evt_src_mask) != 0)) ||
                        (pt_irrc_setting->b_notify_raw_data))
                    {       
#if 0
#ifdef IOM_IRRC_LONG_PRESS_DETECT                        
                        /* Check if long-press occurs */
                        if (_iom_is_long_press(pt_evt_filtr) == TRUE)
                        {
                            pt_evt_filtr->u.s_inp.ui4_evt_code |= IOM_EVT_ATTR_LONG_PRESS;
                        }
#endif                            
#endif
#ifdef IOM_EXTENDABLE_RAW_DATA_SUPPORT   
                        IRRC_DATA_T*    pt_raw_data = NULL;
                                                  
                        if (pt_irrc_setting->b_notify_raw_data)
                        {
                            pt_raw_data = &pt_inp_evt->u.s_irrc.t_data;
                        }                                
                        
                        (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                               pt_inp_evt->e_cond,
                                               pt_evt_filtr->u.s_inp.ui4_evt_code,
                                               (UINT32)pt_raw_data);                                
#else
                        UINT32      ui4_raw_data = 0;
                        
                        if (pt_irrc_setting->b_notify_raw_data)
                        {
                            ui4_raw_data = GET_UINT32_FROM_PTR_BIG_END(&pt_inp_evt->u.s_irrc.t_data.aui1_data[0]);
                        }
                        
                        (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                               pt_inp_evt->e_cond,
                                               pt_evt_filtr->u.s_inp.ui4_evt_code,
                                               ui4_raw_data);
#endif
                    }                                                   
  
                    pt_evt_filtr->u.s_inp.ui4_evt_code = 0;
                    x_memset(&pt_evt_filtr->u.s_inp.t_data, 0 , sizeof(IRRC_DATA_T));
                    pt_evt_filtr->u.s_inp.e_timer = EVT_FILTR_TIMER_STOP;
                    pt_evt_filtr->u.s_inp.ui2_long_press_cnt = 0;
                    }
                    break;

                default:
                    break;
            }

            pt_evt_filtr = DLIST_NEXT(pt_evt_filtr, t_link);
        }

        if (b_loop_dev)
        {
            t_tag++;
        }
    } while(b_loop_dev);

    /* Restart auto sleep timer */
    iom_op_set_auto_sleep_timer(&t_auto_sleep_timer);

    /* Unlock DB */
    iom_unlock_db();
}

/*-----------------------------------------------------------------------------
 * Name: _dfb_process_irrc_timeout_evt
 *
 * Description: This function is expected to be invoked every
 *              ui2_tick_cnt * IOM_TIMER_RESOLUTION milliseconds
 *              after BTN_DOWN before BTN_UP.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID _dfb_process_irrc_timeout_evt(INP_EVT_T* pt_inp_evt, UINT16 ui2_tick_cnt)
{
    DEV_INFO_T*         pt_dev;
    EVT_FILTR_T*        pt_evt_filtr;
    IRRC_SETTING_T*     pt_irrc_setting;
    IOM_TAG             t_tag;
    UINT64              ui8_evt_grp_mask;
    UINT8               ui1_evt_filtr_cnt;
    UINT16              ui2_i;
    UINT16              ui2_next_nfy_cnt;
    BOOL                b_loop_dev;
    BOOL                b_notified = FALSE;

    /* Check arguments */
    if ((pt_inp_evt == NULL) ||
        (pt_inp_evt->e_evt_type != INP_EVT_TYPE_IRRC) ||
        ui2_tick_cnt == 0)
    {
        return;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Collect all the information we need */
    ui8_evt_grp_mask = IOM_EVT_GRP_TO_MASK(pt_inp_evt->u.s_irrc.ui4_evt_code);
    if (ui8_evt_grp_mask > IOM_EVT_GRP_TO_MASK(KEY_GROUP_MAX))
    {
        iom_unlock_db();
        return;
    }

    if (pt_inp_evt->pv_tag == NULL)
    {
        t_tag = 0;
        b_loop_dev = TRUE;
    }
    else
    {
        t_tag = *(IOM_TAG*)(pt_inp_evt->pv_tag);
        b_loop_dev = FALSE;
    }

    pt_dev = iom_get_in_dev_info_struct();

    do
    {
        pt_evt_filtr = DLIST_HEAD(&(pt_dev[t_tag].t_evt_filtr_list));
        ui1_evt_filtr_cnt = pt_dev[t_tag].ui1_evt_filtr_cnt;

        if (pt_dev[t_tag].e_dev_type != IOM_DEV_TYPE_IN_IRRC)
        {
            if (b_loop_dev)
            {
                if (pt_dev[++t_tag].e_dev_type == IOM_DEV_TYPE_NONE)
                {
                    b_loop_dev = FALSE;
                }

                continue;
            }
            else
            {
                break;
            }
        }

        /* Proceed dispatching */
        for (ui2_i = 0; ui2_i < ui1_evt_filtr_cnt; ui2_i++)
        {
            pt_irrc_setting = &(pt_evt_filtr->u.s_inp.t_irrc_setting);

            do
            {
                if ((ui8_evt_grp_mask & pt_irrc_setting->ui8_evt_grp_mask) ||
                    pt_irrc_setting->b_notify_raw_data)
                {
                    /* Check if this is a raw data notification */
                    if ((pt_inp_evt->u.s_irrc.ui4_evt_code == BTN_INVALID) &&
                        (pt_irrc_setting->b_notify_raw_data == FALSE))
                    {
                        break;
                    }
#if IOM_SUPPORT_IRRC_DISABLE_KEYS
                    /* Check if the key is disabled or not */
                    if ((pt_irrc_setting->b_notify_raw_data == FALSE) &&
                        (_iom_irrc_key_isdisabled(&t_global_disable_keys, pt_inp_evt->u.s_irrc.ui4_evt_code) ||
                         (!t_global_disable_keys.b_enable &&
                          (_iom_irrc_key_isdisabled(&(pt_evt_filtr->u.s_inp.t_disable_keys), pt_inp_evt->u.s_irrc.ui4_evt_code)) )
                        ))
                    {
                        break;
                    }
#endif

                    /* Check to dispatch the event */
                    if (((pt_inp_evt->u.s_irrc.ui4_evt_code != BTN_INVALID) &&
                         ((IOM_EVT_SRC_TO_MASK(pt_inp_evt->u.s_irrc.ui4_evt_code) & ui8_evt_src_mask) != 0)) ||
                        (pt_irrc_setting->b_notify_raw_data))
                    {
                        if (pt_inp_evt->u.s_irrc.ui4_evt_code != pt_evt_filtr->u.s_inp.ui4_evt_code)
                        {
                            printf("{IOM} Error: input event code != event filter event code for timeout event\n");
                            break;
                        }

                        pt_evt_filtr->u.s_inp.ui2_long_press_cnt += ui2_tick_cnt;

                        ui2_next_nfy_cnt = _iom_irrc_key_rep_itvl(pt_evt_filtr);

                        if (ui2_next_nfy_cnt == 0)
                        {
                            pt_evt_filtr->u.s_inp.e_timer = EVT_FILTR_TIMER_STOP;
                            break;
                        }

                        ui2_next_nfy_cnt += pt_evt_filtr->u.s_inp.h_timer;

                        /*printf("{IOM} event filter 0x%x, ui2_long_press_cnt=%d, ui2_next_nfy_cnt=%d\n", pt_evt_filtr, pt_evt_filtr->u.s_inp.ui2_long_press_cnt, ui2_next_nfy_cnt);*/

                        if (pt_evt_filtr->u.s_inp.ui2_long_press_cnt >=
                            ui2_next_nfy_cnt)
                        {
#ifdef IOM_EXTENDABLE_RAW_DATA_SUPPORT
                            IRRC_DATA_T*    pt_raw_data = NULL;

                            if (pt_irrc_setting->b_notify_raw_data)
                            {
                                pt_raw_data = &pt_inp_evt->u.s_irrc.t_data;
                            }

                            (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                                   IOM_NFY_COND_BTN_REPEAT,
                                                   pt_inp_evt->u.s_irrc.ui4_evt_code,
                                                   (UINT32)pt_raw_data);
#else
                            UINT32      ui4_raw_data = 0;

                            if (pt_irrc_setting->b_notify_raw_data)
                            {
                                ui4_raw_data = GET_UINT32_FROM_PTR_BIG_END(&pt_inp_evt->u.s_irrc.t_data.aui1_data[0]);
                            }

                            (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                                   IOM_NFY_COND_BTN_REPEAT,
                                                   pt_inp_evt->u.s_irrc.ui4_evt_code,
                                                   ui4_raw_data);
#endif
                            if (b_is_key_mon_enabled)
                            {
                                x_dbg_stmt("{IOM} btn_code=0x%08x ef=0x%x cnt=%d REPEAT (ms=%d)\n\r",
                                           pt_evt_filtr->u.s_inp.ui4_evt_code,
                                           pt_evt_filtr->h_filtr, pt_evt_filtr->u.s_inp.ui2_long_press_cnt,
                                           x_os_get_sys_tick() * x_os_drv_get_tick_period());
                            }

                            pt_evt_filtr->u.s_inp.h_timer = (HANDLE_T) pt_evt_filtr->u.s_inp.ui2_long_press_cnt;
                            if (pt_evt_filtr->u.s_inp.e_timer == EVT_FILTR_TIMER_1ST_ITVL)
                            {
                                pt_evt_filtr->u.s_inp.e_timer = EVT_FILTR_TIMER_OTHER_ITVL;
                            }

                            b_notified = TRUE;
                        }
                    }
                }
            } while (0);

            pt_evt_filtr = DLIST_NEXT(pt_evt_filtr, t_link);
        }

        if (b_loop_dev)
        {
            t_tag++;
        }
    } while(b_loop_dev);

    /* Restart auto sleep timer */
    if (b_notified)
    {
        iom_op_set_auto_sleep_timer(&t_auto_sleep_timer);
    }

    /* Unlock DB */
    iom_unlock_db();
}

/*-----------------------------------------------------------------------------
 * Name: _iom_poll_inp_dptr_msg
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _iom_poll_inp_dptr_msg(HANDLE_T     *ph_iom_inp_msgq)
{
    INP_EVT_T       t_inp_evt;
    SIZE_T          z_size = sizeof(INP_EVT_T);
    UINT16          ui2_idx;

    /* Check if an input event is received */
    if (x_msg_q_receive(&ui2_idx,
                        &t_inp_evt,
                        &z_size,
                        ph_iom_inp_msgq,
                        1,
                        X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
        /* Dispatch the event to individual applications
           having the right of receiving such event */
        switch (t_inp_evt.e_evt_type)
        {
            case INP_EVT_TYPE_TIMER_KEY_GEN:
#if IOM_KEY_GEN_SUPPORT
                if (iom_key_gen_get_status())
                {
                    iom_key_gen_signal();
                }
#endif
                break;

            default:
                break;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: dfb_inp_dptr_thread
 *
 * Description: -
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID dfb_inp_dptr_thread(VOID* pv_arg)
{
#ifndef ANDROID
    IDirectFBWindow        *window1 = 0;
#ifdef IOM_DFB_USE_KEYBOARD_DEV
    DFBInputDeviceDescription devdesc;
#endif
    BOOL b_keydown = FALSE;
    BOOL b_first_keydown = FALSE;
    DFBEvent evt;
    INP_EVT_T        t_inp_evt;

    UINT32          ui4_poll_interval = IOM_TIMER_RESOLUTION * 5; /* in ms */
    UINT32          ui4_next_poll = 0;
    UINT32          ui4_current_time;

    ui8_evt_src_mask = IOM_EVT_SRC_TO_MASK(KEY_SRC_REMOTE_CTRL) |
                       IOM_EVT_SRC_TO_MASK(KEY_SRC_FRONT_PANEL) |
                       IOM_EVT_SRC_TO_MASK(KEY_SRC_SW)          |
                       IOM_EVT_SRC_TO_MASK(KEY_SRC_SW_1)        |
                       IOM_EVT_SRC_TO_MASK(KEY_SRC_SW_2);

#ifdef IOM_DFB_USE_KEYBOARD_DEV
    DFBCHECK(g_p_dfb->GetInputDevice(g_p_dfb, DIDID_KEYBOARD, &inputdev));
    DFBCHECK(inputdev->GetDescription(inputdev, &devdesc));
#endif

    //window1 = gl_dfb_get_main_wnd();
    window1 = NULL;
    if(window1==NULL)
    {
        x_dbg_stmt("Get dfb window fail \n");
        return;
    }
    DFBCHECK(window1->CreateEventBuffer(window1, &keybuffer));

    x_memset(&t_inp_evt,0,sizeof(INP_EVT_T));

    while (1)
    {
        DFBResult nResult;      
        
        unsigned int seconds = 0;
#if 0
        unsigned int first_msecs = 400;
        unsigned int sec_msecs = 100;
        
        _iom_poll_inp_dptr_msg(&h_iom_inp_msgq);

        /* process keybuffer */

        if (b_first_keydown)
        {
            nResult = keybuffer->WaitForEventWithTimeout(keybuffer,seconds,first_msecs);
            b_first_keydown = FALSE;
        }else{
            nResult = keybuffer->WaitForEventWithTimeout(keybuffer,seconds,sec_msecs);
        }

        /* Check if IRRC input is enabled */
        if (b_in_irrc == FALSE)
        {
            continue;
        }

        if (nResult == DFB_TIMEOUT) 
        {             
            if (b_keydown)             
            {                                 
                t_inp_evt.e_cond = IOM_NFY_COND_BTN_REPEAT;
#if IOM_KEY_GEN_SUPPORT
                iom_key_gen_record(&t_inp_evt);
#endif
                _dfb_process_irrc_evt(&t_inp_evt);
        
            }                                 
            continue;         
        }    
#else

        _iom_poll_inp_dptr_msg(&h_iom_inp_msgq);

        if (b_first_keydown)
        {
            b_first_keydown = FALSE;
        }

        ui4_current_time = x_os_get_sys_tick() * x_os_drv_get_tick_period();

        if (ui4_next_poll > ui4_current_time)
        {
            ui4_poll_interval = ui4_next_poll - ui4_current_time;
        }
        else if (ui4_next_poll > 0)
        {
            ui4_poll_interval = IOM_TIMER_RESOLUTION;
        }
        nResult = keybuffer->WaitForEventWithTimeout(keybuffer,seconds,ui4_poll_interval);

#if 0
        ui4_next_poll = x_os_get_sys_tick() * x_os_drv_get_tick_period() + \
                        IOM_TIMER_RESOLUTION * 5;
#else
        ui4_next_poll = x_os_get_sys_tick() * x_os_drv_get_tick_period() + \
                        IOM_TIMER_RESOLUTION * 5 - 1; /* a 1ms error by rule of thumb */
#endif

        if (nResult == DFB_TIMEOUT)
        {
            if (b_keydown)
            {
                _dfb_process_irrc_timeout_evt(&t_inp_evt, 5);
            }
            continue;
        }
#endif

        if (nResult != DFB_OK)         
        {             
            printf("WaitForEventWithTimeout error code %d\n", nResult);
            continue;
        }        
        
        nResult = keybuffer->GetEvent(keybuffer, &evt);         
        if (nResult != DFB_OK)          
        {            
            printf("GetEvent error code %d\n", nResult);
            continue;         
        }

       switch(evt.clazz)
       {          
           case DFEC_WINDOW:
           {
                if(evt.window.type != DWET_KEYDOWN && evt.window.type != DWET_KEYUP)
                {
                   continue;
                }
                if(evt.window.type == DWET_KEYUP && b_keydown == FALSE)
                {
                    continue;
                }

                /* Check if IRRC input is enabled */
                if (b_in_irrc == FALSE)
                {
                    continue;
                }

#if 0
                switch (evt.window.key_symbol & 0xff00)
                {
                    case DIKT_MTK:
                        e_protocol = IOM_IRRC_PROTOCOL_NEC;
                        break;

                    case DIKT_UNICODE:
                    case DIKT_SPECIAL:
                        e_protocol = IOM_IRRC_PROTOCOL_RC6;
                        break;
                    
                    default:
                        break;
                }
#else
                switch (evt.window.key_code & 0xffff0000)
                {
                    case 0x00000000:
                        e_protocol = IOM_IRRC_PROTOCOL_NEC;
                        break;

                    case 0x03000000:
                        e_protocol = IOM_IRRC_PROTOCOL_RC6;
                        break;

                    default:
                        break;
                }
#endif

                t_inp_evt.e_evt_type = INP_EVT_TYPE_IRRC;
                t_inp_evt.pv_tag = NULL;
                t_inp_evt.u.s_irrc.ui4_evt_code = /*_dfb2mw_NEC_keymap*/(evt.window.key_symbol);
                t_inp_evt.u.s_irrc.t_data.aui1_data[0] = 0x1C;

                if ((evt.window.key_symbol & 0xffff0000) == 0) /* for keyboard events */
                {
                    t_inp_evt.u.s_irrc.ui4_evt_code = _dfb2mw_kb_keymap(
                            evt.window.key_symbol, evt.window.modifiers);
                }

                /*printf("key_code=0x%x: key_symbol=0x%x\n", evt.window.key_code, evt.window.key_symbol);
                printf("ui4_evt_code=0x%x\n",t_inp_evt.u.s_irrc.ui4_evt_code);*/
                
                if (evt.window.type == DWET_KEYDOWN)
                {
                    t_inp_evt.e_cond = IOM_NFY_COND_BTN_DOWN;

                    b_keydown = TRUE;
                    b_first_keydown = TRUE;
#if IOM_KEY_GEN_SUPPORT
                    iom_key_gen_record(&t_inp_evt);
#endif
                    _dfb_process_irrc_evt(&t_inp_evt);
                    /*x_iom_send_evt(BTN_MENU,50);*/
                }
                else if (evt.window.type == DWET_KEYUP)
                {
                    t_inp_evt.e_cond = IOM_NFY_COND_BTN_UP;

                    b_keydown = FALSE;
#if IOM_KEY_GEN_SUPPORT
                    iom_key_gen_record(&t_inp_evt);
#endif
                    _dfb_process_irrc_evt(&t_inp_evt);
                }
             }
                   break;

            case DFEC_USER:
                 switch (evt.user.type) 
                 {

                      case IOM_BUTTON_DOWN:
                          t_inp_evt.e_evt_type = INP_EVT_TYPE_IRRC;
                          t_inp_evt.pv_tag = NULL;
                          t_inp_evt.u.s_irrc.ui4_evt_code = (UINT32)(evt.user.data);
                          t_inp_evt.u.s_irrc.t_data.aui1_data[0] = 0x1C;
                          t_inp_evt.e_cond = IOM_NFY_COND_BTN_DOWN;                             
                          b_keydown = TRUE;
                          b_first_keydown = TRUE;
#if IOM_KEY_GEN_SUPPORT
                          iom_key_gen_record(&t_inp_evt);
#endif
                          _dfb_process_irrc_evt(&t_inp_evt); 
                          break;
            
                      case IOM_BUTTON_UP:
                          t_inp_evt.e_evt_type = INP_EVT_TYPE_IRRC;
                          t_inp_evt.pv_tag = NULL;
                          t_inp_evt.u.s_irrc.ui4_evt_code = (UINT32)(evt.user.data);
                          t_inp_evt.u.s_irrc.t_data.aui1_data[0] = 0x1C;
                          t_inp_evt.e_cond = IOM_NFY_COND_BTN_UP;                                
                          b_keydown = FALSE;
#if IOM_KEY_GEN_SUPPORT
                          iom_key_gen_record(&t_inp_evt);
#endif
                         _dfb_process_irrc_evt(&t_inp_evt); 
                           break;               
            
                      default:
                          break;
                  }

                 /*printf("DFEC_USER: evt.user.data=0x%x\n",evt.user.data);
                 printf("DFEC_USER: ui4_evt_code=0x%x\n",t_inp_evt.u.s_irrc.ui4_evt_code);*/
                
                 break;

            default:
                break;    
           }
    }
#endif    
}

#ifndef IOM_DISABLE_UART_FACTORY_MODE
/*-----------------------------------------------------------------------------
 * Name: uart_inp_dptr_thread1
 *
 * Description: - for ttyMT3
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

VOID uart_inp_dptr_thread1(VOID* pv_arg)
{
    UINT16  ui2_id=0;
    UINT16  ui2_i;
    int     fd;
    int     mode = 0;
    INT32   i4_len;
    UINT8   aui1_buf[UART_FACTORY_CMD_BUF_LEN];
    UINT8   ui1_io_dev;
    IOM_UART_T      t_iom_uart;
    EVT_FILTR_T*    pt_evt_filtr;
    /*
    FILE    *fp;
    */
    
    do
    {        
        while (1)
        {
#if 1            
            iom_lock_db_wr();
            if (!ab_factory_mode[ui2_id])
            {
                if( (b_ttymt3_opened==TRUE) && (b_magic_char_set==TRUE) )
                {
                    iom_unlock_db();
                    if(ioctl(TTYMT3_FD, 0xfffd, &mode) < 0)        
                    {            
                        fprintf(stderr, "Fail to wait on /dev/ttyMT3 magic char receiving\n");  
                    }
                    iom_lock_db_wr();
                                
                    if(mode == 1)
                    {
                        if(tcgetattr(TTYMT3_FD, &t_term_cli) == -1)
                        {
                
                            DBG_ERROR(("{IOM} ERR: device get ttyMT3 attribute fail\n\r"));

                        }
                        if(tcsetattr(TTYMT3_FD, TCSAFLUSH, &t_term_factory) == -1)
                        {
                             DBG_ERROR(("{IOM} ERR: device set ttyMT3 attribute fail\n\r"));

                        }

                        fd=TTYMT3_FD;
                        ab_factory_mode[ui2_id] = TRUE ;
                        iom_unlock_db();
                        continue;
                    }
                }
            
                iom_unlock_db();
                x_thread_delay(200);
                continue;
            }
            iom_unlock_db();
#endif
            
            fd=TTYMT3_FD;            
            i4_len = read(fd, aui1_buf, UART_FACTORY_CMD_BUF_LEN);
            if (i4_len < 0)
            {
                /* handle error */
                x_thread_delay(100);
            }
            else if (i4_len == 0)
            {
                /* no data now */
            }
            else
            {
                /*
                INT32 i4_i;
                fprintf(fp, "\r{IOM} notifying:");
                for (i4_i = 0; i4_i < i4_len; i4_i++)
                    fprintf(fp, " %02X", aui1_buf[i4_i]);
                fprintf(fp, "\n\r");
                */
                iom_lock_db_rd();

                for (ui1_io_dev = 0; ui1_io_dev < IOM_MAX_DEV; ui1_io_dev++)
                {
                    if (at_io_dev[ui1_io_dev].e_dev_type != IOM_DEV_TYPE_IO_UART ||
                        UART_GET_PORT(at_io_dev[ui1_io_dev].ui2_id) != ui2_id)
                    {
                        continue;
                    }

                    /* Proceed dispatching */
                    t_iom_uart.z_len = i4_len;
                    t_iom_uart.pui1_data = aui1_buf;

                    pt_evt_filtr = DLIST_HEAD(&(at_io_dev[ui1_io_dev].t_evt_filtr_list));

                    for (ui2_i = 0; ui2_i < at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt; ui2_i++)
                    {
                        if(pt_evt_filtr->pf_nfy != NULL)
                        {
                            (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                                   IOM_NFY_COND_REC_UART_DATA,
                                                   0,
                                                   (UINT32)(&t_iom_uart));
                        }
                        pt_evt_filtr = DLIST_NEXT(pt_evt_filtr, t_link);
                    }
                }

                iom_unlock_db();
            }
        }
    } while (0);

    /*
    if (fp != NULL)
    {
        fclose(fp);
    }
    */

    if (fd != -1)
    {
        close(fd);
    }
    if (pv_arg != NULL)
    {
        x_mem_free(pv_arg);
    }
}

/*-----------------------------------------------------------------------------
 * Name: uart_inp_dptr_thread2
 *
 * Description: - for ttyMT2
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

VOID uart_inp_dptr_thread2(VOID* pv_arg)
{
    UINT16  ui2_id = 1;
    UINT16  ui2_i;
    int     fd;
    INT32   i4_len;
    UINT8   aui1_buf[UART_FACTORY_CMD_BUF_LEN];
    UINT8   ui1_io_dev;
    IOM_UART_T      t_iom_uart;
    EVT_FILTR_T*    pt_evt_filtr;
    /*
    FILE    *fp;
    */


    do
    {
        fd = open(UART_DEV_NAME(ui2_id), O_RDONLY|O_NOCTTY|O_NDELAY);       
        if (-1 == fd)
        {
              printf("{IOM} %s error opening device %s\n", __func__, UART_DEV_NAME(ui2_id));
              break;
        }
        
        while (1)
        {
#if 1
            
            iom_lock_db_rd();
            if (!ab_factory_mode[ui2_id])
            {
                iom_unlock_db();
                x_thread_delay(200);
                continue;
            }
            iom_unlock_db();
#endif
            
            i4_len = read(fd, aui1_buf, UART_FACTORY_CMD_BUF_LEN);
            if (i4_len < 0)
            {
                /* handle error */
                x_thread_delay(100);
            }
            else if (i4_len == 0)
            {
                /* no data now */
            }
            else
            {
                /*
                INT32 i4_i;
                fprintf(fp, "\r{IOM} notifying:");
                for (i4_i = 0; i4_i < i4_len; i4_i++)
                    fprintf(fp, " %02X", aui1_buf[i4_i]);
                fprintf(fp, "\n\r");
                */
                iom_lock_db_rd();

                for (ui1_io_dev = 0; ui1_io_dev < IOM_MAX_DEV; ui1_io_dev++)
                {
                    if (at_io_dev[ui1_io_dev].e_dev_type != IOM_DEV_TYPE_IO_UART ||
                        UART_GET_PORT(at_io_dev[ui1_io_dev].ui2_id) != ui2_id)
                    {
                        continue;
                    }

                    /* Proceed dispatching */
                    t_iom_uart.z_len = i4_len;
                    t_iom_uart.pui1_data = aui1_buf;

                    pt_evt_filtr = DLIST_HEAD(&(at_io_dev[ui1_io_dev].t_evt_filtr_list));

                    for (ui2_i = 0; ui2_i < at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt; ui2_i++)
                    {
                        if(pt_evt_filtr->pf_nfy != NULL)
                        {
                            (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                                   IOM_NFY_COND_REC_UART_DATA,
                                                   0,
                                                   (UINT32)(&t_iom_uart));
                        }
                        pt_evt_filtr = DLIST_NEXT(pt_evt_filtr, t_link);
                    }
                }

                iom_unlock_db();
            }
        }
    } while (0);

    /*
    if (fp != NULL)
    {
        fclose(fp);
    }
    */

    if (fd != -1)
    {
        close(fd);
    }
    if (pv_arg != NULL)
    {
        x_mem_free(pv_arg);
    }
}



#endif

/*-----------------------------------------------------------------------------
 * Name: iom_op_init
 *
 * Description: This API initializes the IO Manager.
 *
 * Inputs:  b_apdemon_irrc              Indicates if IRRC events come from
 *                                      Apdemon or not.
 *          pt_iom_thread_descr         References a thread descriptor
 *                                      structure.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_ALREADY_INIT           The IO Manager has already been
 *                                      initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_MEM_ALLOC_FAILED       Memory allocation is failed.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_init(BOOL            b_apdemon_irrc_enabled,
                  THREAD_DESCR_T* pt_iom_thread_descr,
                  GL_COLORMODE_T  e_colormode,
                  UINT32          ui4_width,
                  UINT32          ui4_height)
{
    INT32       i4_return;
    UINT16      ui2_i;

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
    DISABLE_EVTCODE_T  t_disable_evecode;
#endif

    /* Check arguments */
    if (b_iom_init)
    {
        return IOMR_ALREADY_INIT;
    }
#ifndef ANDROID
    /* Tricky in Linux, for get window from DFB to get key*/
    x_gl_init(e_colormode,ui4_width,ui4_height);
#endif    
    if (pt_iom_thread_descr == NULL)
    {
        return IOMR_INV_ARG;
    }

    {
        t_iom_thread_descr = (*pt_iom_thread_descr);

        /* Convert default thread values to actual ones if needed */
        if (t_iom_thread_descr.ui1_priority == DEFAULT_PRIORITY)
        {
            t_iom_thread_descr.ui1_priority = IOM_THREAD_DEFAULT_PRIORITY;
        }

        if (t_iom_thread_descr.ui2_num_msgs == DEFAULT_NUM_MSGS)
        {
            t_iom_thread_descr.ui2_num_msgs = IOM_NUM_OF_MSGS;
        }

        if (t_iom_thread_descr.z_stack_size == DEFAULT_STACK_SIZE)
        {
            t_iom_thread_descr.z_stack_size = IOM_THREAD_DEFAULT_STACK_SIZE;
        }
    }

    /* Initialize IO Manager lock module */
    iom_init_lock_module();

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Initialize device information array */
    for (ui2_i = 0; ui2_i < (UINT16)IOM_MAX_DEV; ui2_i++)
    {
        /* Input */
        at_in_dev[ui2_i].h_rm_dev = NULL_HANDLE;
        at_in_dev[ui2_i].e_dev_type = IOM_DEV_TYPE_NONE;
        at_in_dev[ui2_i].ui4_tag = (UINT32)ui2_i;
        at_in_dev[ui2_i].ui1_evt_filtr_cnt = 0;
        DLIST_INIT(&(at_in_dev[ui2_i].t_evt_filtr_list));
        at_in_dev[ui2_i].u.i1_btn_down_cnt = 0;
    }

    b_in_irrc = TRUE;

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
    x_memset(&t_disable_evecode, 0, sizeof(DISABLE_EVTCODE_T));
    iom_op_set_global_disable_evtcode(&t_disable_evecode);
#endif

    /* Unlock DB */
    iom_unlock_db();

    /* Create message queue to handle input events */
    i4_return = x_msg_q_create(&h_iom_inp_msgq,
                               IOM_MSGQ_NAME,
                               sizeof(INP_EVT_T),
                               t_iom_thread_descr.ui2_num_msgs);

    if (i4_return != OSR_OK)
    {
        DBG_ERROR(("{IOM} ERR: x_msg_q_create() failed!\n\r"));
        dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_MESSAGE);
    }

#if 0
    /* Create thread for input event dispatcher */
    i4_return = x_thread_create(&h_iom_inp_thread,
                                IOM_THREAD_NAME,
                                t_iom_thread_descr.z_stack_size,
                                t_iom_thread_descr.ui1_priority,
                                iom_inp_dptr_thread,
                                sizeof(HANDLE_T),
                                (VOID*)&h_iom_inp_msgq);

    if (i4_return != OSR_OK)
    {
        DBG_ERROR(("{IOM} ERR: x_thread_create() failed!\n\r"));
        dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_THREAD);
    }
#endif    
#ifdef KEY_FROM_DFB /* MHF Linux - Bin Zhang */
    /* Create thread for dfb input event dispatcher */
    i4_return = x_thread_create(&h_dfb_inp_thread,
                                DFB_THREAD_NAME,
                                t_iom_thread_descr.z_stack_size,
                                t_iom_thread_descr.ui1_priority,
                                dfb_inp_dptr_thread,
                                (SIZE_T)NULL,
                                NULL);

    if (i4_return != OSR_OK)
    {
        DBG_ERROR(("{IOM} ERR: dfb x_thread_create() failed!\n\r"));
        dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_THREAD);
    }
#endif
#ifndef IOM_DISABLE_UART_FACTORY_MODE
    /* Disable SIGTTIN, SIGTTOU to avoid Stopped (tty input/output) */
    {
        /*
         * not works
        sigset_t ss;
        sigemptyset(&ss);
        sigaddset(&ss, SIGTTIN);
        sigaddset(&ss, SIGTTOU);
        sigprocmask(SIG_BLOCK, &ss, NULL);
        */
        struct sigaction sysact = {{0}};

        sigemptyset(&sysact.sa_mask);
        sysact.sa_flags = 0;
        sysact.sa_handler = SIG_IGN;
        sigaction(SIGTTOU, &sysact, NULL);
        sigaction(SIGTTIN, &sysact, NULL);
    }

#ifdef IOM_UART_1_DEFAULT_FACTORY_MODE
    if (_set_non_canonical_input(1, TRUE) != IOMR_OK)
    {
        printf("{IOM} ERR: UART 1 enter factory mode set non-canonical input failed\n");
    }
    else
    {
        ab_factory_mode[1] = TRUE;
    }
#endif
    /* UART input event dispatcher thread */
    {
        UINT16  ui2_id;
        
        ui2_id = 0;
        i4_return = x_thread_create(&ah_uart_inp_thread[ui2_id],
                                                UART_THREAD1_NAME,
                                                t_iom_thread_descr.z_stack_size,
                                                t_iom_thread_descr.ui1_priority,
                                                uart_inp_dptr_thread1,
                                                sizeof(UINT16),
                                                &ui2_id);
        if (i4_return != OSR_OK)
        {
             DBG_ERROR(("{IOM} ERR: uart %d x_thread_create() failed!\n\r", ui2_id));
             dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_THREAD);
        }
        
        ui2_id = 1;
        i4_return = x_thread_create(&ah_uart_inp_thread[ui2_id],
                                                UART_THREAD2_NAME,
                                                t_iom_thread_descr.z_stack_size,
                                                t_iom_thread_descr.ui1_priority,
                                                uart_inp_dptr_thread2,
                                                sizeof(UINT16),
                                                &ui2_id);
        if (i4_return != OSR_OK)
        {
            DBG_ERROR(("{IOM} ERR: uart %d x_thread_create() failed!\n\r", ui2_id));
            dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_THREAD);
        }


    /*   for (ui2_id = 0; ui2_id < UART_PORT_NUM; ui2_id++)
        {
            sprintf(s_name, UART_THREAD_NAME, ui2_id);

            i4_return = x_thread_create(&ah_uart_inp_thread[ui2_id],
                                        s_name,
                                        t_iom_thread_descr.z_stack_size,
                                        t_iom_thread_descr.ui1_priority,
                                        uart_inp_dptr_thread,
                                        sizeof(UINT16),
                                        &ui2_id);

            if (i4_return != OSR_OK)
            {
                DBG_ERROR(("{IOM} ERR: uart %d x_thread_create() failed!\n\r", ui2_id));
                dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_THREAD);
            }
        }    */
    }

    printf("{IOM} UART thread inited.\n");
#endif


    /* Create timers for auto sleep functionality */
    i4_return = x_timer_create(&h_prompt_timer);

    if (i4_return != OSR_OK)
    {
        DBG_ERROR(("{IOM} ERR: x_timer_create() failed\n\r"));
        return IOMR_TIMER_ERROR;
    }

    i4_return = x_timer_create(&h_sleep_timer);

    if (i4_return != OSR_OK)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: x_timer_create() failed\n\r"));
        return IOMR_TIMER_ERROR;
    }

    /* Initialize IO Manager CLI component */
    i4_return = iom_cli_init();

    if (i4_return != IOMR_OK)
    {
        return IOMR_CLI_ERROR;
    }




    b_iom_init = TRUE;

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _iom_handle_free_nfy_fct
 *
 * Description: This is handle free notify function.
 *
 * Inputs:  h_handle        A handle referencing the event filter.
 *          e_dev_type      Contains the handle type.
 *          pv_obj          References the data object.
 *          pv_tag          Contains the handle private tag.
 *          b_req_handle    If this argument is set to TRUE then the free
 *                          operation is specifically requested on this handle.
 *                          If this argument is set to FALSE, the handle is
 *                          being freed but this operation occurred due some
 *                          other effect.
 *
 * Outputs: -
 *
 * Returns: TRUE            Successful.
 *          FALSE           Failed.
 ----------------------------------------------------------------------------*/
static BOOL _iom_handle_free_nfy_fct(HANDLE_T      h_handle,
                                     HANDLE_TYPE_T e_dev_type,
                                     VOID*         pv_obj,
                                     VOID*         pv_tag,
                                     BOOL          b_req_handle)
{
    INT32       i4_return;

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
    EVT_FILTR_T*    pt_evt_filtr = (EVT_FILTR_T*)pv_obj;
#endif

    switch (e_dev_type)
    {
        case IOM_HANDLE_TYPE_IN:
#if  IOM_SUPPORT_IRRC_DISABLE_KEYS
        if ( NULL != pt_evt_filtr->u.s_inp.t_disable_keys.pui4_evtcode_arr)
        {
            x_mem_free(pt_evt_filtr->u.s_inp.t_disable_keys.pui4_evtcode_arr);
            pt_evt_filtr->u.s_inp.t_disable_keys.pui4_evtcode_arr = NULL;
            pt_evt_filtr->u.s_inp.t_disable_keys.z_evtcode_cnt = 0;
        }
#endif
        case IOM_HANDLE_TYPE_OUT:            
        case IOM_HANDLE_TYPE_IO:
            x_mem_free(pv_obj);
            i4_return = TRUE;
            break;

        default:
            i4_return = FALSE;
            break;
    }

    return i4_return;
}



/*-----------------------------------------------------------------------------
 * Name: iom_op_open_in
 *
 * Description: This API creates an event filter. The filter will be
 *              identified by the handle returned in argument ph_dev.
 *              Please note that an application may open multiple filters
 *              simultaneously and each filter is dedicated to one device type
 *              only.
 *
 * Inputs:  e_dev_type      The device type of the input source.
 *          ui2_id          The device id of the input source.
 *          ps_name         The device name of the input source.
 *          pv_setting      Configuration for event filter if required.
 *          pv_tag          Private tag value which must be returned
 *                          in the notify function. It can be set to
 *                          NULL.
 *          pf_nfy          References the caller's notify function.
 *
 * Outputs: ph_dev          A handle referencing the event filter.
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_INV_EVT_GRP_MASK       Invalid event group mask.
 *          IOMR_MEM_ALLOC_FAILED       Memory allocation is failed.
 *          IOMR_DEV_FULL               Device array is full.
 *          IOMR_EVT_FILTR_FULL         Reach the maximum of event filters
 *                                      associated to a device.
 *          IOMR_DEV_OPEN_FAILED        Device open operation is failed.
 *          IOMR_DEV_SET_FAILED         Device set operation is failed.
 *          IOMR_DEV_QUERY_FAILED       Device info query operation is failed.
 *          IOMR_DEV_CLOSE_FAILED       Device close operation is failed.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 *          IOMR_HANDLE_LIB_ERROR       A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_open_in(IOM_DEV_TYPE_T e_dev_type,
                     UINT16         ui2_id,
                     const CHAR*    ps_name,
                     VOID*          pv_setting,
                     VOID*          pv_tag,
                     x_iom_nfy_fct  pf_nfy,
                     HANDLE_T*      ph_dev)
{
    INT32               i4_return;
    DRV_TYPE_T          t_dev_type = DRVT_UNKNOWN;
    EVT_FILTR_T*        pt_evt_filtr = NULL;
    UINT16              ui2_comp_id = ui2_id;
    UINT8               ui1_inp_dev;
    BOOL                b_alloc_evt_filtr = FALSE;

    /* Check arguments */
    if (pf_nfy == NULL)
    {
        return IOMR_INV_ARG;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();

    if (ui2_id == IOM_DEV_ID_ANY)
    {
        ui2_id = ANY_COMP_ID;
    }

    /* Check if requested device has been opened */
    for (ui1_inp_dev = 0; ui1_inp_dev < IOM_MAX_DEV; ui1_inp_dev++)
    {
        if (at_in_dev[ui1_inp_dev].e_dev_type == IOM_DEV_TYPE_NONE) /* Device not opened yet */
        {
            /* Open the device */
            {
                {
                    switch (e_dev_type)
                    {
                        case IOM_DEV_TYPE_IN_IRRC:
                            if (pv_setting == NULL)
                            {
                                iom_unlock_db();
                                return IOMR_INV_ARG;
                            }
                            t_dev_type = DRVT_IRRC;
                            break;

                        case IOM_DEV_TYPE_IN_IRRK:
                        case IOM_DEV_TYPE_IN_IRRM:
                            iom_unlock_db();
                            return IOMR_NOT_SUPPORT;

                        default:
                            iom_unlock_db();
                            return IOMR_INV_ARG;
                    }

                }
            }

            /* Check if there is still a space */
            if (at_in_dev[ui1_inp_dev].ui1_evt_filtr_cnt >= IOM_MAX_EVT_FILTR_PER_DEV)
            {
                iom_unlock_db();
                return IOMR_EVT_FILTR_FULL;
            }

            /* Allocate an event filter */
            pt_evt_filtr = (EVT_FILTR_T*)x_mem_alloc(sizeof(EVT_FILTR_T));

            if (pt_evt_filtr != NULL)
            {
                x_memset(pt_evt_filtr, 0, sizeof(EVT_FILTR_T));
            }
            else
            {
                iom_unlock_db();
                DBG_ERROR(("{IOM} ERR: x_mem_alloc() failed\n\r"));
                return IOMR_MEM_ALLOC_FAILED;
            }

            at_in_dev[ui1_inp_dev].e_dev_type = e_dev_type;
            at_in_dev[ui1_inp_dev].ui2_id = ui2_comp_id;
            DLIST_INSERT_TAIL(pt_evt_filtr, &(at_in_dev[ui1_inp_dev].t_evt_filtr_list), t_link);
            at_in_dev[ui1_inp_dev].ui1_evt_filtr_cnt++;
            break;
        }
        else /* Device opened */
        {

            if ((e_dev_type == at_in_dev[ui1_inp_dev].e_dev_type) &&
                ((ui2_id == at_in_dev[ui1_inp_dev].ui2_id) || (ui2_id == ANY_COMP_ID)) &&
                ((ps_name == NULL) || (ps_name[0] == '\0')))
            {
                b_alloc_evt_filtr = TRUE;
            }
            if (b_alloc_evt_filtr)
            {
                /* Check if there is still a space */
                if (at_in_dev[ui1_inp_dev].ui1_evt_filtr_cnt >= IOM_MAX_EVT_FILTR_PER_DEV)
                {
                    iom_unlock_db();
                    return IOMR_EVT_FILTR_FULL;
                }

                /* Allocate an event filter */
                pt_evt_filtr = (EVT_FILTR_T*)x_mem_alloc(sizeof(EVT_FILTR_T));

                if (pt_evt_filtr != NULL)
                {
                    x_memset(pt_evt_filtr, 0, sizeof(EVT_FILTR_T));
                }
                else
                {
                    iom_unlock_db();
                    DBG_ERROR(("{IOM} ERR: x_mem_alloc() failed\n\r"));
                    return IOMR_MEM_ALLOC_FAILED;
                }

                /* Add the filter to the list */
                DLIST_INSERT_TAIL(pt_evt_filtr, &(at_in_dev[ui1_inp_dev].t_evt_filtr_list), t_link);
                at_in_dev[ui1_inp_dev].ui1_evt_filtr_cnt++;
                break;
            }
        }
    }

    /* Check if input device list is full */
    if (ui1_inp_dev == IOM_MAX_DEV)
    {
        iom_unlock_db();
        return IOMR_DEV_FULL;
    }

    /* Initialize the event filter */
    pt_evt_filtr->pv_tag = pv_tag;
    pt_evt_filtr->pf_nfy = pf_nfy;
    pt_evt_filtr->ui1_dev_idx = ui1_inp_dev;
    pt_evt_filtr->u.s_inp.ui4_evt_code = 0;
    x_memset(&pt_evt_filtr->u.s_inp.t_data, 0 , sizeof(IRRC_DATA_T));
    pt_evt_filtr->u.s_inp.h_timer = NULL_HANDLE;
    pt_evt_filtr->u.s_inp.e_timer = EVT_FILTR_TIMER_STOP;
    pt_evt_filtr->u.s_inp.ui2_long_press_cnt = 0;

    if (e_dev_type == IOM_DEV_TYPE_IN_IRRC)
    {
        if (((IRRC_SETTING_T*)pv_setting)->ui8_evt_grp_mask >= IOM_EVT_GRP_TO_MASK(KEY_GROUP_MAX))
        {
            DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_inp_dev].t_evt_filtr_list), t_link);
            at_in_dev[ui1_inp_dev].ui1_evt_filtr_cnt--;
            iom_unlock_db();
            return IOMR_INV_EVT_GRP_MASK;
        }
        else
        {
            pt_evt_filtr->u.s_inp.t_irrc_setting = *((IRRC_SETTING_T*)pv_setting);
        }
    }
    i4_return = handle_alloc(IOM_HANDLE_TYPE_IN,
                             pt_evt_filtr,
                             pv_tag,
                             &_iom_handle_free_nfy_fct,
                             &(pt_evt_filtr->h_filtr));

    if (i4_return != HR_OK)
    {
        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_inp_dev].t_evt_filtr_list), t_link);
        at_in_dev[ui1_inp_dev].ui1_evt_filtr_cnt--;
        x_mem_free(pt_evt_filtr);
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: handle_alloc() failed\n\r"));
        return IOMR_HANDLE_LIB_ERROR;
    }

    *ph_dev = pt_evt_filtr->h_filtr;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_close_in
 *
 * Description: This API frees an event filter referenced by h_dev.
 *
 * Inputs:  h_dev   A handle referencing the event filter.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 *          IOMR_HANDLE_LIB_ERROR   A handle library error occurred.
 *          IOMR_TIMER_ERROR        A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_close_in(HANDLE_T h_dev)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
#if 0
    /* Delete the timer */
    i4_return = x_timer_delete(pt_evt_filtr->u.s_inp.h_timer);

    if (i4_return != OSR_OK)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: x_timer_delete() failed\n\r"));
        return IOMR_TIMER_ERROR;
    }
#endif
    /* Remove the event filter from the event filter list */
    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;
    }
            
    DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[pt_evt_filtr->ui1_dev_idx].t_evt_filtr_list), t_link);
    at_in_dev[pt_evt_filtr->ui1_dev_idx].ui1_evt_filtr_cnt--;

    i4_return = handle_free(h_dev, TRUE);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: handle_free() failed\n\r"));
        return IOMR_HANDLE_LIB_ERROR;
    }

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_open_out
 *
 * Description: This API opens an output device based on the requirements.
 *              Please note that an application may open a device multiple
 *              times.
 *
 * Inputs:  e_dev_type      The device type of the output source.
 *          ui2_id          The device id of the output source.
 *          ps_name         The device name of the output source.
 *          pv_setting      Configuration for event filter if required.
 *          pv_tag          Private tag value which must be returned
 *                          in the notify function. It can be set to
 *                          NULL.
 *          pf_nfy          References the caller's notify function.
 *
 * Outputs: ph_dev          A handle referencing the device.
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_DEV_FULL               Device array is full.
 *          IOMR_DEV_OPEN_FAILED        Device open operation is failed.
 *          IOMR_DEV_QUERY_FAILED       Device info query operation is failed.
 *          IOMR_DEV_CLOSE_FAILED       Device close operation is failed.
 *          IOMR_HANDLE_LIB_ERROR       A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_open_out(IOM_DEV_TYPE_T e_dev_type,
                      UINT16         ui2_id,
                      const CHAR*    ps_name,
                      VOID*          pv_setting,
                      VOID*          pv_tag,
                      x_iom_nfy_fct  pf_nfy,
                      HANDLE_T*      ph_dev)
{
    INT32               i4_return;
    DRV_TYPE_T          t_dev_type = DRVT_UNKNOWN;
    EVT_FILTR_T*        pt_evt_filtr = NULL;
    UINT16              ui2_comp_id = ui2_id;
    UINT8               ui1_otp_dev;

    /* Check arguments */
    if (ph_dev == NULL)
    {
        return IOMR_INV_ARG;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();

    if (ui2_id == IOM_DEV_ID_ANY)
    {
        ui2_id = ANY_COMP_ID;
    }

    /* Check if requested device has been opened */
    for (ui1_otp_dev = 0; ui1_otp_dev < IOM_MAX_DEV; ui1_otp_dev++)
    {
        if (at_out_dev[ui1_otp_dev].e_dev_type == IOM_DEV_TYPE_NONE) /* Device not opened yet */
        {
            switch (e_dev_type)
            {
                case IOM_DEV_TYPE_OUT_IND_POWER:
                    t_dev_type = DRVT_IND_POWER;
                    break;

                case IOM_DEV_TYPE_OUT_FPD:
                    t_dev_type = DRVT_FP_DISPLAY;
                    break;

                case IOM_DEV_TYPE_OUT_IRB:
                    iom_unlock_db();
                    return IOMR_NOT_SUPPORT;

                default:
                    iom_unlock_db();
                    return IOMR_INV_ARG;
            }

            /* Open the device */

            /* Check if there is still a space */
            if (at_out_dev[ui1_otp_dev].ui1_evt_filtr_cnt >= IOM_MAX_EVT_FILTR_PER_DEV)
            {
                iom_unlock_db();
                return IOMR_EVT_FILTR_FULL;
            }

            /* Allocate an event filter */
            pt_evt_filtr = (EVT_FILTR_T*)x_mem_alloc(sizeof(EVT_FILTR_T));

            if (pt_evt_filtr != NULL)
            {
                x_memset(pt_evt_filtr, 0, sizeof(EVT_FILTR_T));
            }
            else
            {
                iom_unlock_db();
                DBG_ERROR(("{IOM} ERR: x_mem_alloc() failed\n\r"));
                return IOMR_MEM_ALLOC_FAILED;
            }

            /* Add the filter to the list */
            /* Query component info again in case ui2_id = IOM_DEV_ID_ANY */

            at_out_dev[ui1_otp_dev].e_dev_type = e_dev_type;
            at_out_dev[ui1_otp_dev].ui2_id = ui2_comp_id;
            DLIST_INSERT_TAIL(pt_evt_filtr, &(at_out_dev[ui1_otp_dev].t_evt_filtr_list), t_link);
            at_out_dev[ui1_otp_dev].ui1_evt_filtr_cnt++;
            break;
        }
        else /* Device opened */
        {
            if ((e_dev_type == at_out_dev[ui1_otp_dev].e_dev_type) &&
                ((ui2_id == at_out_dev[ui1_otp_dev].ui2_id) || (ui2_id == ANY_COMP_ID)) &&
                ((ps_name == NULL) || (ps_name[0] == '\0')))
            {
                /* Check if there is still a space */
                if (at_out_dev[ui1_otp_dev].ui1_evt_filtr_cnt >= IOM_MAX_EVT_FILTR_PER_DEV)
                {
                    iom_unlock_db();
                    return IOMR_EVT_FILTR_FULL;
                }

                /* Allocate an event filter */
                pt_evt_filtr = (EVT_FILTR_T*)x_mem_alloc(sizeof(EVT_FILTR_T));

                if (pt_evt_filtr != NULL)
                {
                    x_memset(pt_evt_filtr, 0, sizeof(EVT_FILTR_T));
                }
                else
                {
                    iom_unlock_db();
                    DBG_ERROR(("{IOM} ERR: x_mem_alloc() failed\n\r"));
                    return IOMR_MEM_ALLOC_FAILED;
                }

                /* Add the filter to the list */
                DLIST_INSERT_TAIL(pt_evt_filtr, &(at_out_dev[ui1_otp_dev].t_evt_filtr_list), t_link);
                at_out_dev[ui1_otp_dev].ui1_evt_filtr_cnt++;
                break;
            }
        }
    }

    /* Check if output device list is full */
    if (ui1_otp_dev == IOM_MAX_DEV)
    {
        iom_unlock_db();
        return IOMR_DEV_FULL;
    }

    /* Initialize the event filter */
    pt_evt_filtr->pv_tag = pv_tag;
    pt_evt_filtr->pf_nfy = pf_nfy;
    pt_evt_filtr->ui1_dev_idx = ui1_otp_dev;

    i4_return = handle_alloc(IOM_HANDLE_TYPE_OUT,
                             pt_evt_filtr,
                             pv_tag,
                             &_iom_handle_free_nfy_fct,
                             &(pt_evt_filtr->h_filtr));

    if (i4_return != HR_OK)
    {
        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_otp_dev].t_evt_filtr_list), t_link);
        at_out_dev[ui1_otp_dev].ui1_evt_filtr_cnt--;
        x_mem_free(pt_evt_filtr);
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: handle_alloc() failed\n\r"));
        return IOMR_HANDLE_LIB_ERROR;
    }

    *ph_dev = pt_evt_filtr->h_filtr;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_close_out
 *
 * Description: This API frees the reference to the output device.
 *
 * Inputs:  h_dev       A handle referencing the output device.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_DEV_BUSY           Device is busy.
 *          IOMR_INV_HANDLE         The handle is invalid.
 *          IOMR_HANDLE_LIB_ERROR   A handle library error occurred.
 *          IOMR_TIMER_ERROR        A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_close_out(HANDLE_T h_dev)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
    
    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }    

    if (at_out_dev[pt_evt_filtr->ui1_dev_idx].u.b_busy)
    {
        iom_unlock_db();
        return IOMR_DEV_BUSY;
    }

    /* Remove the event filter from the event filter list */
    DLIST_REMOVE(pt_evt_filtr, &(at_out_dev[pt_evt_filtr->ui1_dev_idx].t_evt_filtr_list), t_link);
    at_out_dev[pt_evt_filtr->ui1_dev_idx].ui1_evt_filtr_cnt--;

    i4_return = handle_free(h_dev, TRUE);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: handle_free() failed\n\r"));
        return IOMR_HANDLE_LIB_ERROR;
    }

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_open_io
 *
 * Description: This API opens an IO device based on the requirements.
 *              Please note that an application may open a device multiple
 *              times.
 *
 * Inputs:  e_dev_type      The device type of the IO source.
 *          ui2_id          The device id of the IO source.
 *          ps_name         The device name of the IO source.
 *          pv_setting      Configuration for event filter if required.
 *          pv_tag          Private tag value which must be returned
 *                          in the notify function. It can be set to
 *                          NULL.
 *          pf_nfy          References the caller's notify function.
 *
 * Outputs: ph_dev          A handle referencing the device.
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_DEV_FULL               Device array is full.
 *          IOMR_DEV_OPEN_FAILED        Device open operation is failed.
 *          IOMR_DEV_QUERY_FAILED       Device info query operation is failed.
 *          IOMR_DEV_CLOSE_FAILED       Device close operation is failed.
 *          IOMR_HANDLE_LIB_ERROR       A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_open_io(IOM_DEV_TYPE_T e_dev_type,
                     UINT16         ui2_id,
                     const CHAR*    ps_name,
                     VOID*          pv_setting,
                     VOID*          pv_tag,
                     x_iom_nfy_fct  pf_nfy,
                     HANDLE_T*      ph_dev)
{
    INT32                       i4_return;
    DRV_TYPE_T                  t_dev_type = DRVT_UNKNOWN;
    EVT_FILTR_T*                pt_evt_filtr = NULL;
    UINT16                      ui2_comp_id = ui2_id;
    UINT8                       ui1_io_dev;
    INT32                       fd = 0;

    /* Check arguments */
    if (ph_dev == NULL)
    {
        return IOMR_INV_ARG;
    }
    if (e_dev_type == IOM_DEV_TYPE_IO_UART &&
        !(ui2_id < UART_PORT_NUM || ui2_id == FACTORY_PORT_ID))
    {
        return IOMR_INV_ARG;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();

    if (ui2_id == IOM_DEV_ID_ANY)
    {
        ui2_id = ANY_COMP_ID;
    }

    /* Check if requested device has been opened */
    for (ui1_io_dev = 0; ui1_io_dev < IOM_MAX_DEV; ui1_io_dev++)
    {
        if (at_io_dev[ui1_io_dev].e_dev_type == IOM_DEV_TYPE_NONE) /* Device not opened yet */
        {
            switch (e_dev_type)
            {
                case IOM_DEV_TYPE_IO_UART:
                    t_dev_type = DRVT_COM_RS_232;
                    break;

                case IOM_DEV_TYPE_IO_I2C:
                    iom_unlock_db();
                    return IOMR_NOT_SUPPORT;

                default:
                    iom_unlock_db();
                    return IOMR_INV_ARG;
            }

            /* Open the device */
            if (e_dev_type == IOM_DEV_TYPE_IO_UART )
            {
                //INT32   fd;
                if(UART_GET_PORT(ui2_id)== 1)
                {

                    fd = open(UART_DEV_NAME(ui2_id), O_RDWR|O_NOCTTY|O_NDELAY);
                    if (-1 == fd)
                    {
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: device %s open fail, errno: %d\n", __LINE__, UART_DEV_NAME(ui2_id), errno);
                        return IOMR_DEV_OPEN_FAILED;
                    }
                    close(fd);
                }
                else if(b_ttymt3_opened==FALSE)
                {    
                    fd = open(UART_DEV_NAME(ui2_id), O_RDWR|O_NOCTTY|O_NDELAY);
                    TTYMT3_FD = fd;
                    
                    if (-1 == fd)
                    {
                         iom_unlock_db();
                        printf("{IOM} %s error opening device %s\n", __func__, UART_DEV_NAME(ui2_id));
                        return IOMR_DEV_OPEN_FAILED;
                    }
                   
                    if( tcgetattr(fd, &t_term_cli) == -1 )
                    {
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: device %s get original CLI attribute fail, errno: %d\n", __LINE__, UART_DEV_NAME(ui2_id), errno);
                        return IOMR_DEV_OPEN_FAILED;
                    }
                    t_term_factory=t_term_cli;
                    t_term_factory.c_cflag &= ~CSIZE;
                    t_term_factory.c_cflag |= CS8;
                    t_term_factory.c_cflag &= ~PARENB;
                    t_term_factory.c_cflag &= ~PARODD;
                    t_term_factory.c_cflag &= ~CMSPAR;
                    t_term_factory.c_cflag &= ~CSTOPB; 
                    t_term_factory.c_iflag = IGNPAR;
                    t_term_factory.c_oflag = 0;         
                    t_term_factory.c_lflag = 0;         
                    t_term_factory.c_cc[VTIME] = UART_FACTORY_CMD_INTER_BYTE_TIMEOUT;
                    t_term_factory.c_cc[VMIN]  = UART_FACTORY_CMD_BUF_LEN;
                    b_ttymt3_opened=TRUE;
                }
            }

            /* Check if there is still a space */
            if (at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt >= IOM_MAX_EVT_FILTR_PER_DEV)
            {
                iom_unlock_db();
                return IOMR_EVT_FILTR_FULL;
            }

            /* Allocate an event filter */
            pt_evt_filtr = (EVT_FILTR_T*)x_mem_alloc(sizeof(EVT_FILTR_T));

            if (pt_evt_filtr != NULL)
            {
                x_memset(pt_evt_filtr, 0, sizeof(EVT_FILTR_T));
            }
            else
            {
                iom_unlock_db();
                DBG_ERROR(("{IOM} ERR: x_mem_alloc() failed\n\r"));
                return IOMR_MEM_ALLOC_FAILED;
            }

            /* Add the filter to the list */

            at_io_dev[ui1_io_dev].e_dev_type = e_dev_type;
            at_io_dev[ui1_io_dev].ui2_id = ui2_comp_id;
            DLIST_INSERT_TAIL(pt_evt_filtr, &(at_io_dev[ui1_io_dev].t_evt_filtr_list), t_link);
            at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt++;
            break;
        }
        else /* Device opened */
        {
            if ((e_dev_type == at_io_dev[ui1_io_dev].e_dev_type) &&
                ((ui2_id == at_io_dev[ui1_io_dev].ui2_id) || (ui2_id == ANY_COMP_ID)) &&
                ((ps_name == NULL) || (ps_name[0] == '\0')))
            {
                /* Check if there is still a space */
                if (at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt >= IOM_MAX_EVT_FILTR_PER_DEV)
                {
                    iom_unlock_db();
                    return IOMR_EVT_FILTR_FULL;
                }

                /* Allocate an event filter */
                pt_evt_filtr = (EVT_FILTR_T*)x_mem_alloc(sizeof(EVT_FILTR_T));

                if (pt_evt_filtr != NULL)
                {
                    x_memset(pt_evt_filtr, 0, sizeof(EVT_FILTR_T));
                }
                else
                {
                    iom_unlock_db();
                    DBG_ERROR(("{IOM} ERR: x_mem_alloc() failed\n\r"));
                    return IOMR_MEM_ALLOC_FAILED;
                }

                /* Add the filter to the list */
                DLIST_INSERT_TAIL(pt_evt_filtr, &(at_io_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt++;
                break;
            }
        }
    }

    /* Check if output device list is full */
    if (ui1_io_dev == IOM_MAX_DEV)
    {
        iom_unlock_db();
        return IOMR_DEV_FULL;
    }

    /* Configure the device if requested */
    switch (e_dev_type)
    {
        case IOM_DEV_TYPE_IO_UART:
            if (pv_setting != NULL)
            {
                //INT32           fd = 0;
                struct termios  t_term;
                UART_SETTING_T* pt_uart_setting;
                pt_uart_setting = (UART_SETTING_T *) pv_setting;

                /* Get UART capability */
                if(UART_GET_PORT(ui2_id)==1)
                {
                    fd = open(UART_DEV_NAME(ui2_id), O_RDWR|O_NOCTTY|O_NDELAY);
                    if (-1 == fd)
                        {
                        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                        at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: device %s open fail, errno: %d\n", __LINE__, UART_DEV_NAME(ui2_id), errno);
                        return IOMR_DEV_OPEN_FAILED;
                    }
                }
                else
                {    
                    fd = TTYMT3_FD;
                }
                
                if(tcgetattr(fd, &t_term) == -1)
                {
                    if(UART_GET_PORT(ui2_id)==1)
                    {
                        close(fd);
                    }
                    DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                    at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                    iom_unlock_db();
                    printf("{IOM}:%d ERR: device %s get attribute fail, errno: %d\n", __LINE__, UART_DEV_NAME(ui2_id), errno);
                    return IOMR_DEV_QUERY_FAILED;
                }
                
                /* Do UART setup */
                t_term.c_cflag &= ~CBAUD;

                switch(pt_uart_setting->e_speed)
                {
                    case IOM_UART_SPEED_75:
                        t_term.c_cflag |= B75;
                        break;
                    case IOM_UART_SPEED_110:
                        t_term.c_cflag |= B110;
                        break;
                    case IOM_UART_SPEED_134:
                        t_term.c_cflag |= B134;
                        break;
                    case IOM_UART_SPEED_150:
                        t_term.c_cflag |= B150;
                        break;
                    case IOM_UART_SPEED_300:
                        t_term.c_cflag |= B300;
                        break;
                    case IOM_UART_SPEED_600:
                        t_term.c_cflag |= B600;
                        break;
                    case IOM_UART_SPEED_1200:
                        t_term.c_cflag |= B1200;
                        break;
                    case IOM_UART_SPEED_1800:
                        t_term.c_cflag |= B1800;
                        break;
                    case IOM_UART_SPEED_2400:
                        t_term.c_cflag |= B2400;
                        break;
                    case IOM_UART_SPEED_4800:
                        t_term.c_cflag |= B4800;
                        break;
                    case IOM_UART_SPEED_9600:
                        t_term.c_cflag |= B9600;
                        break;
                    case IOM_UART_SPEED_19200:
                        t_term.c_cflag |= B19200;
                        break;
                    case IOM_UART_SPEED_38400:
                        t_term.c_cflag |= B38400;
                        break;
                    case IOM_UART_SPEED_57600:
                        t_term.c_cflag |= B57600;
                        break;
                    case IOM_UART_SPEED_115200:
                        t_term.c_cflag |= B115200;
                        break;
                    case IOM_UART_SPEED_230400:
                        t_term.c_cflag |= B230400;
                        break;
                    case IOM_UART_SPEED_460800:
                        t_term.c_cflag |= B460800;
                        break;
                    case IOM_UART_SPEED_921600:
                        t_term.c_cflag |= B921600;
                        break;
                    default:
                        if(UART_GET_PORT(ui2_id)==1)
                        {
                            close(fd);
                        }
                        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                        at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: %s set IOM_UART_SPEED_T fail\n", __LINE__, UART_DEV_NAME(ui2_id));
                        return IOMR_DEV_QUERY_FAILED;
                }

#ifndef IOM_DISABLE_UART_FACTORY_MODE
                /* Handle factory setting first. see FACTORY_SetParameter in uart_mw.c */
                if (ui2_id == FACTORY_PORT_ID)
                {
                    if (!(pt_uart_setting->e_data_len == IOM_UART_DATA_LEN_8  &&
                          pt_uart_setting->e_parity   == IOM_UART_PARITY_NONE &&
                          pt_uart_setting->e_stop_bit == IOM_UART_STOP_BIT_1))
                    {
                        //close(fd);
                        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                        at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: factory set only support e_speed\n", __LINE__);
                        return IOMR_NOT_SUPPORT;
                    }
                    if (pt_uart_setting->e_speed == IOM_UART_SPEED_75)
                    {
                        if (ab_factory_mode[UART_GET_PORT(ui2_id)] == TRUE)
                        {    

                            e_factory_speed = pt_uart_setting->e_speed;
                            if(_set_non_canonical_input(ui2_id, FALSE)==IOMR_OK)
                            {
                                ab_factory_mode[UART_GET_PORT(ui2_id)] = FALSE;
                            }
                            else
                            {
                                iom_unlock_db();
                                printf("{IOM}:%d ERR: factory set attribute fail, errno: %d\n", __LINE__, errno);
                                return IOMR_DEV_SET_FAILED;
                            }
                            
                        }
                    }
                    else
                    {
                        t_term.c_cflag &= ~CSIZE;
                        t_term.c_cflag |= CS8;
                        t_term.c_cflag &= ~PARENB;
                        t_term.c_cflag &= ~PARODD;
                        t_term.c_cflag &= ~CMSPAR;
                        t_term.c_cflag &= ~CSTOPB;

                        t_term_factory.c_cflag = t_term.c_cflag;
                        if(ab_factory_mode[UART_GET_PORT(ui2_id)]==TRUE)
                        {
                            if(tcsetattr(TTYMT3_FD, TCSAFLUSH, &t_term_factory) == -1)
                            {

                                DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                                at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                                iom_unlock_db();
                                printf("{IOM}:%d ERR: factory set attribute fail, errno: %d\n", __LINE__, errno);
                                return IOMR_DEV_SET_FAILED;
                            }
                        }

                        e_factory_speed = pt_uart_setting->e_speed;
                    }
                    break;
                }
#endif

                t_term.c_cflag &= ~CSIZE;

                switch(pt_uart_setting->e_data_len)
                {
                    case IOM_UART_DATA_LEN_5:
                        t_term.c_cflag |= CS5;
                        break;
                    case IOM_UART_DATA_LEN_6:
                        t_term.c_cflag |= CS6;
                        break;
                    case IOM_UART_DATA_LEN_7:
                        t_term.c_cflag |= CS7;
                        break;
                    case IOM_UART_DATA_LEN_8:
                        t_term.c_cflag |= CS8;
                        break;
                    default:
                        close(fd);
                        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                        at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: %s set IOM_UART_DATA_LEN_T fail\n", __LINE__, UART_DEV_NAME(ui2_id));
                        return IOMR_DEV_SET_FAILED;
                }

                t_term.c_cflag &= ~PARENB;
                t_term.c_cflag &= ~PARODD;
                t_term.c_cflag &= ~CMSPAR;

                switch(pt_uart_setting->e_parity)
                {
                    case IOM_UART_PARITY_NONE:
                        break;
                    case IOM_UART_PARITY_EVEN:
                        t_term.c_cflag |= PARENB;
                        break;
                    case IOM_UART_PARITY_ODD:
                        t_term.c_cflag |= PARENB;
                        t_term.c_cflag |= PARODD;
                        break;
                    case IOM_UART_PARITY_SPACE:
                        t_term.c_cflag |= PARENB;
                        t_term.c_cflag |= CMSPAR;
                        break;
                    case IOM_UART_PARITY_MARK:
                        t_term.c_cflag |= PARENB;
                        t_term.c_cflag |= CMSPAR;
                        t_term.c_cflag |= PARODD;
                        break;
                    default:
                        close(fd);
                        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                        at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: %s set IOM_UART_PARITY_T fail\n", __LINE__, UART_DEV_NAME(ui2_id));
                        return IOMR_DEV_SET_FAILED;
                }

                t_term.c_cflag &= ~CSTOPB;

                switch(pt_uart_setting->e_stop_bit)
                {
                    case IOM_UART_STOP_BIT_1:
                        break;
                    case IOM_UART_STOP_BIT_2:
                        t_term.c_cflag |= CSTOPB;
                        break;
                    default:
                        close(fd);
                        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                        at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                        iom_unlock_db();
                        printf("{IOM}:%d ERR: %s set IOM_UART_STOP_BIT_T fail\n", __LINE__, UART_DEV_NAME(ui2_id));
                        return IOMR_DEV_SET_FAILED;
                }

                if(tcsetattr(fd, TCSAFLUSH, &t_term) == -1)
                {
                    close(fd);
                    DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
                    at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
                    iom_unlock_db();
                    printf("{IOM}:%d ERR: device %s set attribute fail, errno: %d\n", __LINE__, UART_DEV_NAME(ui2_id), errno);
                    return IOMR_DEV_SET_FAILED;
                }

                close(fd);
            }
            break;

        case IOM_DEV_TYPE_IO_I2C:
            /* To be implemented */
            break;

        default:
            iom_unlock_db();
            return IOMR_INV_ARG;
    }

    /* Initialize the event filter */
    pt_evt_filtr->pv_tag = pv_tag;
    pt_evt_filtr->pf_nfy = pf_nfy;
    pt_evt_filtr->ui1_dev_idx = ui1_io_dev;

    i4_return = handle_alloc(IOM_HANDLE_TYPE_IO,
                             pt_evt_filtr,
                             pv_tag,
                             &_iom_handle_free_nfy_fct,
                             &(pt_evt_filtr->h_filtr));

    if (i4_return != HR_OK)
    {
        DLIST_REMOVE(pt_evt_filtr, &(at_in_dev[ui1_io_dev].t_evt_filtr_list), t_link);
        at_io_dev[ui1_io_dev].ui1_evt_filtr_cnt--;
        x_mem_free(pt_evt_filtr);
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: handle_alloc() failed\n\r"));
        return IOMR_HANDLE_LIB_ERROR;
    }

    *ph_dev = pt_evt_filtr->h_filtr;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_close_io
 *
 * Description: This API frees the reference to the IO device.
 *
 * Inputs:  h_dev       A handle referencing the IO device.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_DEV_BUSY           Device is busy.
 *          IOMR_INV_HANDLE         The handle is invalid.
 *          IOMR_HANDLE_LIB_ERROR   A handle library error occurred.
 *          IOMR_TIMER_ERROR        A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_close_io(HANDLE_T h_dev)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }
    
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].u.b_busy)
    {
        iom_unlock_db();
        return IOMR_DEV_BUSY;
    }

    /* Remove the event filter from the event filter list */
    DLIST_REMOVE(pt_evt_filtr, &(at_io_dev[pt_evt_filtr->ui1_dev_idx].t_evt_filtr_list), t_link);
    at_io_dev[pt_evt_filtr->ui1_dev_idx].ui1_evt_filtr_cnt--;

    i4_return = handle_free(h_dev, TRUE);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: handle_free() failed\n\r"));
        return IOMR_HANDLE_LIB_ERROR;
    }

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 iom_op_get_dbg_level(VOID)
{
    if (iom_is_init())
    {
        return ui2_iom_dbg_level;
    }
    else
    {
        return 0;
    }
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs:  ui2_level       The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
BOOL iom_op_set_dbg_level(UINT16 ui2_level)
{
    if (iom_is_init())
    {
        ui2_iom_dbg_level = ui2_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_get_in_dev_info_struct
 *
 * Description: This API returns the address of input device information
 *              structure.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of input device information structure.
 ----------------------------------------------------------------------------*/
DEV_INFO_T* iom_op_get_in_dev_info_struct(VOID)
{
    return at_in_dev;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_get_out_dev_info_struct
 *
 * Description: This API returns the address of output device information
 *              structure.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of output device information structure.
 ----------------------------------------------------------------------------*/
DEV_INFO_T* iom_op_get_out_dev_info_struct(VOID)
{
    return at_out_dev;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_get_io_dev_info_struct
 *
 * Description: This API returns the address of IO device information
 *              structure.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of IO device information structure.
 ----------------------------------------------------------------------------*/
DEV_INFO_T* iom_op_get_io_dev_info_struct(VOID)
{
    return at_io_dev;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_set_in_irrc
 *
 * Description: This API enables/disables IRRC event input path.
 *
 * Inputs: b_enable     The control of IRRC event input status.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_op_set_in_irrc(BOOL b_enable)
{
    /* Lock DB for write operation */
    iom_lock_db_wr();

    b_in_irrc = b_enable;

    /* Unlock DB */
    iom_unlock_db();
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_irrc_protocol
 *
 * Description: This API gets current IRRC protocol from IR driver.
 *
 * Inputs:  h_dev           A handle referencing the event filter.
 *
 * Outputs: pe_protocol     References the IRRC protocol.
 *
 * Returns: IOMR_OK             Routine successful.
 *          IOMR_INV_HANDLE     The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_get_irrc_protocol(HANDLE_T             h_dev,
                               IOM_IRRC_PROTOCOL_T* pe_protocol)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;
    
    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
    
    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }    

    if (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    /* Get IRRC protocol */
    *pe_protocol = e_protocol;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_set_irrc_protocol
 *
 * Description: This API sets new IRRC protocol to IR driver.
 *
 * Inputs:  h_dev           A handle referencing the event filter.
 *          pe_protocol     References the IRRC protocol.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK             Routine successful.
 *          IOMR_INV_HANDLE     The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_set_irrc_protocol(HANDLE_T             h_dev,
                               IOM_IRRC_PROTOCOL_T* pe_protocol)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
    
    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }    

    if (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }

    /* Set IRRC protocol */
    i4_return = (*pe_protocol == e_protocol) ? IOMR_OK : IOMR_NOT_SUPPORT;

    /* Unlock DB */
    iom_unlock_db();

    return i4_return;
}

#if IOM_SUPPORT_IRRC_DISABLE_KEYS
/*-----------------------------------------------------------------------------
 * Name: iom_op_init_disable_evtcode_info
 *
 * Description: This API initializes the pt_dst_disableKeys with pt_src_disableKeys. 
 *                     if the   pt_dst_disableKeys.z_Keys_Cnt != 0, the values in the
 *                    pt_dst_disableKeys.pui4_Keys_arr will be refresh with 
 *                    pt_src_disableKeys.pui4_Keys_arr
 *
 * Inputs:  pt_dst_disableKeys              Reference the destnation to be inited 
 *            pt_src_disableKeys                References the source of disable keys
 *                                      structure.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_INV_ARG                One or more of the arguments contain   invalid data.
 *          IOMR_MEM_ALLOC_FAILED       Memory allocation is failed.
 ----------------------------------------------------------------------------*/
extern INT32 iom_op_init_disable_evtcode_info(DISABLE_EVTCODE_T *pt_dst_disablekeys, 
                                               DISABLE_EVTCODE_T *pt_src_disablekeys)                                             
{
   if ((NULL == pt_dst_disablekeys) ||
          (NULL == pt_src_disablekeys) ||
          ((0 != pt_src_disablekeys->z_evtcode_cnt) && 
           (NULL == pt_src_disablekeys->pui4_evtcode_arr)))
   {
     return IOMR_INV_ARG;
   }

   if (0 != pt_dst_disablekeys->z_evtcode_cnt)
   {
       x_mem_free(pt_dst_disablekeys->pui4_evtcode_arr);
       pt_dst_disablekeys->pui4_evtcode_arr = NULL;     
   }

   pt_dst_disablekeys->z_evtcode_cnt = pt_src_disablekeys->z_evtcode_cnt;

   if (0 != pt_dst_disablekeys->z_evtcode_cnt)
   {
       pt_dst_disablekeys->pui4_evtcode_arr = (UINT32 *) x_mem_alloc(sizeof(UINT32) * pt_dst_disablekeys->z_evtcode_cnt);
       if ( NULL == pt_dst_disablekeys->pui4_evtcode_arr)
       {
            pt_dst_disablekeys->z_evtcode_cnt = 0;
            return IOMR_MEM_ALLOC_FAILED;
       }
       else
       {
            x_memcpy(pt_dst_disablekeys->pui4_evtcode_arr,
                     pt_src_disablekeys->pui4_evtcode_arr,
                     pt_dst_disablekeys->z_evtcode_cnt * sizeof(UINT32));
       }         
   }

   /* init  disable/enable and filter */
   pt_dst_disablekeys->b_enable = pt_src_disablekeys->b_enable;
   pt_dst_disablekeys->b_positive_filter = pt_src_disablekeys->b_positive_filter;

   return IOMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_irrc_disablekeys_cnt
 *
 * Description: This API gets the disable keys  information from an
 *              event filter referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IN_IRRC device type.
 *
 * Inputs:  h_dev               A handle referencing the event filter.
 *
 * Outputs: z_Cnt              References the disable keys  count.
 *
 * Returns: IOMR_OK             Routine successful.
 *        IOMR_INV_HANDLE    The handle is invalid. */


INT32 iom_op_get_irrc_disablekeys_cnt(HANDLE_T h_dev,
                                         SIZE_T*     pz_Cnt)
{
    INT32                i4_return;   
    EVT_FILTR_T*           pt_evt_filtr;
    
      
   /* Lock DB for read operation */
    iom_lock_db_rd();
    
    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);
    
    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
    
    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
       (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }
    
    *pz_Cnt = pt_evt_filtr->u.s_inp.t_disable_keys.z_evtcode_cnt;
          
     /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;

}



/*-----------------------------------------------------------------------------
 * Name: iom_op_get_irrc_disablekeys_info
 *
 * Description: This API gets the disable keys  information from an
 *              event filter referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IN_IRRC device type.
 *
 * Inputs:  h_dev               A handle referencing the event filter.
 *
 * Outputs: pt_DisableKeys_info     References the disable keys 
 *                              information.
 *
 * Returns: IOMR_OK             Routine successful.
 *        IOMR_INV_HANDLE     The handle is invalid. */


INT32 iom_op_get_irrc_disablekeys_info(HANDLE_T             h_dev,
                                       DISABLE_EVTCODE_T* pt_disablekeys_info)
{
    INT32                i4_return;   
    EVT_FILTR_T*           pt_evt_filtr;
    
      
   /* Lock DB for write operation */
    iom_lock_db_rd();
    
    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);
    
    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
    
    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
       (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }
    
    if (pt_disablekeys_info->z_evtcode_cnt != pt_evt_filtr->u.s_inp.t_disable_keys.z_evtcode_cnt)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_ARG;
    }

    if ( 0 != pt_disablekeys_info->z_evtcode_cnt)
    {
        x_memcpy(pt_disablekeys_info->pui4_evtcode_arr, pt_evt_filtr->u.s_inp.t_disable_keys.pui4_evtcode_arr, pt_disablekeys_info->z_evtcode_cnt * sizeof(UINT32));
    }  

    pt_disablekeys_info->b_enable = pt_evt_filtr->u.s_inp.t_disable_keys.b_enable;
    pt_disablekeys_info->b_positive_filter = pt_evt_filtr->u.s_inp.t_disable_keys.b_positive_filter;
        
     /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: iom_op_set_irrc_disablekeys_info
 *
 * Description: This API sets the disable keys  information from an
 *              event filter referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IN_IRRC device type.
 *
 * Inputs:  h_dev               A handle referencing the event filter. 
 *   pt_DisableKeys_info     References the disable keys  information.
 *
 *  Outputs: -
 *
 * Returns: IOMR_OK             Routine successful.
 * IOMR_MEM_ALLOC_FAILED Allocate memory  failed
 *        IOMR_INV_HANDLE     The handle is invalid. */
 INT32 iom_op_set_irrc_disablekeys_info(HANDLE_T             h_dev,
                                       DISABLE_EVTCODE_T* pt_disablekeys_info)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;
    
    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (!IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV) &&
        (at_in_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IN_IRRC))
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IN_IRRC\n\r"));
        return IOMR_INV_HANDLE;
    }
    
    i4_return = iom_op_init_disable_evtcode_info(&(pt_evt_filtr->u.s_inp.t_disable_keys),
                                                  pt_disablekeys_info);

    if (IOMR_MEM_ALLOC_FAILED == i4_return)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: alloc mem failed while set diablekeys info\n\r"));
        return IOMR_MEM_ALLOC_FAILED;
    }

    if (IOMR_INV_ARG == i4_return)
    {
       iom_unlock_db();
       DBG_ERROR(("{IOM} ERR: Arg invalid while set diablekeys info\n\r"));
       return IOMR_INV_ARG;
    }
        
    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;

}

/*-----------------------------------------------------------------------------
 * Name: iom_op_set_global_disable_evtcode
 *
 * Description: this API reset the global disable evtcode with pt_disable_evtcode
 * Inputs:  pt_disable_evtcode       References the disable event code info
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_INV_ARG                One or more of the arguments contain   invalid data.
 *          IOMR_MEM_ALLOC_FAILED       Memory allocation is failed.
 ----------------------------------------------------------------------------*/
INT32   iom_op_set_global_disable_evtcode(DISABLE_EVTCODE_T* pt_disable_evtcode)
{
    if (NULL == pt_disable_evtcode)
    {
        return IOMR_INV_ARG;
    }

    iom_op_init_disable_evtcode_info(&t_global_disable_keys, pt_disable_evtcode);

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_global_disable_evtcode
 *
 * Description: this API reset the global disable evtcode with pt_disable_evtcode
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:      References the disable event code info
 ----------------------------------------------------------------------------*/
DISABLE_EVTCODE_T*   iom_op_get_global_disable_evtcode(VOID)
{
    return &t_global_disable_keys;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_uart_setting
 *
 * Description: This API gets the UART driver setting via an event filter
 *              referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IO_UART device type.
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *
 * Outputs: pt_uart_setting         References the UART setting structure.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_get_uart_setting(HANDLE_T        h_dev,
                              UART_SETTING_T* pt_uart_setting)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;
    struct termios      t_term;
    INT32               fd;
    UINT16                ui2_uart_port;

    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }
    
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IO_UART)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IO_UART\n\r"));
        return IOMR_INV_HANDLE;
    }

    /* Get UART setup */
    ui2_uart_port = UART_GET_PORT(at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id);
    if(ui2_uart_port==1)
    {
        fd = open(UART_DEV_NAME(at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id), O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: device open fail\n\r"));
            return IOMR_DEV_OPEN_FAILED;
           }
        

        if(tcgetattr(fd, &t_term) == -1)
        {
            close(fd);
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: device get attribute fail\n\r"));
            return IOMR_DEV_QUERY_FAILED;
        }
    }
    else
    {
        fd=TTYMT3_FD;
        if(tcgetattr(fd, &t_term) == -1)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: device get attribute fail\n\r"));
            return IOMR_DEV_QUERY_FAILED;
        }
    }
    /*else if(ab_factory_mode[ui2_uart_port] == FALSE)
    {
        t_term = t_term_cli;
    }
    else
    {
        t_term = t_term_factory;
    }*/

    /* Handle factory setting first. see FACTORY_GetParameter in uart_mw.c */
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id == FACTORY_PORT_ID)
    {
        pt_uart_setting->e_speed    = e_factory_speed;
        pt_uart_setting->e_data_len = IOM_UART_DATA_LEN_8;
        pt_uart_setting->e_parity   = IOM_UART_PARITY_NONE;
        pt_uart_setting->e_stop_bit = IOM_UART_STOP_BIT_1;
        //close(fd);
        iom_unlock_db();
        return IOMR_OK;
    }

    switch(t_term.c_cflag & CBAUD)
    {
        case B75:
            pt_uart_setting->e_speed = IOM_UART_SPEED_75;
            break;
        case B110:
            pt_uart_setting->e_speed = IOM_UART_SPEED_110;
            break;
        case B134:
            pt_uart_setting->e_speed = IOM_UART_SPEED_134;
            break;
        case B150:
            pt_uart_setting->e_speed = IOM_UART_SPEED_150;
            break;
        case B300:
            pt_uart_setting->e_speed = IOM_UART_SPEED_300;
            break;
        case B600:
            pt_uart_setting->e_speed = IOM_UART_SPEED_600;
            break;
        case B1200:
            pt_uart_setting->e_speed = IOM_UART_SPEED_1200;
            break;
        case B1800:
            pt_uart_setting->e_speed = IOM_UART_SPEED_1800;
            break;
        case B2400:
            pt_uart_setting->e_speed = IOM_UART_SPEED_2400;
            break;
        case B4800:
            pt_uart_setting->e_speed = IOM_UART_SPEED_4800;
            break;
        case B9600:
            pt_uart_setting->e_speed = IOM_UART_SPEED_9600;
            break;
        case B19200:
            pt_uart_setting->e_speed = IOM_UART_SPEED_19200;
            break;
        case B38400:
            pt_uart_setting->e_speed = IOM_UART_SPEED_38400;
            break;
        case B57600:
            pt_uart_setting->e_speed = IOM_UART_SPEED_57600;
            break;
        case B115200:
            pt_uart_setting->e_speed = IOM_UART_SPEED_115200;
            break;
        case B230400:
            pt_uart_setting->e_speed = IOM_UART_SPEED_230400;
            break;
        case B460800:
            pt_uart_setting->e_speed = IOM_UART_SPEED_460800;
            break;
        case B921600:
            pt_uart_setting->e_speed = IOM_UART_SPEED_921600;
            break;
        default:
            if(ui2_uart_port==1)
            {    
                close(fd);
            }
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: get IOM_UART_SPEED_T failed\n\r"));
            return IOMR_DEV_QUERY_FAILED;
    }

    switch(t_term.c_cflag & CSIZE)
    {
        case CS5:
            pt_uart_setting->e_data_len = IOM_UART_DATA_LEN_5;
            break;
        case CS6:
            pt_uart_setting->e_data_len = IOM_UART_DATA_LEN_6;
            break;
        case CS7:
            pt_uart_setting->e_data_len = IOM_UART_DATA_LEN_7;
            break;
        case CS8:
            pt_uart_setting->e_data_len = IOM_UART_DATA_LEN_8;
            break;
        default:
            if(ui2_uart_port==1)
            {    
                close(fd);
            }
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: get IOM_UART_DATA_LEN_T failed\n\r"));
            return IOMR_DEV_QUERY_FAILED;
    }
    
    if (t_term.c_cflag & PARENB)
    {
        if (t_term.c_cflag & CMSPAR)
        {
            if (t_term.c_cflag & PARODD)
            {
                pt_uart_setting->e_parity = IOM_UART_PARITY_MARK;
            }
            else
            {
                pt_uart_setting->e_parity = IOM_UART_PARITY_SPACE;
            }
        }
        else
        {
            if (t_term.c_cflag & PARODD)
            {
                pt_uart_setting->e_parity = IOM_UART_PARITY_ODD;
            }
            else
            {
                pt_uart_setting->e_parity = IOM_UART_PARITY_EVEN;
            }
        }        
    }
    else
    {
        pt_uart_setting->e_parity = IOM_UART_PARITY_NONE;
    }
    
    if (t_term.c_cflag & CSTOPB)
    {
        pt_uart_setting->e_stop_bit = IOM_UART_STOP_BIT_2;
    }
    else
    {
        pt_uart_setting->e_stop_bit = IOM_UART_STOP_BIT_1;
    }

    if(ui2_uart_port==1)
    {    
        close(fd);
    }
    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_set_uart_setting
 *
 * Description: This API does the UART driver setting via an event filter
 *              referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IO_UART device type.
 *
 * Inputs:  h_dev               A handle referencing the event filter.
 *          pt_uart_setting     References the UART setting structure.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_set_uart_setting(HANDLE_T        h_dev,
                              UART_SETTING_T* pt_uart_setting)
{
    INT32                       i4_return;
    EVT_FILTR_T*                pt_evt_filtr;
    struct termios              t_term;
    INT32                       fd;
    UINT16                ui2_uart_port;

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }
    
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IO_UART)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IO_UART\n\r"));
        return IOMR_INV_HANDLE;
    }

    /* Get UART capability */
    ui2_uart_port = UART_GET_PORT(at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id);
    if(ui2_uart_port==1)
    {
        fd = open(UART_DEV_NAME(at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id), O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: device open fail\n\r"));
            return IOMR_DEV_OPEN_FAILED;
        }
    }
    else
    {    
        fd = TTYMT3_FD;
    }
    if(tcgetattr(fd, &t_term) == -1)
    {
        if(ui2_uart_port==1)
        {    
            close(fd);
        }
           iom_unlock_db();
           DBG_ERROR(("{IOM} ERR: device get attribute fail\n\r"));
           return IOMR_DEV_QUERY_FAILED;
    }
    
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id == FACTORY_PORT_ID)
    {
        if (!(pt_uart_setting->e_data_len == IOM_UART_DATA_LEN_8  &&
              pt_uart_setting->e_parity   == IOM_UART_PARITY_NONE &&
              pt_uart_setting->e_stop_bit == IOM_UART_STOP_BIT_1))
        {
            //close(fd);
            iom_unlock_db();
            printf("{IOM}:%d ERR: factory set only support e_speed\n", __LINE__);
            return IOMR_NOT_SUPPORT;
        }
    }

    /* Do UART setup */
    t_term.c_cflag &= ~CBAUD;
    
    switch(pt_uart_setting->e_speed)
    {
        case IOM_UART_SPEED_75:
            t_term.c_cflag |= B75;
            break;
        case IOM_UART_SPEED_110:
            t_term.c_cflag |= B110;
            break;
        case IOM_UART_SPEED_134:
            t_term.c_cflag |= B134;
            break;
        case IOM_UART_SPEED_150:
            t_term.c_cflag |= B150;
            break;
        case IOM_UART_SPEED_300:
            t_term.c_cflag |= B300;
            break;
        case IOM_UART_SPEED_600:
            t_term.c_cflag |= B600;
            break;
        case IOM_UART_SPEED_1200:
            t_term.c_cflag |= B1200;
            break;
        case IOM_UART_SPEED_1800:
            t_term.c_cflag |= B1800;
            break;
        case IOM_UART_SPEED_2400:
            t_term.c_cflag |= B2400;
            break;
        case IOM_UART_SPEED_4800:
            t_term.c_cflag |= B4800;
            break;
        case IOM_UART_SPEED_9600:
            t_term.c_cflag |= B9600;
            break;
        case IOM_UART_SPEED_19200:
            t_term.c_cflag |= B19200;
            break;
        case IOM_UART_SPEED_38400:
            t_term.c_cflag |= B38400;
            break;
        case IOM_UART_SPEED_57600:
            t_term.c_cflag |= B57600;
            break;
        case IOM_UART_SPEED_115200:
            t_term.c_cflag |= B115200;
            break;
        case IOM_UART_SPEED_230400:
            t_term.c_cflag |= B230400;
            break;
        case IOM_UART_SPEED_460800:
            t_term.c_cflag |= B460800;
            break;
        case IOM_UART_SPEED_921600:
            t_term.c_cflag |= B921600;
            break;
        default:
            if(ui2_uart_port==1)
            {    
                close(fd);
            }
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: set IOM_UART_SPEED_T failed\n\r"));
            return IOMR_DEV_QUERY_FAILED;
    }

#ifndef IOM_DISABLE_UART_FACTORY_MODE
    /* Handle factory setting first. see FACTORY_SetParameter in uart_mw.c */
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id == FACTORY_PORT_ID)
    {
        if (pt_uart_setting->e_speed == IOM_UART_SPEED_75)
        {
            if (ab_factory_mode[ui2_uart_port] == TRUE)
            {                
                e_factory_speed = pt_uart_setting->e_speed;         
                if(_set_non_canonical_input(at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id, FALSE)==IOMR_OK)
                {
                    ab_factory_mode[ui2_uart_port] = FALSE;
                }
                else
                {
                    iom_unlock_db();
                    printf("{IOM}:%d ERR: factory set attribute fail, errno: %d\n", __LINE__, errno);
                    return IOMR_DEV_SET_FAILED;
                }                
            }
        }
        else
        {
            t_term.c_cflag &= ~CSIZE;
            t_term.c_cflag |= CS8;
            t_term.c_cflag &= ~PARENB;
            t_term.c_cflag &= ~PARODD;
            t_term.c_cflag &= ~CMSPAR;
            t_term.c_cflag &= ~CSTOPB;
            
            e_factory_speed = pt_uart_setting->e_speed;
            t_term_factory.c_cflag = t_term.c_cflag;
            if(ab_factory_mode[ui2_uart_port]==TRUE)
            {
                if(tcsetattr(TTYMT3_FD, TCSAFLUSH, &t_term_factory) == -1)
                {
                    iom_unlock_db();
                    printf("{IOM}:%d ERR: factory set attribute fail, errno: %d\n", __LINE__, errno);
                    return IOMR_DEV_SET_FAILED;
                }
            }
        }

        iom_unlock_db();

        return IOMR_OK;
    }
#endif

    t_term.c_cflag &= ~CSIZE;

    switch(pt_uart_setting->e_data_len)
    {
        case IOM_UART_DATA_LEN_5:
            t_term.c_cflag |= CS5;
            break;
        case IOM_UART_DATA_LEN_6:
            t_term.c_cflag |= CS6;
            break;
        case IOM_UART_DATA_LEN_7:
            t_term.c_cflag |= CS7;
            break;
        case IOM_UART_DATA_LEN_8:
            t_term.c_cflag |= CS8;
            break;
        default:
             close(fd);
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: set IOM_UART_DATA_LEN_T failed\n\r"));
            return IOMR_DEV_SET_FAILED;
    }

    t_term.c_cflag &= ~PARENB;
    t_term.c_cflag &= ~PARODD;
    t_term.c_cflag &= ~CMSPAR;
    
    switch(pt_uart_setting->e_parity)
    {
        case IOM_UART_PARITY_NONE:
            break;
        case IOM_UART_PARITY_EVEN:
            t_term.c_cflag |= PARENB;
            break;
        case IOM_UART_PARITY_ODD:
            t_term.c_cflag |= PARENB;
            t_term.c_cflag |= PARODD;
            break;
        case IOM_UART_PARITY_SPACE:
            t_term.c_cflag |= PARENB;
            t_term.c_cflag |= CMSPAR;
            break;
        case IOM_UART_PARITY_MARK:
            t_term.c_cflag |= PARENB;
            t_term.c_cflag |= CMSPAR;
            t_term.c_cflag |= PARODD;
            break;
        default:
            close(fd);
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: set IOM_UART_PARITY_T failed\n\r"));
            return IOMR_DEV_SET_FAILED;
    }

    t_term.c_cflag &= ~CSTOPB;
    
    switch(pt_uart_setting->e_stop_bit)
    {
        case IOM_UART_STOP_BIT_1:
            break;
        case IOM_UART_STOP_BIT_2:
            t_term.c_cflag |= CSTOPB;
            break;
        default:
            close(fd);
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: set IOM_UART_STOP_BIT_T failed\n\r"));
            return IOMR_DEV_SET_FAILED;
    }

    if(tcsetattr(fd, TCSAFLUSH, &t_term) == -1)
    {
        close(fd);
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device set attribute fail\n\r"));
        return IOMR_DEV_SET_FAILED;
    }   
    
    close(fd);

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_get_uart_operation_mode
 *
 * Description: This API gets the UART operation mode via an event filter
 *              referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IO_UART device type.
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *
 * Outputs: pui4_operation_mode     References the UART operation mode.
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_get_uart_operation_mode(HANDLE_T h_dev,
                                     UINT32*  pui4_operation_mode)
{
    INT32               i4_return;
    EVT_FILTR_T*        pt_evt_filtr;
    INT32               ui4_operation_mode;

    /* Lock DB for read operation */
    iom_lock_db_rd();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }
    
    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }

    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IO_UART)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IO_UART\n\r"));
        return IOMR_INV_HANDLE;
    }

    /* Get UART operation mode */
    ui4_operation_mode = IOM_UART_OPERATION_MODE_TRANSPARENT;
    if (ab_factory_mode[UART_GET_PORT(at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id)])
    {
        ui4_operation_mode |= IOM_UART_OPERATION_MODE_FACTORY;
    }
    else
    {
        ui4_operation_mode |= IOM_UART_OPERATION_MODE_CLI;
    }

    *pui4_operation_mode = ui4_operation_mode;

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: iom_op_set_uart_operation_mode
 *
 * Description: This API sets the UART operation mode via an event filter
 *              referenced by h_dev. The filter must belong to
 *              IOM_DEV_TYPE_IO_UART device type.
 *
 * Inputs:  h_dev                   A handle referencing the event filter.
 *          pui4_operation_mode     References the UART operation mode.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                 Routine successful.
 *          IOMR_INV_HANDLE         The handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 iom_op_set_uart_operation_mode(HANDLE_T h_dev,
                                     UINT32*  pui4_operation_mode)
{
    INT32                       i4_return;
    EVT_FILTR_T*                pt_evt_filtr;
#ifndef IOM_DISABLE_UART_FACTORY_MODE
    UINT16                      ui2_uart_port;
#endif

    /* Lock DB for write operation */
    iom_lock_db_wr();

    /* Get the object reference */
    i4_return = handle_get_obj(h_dev, (VOID*)&pt_evt_filtr);

    if (i4_return != HR_OK)
    {
        iom_unlock_db();
        return IOMR_INV_HANDLE;
    }

    if (IOM_EXCEED_MAX(pt_evt_filtr->ui1_dev_idx, IOM_MAX_DEV))
    {
        iom_unlock_db();
        return IOMR_ERROR;        
    }
            
    if (at_io_dev[pt_evt_filtr->ui1_dev_idx].e_dev_type != IOM_DEV_TYPE_IO_UART)
    {
        iom_unlock_db();
        DBG_ERROR(("{IOM} ERR: device type is not IOM_DEV_TYPE_IO_UART\n\r"));
        return IOMR_INV_HANDLE;
    }

#ifndef IOM_DISABLE_UART_FACTORY_MODE
    /* Set UART operation mode */
    ui2_uart_port = UART_GET_PORT(at_io_dev[pt_evt_filtr->ui1_dev_idx].ui2_id);

    switch (*pui4_operation_mode) {
        case (IOM_UART_OPERATION_MODE_FACTORY | IOM_UART_OPERATION_MODE_TRANSPARENT):
            if (ab_factory_mode[ui2_uart_port] == FALSE)
            {                
                i4_return = _set_non_canonical_input(ui2_uart_port, TRUE);
                if (i4_return != IOMR_OK)
                {
                    iom_unlock_db();
                       printf("{IOM}:%d ERR: enter factory mode set non-canonical input failed %d\n", __LINE__, (int)i4_return);
                    return IOMR_DEV_SET_FAILED;
                }
                ab_factory_mode[ui2_uart_port] = TRUE;
            }
            break;
        case (IOM_UART_OPERATION_MODE_CLI | IOM_UART_OPERATION_MODE_TRANSPARENT):
            if (ab_factory_mode[ui2_uart_port] == TRUE)
            {
                i4_return = _set_non_canonical_input(ui2_uart_port, FALSE);
                if (i4_return != IOMR_OK)
                {
                     iom_unlock_db();
                     printf("{IOM}:%d ERR: exit factory mode set non-canonical input failed %d\n", __LINE__, (int)i4_return);
                     return IOMR_DEV_SET_FAILED;
                }                
                ab_factory_mode[ui2_uart_port] = FALSE;
            }
            break;
        case (IOM_UART_OPERATION_MODE_TRANSPARENT):
            /* nothing needs to be done as we only support transparent mode
             * there is no normal mode in Linux currently */
            break;
        default:
            iom_unlock_db();
            printf("{IOM}:%d ERR: operation mode 0x%x not support\n", __LINE__, (unsigned)*pui4_operation_mode);
            return IOMR_NOT_SUPPORT;
    }
#endif

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_auto_sleep_timer_cb
 *
 * Description: This callback function is called when auto sleep timer is 
 *              expired.
 *
 * Inputs:  h_handle        A handle referencing the timer.
 *          pv_tag          Contains the handle private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_op_auto_sleep_timer_cb(HANDLE_T h_timer,
                                VOID*    pv_tag)
{
    if ((!b_prompt_timer_running && (h_timer == h_prompt_timer)) ||
        (!b_sleep_timer_running && (h_timer == h_sleep_timer)))
    {
        return;
    }

    if (h_timer == h_prompt_timer)
    {    
        b_prompt_timer_running = FALSE;

        /* Notify the client of expiration event */
        if (t_auto_sleep_timer.pf_nfy)
        {
            t_auto_sleep_timer.pf_nfy(IOM_NFY_COND_AUTO_SLEEP_PROMPT, t_auto_sleep_timer.pv_nfy_tag);
        }
    }
    else
    {
        b_sleep_timer_running = FALSE;

        /* Notify the client of expiration event */
        if (t_auto_sleep_timer.pf_nfy)
        {
            t_auto_sleep_timer.pf_nfy(IOM_NFY_COND_AUTO_SLEEP_TIMEOUT, t_auto_sleep_timer.pv_nfy_tag);
        }
        else
        {
            /* Generate a pseudo POWER event */
            x_iom_send_evt(BTN_POWER, 120);                 
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_set_auto_sleep_timer
 *
 * Description: This API configures the auto sleep timer which will set the
 *              system into standby mode when none of IRRC key is received  
 *              during specific period of time. The timer will be restarted if 
 *              any IRRC key is received before expiration.
 *
 * Inputs:  pt_auto_sleep_timer     References the auto sleep timer data
 *                                  structure.
 *
 * Outputs: -
 *
 * Returns: IOMR_OK                     Routine successful.
 *          IOMR_NOT_INIT               The IO Manager is not initialized.
 *          IOMR_INV_ARG                One or more of the arguments contain
 *                                      invalid data.
 *          IOMR_TIMER_ERROR            A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 iom_op_set_auto_sleep_timer(AUTO_SLEEP_TIMER_T* pt_auto_sleep_timer)
{
    INT32       i4_return;
    BOOL        b_timer_reset = FALSE;

    DBG_API(("{IOM} x_iom_set_auto_sleep_timer\n\r"));

    /* Check arguments */
    if (!iom_is_init())
    {
        return IOMR_NOT_INIT;
    }

    if ((pt_auto_sleep_timer == NULL) || 
        ((pt_auto_sleep_timer->ui4_prompt_timeout > 0) &&
         (pt_auto_sleep_timer->ui4_sleep_timeout > 0) &&
         (pt_auto_sleep_timer->ui4_sleep_timeout <= pt_auto_sleep_timer->ui4_prompt_timeout)) ||
        ((pt_auto_sleep_timer->ui4_prompt_timeout > 0) &&
         (pt_auto_sleep_timer->ui4_sleep_timeout == 0)))
    {
        return IOMR_INV_ARG;
    }

    /* Lock DB for write operation */
    iom_lock_db_wr();
        
    /* Stop prompt timer */
    if (b_prompt_timer_running || 
        (t_auto_sleep_timer.ui4_prompt_timeout == 0))
    {    
        i4_return = x_timer_stop(h_prompt_timer);

        if (i4_return != OSR_OK)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: x_timer_stop() failed\n\r"));
            return IOMR_TIMER_ERROR;
        }

        b_prompt_timer_running = FALSE;
        b_timer_reset = TRUE;
    }        

    /* Stop sleep timer */
    if (b_sleep_timer_running || 
        (t_auto_sleep_timer.ui4_sleep_timeout == 0))
    {    
        i4_return = x_timer_stop(h_sleep_timer);

        if (i4_return != OSR_OK)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: x_timer_stop() failed\n\r"));
            return IOMR_TIMER_ERROR;
        }

        b_sleep_timer_running = FALSE;
        b_timer_reset = TRUE;
    }        

    /* Notify the client of auto aleep timer reset action */
    if (b_timer_reset && t_auto_sleep_timer.pf_nfy)
    {
        t_auto_sleep_timer.pf_nfy(IOM_NFY_COND_AUTO_SLEEP_TIMER_RESET, t_auto_sleep_timer.pv_nfy_tag);
    }        

    t_auto_sleep_timer = *pt_auto_sleep_timer;

    /* Start prompt timer */
    if (t_auto_sleep_timer.ui4_prompt_timeout > 0)
    {
        i4_return = x_timer_start(h_prompt_timer,
                                  (UINT32)(t_auto_sleep_timer.ui4_prompt_timeout*1000),
                                  X_TIMER_FLAG_ONCE,
                                  iom_op_auto_sleep_timer_cb,
                                  NULL);

        if (i4_return != OSR_OK)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: x_timer_start() failed\n\r"));
            return IOMR_TIMER_ERROR;
        }

        b_prompt_timer_running = TRUE;
    }

    /* Start sleep timer */
    if (t_auto_sleep_timer.ui4_sleep_timeout > 0)
    {
        i4_return = x_timer_start(h_sleep_timer,
                                  (UINT32)(t_auto_sleep_timer.ui4_sleep_timeout*1000),
                                  X_TIMER_FLAG_ONCE,
                                  iom_op_auto_sleep_timer_cb,
                                  NULL);

        if (i4_return != OSR_OK)
        {
            iom_unlock_db();
            DBG_ERROR(("{IOM} ERR: x_timer_start() failed\n\r"));
            return IOMR_TIMER_ERROR;
        }

        b_sleep_timer_running = TRUE;
    }

    /* Unlock DB */
    iom_unlock_db();

    return IOMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: iom_op_get_in_msgq_hdl
 *
 * Description: This API gets the input event message queue handle.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: Input event message queue handle    Successful.
 *          NULL                                Failed.
 ----------------------------------------------------------------------------*/
HANDLE_T* iom_op_get_in_msgq_hdl(VOID)
{
    return &h_iom_inp_msgq;
}


