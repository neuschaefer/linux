/**
 * Copyright 2010 Vestel/RT-RK.  All rights reserved.
 *
 * @file
 * @brief  ICE Interface for 2M ISB communication
 *
 * This file defines the ICE ISB structures used by the ice_isb interface
 */

#ifndef CABOT_ICE_ICE_ISB_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_ISB_TYPES_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_device_number.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Enumeration for different types of messages that we can receive from ISB device
 */
typedef enum ICEISB_message_type_receive_t
{
    isb_msg_recv_isb_identify = 0x80,
    isb_msg_recv_status_from_isb = 0x81,
    isb_msg_recv_power = 0x82,
    isb_msg_recv_tv_identify = 0x83,
    isb_msg_recv_add_program_to_tv = 0x84,
    isb_msg_recv_set_power_up_parameters = 0x85,
    isb_msg_recv_set_clock_data = 0x86,
    isb_msg_recv_use_panel_led = 0x87,
    isb_msg_recv_use_clock_display = 0x88,
    isb_msg_recv_osd_control = 0x89,
    isb_msg_recv_display_osd_text = 0x8a,
    isb_msg_recv_set_analog_level = 0x8b,
    isb_msg_recv_ir_code_to_tv = 0x8c,
    isb_msg_recv_select_av_mode = 0x8d,
    isb_msg_recv_tune_frequency = 0x8e,
    isb_msg_recv_select_teletext = 0x8f,
    isb_msg_recv_change_program = 0x90,
    isb_msg_recv_enable_factory_remote = 0x91,
    isb_msg_recv_picture_size = 0x92,
    isb_msg_recv_picture_mode = 0x93,
    isb_msg_recv_auto_adjustment = 0x94,
    isb_msg_recv_picture_enhancement = 0x95,
    isb_msg_recv_set_number_of_programs = 0x96,
    isb_msg_recv_card_reader = 0x97,
    isb_msg_recv_dvb_command = 0x98,
    isb_msg_recv_rfid_card_reader = 0x99,
    isb_msg_recv_vendor_command = 0x9a,
    isb_msg_recv_mediahub_cmd = 0x9b,
    isb_msg_recv_set_power_on_av_input = 0x9c,
    isb_msg_recv_capability = 0x9d
} ICEISB_message_type_receive_t;

typedef enum ICEISB_change_status_field //command 0x84
{
    isb_change_status_is_tv_on = 0,
    isb_change_status_is_teletext_on,
    isb_change_status_is_valid_signal,
    isb_change_status_is_isb_osb_on_screen,
    isb_change_status_is_last_cmd_valid,
    isb_change_status_is_aux_active,
    isb_change_status_is_channel_digital,
    isb_change_status_is_usb_menu_on,
    isb_change_status_is_dvr_playback_on,
    isb_change_status_is_is_dvr_recording_on,
    isb_change_status_current_program,
    isb_change_status_current_aux_input_type,
    isb_change_status_current_aux_input_number, 
} ICEISB_change_status_field;

/**
 * @brief Enumeration for different types of system ID used in command 0x84 ADD PROGRAM TO TV
 */
typedef enum ICEISB_system_id //command 0x84
{
    isb_bg_pal = 0,
    isb_l_secam,
    isb_i_pal,
    isb_m_ntsc,
    isb_n_pal,
    isb_auto_recognition,
    isb_dk_pal,
    isb_dk_secam,
    isb_bg_secam
} ICEISB_system_id;

/**
 * @brief Enumeration for different program types used in command 0x84 ADD PROGRAM TO TV 
 */
typedef enum ICEISB_program_type //command 0x84
{
    isb_normal_open_programs = 0,
    isb_protected_programs,
    isb_info,
    isb_radio
} ICEISB_program_type;

/**
 * @brief Enumeration for different default power up parameters 
 * used in command 0x85 SET POWER UP PARAMETERS
 */
typedef enum ICEISB_power_up_parameters
{
    isb_power_up_volume = 0,
    isb_power_up_max_volume,
    isb_power_up_brightness,
    isb_power_up_contrast,
    isb_power_up_colour,
    isb_power_up_tint,
    isb_power_up_sharpness
} ICEISB_power_up_parameters;

/**
 * @brief Enumeration for different types of led status used in command 0x87 USE PANEL LED
 */
typedef enum ICEISB_led_status //command 0x87
{
    isb_tv_normal_control = 0,
    isb_slow_blinking = 1,
    isb_fast_blinking = 2,
    isb_led_off = 4,
    isb_led_on = 5
} ICEISB_led_status;

/**
 * @brief Enumeration for different led brightness used in command 0x87 USE PANEL LED
 */
typedef enum ICEISB_led_brightness //command 0x87
{
    isb_weak = 0,
    isb_low_red = 1,
    isb_normal_green = 2,
    isb_bright = 3
} ICEISB_led_brightness;

/**
 * @brief Enumeration for different types of audio and picture settings used in command 0x8B SET ANALOG LEVEL
 */
typedef enum ICEISB_current_parameters //command 0x85
{
    isb_current_volume = 0,
    isb_current_brightness,
    isb_current_contrast,
    isb_current_colour,
    isb_current_tint,
} ICEISB_current_parameters;

/**
 * @brief Enumeration for different ir commands sent from ISB device and used in command 0x8C IR CODE TO TV
 */
typedef enum ICEISB_ir_commands //command 0x8C
{
    isb_digit_0 = 0x00,
    isb_digit_1 = 0x01,
    isb_digit_2 = 0x02,
    isb_digit_3 = 0x03,
    isb_digit_4 = 0x04,
    isb_digit_5 = 0x05,
    isb_digit_6 = 0x06,
    isb_digit_7 = 0x07,
    isb_digit_8 = 0x08,
    isb_digit_9 = 0x09,
    isb_picture_mode_step_next = 0x0a,
    isb_mute = 0x0d,
    isb_ok_key = 0x0e,
    isb_volume_up = 0x10,
    isb_volume_down = 0x11,
    isb_brightness_up = 0x12,
    isb_brightness_down = 0x13,
    isb_color_up = 0x14,
    isb_color_down = 0x15,
    isb_contrast_up = 0x1c,
    isb_contrast_down = 0x1d,
    isb_tint_down = 0x1f,
    isb_cursor_up = 0x20,
    isb_cursor_down = 0x21,
    isb_tint_up = 0x27,
    isb_show_clock = 0x2f,
    isb_pause = 0x30,
    isb_rewind = 0x32,
    isb_fast_forward = 0x34,
    isb_play = 0x35,
    isb_stop = 0x36,
    isb_record = 0x37,
    isb_cursor_left = 0x55,
    isb_cursor_right = 0x56,
    isb_red_key = 0x6b,
    isb_green_key = 0x6c,
    isb_yellow_key = 0x6d,
    isb_blue_key = 0x6e,
    isb_picture_format = 0x7e
} ICEISB_ir_commands;

/**
  * @brief Enumeration for the different AV modes used in command 0x8D SELECT AV MODE
 */
typedef enum ICEISB_av_mode //command 0x8D
{
    isb_av_scart = 0,
    isb_av_front = 1,
    isb_av_rca = 2,
    isb_av_svhs = 3,
    isb_av_vga = 4,
    isb_av_ypbpr = 5,
    isb_av_hdmi = 6,
    isb_av_usb_digital_source = 15,
    isb_av_selected_with_command_0x9c = 16
} ICEISB_av_mode;

/**
  * @brief Enumeration for the different picture sizes used in command 0x92 PICTURE SIZE
 */
typedef enum ICEISB_picture_size //command 0x92
{
    isb_pic_size_16_9 = 0,
    isb_pic_size_panorama,
    isb_pic_size_zoom,
    isb_pic_size_4_3,
    isb_pic_size_unknown
} ICEISB_picture_size;

/**
  * @brief Enumeration for the different picture modes used in command 0x93 PICTURE MODE
 */
typedef enum ICEISB_picture_mode //command 0x93
{
    isb_pic_mode_shop = 0,
    isb_pic_mode_eco,
    isb_pic_mode_premium,
    isb_pic_mode_personal
} ICEISB_picture_mode;

/////////////////////////////////////////
//////// DVB  SUBCOMMAND ENUMERATIONS
/////////////////////////////////////////

/**
  * @brief Enumeration for different types of DVB subcommands that we can receive from ISB device
 */
typedef enum ICEISB_dvb_subcommand_t
{
    isb_msg_dvb_collect_ts_info = 0x00,
    isb_msg_dvb_get_prg_name_and_srvc_id = 0x01,
    isb_msg_dvb_get_video_pid = 0x02,
    isb_msg_dvb_get_audio_pid = 0x03,
    isb_msg_dvb_get_ttx_pid = 0x04,
    isb_msg_dvb_get_subtitle_pid = 0x05,
    isb_msg_dvb_store_program_at_position = 0x06,
    isb_msg_dvb_mark_program_for_edit = 0x07,
    isb_msg_dvb_set_new_video_pid_in_marked_program = 0x08,
    isb_msg_dvb_set_new_audio_pid_in_marked_program = 0x09,
    isb_msg_dvb_set_new_ttx_pid_in_marked_program = 0x0A,
    isb_msg_dvb_set_new_sub_pid_in_marked_program = 0x0B,
    isb_msg_dvb_store_label_in_program_memory = 0x0C,
    isb_msg_dvb_tune_to_program_in_found_programs = 0x0D,
    isb_msg_dvb_tune_to_stored_program = 0x0E,
    isb_msg_dvb_send_diseqc_data = 0x0F,
    isb_msg_dvb_set_pincode_in_unit = 0x10,
    isb_msg_dvb_set_room_number_in_unit = 0x11,
    isb_msg_dvb_set_multicast_number_in_unit = 0x12,
    isb_msg_dvb_set_private_pid_code_in_unit = 0x13,
    isb_msg_dvb_turn_subtitle_on_or_off = 0x14,
    isb_msg_dvb_set_new_pcr_in_marked_program = 0x15,
    isb_msg_dvb_set_new_pmt_in_marked_program = 0x16,
    isb_msg_dvb_set_language = 0x17,
    isb_msg_dvb_store_ts_parameters = 0x18,
    isb_msg_dvb_store_program_using_ts_parameters = 0x19,
    isb_msg_dvb_ask_firmware_revision = 0x1A,
    isb_msg_dvb_download_new_firmware = 0x1B,
    isb_msg_dvb_request_current_languages = 0x1C,
    isb_msg_dvb_request_channel_search = 0x1D
} ICEISB_dvb_subcommand_t;

/**
  * @brief Enumeration for different status responses from DVB decoder
 */
typedef enum ICEISB_dvb_subcommand_status
{
    isb_dvb_status_command_accepted_ok = 0x01,
    isb_dvb_status_checksum_error,
    isb_dvb_status_subcmd_error,
    isb_dvb_status_data_error,
    isb_dvb_status_execution_error,
    isb_dvb_status_new_status_pending,
    isb_dvb_status_0x00_or_0x18_not_issued,
    isb_dvb_status_dvb_system_not_supported,
    isb_dvb_status_no_tsdata_found
} ICEISB_dvb_subcommand_status;

typedef enum ICEISB_dvb_subcommand_subtitling_modes
{
    isb_dvb_subtitling_modes_page_number = 0x50,
    isb_dvb_subtitling_modes_TS_packet = 0x53
} ICEISB_dvb_subcommand_subtitling_modes;


/**
  * @brief Enumeration for different of DVB systems used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_system
{
    isb_dvb_system_dvb_c = 0x00,
    isb_dvb_system_dvb_t = 0x01,
    isb_dvb_system_dvb_s = 0x02
} ICEISB_dvb_subcommand_system;

/**
  * @brief Enumeration for different of guard intervals used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_guard
{
    isb_dvb_subcommand_guard_1_4 = 0,
    isb_dvb_subcommand_guard_1_8,
    isb_dvb_subcommand_guard_1_16,
    isb_dvb_subcommand_guard_1_32
} ICEISB_dvb_subcommand_guard;

/**
  * @brief Enumeration for different types of DVB modulations used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_modulation
{
    isb_dvb_subcommand_modulation_qpsk = 0,
    isb_dvb_subcommand_modulation_16qam,
    isb_dvb_subcommand_modulation_32qam,
    isb_dvb_subcommand_modulation_64qam,
    isb_dvb_subcommand_modulation_128qam,
    isb_dvb_subcommand_modulation_256qam,
    isb_dvb_subcommand_modulation_8psk,
    isb_dvb_subcommand_modulation_16apsk,
    isb_dvb_subcommand_modulation_32apsk,
    isb_dvb_subcommand_modulation_8vsb,
    isb_dvb_subcommand_modulation_16vsb,
    isb_dvb_subcommand_modulation_1024qam,
    isb_dvb_subcommand_modulation_4096vsb,
} ICEISB_dvb_subcommand_modulation;

/**
  * @brief Enumeration for different values of DVB-T bandwidth used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_t_bandwidth
{
    isb_dvb_subcommand_dvb_t_bandwidth_5_mhz = 0,
    isb_dvb_subcommand_dvb_t_bandwidth_6_mhz,
    isb_dvb_subcommand_dvb_t_bandwidth_7_mhz,
    isb_dvb_subcommand_dvb_t_bandwidth_8_mhz 
} ICEISB_dvb_subcommand_dvb_t_bandwidth;

/**
  * @brief Enumeration for different values of Forward Error Control used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_coderate_of_fec
{
    isb_dvb_subcommand_coderate_of_fec_1_4 = 0,
    isb_dvb_subcommand_coderate_of_fec_1_3,
    isb_dvb_subcommand_coderate_of_fec_2_5,
    isb_dvb_subcommand_coderate_of_fec_1_2,
    isb_dvb_subcommand_coderate_of_fec_3_5,
    isb_dvb_subcommand_coderate_of_fec_2_3,
    isb_dvb_subcommand_coderate_of_fec_3_4,
    isb_dvb_subcommand_coderate_of_fec_4_5,
    isb_dvb_subcommand_coderate_of_fec_5_6,
    isb_dvb_subcommand_coderate_of_fec_8_9,
    isb_dvb_subcommand_coderate_of_fec_9_10
} ICEISB_dvb_subcommand_coderate_of_fec;

/**
  * @brief Enumeration for different carrier types used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_carrier_type
{
    isb_dvb_subcommand_carrier_type_2k = 0,
    isb_dvb_subcommand_carrier_type_4k,
    isb_dvb_subcommand_carrier_type_8k,
} ICEISB_dvb_subcommand_carrier_type;

/**
  * @brief Enumeration for different DVB-T standards used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_t_standard
{
    isb_dvb_subcommand_dvb_t_standard_dvb_t = 0x00,
    isb_dvb_subcommand_dvb_t_standard_dvb_t2 = 0x01
} ICEISB_dvb_subcommand_dvb_t_standard;

/**
  * @brief Enumeration for different values of DVB-T2 guard interval used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_t2_guard
{
    isb_dvb_subcommand_dvb_t2_guard_19_256 = 0x00,
    isb_dvb_subcommand_dvb_t2_guard_19_128 = 0x01,
    isb_dvb_subcommand_dvb_t2_guard_1_128 = 0x03,
} ICEISB_dvb_subcommand_extended_guard;

/**
  * @brief Enumeration for different values of DVB-T2 FFT sizes used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_t2_fft_size
{
    isb_dvb_subcommand_dvb_t2_fft_size_1k = 0,
    isB_dvb_subcommand_dvb_t2_fft_size_2k,
    isb_dvb_subcommand_dvb_t2_fft_size_4k,
    isb_dvb_subcommand_dvb_t2_fft_size_8k,
    isb_dvb_subcommand_dvb_t2_fft_size_16k,
    isb_dvb_subcommand_dvb_t2_fft_size_32k
} ICEISB_dvb_subcommand_dvb_t2_fft_size;

/**
  * @brief Enumeration for different of values of DVB-T2 bandwidth used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_t2_bandwidth
{
    isb_dvb_subcommand_dvb_t2_bandwidth_1_7mhz = 0,
    isb_dvb_subcommand_dvb_t2_bandwidth_5_mhz ,
    isb_dvb_subcommand_dvb_t2_bandwidth_6_mhz,
    isb_dvb_subcommand_dvb_t2_bandwidth_7_mhz,
    isb_dvb_subcommand_dvb_t2_bandwidth_8_mhz,
    isb_dvb_subcommand_dvb_t2_bandwidth_10_mhz
} ICEISB_dvb_subcommand_dvb_t2_bandwidth;

/**
  * @brief Enumeration for different DVB-C standards used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_c_standard
{
    isb_dvb_subcommand_dvb_t_standard_dvb_c = 0x00,
    isb_dvb_subcommand_dvb_t_standard_dvb_c2 = 0x01
} ICEISB_dvb_subcommand_dvb_c_standard;

/**
  * @brief Enumeration for different values of DVB-C bandwidth used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_c_bandwidth
{
    isb_dvb_subcommand_dvb_c_bandwidth_2_mhz = 0,
    isb_dvb_subcommand_dvb_c_bandwidth_4_mhz,
    isb_dvb_subcommand_dvb_c_bandwidth_6_mhz,
    isb_dvb_subcommand_dvb_c_bandwidth_8_mhz,
    isb_dvb_subcommand_dvb_c_bandwidth_10_mhz 
} ICEISB_dvb_subcommand_dvb_c_bandwidth;

/**
  * @brief Enumeration for different types of DVB-S polarity used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_s_polarity
{
    isb_dvb_subcommand_dvb_s_polarity_horizontal = 0x00,
    isb_dvb_subcommand_dvb_s_polarity_vertical = 0x01
} ICEISB_dvb_subcommand_dvb_s_polarity;

/**
  * @brief Enumeration for different values of DVB-S BAND used in DVB subcommands
 */
typedef enum ICEISB_dvb_subcommand_dvb_s_band
{
    isb_dvb_subcommand_dvb_s_band_low = 0x00,
    isb_dvb_subcommand_dvb_s_band_high = 0x01
} ICEISB_dvb_subcommand_dvb_s_band;

/**
  * @brief Enumeration for different types of program used in DVB subcommand 0x01 GET PROGRAM NAME AND SERVICE ID
 */
typedef enum ICEISB_dvb_subcommand_type_of_program
{
    isb_dvb_subcommand_type_of_program_any = 0x00,
    isb_dvb_subcommand_type_of_program_radio = 0x52,
    isb_dvb_subcommand_type_of_program_tv = 0x54
} ICEISB_dvb_subcommand_type_of_program_t;

/**
  * @brief Enumeration for different language parameters used in DVB subcommand 0x17 SET LANGUAGE PARAMETER
 */
typedef enum ICEISB_dvb_subcommand_type_of_language_parameter_t
{
    isb_dvb_subcommand_type_of_language_parameter_audio = 0x00,
    isb_dvb_subcommand_type_of_language_parameter_subtitle = 0x01,
    isb_dvb_subcommand_type_of_language_parameter_teletext = 0x10,
    isb_dvb_subcommand_type_of_language_parameter_video = 0x11
} ICEISB_dvb_subcommand_type_of_language_parameter_t;

/////////////////////////////////////////
/////////// VENDOR ENUMERATIONS
/////////////////////////////////////////

/**
  * @brief Enumeration for different types of Vendor subcommands that we can receive from ISB device
 */
typedef enum ICEISB_vendor_subcommand_receive_t
{
    isb_msg_vendor_recv_get_identification = 0x00,
    isb_msg_vendor_recv_get_channel_list = 0x02,
    isb_msg_vendor_recv_set_channel_list = 0x03,
    isb_msg_vendor_recv_get_channel_data = 0x05,
    isb_msg_vendor_recv_set_channel_data = 0x06,
    isb_msg_vendor_recv_get_favourite_list = 0x08,
    isb_msg_vendor_recv_set_favourite_list = 0x09,
    isb_msg_vendor_recv_get_favourite_list_data = 0x0B,
    isb_msg_vendor_recv_set_favourite_list_data = 0x0C,
    isb_msg_vendor_recv_get_tv_settings = 0x0E,
    isb_msg_vendor_recv_set_tv_settings = 0x0F
} ICEISB_vendor_subcommand_receive_t;

/**
  * @brief Enumeration for different frontend types used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_frontend_type
{
    isb_vendor_frontend_analogue = 0,
    isb_vendor_frontend_dvb_t = 1,
    isb_vendor_frontend_dvb_s = 2,
    isb_vendor_frontend_dvb_c = 3    
} ICEISB_vendor_subcommand_frontend_type;

/**
  * @brief Enumeration for different station types used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_station_type
{
    isb_vendor_station_type_tv = 0,    
    isb_vendor_station_type_radio = 1            
} ICEISB_vendor_subcommand_station_type;

/**
  * @brief Enumeration for different type of color system used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_color_system
{
    isb_vendor_color_system_pal = 0,
    isb_vendor_color_system_secam,
    isb_vendor_color_system_ntsc,
    isb_vendor_color_system_ntscv,
    isb_vendor_color_system_pal60,
    isb_vendor_color_system_pal_m,
    isb_vendor_color_system_pal_n
} ICEISB_vendor_subcommand_color_system;

/**
  * @brief Enumeration for different type of broadcast systems used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_broadcast_system
{
    isb_vendor_broadcast_system_bg = 0,
    isb_vendor_broadcast_system_i,
    isb_vendor_broadcast_system_dk,
    isb_vendor_broadcast_system_l,
    isb_vendor_broadcast_system_m,
    isb_vendor_broadcast_system_n,
} ICEISB_vendor_subcommand_broadcast_system;

/**
  * @brief Enumeration for different values of bandwidth used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_bandwidth
{
    isb_vendor_subcommand_bandwidth_5_mhz = 0,
    isb_vendor_subcommand_bandwidth_6_mhz,
    isb_vendor_subcommand_bandwidth_7_mhz,
    isb_vendor_subcommand_bandwidth_8_mhz 
} ICEISB_vendor_subcommand_bandwidth;

/**
  * @brief Enumeration for different values of carrier FFT used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_carrier_fft
{
    isb_vendor_subcommand_carrier_fft_2k = 0,
    isb_vendor_subcommand_carrier_fft_4k,
    isb_vendor_subcommand_carrier_fft_8k,
} ICEISB_vendor_subcommand_carrier_fft;

/**
  * @brief Enumeration for different values of guard intervals used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_guard
{
    isb_vendor_subcommand_guard_1_4 = 0,
    isb_vendor_subcommand_guard_1_8,
    isb_vendor_subcommand_guard_1_16,
    isb_vendor_subcommand_guard_1_32
} ICEISB_vendor_subcommand_guard;

/**
  * @brief Enumeration for different types of modulation used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_modulation
{
    isb_vendor_subcommand_modulation_qpsk = 0,
    isb_vendor_subcommand_modulation_16qam,
    isb_vendor_subcommand_modulation_32qam,
    isb_vendor_subcommand_modulation_64qam,
    isb_vendor_subcommand_modulation_128qam,
    isb_vendor_subcommand_modulation_256qam,
    isb_vendor_subcommand_modulation_8psk,
    isb_vendor_subcommand_modulation_16apsk,
    isb_vendor_subcommand_modulation_32apsk,
    isb_vendor_subcommand_modulation_8vsb,
    isb_vendor_subcommand_modulation_auto
} ICEISB_vendor_subcommand_modulation;

/**
  * @brief Enumeration for different values of coderate of fec used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_coderate_of_fec
{
    isb_vendor_subcommand_coderate_of_fec_1_4 = 0,
    isb_vendor_subcommand_coderate_of_fec_1_3,
    isb_vendor_subcommand_coderate_of_fec_2_5,
    isb_vendor_subcommand_coderate_of_fec_1_2,
    isb_vendor_subcommand_coderate_of_fec_3_5,
    isb_vendor_subcommand_coderate_of_fec_2_3,
    isb_vendor_subcommand_coderate_of_fec_3_4,
    isb_vendor_subcommand_coderate_of_fec_4_5,
    isb_vendor_subcommand_coderate_of_fec_5_6,
    isb_vendor_subcommand_coderate_of_fec_8_9,
    isb_vendor_subcommand_coderate_of_fec_9_10
} ICEISB_vendor_subcommand_coderate_of_fec;

/**
  * @brief Enumeration for different values of carrier offset used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_carrier_offset
{
    isb_vendor_subcommand_carrier_offset_0 = 0,
    isb_vendor_subcommand_carrier_offset_positive_1_6,
    isb_vendor_subcommand_carrier_offset_negative_1_6,
    isb_vendor_subcommand_carrier_offset_positive_2_6,
    isb_vendor_subcommand_carrier_offset_negative_2_6,
    isb_vendor_subcommand_carrier_offset_positive_3_6,
    isb_vendor_subcommand_carrier_offset_negative_3_6,
} ICEISB_vendor_subcommand_carrier_offset;

/**
  * @brief Enumeration for different priority level used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_priority
{
    isb_vendor_subcommand_priority_low = 0,
    isb_vendor_subcommand_priority_high = 1
} ICEISB_vendor_subcommand_priority;

/**
  * @brief Enumeration for different hierarchy values used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_hierarchy
{
    isb_vendor_subcommand_hierarchy_none = 0,
    isb_vendor_subcommand_hierarchy_alpha1,
    isb_vendor_subcommand_hierarchy_alpha2,
    isb_vendor_subcommand_hierarchy_alpha4
} ICEISB_vendor_subcommand_hierarchy;

/**
  * @brief Enumeration for different attriburtes used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_attributes
{
    isb_vendor_subcommand_attributes_unscrambled = 0,    
    isb_vendor_subcommand_attributes_scrambled
} ICEISB_vendor_subcommand_attributes;

/**
  * @brief Enumeration for different polarity values used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_polarity
{
    isb_vendor_subcommand_polarity_horizontal = 0,    
    isb_vendor_subcommand_polarity_vertical
} ICEISB_vendor_subcommand_polarity;

/**
  * @brief Enumeration for different vand values used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_band
{
    isb_vendor_subcommand_band_low = 0,    
    isb_vendor_subcommand_band_high
} ICEISB_vendor_subcommand_band;

/**
  * @brief Enumeration for different DVB-S standards used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_sat_standard
{
    isb_vendor_subcommand_sat_standard_dvb_s = 0,    
    isb_vendor_subcommand_sat_standard_dvb_s2
} ICEISB_vendor_subcommand_sat_standard;

/**
  * @brief Enumeration for different audio and video settigns used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_type_of_settings
{
    isb_vendor_subcommand_type_of_settings_picture = 0,    
    isb_vendor_subcommand_type_of_settings_sound,
    isb_vendor_subcommand_type_of_settings_configuration,
    isb_vendor_subcommand_type_of_settings_hotel
} ICEISB_vendor_subcommand_type_of_settings;

/**
  * @brief Enumeration for different color temperature values used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_color_temperature
{
    isb_vendor_subcommand_color_temperature_intesive = 0,    
    isb_vendor_subcommand_color_temperature_normal,
    isb_vendor_subcommand_color_temperature_soft
} ICEISB_vendor_subcommand_color_temperature;

/**
  * @brief Enumeration for different sound effects types used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_sound_effects
{
    isb_vendor_subcommand_sound_effects_stereo = 0,
    isb_vendor_subcommand_sound_effects_panorama,
    isb_vendor_subcommand_sound_effects_3d_surround
} ICEISB_vendor_subcommand_sound_effects;

/**
  * @brief Enumeration for different sound equalizer values used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_sound_adjust
{
    isb_vendor_subcommand_sound_adjust_off = 0,
    isb_vendor_subcommand_sound_adjust_speech,
    isb_vendor_subcommand_sound_adjust_classical,
    isb_vendor_subcommand_sound_adjust_pop,
    isb_vendor_subcommand_sound_adjust_custom
} ICEISB_vendor_subcommand_sound_adjust;

/**
  * @brief Enumeration for different eco modes used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_eco_mode
{
    isb_vendor_subcommand_eco_mode_eco = 0,
    isb_vendor_subcommand_eco_mode_premium,
    isb_vendor_subcommand_eco_mode_shop
} ICEISB_vendor_subcommand_eco_mode;

/**
  * @brief Enumeration for different hotel info modes used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_hotel_info_mode
{
    isb_vendor_subcommand_hotel_info_mode_analog = 0,
    isb_vendor_subcommand_hotel_info_mode_digital
} ICEISB_vendor_subcommand_hotel_info_mode;

/**
  * @brief Enumeration for different switch on channel modes used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_switch_on_channel_mode
{
    isb_vendor_subcommand_switch_on_channel_mode_off = 0,
    isb_vendor_subcommand_switch_on_channel_mode_analog,
    isb_vendor_subcommand_switch_on_channel_mode_digital
} ICEISB_vendor_subcommand_switch_on_channel_mode;

/**
  * @brief Enumeration for different hotel volume values values used in Vendor subcommands
 */
typedef enum ICEISB_vendor_subcommand_hotel_volume
{
    isb_vendor_subcommand_hotel_volume_tv = 0,
    isb_vendor_subcommand_hotel_volume_separate,
    isb_vendor_subcommand_hotel_volume_external,
    isb_vendor_subcommand_hotel_volume_off
} ICEISB_vendor_subcommand_hotel_volume;

/////////////////////////////////////////
///////// STRUCTURES
/////////////////////////////////////////

/**
 * @brief Structure that holds TV status information used when sending ISB message 0x01 SEND TV STATUS
 */
typedef struct ICEISB_tv_status_t
{
    frost_bool is_tv_on;
    frost_bool is_teletext_on;
    frost_bool is_valid_signal;
    frost_bool is_isb_osb_on_screen;
    frost_bool is_last_cmd_valid;
    frost_bool card_reader_have_data;
    frost_bool is_power_switch_off; 
    frost_bool is_aux_active;
    
    frost_bool is_channel_digital;
    frost_bool is_usb_menu_on;
    frost_bool last_cmd_not_supported;
    frost_bool is_dvr_playback_on;
    frost_bool is_dvr_recording_on; 
    frost_uint16 current_program;

    ICEISB_av_mode current_aux_input_type;
    frost_uint8 current_aux_input_number;
} ICEISB_tv_status_t;

/**
 * @brief Structure that holds TV capabilities used when sending ISB message 0x0D SEND TV STATUS
 */
typedef struct ICEISB_tv_capability_t 
{
    frost_bool is_timeshift_supported;
    frost_bool is_dvb_supported;
    frost_bool is_av_power_on_supported;
    frost_bool is_av_power_on_program_max_supported;
    frost_bool is_av_power_on_program_0_supported;
    frost_bool is_av_in_status_databyte2_supported;
    frost_bool is_av_in_status_databyte3_supported; 
} ICEISB_tv_capability_t;

/**
 * @brief Structure that holds information about received ISB command
 *
 * @note    command holds the received command
 * @note    command_options holds address of one of the structs below, that has command options, depending on the command value 
 *
 */        
typedef struct ICEISB_received_command_t
{
    ICEISB_message_type_receive_t command;
    void *command_options;
} ICEISB_received_command_t;

/**
 * @brief Structure that holds information from received ISB message 0x82 POWER OPTIONS
 */
typedef struct 
{
    frost_bool turn_tv_on_or_off; //false - turn off, true - turn on
    frost_uint8 program_to_select_at_power_on;
} ICEISB_power_options_t; //comand 0x82 power

/**
 * @brief Structure that holds information from received ISB message 0x84 ADD PROGRAM TO TV
 */    
typedef struct 
{
    frost_uint16 program_number;
    ICEISB_system_id program_system_id;
    ICEISB_program_type program_type;
    frost_uint16 program_frequency;
    char program_label[6];
} ICEISB_add_program_to_tv_options_t; //command 0x84 add program

/**
 * @brief Structure that holds information from received ISB message 0x85 SET POWER UP PARAMETERS
 */
typedef struct 
{
    ICEISB_power_up_parameters power_up_parameters;
    frost_uint8 power_up_parameters_value;
} ICEISB_set_power_up_parameters_options_t; //command 0x85 set parameters

/**
 * @brief Structure that holds information from received ISB message 0x86 SET CLOCK
 */    
typedef struct 
{
    frost_bool set_clock_or_alarm; //true - alarm, false - clock
    frost_bool alarm_on_or_off;
    frost_uint8 hours;
    frost_uint8 minutes;
} ICEISB_set_clock_options_t; //command 0x86 set clock

/**
 * @brief Structure that holds information color information for ISB message 0x89 OSD CONTROL
 */    
typedef struct 
{
    frost_bool red;
    frost_bool green;
    frost_bool blue;
} ICEISB_osd_control_color_t; 
  
/**
 * @brief Structure that holds information from received ISB message 0x89 OSD CONTROL
 */    
typedef struct 
{
    frost_bool erase_all_text;
    frost_bool center_text_on_screen;
    frost_uint8 time_to_display_text;
    ICEISB_osd_control_color_t select_background_color;
    ICEISB_osd_control_color_t select_text_color;
    frost_bool set_blinking_text;
    frost_bool switch_off_osd;
} ICEISB_osd_control_options_t; //command 0x89 osd control

/**
 * @brief Structure that holds information from received ISB message 0x8A DISPLAY OSD TEXT
 */
typedef struct 
{
    frost_uint8 vertical_position;
    frost_uint8 horizontal_position;
    char osd_text[32];
} ICEISB_display_osd_text_options_t; //command 0x8a display osd text

/**
 * @brief Structure that holds information from received ISB message 0x8B SET ANALOG LEVEL
 */    
typedef struct 
{
    ICEISB_current_parameters analog_parameter;
    frost_uint8 analog_parameter_value;
} ICEISB_set_analog_level_options_t; //command 0x8b set analog level

/**
 * @brief Structure that holds information from received ISB message 0x8C IR CODE TO TV
 */    
typedef struct 
{
    frost_bool repeat_last_ir_received;
    frost_uint8 ir_codeset;
    ICEISB_ir_commands ir_command;
} ICEISB_ir_code_to_tv_options_t; //command 0x8c ir code to tv

/**
 * @brief Structure that holds information from received ISB message 0x8D SELECT AV MODE
 */    
typedef struct 
{
    ICEISB_av_mode av_mode;
    frost_uint8 av_input_number;
} ICEISB_select_av_mode_options_t; //command 0x8d select av mode

/**
 * @brief Structure that holds information from received ISB message 0x8E TUNE TO FREQUENCY
 */    
typedef struct 
{
    frost_uint16 frequency;
    frost_bool should_be_saved; //todo ???
    frost_bool radio_or_tv; //false for radio, true for tv
    frost_bool indicate_program_osd; //todo ???????????????????????????
    ICEISB_system_id system_id;
} ICEISB_tune_to_frequency_options_t; //command 0x8e //todo?????

/**
 * @brief Structure that holds information from received ISB message 0x8F SELECT TELETEXT
 */    
typedef struct 
{
    frost_bool turn_teletext_on_or_off; //true - on, false - off
} ICEISB_select_teletext_options_t; //command 0x8f select_teletext

/**
 * @brief Structure that holds information from received ISB message 0x90 TUNE ANALOGUE PROGRAM
 */    
typedef struct 
{
    frost_uint8 change_to_program_number;
} ICEISB_change_program_options_t; //command 0x90 tune analogue program


/**
 * @brief Structure that holds information from received ISB message 0x92 PICTURE SIZE
 */
typedef struct 
{
    ICEISB_picture_size picture_size;
} ICEISB_picture_size_options_t; //command 0x92 picture size

/**
 * @brief Structure that holds information from received ISB message 0x93 PICTURE MODE
 */    
typedef struct 
{
    ICEISB_picture_mode picture_mode;
} ICEISB_picture_mode_options_t; //command 0x93 picture enhancement

/**
 * @brief Structure that holds information from received ISB message 0x94 PC AUTOADJUSTMENT
 */    
typedef struct 
{
    frost_bool auto_adjust;
} ICEISB_auto_adjustment_options_t; //command 0x94 pc autoadjustment

/**
 * @brief Structure that holds information from received ISB message 0x95 PICTURE ENHANCEMENT
 */        
typedef struct 
{
    frost_bool turn_pe2_on;
    frost_bool turn_pe1_on;
    frost_bool turn_nr2_on;
    frost_bool turn_nr1_on;
} ICEISB_picture_enhancement_options_t; //command 0x95 picture enhancement

/**
 * @brief Structure that holds information from received ISB message 0x96 NUMBER OF TV PROGRAMS
 */    
typedef struct 
{
    frost_uint8 number_of_tv_programs;
} ICEISB_set_number_of_tv_programs_options_t; //command 0x96 number of tv programs

/**
 * @brief Structure that holds information from received ISB message 0x9C SET POWER-ON AV INPUT
 */    
typedef struct 
{
    ICEISB_av_mode av_power_on_mode;
    frost_uint8 av_power_on_input_number;
} ICEISB_set_power_on_av_input_options_t; //command 0x9c set power on av input

/////////////////////////////////////////
///////// DVB SUBCOMMAND STRUCTURES
/////////////////////////////////////////
/**
*Structure of status  of ISB response.
*/
typedef struct 
{
    ICEISB_dvb_subcommand_status status;
} ICEISB_dvb_response_status_only_t;

/**
* ISB response structure of collection of TS  information  that includes status, number of tv programs and number of radio programs. 
*/

typedef struct
{
    ICEISB_dvb_subcommand_status status;
    frost_uint8 number_of_tv_programs;
    frost_uint8 number_of_radio_programs;
} ICEISB_dvb_response_collect_ts_information_t;

/**
* ISB response structure of Program information  that includes status, type, service_id and name of program.
*/
typedef struct
{
    ICEISB_dvb_subcommand_status status;
    ICEISB_dvb_subcommand_type_of_program_t type_of_program;
    frost_uint16 service_id;
    unsigned char program_name[251];
} ICEISB_dvb_response_get_prg_name_and_srvc_id_t;

/**
 * @brief Structure that used for getting video_pid version
 */

typedef struct
{
    ICEISB_dvb_subcommand_status status;    
    frost_uint16 video_pid;
} ICEISB_dvb_response_get_video_pid_t;

/**
 * @brief Structure that used for getting audio_pid version
 */
typedef struct
{
    ICEISB_dvb_subcommand_status status;    
    unsigned char language_code[3];
    frost_uint16 audio_pid;
} ICEISB_dvb_response_get_audio_pid_t;
/**
 * @brief Structure that used for getting txt_pid version
 */

typedef struct
{
    ICEISB_dvb_subcommand_status status;    
    frost_uint16 ttx_pid;
} ICEISB_dvb_response_get_ttx_pid_t;

/**
* @brief  Response structure of subtitle  pid that includes status, subtitleing mode,language, text page, pid .
*/
typedef struct
{
    ICEISB_dvb_subcommand_status status;
    ICEISB_dvb_subcommand_subtitling_modes subtitling_mode;
    unsigned char language_code[3];
    frost_uint16 text_page;
    frost_uint16 subtitle_pid;
} ICEISB_dvb_response_get_subtitle_pid_t;

/**
 * @brief Structure that used for asking firmware version
 */
typedef struct
{
    ICEISB_dvb_subcommand_status status;    
    unsigned char firmware_revision[100];
} ICEISB_dvb_response_ask_firmware_revision_t;

/**
 * @brief Structure that used for getting current languges
 */

typedef struct
{
    ICEISB_dvb_subcommand_status status;
    ICEISB_dvb_subcommand_type_of_language_parameter_t type_of_language_parameter;
    char language_code[10][4];
    frost_uint16 pid[10];
    unsigned char number_of_languages;
} ICEISB_dvb_response_request_current_languages_t;


/**
 * @brief Structure that holds information from received ISB message 0x98 DVB CONTROL
 *
 * @note    subcommand_options, depending on the subcommand value, holds address of one of the "ICEISB_dvb_"
 *        structs below that stores the subcommand options
 *
 */        
typedef struct 
{
    ICEISB_dvb_subcommand_t subcommand;
    void* subcommand_options;
} ICEISB_dvb_command_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x00 COLLECT TS INFORMATION
 *
 * @note    dvb_system_dependant_options is used, depending on the dvb_system value, as a address holder of one of
 *        "ICEISB_dvb_collect_ts_information_" structs below that holds dvb systems dependant options
 *
 */
typedef struct
{
    frost_uint32 frequency;
    ICEISB_dvb_subcommand_system dvb_system;
    void* dvb_system_dependant_options;
} ICEISB_dvb_collect_ts_information_options_t;

/**
 * @brief Structure that holds DVB-T system dependant options for command 0x00 COLLECT TS INFORMATION
 */    
typedef struct
{
    ICEISB_dvb_subcommand_guard guard;
    ICEISB_dvb_subcommand_modulation modulation;
    ICEISB_dvb_subcommand_dvb_t_bandwidth bandwidth;
    ICEISB_dvb_subcommand_coderate_of_fec fec;
    ICEISB_dvb_subcommand_carrier_type carrier_type;
    ICEISB_dvb_subcommand_dvb_t_standard dvb_t_standard;
    ICEISB_dvb_subcommand_extended_guard dvb_t2_guard;
    ICEISB_dvb_subcommand_dvb_t2_fft_size dvb_t2_fft_size;
    ICEISB_dvb_subcommand_dvb_t2_bandwidth dvb_t2_bandwidth;    
} ICEISB_dvb_collect_ts_information_dvb_t_t;

/**
 * @brief Structure that holds DVB-C system dependant options for command 0x00 COLLECT TS INFORMATION
 */    
typedef struct
{
    ICEISB_dvb_subcommand_dvb_c_standard dvb_c_standard;
    ICEISB_dvb_subcommand_guard guard;
    ICEISB_dvb_subcommand_modulation modulation;
    ICEISB_dvb_subcommand_dvb_c_bandwidth bandwidth;
    frost_uint16 symbolrate;
} ICEISB_dvb_collect_ts_information_dvb_c_t;

/**
 * @brief Structure that holds DVB-S system dependant options for command 0x00 COLLECT TS INFORMATION
 */    
typedef struct
{
    ICEISB_dvb_subcommand_dvb_s_polarity polarity;
    ICEISB_dvb_subcommand_dvb_s_band band;    
    ICEISB_dvb_subcommand_modulation modulation;
    ICEISB_dvb_subcommand_coderate_of_fec fec;
    frost_uint16 symbolrate;
} ICEISB_dvb_collect_ts_information_dvb_s_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x01 GET PROGRAM NAME AND SERVICE ID 
 *        FROM PROGRAM AT PROGRAM_INDEX
 */    
typedef struct
{
    frost_uint8 program_index;
    ICEISB_dvb_subcommand_type_of_program_t type_of_program;
} ICEISB_dvb_get_prg_name_and_srvc_id_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x02 GET VIDEO PID 
 *        FROM PROGRAM AT PROGRAM_INDEX
 */    
typedef struct
{
    frost_uint8 program_index;
} ICEISB_dvb_get_video_pid_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x03 GET AUDIO PID 
 *        FROM PROGRAM AT PROGRAM_INDEX
 */    
typedef struct
{
    frost_uint8 program_index;
} ICEISB_dvb_get_audio_pid_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x04 GET TTX PID 
 *        FROM PROGRAM AT PROGRAM_INDEX
 */    
typedef struct
{
    frost_uint8 program_index;
} ICEISB_dvb_get_ttx_pid_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x05 GET SUBTITLE PID 
 *        FROM PROGRAM AT PROGRAM_INDEX
 */    
typedef struct
{
    frost_uint8 program_index;
} ICEISB_dvb_get_subtitle_pid_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x06 STORE PROGRAM
 *        FROM PROGRAM_INDEX AT PROGRAM_POSITION
 */    
typedef struct
{
    frost_uint8 program_index;
    frost_uint16 program_position;
} ICEISB_dvb_store_program_at_position_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x07 MARK PROGRAM 
 *        AT PROGRAM_INDEX FOR WRITING NEW PID
 */    
typedef struct
{
    frost_uint8 program_index;
} ICEISB_dvb_mark_program_for_edit_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x08 SET NEW VIDEO PID IN MARKED PROGRAM
 */    
typedef struct
{
    frost_uint8 new_video_pid;
} ICEISB_dvb_set_new_video_pid_in_marked_program_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x09 SET NEW AUDIO PID IN MARKED PROGRAM
 */    
typedef struct
{
    frost_uint8 new_audio_pid;
} ICEISB_dvb_set_new_audio_pid_in_marked_program_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x0A SET NEW TTX PID IN MARKED PROGRAM
 */    
typedef struct
{
    frost_uint8 new_ttx_pid;
} ICEISB_dvb_set_new_ttx_pid_in_marked_program_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x0B SET NEW SUBTITLE PID IN MARKED PROGRAM
 */    
typedef struct
{
    frost_uint8 new_subtitle_pid;
} ICEISB_dvb_set_new_subtitle_pid_in_marked_program_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x0C STORE PROGRAM LABEL IN PROGRAM MEMORY
 */    
typedef struct
{
    frost_uint16 program_number;
    char program_label[6];
} ICEISB_dvb_store_label_in_program_memory_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x0D TUNE TO PROGRAM AT PROGRAM_INDEX
 *        IN CURRENT TRANSPORT STREAM
 */    
typedef struct
{
    frost_uint8 program_index;
} ICEISB_dvb_tune_to_program_in_found_programs_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x0E TUNE TO STORED PROGRAM
 */    
typedef struct
{
    frost_uint16 program_number;
} ICEISB_dvb_tune_to_stored_program_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x0F SEND DISEQC DATA
 */    
typedef struct
{
    unsigned char diseqc_data[252];
} ICEISB_dvb_send_diseqc_data_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x10 SET PINCODE
 */    
typedef struct
{
    frost_uint16 pin_code;
} ICEISB_dvb_set_pincode_in_unit_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x11 SET ROOM NUMBER
 */    
typedef struct
{
    frost_uint16 room_number;
} ICEISB_dvb_set_room_number_in_unit_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x12 SET MULTICAST NUMBER
 */    
typedef struct
{
    frost_uint16 multicast_number;
} ICEISB_dvb_set_multicast_number_in_unit_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x13 SET PRIVATE PID NUMBER IN UNIT
 */    
typedef struct
{
    frost_uint16 private_pid;
} ICEISB_dvb_set_private_pid_code_in_unit_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x14 SUBTITLE ON/OFF
 */    
typedef struct
{
    frost_bool turn_subtitle_on_or_off;
} ICEISB_dvb_turn_subtitle_on_or_off_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x15 SET NEW PCR PID IN MARKED PROGRAM
 */    
typedef struct
{
    frost_uint16 new_pcr;
} ICEISB_dvb_set_new_pcr_in_marked_program_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x16 SET NEW PMT PID IN MARKED PROGRAM
 */    
typedef struct
{
    frost_uint16 new_pmt;
} ICEISB_dvb_set_new_pmt_in_marked_program_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x17 SET LANGUAGE PARAMETER
 *
 * @note    Depending on the type of language parameter, either language_code or pid is populated
 */    
typedef struct
{
    ICEISB_dvb_subcommand_type_of_language_parameter_t type_of_language_parameter;
    char language_code[4];
    frost_uint16 pid;
} ICEISB_dvb_set_language_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x18 STORE TRANSPORT STREAM PARAMETERS
 *
 * @note    Depending on dvb_system value, dvb_system_dependant_options holds address of one of the 
 *        "ICEISB_dvb_store_ts_parameters_" structs.
 */    
typedef struct
{
    frost_uint8 ts_data_index;
    frost_uint32 frequency;
    ICEISB_dvb_subcommand_system dvb_system;
    void* dvb_system_dependant_options;    
} ICEISB_dvb_store_ts_parameters_options_t;

/**
 * @brief Structure that holds DVB-T system dependant options for command 0x18 STORE TRANSPORT STREAM PARAMETERS
 */    
typedef struct
{
    ICEISB_dvb_subcommand_guard guard;
    ICEISB_dvb_subcommand_modulation modulation;
    ICEISB_dvb_subcommand_dvb_t_bandwidth bandwidth;
    ICEISB_dvb_subcommand_coderate_of_fec fec;
    ICEISB_dvb_subcommand_carrier_type carrier_type;
    ICEISB_dvb_subcommand_dvb_t_standard dvb_t_standard;
    ICEISB_dvb_subcommand_extended_guard dvb_t2_guard;
    ICEISB_dvb_subcommand_dvb_t2_fft_size dvb_t2_fft_size;
    ICEISB_dvb_subcommand_dvb_t2_bandwidth dvb_t2_bandwidth;    
} ICEISB_dvb_store_ts_parameters_dvb_t_t;

/**
 * @brief Structure that holds DVB-C system dependant options for command 0x18 STORE TRANSPORT STREAM PARAMETERS
 */    
typedef struct
{
    ICEISB_dvb_subcommand_dvb_c_standard dvb_c_standard;
    ICEISB_dvb_subcommand_guard guard;
    ICEISB_dvb_subcommand_modulation modulation;
    ICEISB_dvb_subcommand_dvb_c_bandwidth bandwidth;
    frost_uint16 symbolrate;
} ICEISB_dvb_store_ts_parameters_dvb_c_t;

/**
 * @brief Structure that holds DVB-S system dependant options for command 0x18 STORE TRANSPORT STREAM PARAMETERS
 */    
typedef struct
{
    ICEISB_dvb_subcommand_dvb_s_polarity polarity;
    ICEISB_dvb_subcommand_dvb_s_band band;
    ICEISB_dvb_subcommand_modulation modulation;
    ICEISB_dvb_subcommand_coderate_of_fec fec;
    frost_uint16 symbolrate;
} ICEISB_dvb_store_ts_parameters_dvb_s_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x19 STORE PROGRAM USING TS PARAMETERS
 *
 * @note    ts_data_index holds index of stored TS data using command 0x18
 */    
typedef struct
{
    frost_bool protected_channel;
    frost_uint16 program_number;
    frost_uint8 ts_data_index; // linked to subcommand ts_data_index in STORE TS PARAMETERS 0x18
    frost_uint16 service_id;
    frost_uint16 pcr;
    frost_uint16 pmt;
    frost_uint16 video_pid;
    frost_uint16 audio_pid;
    frost_uint16 ttx_pid;
    frost_uint16 subtitle_pid;    
} ICEISB_dvb_store_program_using_ts_parameters_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x1A ASK FIRMWARE REVISION
 */    
typedef struct
{
    frost_bool ask_firmware_revision;
} ICEISB_dvb_ask_firmware_revision_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x1B DOWNLOAD NEW FIRMWARE
 */    
typedef struct
{
    frost_uint8 ts_index;
} ICEISB_dvb_download_new_firmware_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x1C GET CURRENT LANGUAGE PARAMETERS
 */    
typedef struct
{
    ICEISB_dvb_subcommand_type_of_language_parameter_t type_of_language_parameter;
} ICEISB_dvb_request_current_languages_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x1D REQUEST CHANNEL SEARCH
 */    
typedef struct
{
    ICEISB_dvb_subcommand_system dvb_system;
} ICEISB_dvb_request_channel_search_options_t;

/**
* @brief
/////////////////////////////////////////
/////////// VENDOR COMMAND
/////////////////////////////////////////
*/
typedef struct  
{
    frost_uint8 length_of_chassis_name; 
    frost_uint8 chassis_name[249];
    frost_uint8 major_version;
    frost_uint8 minor_version;
    frost_uint8 subversion;    
} ICEISB_vendor_send_identification_options_t;

/**
*@ brief Structure of confirmation of vendor send command
*/
typedef struct  
{
    frost_bool confirmation;
} ICEISB_vendor_send_confirmation_options_t;


/**
 * @brief Structure that holds information from received ISB message 0x9A VENDOR COMMANDS
 *
 * @note    subcommand_options, depending on the subcommand value, holds address of one of the "ICEISB_vendor_"
 *        structs below that stores the subcommand options
 *
 */
typedef struct 
{
    ICEISB_vendor_subcommand_receive_t subcommand;
    void* subcommand_options;
} ICEISB_vendor_command_options_t;

/**
 * @brief Structure that holds information from received Vendor subcommand 0x02 GET CHANNEL LIST
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type station_type;
    ICEISB_vendor_subcommand_frontend_type frontend_type;
    frost_bool request_info_for_all_channel_types;
    frost_uint8 satellite_index; 
    
} ICEISB_vendor_get_channel_list_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x03 SET CHANNEL LIST INFO
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type station_type[8];
    ICEISB_vendor_subcommand_frontend_type frontend_type[8];
    frost_uint16 number_of_channels[8];
    frost_uint8 number_of_channel_lists;
} ICEISB_vendor_set_or_put_channel_list_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x05 GET CHANNEL DATA
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type station_type;
    ICEISB_vendor_subcommand_frontend_type frontend_type;
    frost_uint16 start_index;
    frost_uint16 end_index;
} ICEISB_vendor_get_channel_data_options_t;

/**
 * @brief Structure that holds information from received DVB subcommand 0x06 SET CHANNEL DATA
 *
 * @note    channel_type_dependant_options is used, depending on the dvb_system value, as a address holder of one of
 *        "ICEISB_vendor_set_channel_data_" structs below that holds channel type dependant options
 *
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type station_type;
    ICEISB_vendor_subcommand_frontend_type frontend_type;
    frost_uint16 channel_index;
    frost_uint16 program_number;
    frost_uint8 channel_name_length;
    unsigned char channel_name[246];
    void* channel_type_dependant_options;
} ICEISB_vendor_set_or_put_channel_data_options_t;

/**
 * @brief Structure that holds analogue radio options for command 0x06 SET CHANNEL DATA
 */    
typedef struct
{
    frost_uint16 frequency;
} ICEISB_vendor_set_or_put_channel_data_analogue_radio_t;

/**
 * @brief Structure that holds analogue TV options for command 0x06 SET CHANNEL DATA
 */
typedef struct
{
    frost_uint16 frequency;
    frost_uint16 cni_code;
    ICEISB_vendor_subcommand_color_system color_system;
    ICEISB_vendor_subcommand_broadcast_system broadcast_system;
} ICEISB_vendor_set_or_put_channel_data_analogue_tv_t;

/**
 * @brief Structure that holds DVB-T options for command 0x06 SET CHANNEL DATA
 */
typedef struct
{
    frost_uint32 frequency;
    ICEISB_vendor_subcommand_bandwidth bandwidth;
    ICEISB_vendor_subcommand_carrier_fft carrier_fft;
    ICEISB_vendor_subcommand_guard guard;
    ICEISB_vendor_subcommand_modulation modulation;
    ICEISB_vendor_subcommand_coderate_of_fec coderate_of_fec;
    ICEISB_vendor_subcommand_carrier_offset carrier_offset;
    ICEISB_vendor_subcommand_priority priority;
    ICEISB_vendor_subcommand_hierarchy hierarchy;

    frost_uint16 original_network_id;
    frost_uint16 transport_stream_id;
    frost_uint16 service_id;
    frost_uint16 video_pid;
    frost_uint16 audio_pid;
    frost_uint16 pmt_pid;
    frost_uint16 ttx_pid;
    frost_uint16 pcr_pid;

    unsigned char language_code[3];
    ICEISB_vendor_subcommand_attributes attributes;    
}ICEISB_vendor_set_or_put_channel_data_dvb_t_t;

/**
 * @brief Structure that holds DVB-S options for command 0x06 SET CHANNEL DATA
 */
typedef struct
{
    frost_uint32 frequency;
    frost_uint16 symbol_rate;

    ICEISB_vendor_subcommand_polarity polarity;
    ICEISB_vendor_subcommand_band band;
    ICEISB_vendor_subcommand_sat_standard sat_standard;
    ICEISB_vendor_subcommand_coderate_of_fec coderate_of_fec;
    ICEISB_vendor_subcommand_modulation modulation;

    frost_uint16 original_network_id;
    frost_uint16 transport_stream_id;
    frost_uint16 service_id;
    frost_uint16 video_pid;
    frost_uint16 audio_pid;
    frost_uint16 ttx_pid;
    frost_uint16 pcr_pid;

    unsigned char language_code[3];
    ICEISB_vendor_subcommand_attributes attributes;    

}ICEISB_vendor_set_or_put_channel_data_dvb_s_t;

/**
 * @brief Structure that holds DVB-C options for command 0x06 SET CHANNEL DATA
 */
typedef struct
{
    frost_uint32 frequency;
    frost_uint16 symbol_rate;

    ICEISB_vendor_subcommand_modulation modulation;

    frost_uint16 original_network_id;
    frost_uint16 transport_stream_id;
    frost_uint16 service_id;
    frost_uint16 video_pid;
    frost_uint16 audio_pid;    
    frost_uint16 pmt_pid;
    frost_uint16 ttx_pid;
    frost_uint16 pcr_pid;

    unsigned char language_code[3];
    ICEISB_vendor_subcommand_attributes attributes;    
} ICEISB_vendor_set_or_put_channel_data_dvb_c_t;

/**
 * @brief Structure that holds DVB-T options for command 0x08 GET FAVOURITE LIST
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type favourite_type;
    frost_uint8 favourite_list_index;
    frost_bool info_for_all_tv_favourite_list;
    frost_bool info_for_all_radio_favourite_list;
    
} ICEISB_vendor_get_favourite_list_options_t;


/**
 * @brief Structure that holds DVB-T options for command 0x09 SET FAVOURITE LIST
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type favourite_type;
    frost_uint8 favourite_list_index;

    frost_uint16 number_of_channels_in_favourite_list;
    frost_uint8 favourite_list_name_length;
    unsigned char favourite_list_name[248];    
} ICEISB_vendor_set_or_put_favourite_list_options_t;

/**
 * @brief Structure that holds DVB-T options for command 0x0B GET FAVOURITE LIST DATA
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type favourite_type;    
    frost_uint8 favourite_list_index;
    frost_uint16 start_index;
    frost_uint16 end_index;    
} ICEISB_vendor_get_favourite_list_data_options_t;

/**
 * @brief Structure that holds DVB-T options for command 0x0C SET FAVOURITE LIST DATA
 */
typedef struct  
{
    ICEISB_vendor_subcommand_station_type favourite_type;    
    frost_uint8 favourite_list_index;
    frost_uint16 start_item_index;
    frost_uint16 end_item_index;    

    ICEISB_vendor_subcommand_station_type station_type[84];
    ICEISB_vendor_subcommand_frontend_type frontend_type[84];
    frost_uint16 channel_index[84];
} ICEISB_vendor_set_or_put_favourite_list_data_options_t;

/**
 * @brief Structure that holds DVB-T options for command 0x0E GET TV SETTINGS
 */
typedef struct  
{
    ICEISB_vendor_subcommand_type_of_settings settings_type;
} ICEISB_vendor_get_tv_settings_options_t;

/**
 * @brief Structure that holds options for command 0x0B SET TV SETTINGS
 *
 * @note    settings_type_dependant_options, holds address of one of the structs "ICEISB_vendor_set_tv_settings_",
 *        depending on the settings_type's value
 */
typedef struct  
{
    ICEISB_vendor_subcommand_type_of_settings settings_type;    
    void* settings_type_dependant_options;    
} ICEISB_vendor_set_or_put_tv_settings_options_t;

/**
 * @brief Structure that holds picture options for command 0x0B SET TV SETTINGS
 */
typedef struct  
{
    frost_uint8 contrast;
    frost_uint8 color_intesity;
    ICEISB_vendor_subcommand_color_temperature color_temperature;
    frost_uint8 brightness;
    frost_uint8 sharpness;
    frost_bool auto_format;
    
} ICEISB_vendor_set_or_put_tv_settings_picture_t;

/**
 * @brief Structure that holds sound options for command 0x0B SET TV SETTINGS
 */
typedef struct  
{
    ICEISB_vendor_subcommand_sound_effects sound_effects;
    ICEISB_vendor_subcommand_sound_adjust sound_adjust;
    frost_bool loudness;
    frost_bool automatic_volume;
    frost_uint8 maximum_volume;
    frost_uint8 switch_on_volume;    
} ICEISB_vendor_set_or_put_tv_settings_sound_t;

/**
 * @brief Structure that holds configuration options for command 0x0B SET TV SETTINGS
 */
typedef struct  
{
    frost_uint8 language;
    ICEISB_vendor_subcommand_eco_mode eco_mode;
    
} ICEISB_vendor_set_or_put_tv_settings_configuration_t;

/**
 * @brief Structure that holds hotel options for command 0x0B SET TV SETTINGS
 */
typedef struct  
{
    frost_bool welcome_screen;
    frost_uint8 hotel_info_channel;
    ICEISB_vendor_subcommand_hotel_info_mode hotel_info_mode;
    ICEISB_vendor_subcommand_switch_on_channel_mode switch_on_channel_mode;
    frost_uint8 switch_on_channel;
    ICEISB_vendor_subcommand_hotel_volume hotel_volume;
    frost_bool volume_front_control;    
} ICEISB_vendor_set_or_put_tv_settings_hotel_t;

/**
 * @brief Structure that holds connection status;
 */
typedef enum
{
    uninitialized = 0,
    standalone_mode,
    online_mode
} ICEISB_connection_state;

/**
 * ICE representation of a handle to ISB component
 *
 * ICE_ISBHandle is an opaque type, to be manipulated only by the
 * functions specified in this header file. 
 */
typedef struct ICE_ISBHandle ICE_ISBHandle;

/**
 * Callback function to inform Eclipse of a received command from ISB device
 *
 * This function will not call back into any ICEISB_ functions.
 *
 * This function may block (e.g. on mutexes and message queues).  Do not call
 * this in real-time code, use a separate thread.
 *
 * The ICE implementation must not call this function directly from within
 * any ICEISB_ functions - it must be called from a separate thread.
 *
 * @param pReceivedCmd    pointer to ICEISB_received_command_t struct holding received command options
 * @param context               The context parameter passed to ICE_ISBRegisterReceivedCommandCallback()
 */
typedef void (*ICEISB_ReceivedCmdCallbackFunction)(ICEISB_received_command_t* pReceivedCmd, void* context);

/**
 * Callback function to inform Eclipse of a received command from ISB device
 *
 * This function will not call back into any ICEISB_ functions.
 *
 * This function may block (e.g. on mutexes and message queues).  Do not call
 * this in real-time code, use a separate thread.
 *
 * The ICE implementation must not call this function directly from within
 * any ICEISB_ functions - it must be called from a separate thread.
 *
 * @param connection_state    holds current connection state
 * @param context               The context parameter passed to ICE_ISBRegisterConnectionStateCallback()
 */
typedef void (*ICEISB_ConnectionStateCallbackFunction)(ICEISB_connection_state connection_state, void* context);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ISB_TYPES_H_INCLUDED */
