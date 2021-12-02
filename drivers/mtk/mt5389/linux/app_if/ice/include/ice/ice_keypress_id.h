/**
 * Copyright 2002-5 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief   ICE remote control interface
 *
 * This file defines the ICE keypress enumeration.
 */

#ifndef CABOT_ICE_ICE_KEYPRESS_ID_H_INCLUDED
#define CABOT_ICE_ICE_KEYPRESS_ID_H_INCLUDED

#include "config/include_skype.h"
#ifdef __cplusplus
extern "C" {
#endif


/*defines for ascii extended characters*/
#define ASCII_EXTENDED_ISO_8859_1

/**
 * The Eclipse system is interested in the keys listed in the
 * #ICE_KeypressId enumeration.
 *
 * @note Implementations that include MHEG-5 functionality MUST be able
 *       to report keys within registers 3, 4, and 5.  The remaining keys
 *       are never consumed by MHEG applications and are optional.
 * @internal Any changes to this header should be carried over to the key_map table in Testtool.cpp
 */
typedef enum ICE_keypressId
{
    /*www.ascii-code.com*/
    /*ASCII keyboard     */
    ICE_keyboard_bs=8,//8
    ICE_keyboard_tab,//9
    ICE_keyboard_lf,//10
    ICE_keyboard_shift, //16
    ICE_keyboard_esc=27,//27
    ICE_keyboard_space=32,//32
    ICE_keyboard_exlamation_mark,//33
    ICE_keyboard_double_quotation_first,//34
    ICE_keyboard_hash,//35
    ICE_keyboard_dollar,//36
    ICE_keyboard_percent,//37
    ICE_keyboard_ampersand,//38
    ICE_keyboard_single_quotation_first,//39
    ICE_keyboard_left_round_bracket,//40
    ICE_keyboard_right_round_bracket,//41
    ICE_keyboard_asterisk,//42
    ICE_keyboard_plus,//43
    ICE_keyboard_comma,//44
    ICE_keyboard_hyphen,//45
    ICE_keyboard_period,//46
    ICE_keyboard_slash,//47
    ICE_keyboard_zero,//48
    ICE_keyboard_one,//49
    ICE_keyboard_two,//50
    ICE_keyboard_three,//51
    ICE_keyboard_four,//52
    ICE_keyboard_five,//53
    ICE_keyboard_six,//54
    ICE_keyboard_seven,//55
    ICE_keyboard_eight,//56
    ICE_keyboard_nine,//57
    ICE_keyboard_colon,//58
    ICE_keyboard_semi_colon,//59
    ICE_keyboard_less,//60
    ICE_keyboard_equal,//61
    ICE_keyboard_greater,//62
    ICE_keyboard_question_mark,//63
    ICE_keyboard_at_sign,//64
    ICE_keyboard_uppercase_a,//65
    ICE_keyboard_uppercase_b,//66
    ICE_keyboard_uppercase_c,//67
    ICE_keyboard_uppercase_d,//68
    ICE_keyboard_uppercase_e,//69
    ICE_keyboard_uppercase_f,//70
    ICE_keyboard_uppercase_g,//71
    ICE_keyboard_uppercase_h,//72
    ICE_keyboard_uppercase_i,//73
    ICE_keyboard_uppercase_j,//74
    ICE_keyboard_uppercase_k,//75
    ICE_keyboard_uppercase_l,//76
    ICE_keyboard_uppercase_m,//77
    ICE_keyboard_uppercase_n,//78
    ICE_keyboard_uppercase_o,//79
    ICE_keyboard_uppercase_p,//80
    ICE_keyboard_uppercase_q,//81
    ICE_keyboard_uppercase_r,//82
    ICE_keyboard_uppercase_s,//83
    ICE_keyboard_uppercase_t,//84
    ICE_keyboard_uppercase_u,//85
    ICE_keyboard_uppercase_v,//86
    ICE_keyboard_uppercase_w,//87
    ICE_keyboard_uppercase_x,//88
    ICE_keyboard_uppercase_y,//89
    ICE_keyboard_uppercase_z,//90
    ICE_keyboard_left_square_bracket,//91
    ICE_keyboard_backslash,//92
    ICE_keyboard_right_square_bracket,//93
    ICE_keyboard_caret,//^94
    ICE_keyboard_underscore,//95
    ICE_keyboard_single_quotation_second,//96
    ICE_keyboard_a,//97
    ICE_keyboard_b,//98
    ICE_keyboard_c,//99
    ICE_keyboard_d,//100
    ICE_keyboard_e,//101
    ICE_keyboard_f,//102
    ICE_keyboard_g,//103
    ICE_keyboard_h,//104
    ICE_keyboard_i,//105
    ICE_keyboard_j,//106
    ICE_keyboard_k,//107
    ICE_keyboard_l,//108
    ICE_keyboard_m,//109
    ICE_keyboard_n,//110
    ICE_keyboard_o,//111
    ICE_keyboard_p,//112
    ICE_keyboard_q,//113
    ICE_keyboard_r,//114
    ICE_keyboard_s,//115
    ICE_keyboard_t,//116
    ICE_keyboard_u,//117
    ICE_keyboard_v,//118
    ICE_keyboard_w,//119
    ICE_keyboard_x,//120
    ICE_keyboard_y,//121
    ICE_keyboard_z,//122
    ICE_keyboard_left_curly_bracket,//123
    ICE_keyboard_or,// |124
    ICE_keyboard_right_curly_bracket,//125
    ICE_keyboard_tilde,//126
    ICE_keyboard_del,//127
#ifdef ASCII_EXTENDED_ISO_8859_1
     ICE_keyboard_euro,
     ICE_keyboard_single_low_9_quotation_mark=130,
     ICE_keyboard_small_letterf_with_hook,
     ICE_keyboard_double_low_quotation_mark,
     ICE_keyboard_dagger,
     ICE_keyboard_double_dagger,
     ICE_keyboard_circumflex_accent,
     ICE_keyboard_per_mile_sign,
     ICE_keyboard_uppercase_s_with_caron,
     ICE_keyboard_single_left_point_angle_quotation,
     ICE_keyboard_latin_capital_ligature_OE,
     ICE_keyboard_latin_capital_z_with_caron=142,
     ICE_keyboard_left_single_quotation_mark=145,
     ICE_keyboard_uppercase_c_with_cedil = 199,
     ICE_keyboard_c_with_cedil, //200
     ICE_keyboard_uppercase_g_with_breve, //201,
     ICE_keyboard_g_with_breve, //202
     ICE_keyboard_uppercase_i_with_dotted,//203
     ICE_keyboard_i_with_dotless, //204
     ICE_keyboard_uppercase_s_with_cedil,//205
     ICE_keyboard_s_with_cedil, //206
     ICE_keyboard_uppercase_o_with_umlaut, //207
     ICE_keyboard_o_with_umlaut, //208
     ICE_keyboard_uppercase_u_with_umlaut, //209
     ICE_keyboard_u_with_umlaut, //210
        /*TODO fill extended ascii*/
     ICE_keyboard_latin_small_letter_y_with_diaeresis=255,
#endif
    /* Register 4 key group (numbers) */
    /* These MAY be consumed by a running MHEG-5 application */
    ICE_keypress_zero,
    ICE_keypress_one,
    ICE_keypress_two,
    ICE_keypress_three,
    ICE_keypress_four,
    ICE_keypress_five,
    ICE_keypress_six,
    ICE_keypress_seven,
    ICE_keypress_eight,
    ICE_keypress_nine,

    /* Register 5 key group (pointer) */
    /* These MAY be consumed by a running MHEG-5 application */
    ICE_keypress_up,
    ICE_keypress_down,
    ICE_keypress_left,
    ICE_keypress_right,
    ICE_keypress_select,            /* or OK */

    /* Register 3 key group (application) */
    /* These are always consumed by a running MHEG-5 application */
    ICE_keypress_text,
    ICE_keypress_exit,              /* or cancel */
    ICE_keypress_red,
    ICE_keypress_green,
    ICE_keypress_yellow,
    ICE_keypress_blue,

    /* Teletext mixing keys */
    ICE_keypress_text_pat_mix,
    ICE_keypress_mix,

    /* All remaining keys are optional */
    ICE_keypress_power,             /* or standby */

    ICE_keypress_volume_up,
    ICE_keypress_volume_down,
    ICE_keypress_mute,

    ICE_keypress_program_up,        /* or ch+ */
    ICE_keypress_program_down,      /* or ch- */

    ICE_keypress_tv,                /* or DTV/TV, or AV */
    ICE_keypress_video,
    ICE_keypress_audio,
    ICE_keypress_aux,
    ICE_keypress_radio,
    ICE_keypress_services,          /* or list */
    ICE_keypress_favourites,
    ICE_keypress_info,
    ICE_keypress_menu,
    ICE_keypress_subtitle,
    ICE_keypress_wide,
    ICE_keypress_epg,               /* or guide */
    ICE_keypress_previous,          /* or P<P */
    ICE_keypress_page_up,
    ICE_keypress_page_down,
    ICE_keypress_preset,
    ICE_keypress_pip,               /* picture-in-picture */
    ICE_keypress_pipprogramup,
    ICE_keypress_pipprogramdown,
    ICE_keypress_pap,               /* picture-and-picture */
    ICE_keypress_pip_pap,           /* picture-in-picture/picture-and-picture keys together*/
    ICE_keypress_pip_swap,
    ICE_keypress_pap_swap,
    ICE_keypress_swap,              /* if pip active do pip swap else if pap active do pap swap */
    ICE_keypress_toggle_active_window,
    ICE_keypress_sleep,
    ICE_keypress_lang,

    ICE_keypress_mhp_app,           /* Italian/Nordig MHP "App" key */
    ICE_keypress_mhp_tv,            /* Italian/Nordig MHP "TV" key */
    ICE_keypress_mhp_back,          /* Italian/Nordig MHP "Back" key */

    /* DVR group (may be used by other apps if required) */
    ICE_keypress_record,
    /* Register 6 key group (non-linear playback) */
    /* These MAY be consumed by a running MHEG-5 application but are optional */
    ICE_keypress_rewind,
    ICE_keypress_play,
    ICE_keypress_fforward,
    ICE_keypress_pause,
    ICE_keypress_stop,
    ICE_keypress_timers,
    ICE_keypress_playlist,

    /* Teletext keys */
    ICE_keypress_teletext_mix,
    ICE_keypress_teletext_hold,
    ICE_keypress_teletext_reveal,
    ICE_keypress_teletext_index,
    ICE_keypress_teletext_subpage,
    ICE_keypress_teletext_update,

    /* MHEG EPG key - required by the New Zealand MHEG Profile */
    ICE_keypress_mheg_epg,

    /* PC key - required by the TV */
    ICE_keypress_pc,

    /* Dual key - required by the TV */
    ICE_keypress_dual,

    ICE_keypress_media_browser,
    ICE_keypress_internet_settings,

    ICE_keypress_quick_menu,
    ICE_keypress_multimedia,
    ICE_keypress_options,
    ICE_keypress_devices,
    ICE_keypress_channels,
    ICE_keypress_prev,
    ICE_keypress_next,
    ICE_keypress_help,

    ICE_keypress_effect,
    ICE_keypress_tvdvd,

    /* YAHOO! key */
    ICE_keypress_yahoo,

    ICE_keypress_exit_2,
    ICE_keypress_mybutton,
    ICE_keypress_long_mybutton,
    ICE_keypress_mybutton2,
    ICE_keypress_long_mybutton2,

    ICE_keypress_skip_backward,
    ICE_keypress_skip_forward,
    ICE_keypress_playpause,

    ICE_keypress_previous_track, /* |<< */
    ICE_keypress_next_track,    /* >>| */

    ICE_keypress_freeze,
#ifdef INCLUDE_SKYPE        
    ICE_keypress_skype,
#endif
    ICE_keypress_equalizer_mode,
    /* !!!NOTICE : End of remote control keys, any new remote key should be defined above this */
    /* Any remote key defined above must have a match at same order at key_names char array*/
    ICE_keypress_end_of_remote_keys,

    /* Menu short cut keys */
    ICE_keypress_auto_install_menu,
    ICE_keypress_install_menu,
    ICE_keypress_settings_menu,
    ICE_keypress_service_menu,
    ICE_keypress_channel_list,
    ICE_keypress_sources_menu,
    ICE_keypress_option,
    ICE_keypress_tools,
    ICE_keypress_link,
    ICE_keypress_browser,
    ICE_keypress_surround,

    /* implementation-specific keys - may be used for other purposes */
    ICE_keypress_user_0,
    ICE_keypress_user_1,
    ICE_keypress_user_2,
    ICE_keypress_user_3,
    ICE_keypress_user_4,
    ICE_keypress_user_5,
    ICE_keypress_user_6,
    ICE_keypress_user_7,
    ICE_keypress_user_8,
    ICE_keypress_user_9,
    ICE_keypress_user_a,
    ICE_keypress_user_b,
    ICE_keypress_user_c,
    ICE_keypress_user_d,
    ICE_keypress_user_e,
    ICE_keypress_user_f,
    ICE_keypress_user_10,
    ICE_keypress_user_11,
    ICE_keypress_user_12,
    ICE_keypress_user_13,
    ICE_keypress_user_14,
    ICE_keypress_user_15,
    ICE_keypress_user_16,
    ICE_keypress_user_17,
    ICE_keypress_user_18,
    ICE_keypress_user_19,
    ICE_keypress_user_1a,
    ICE_keypress_user_1b,
    ICE_keypress_user_1c,
    ICE_keypress_user_1d,
    ICE_keypress_user_1e,
    ICE_keypress_user_1f,

    /* implementation-specific keys - may be mapped to front panel keys */
    ICE_keypress_front_one,
    ICE_keypress_front_two,
    ICE_keypress_front_three,
    ICE_keypress_front_four,
    ICE_keypress_front_down,
    ICE_keypress_front_up,
    ICE_keypress_front_left,
    ICE_keypress_front_right,
    ICE_keypress_front_select,
    ICE_keypress_front_menu,
    ICE_keypress_front_standby,
    ICE_keypress_mmi_close,

    ICE_keypress_long_offset,
    ICE_keypress_long_zero = ICE_keypress_long_offset,
    ICE_keypress_long_one,
    ICE_keypress_long_two,
    ICE_keypress_long_three,
    ICE_keypress_long_four,
    ICE_keypress_long_five,
    ICE_keypress_long_six,
    ICE_keypress_long_seven,
    ICE_keypress_long_eight,
    ICE_keypress_long_nine,
    ICE_keypress_long_fastforward,
    ICE_keypress_long_rewind,
    ICE_keypress_go_first_item,
    ICE_keypress_go_first_item_group,
    ICE_keypress_translation_fit_check,

    ICE_keypress_front_down_sense,
    ICE_keypress_front_up_sense,
    ICE_keypress_front_left_sense,
    ICE_keypress_front_right_sense,
    ICE_keypress_front_select_sense,
    ICE_keypress_front_menu_sense,
    ICE_keypress_front_standby_sense,
    ICE_keypress_front_no_key_sense,
    ICE_keypress_cancel,

    ICE_keypress_invalid,

    ICE_keypress_release_offset = 1000

} ICE_KeypressId;


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_KEYPRESS_ID_H_INCLUDED*/
