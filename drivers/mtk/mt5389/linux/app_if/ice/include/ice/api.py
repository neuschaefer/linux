# This file specifies what ICE files make up the distributed interface
header_files = (
    'ice_aacs.h',
    'ice_audio.h',
    'ice_audio_types.h',
    'ice_av.h',
    'ice_bootloader.h',
    'ice_callisto.h',
    'ice_common_interface.h',
    'ice_common_interface_plus.h',
    'ice_component_video.h',
    'ice_crypto.h',
    'ice_debug_init.h',
    'ice_descrambler.h',
    'ice_device_number.h',
    'ice_display_types.h',
    'ice_dtcp.h',
    'ice_hardware.h',
    'ice_hdmi.h',
    'ice_image.h',
    'ice_keypress_id.h',
    'ice_low_power_mode.h',
    'ice_low_power_mode_types.h',
    'ice_media_player.h',
    'ice_media_player_types.h',
    'ice_memory_init.h',
    'ice_mheg_debug_out.h',
    'ice_nonvolatile.h',
    'ice_osd.h',
    'ice_osd_types.h',
    'ice_receiver_version.h',
    'ice_remote_control.h',
    'ice_rtc.h',
    'ice_scart.h',
    'ice_section_acquisition_common.h',
    'ice_settings.h',
    'ice_software_update.h',
    'ice_spdif.h',
    'ice_splash_iframe.h',
    'ice_stc.h',
    'ice_system_test.h',
    'ice_teletext.h',
    'ice_test_menu.h',
    'ice_tpdata.h',
    'ice_tsfileplayer.h',
    'ice_tuner.h',
    'ice_vbi.h',
    'ice_version.h',
    'ice_vga.h',
    'ice_video.h',
    'ice_video_types.h',
    'ice_removable_device.h',
    'ice_exif.h'
)

if INCLUDE_TV_EXTENSIONS:
    header_files += (
         'ice_backlight_control.h',
         'ice_external.h',
         'ice_external_types.h',
         'ice_picture_controls.h',
         'ice_sound_controls.h'
    )

if INCLUDE_DETERMINE_ZOOM:
    header_files += (
         'ice_zoom_mode.h',
    )

if USE_TERR_ANALOG_DIGITAL_TUNER:
    header_files += (
         'ice_vbi_collector.h',
    )


if INCLUDE_CEC_SUPPORT:
    header_files += (
         'ice_cec.h',
         'ice_cec_ids.h'
    )

if USE_PROFILE_MANAGER:
    header_files += (
         'ice_profile.h',
    )

if INCLUDE_MP3:
    header_files += (
         'ice_mp3.h',
    )
        
if INCLUDE_DYNAMIC_UI:
    header_files += (
         'ice_ui.h',
    )

if INCLUDE_CARPO:
    header_files += (
         'ice_carpo.h',
    )
   
if INCLUDE_CONAX:
    header_files += (
        'ice_smartcard.h',
        'ice_descrambler.h',
    )

if INCLUDE_DVR:
    header_files += (
        'ice_dvr.h',
        'ice_filesystem.h',
    )
    
if not USE_CABOT_SUBTITLES:
    header_files += (
        'ice_subtitle.h',
    )

if 0: # TODO: if INCLUDE_MODEM:
    header_files += (
        'ice_socket.h',
    )
    
if INCLUDE_IP_NETWORK:
    header_files += (
        'ice_ip.h',
    )
      
if INCLUDE_DSMCC_NVM_CACHING:
    header_files += (
        'ice_dsmcc_nvm_caching.h',
    )
    
if INCLUDE_USB_SOFTWARE_UPGRADE:
    header_files += (
        'ice_software_burner.h',
    )

if INCLUDE_DVBT2:
    header_files += (
        'ice_network.h',
        'ice_ts_player.h',
        'ice_ts_player_types.h'
    )

# TODO: The section acquisition API choice should be an option.
if 0:
    header_files += (
        'ice_section_acquisition_posneg.h',
    )
else:
    header_files += (
        'ice_section_acquisition_simple.h',
    )

# These scripts will be run whenever a distribution is made using ICE headers
scripts = {
    # Check that ICE headers do not use built-in types
    'ice_check_headers' :
                    'python $(MAKEROOT)/scripts/check_basic_types.py *.h',
    # Check that ICE headers can be compiled standalone for C and C++
    'ice_compile_headers' :
                    'python $(MAKEROOT)/scripts/compile_headers.py *.h',
    }

doxygen_title = 'ICE'
