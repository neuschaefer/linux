/*************************************************************************************
                  Silicon Laboratories Broadcast Si2156 Layer 1 API
   API functions prototypes used by commands and properties
   FILE: Si2156_Commands_Prototypes.h
   Supported IC : Si2156
   Compiled for ROM 12 firmware 1_E_build_6
   Revision: 0.1
   Date: May 31 2011
  (C) Copyright 2011, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef    Si2156_COMMANDS_PROTOTYPES_H
#define    Si2156_COMMANDS_PROTOTYPES_H

unsigned char Si2156_CurrentResponseStatus (L1_Si2156_Context *api, unsigned char ptDataBuffer);
unsigned char Si2156_pollForCTS            (L1_Si2156_Context *api);
unsigned char Si2156_pollForResponse       (L1_Si2156_Context *api, unsigned int nbBytes, unsigned char *pByteBuffer);
unsigned char Si2156_L1_SendCommand2       (L1_Si2156_Context *api, unsigned int cmd_code);
unsigned char Si2156_L1_SetProperty        (L1_Si2156_Context *api, unsigned int prop, int  data);
unsigned char Si2156_L1_GetProperty        (L1_Si2156_Context *api, unsigned int prop, int *data);
unsigned char Si2156_L1_SetProperty2       (L1_Si2156_Context *api, unsigned int prop);
unsigned char Si2156_L1_GetProperty2       (L1_Si2156_Context *api, unsigned int prop);

#ifdef    Si2156_GET_PROPERTY_STRING
unsigned char    Si2156_L1_GetPropertyString        (L1_Si2156_Context *api, unsigned int prop, char *separator, char *msg);
#endif /* Si2156_GET_PROPERTY_STRING */

#ifdef    Si2156_GET_COMMAND_STRINGS
  unsigned char   Si2156_L1_GetCommandResponseString(L1_Si2156_Context *api, unsigned int cmd_code, char *separator, char *msg);
#endif /* Si2156_GET_COMMAND_STRINGS */

/* _commands_prototypes_insertion_start */
#ifdef    Si2156_AGC_OVERRIDE_CMD
unsigned char Si2156_L1_AGC_OVERRIDE              (L1_Si2156_Context *api,
                                                   unsigned char   force_max_gain,
                                                   unsigned char   force_top_gain);
#endif /* Si2156_AGC_OVERRIDE_CMD */
#ifdef    Si2156_ATV_CW_TEST_CMD
unsigned char Si2156_L1_ATV_CW_TEST               (L1_Si2156_Context *api,
                                                   unsigned char   pc_lock);
#endif /* Si2156_ATV_CW_TEST_CMD */
#ifdef    Si2156_ATV_RESTART_CMD
unsigned char Si2156_L1_ATV_RESTART               (L1_Si2156_Context *api);
#endif /* Si2156_ATV_RESTART_CMD */
#ifdef    Si2156_ATV_STATUS_CMD
unsigned char Si2156_L1_ATV_STATUS                (L1_Si2156_Context *api,
                                                   unsigned char   intack);
#endif /* Si2156_ATV_STATUS_CMD */
#ifdef    Si2156_CONFIG_PINS_CMD
unsigned char Si2156_L1_CONFIG_PINS               (L1_Si2156_Context *api,
                                                   unsigned char   gpio1_mode,
                                                   unsigned char   gpio1_read,
                                                   unsigned char   gpio2_mode,
                                                   unsigned char   gpio2_read,
                                                   unsigned char   gpio3_mode,
                                                   unsigned char   gpio3_read,
                                                   unsigned char   bclk1_mode,
                                                   unsigned char   bclk1_read,
                                                   unsigned char   xout_mode);
#endif /* Si2156_CONFIG_PINS_CMD */
#ifdef    Si2156_DOWNLOAD_DATASET_CONTINUE_CMD
unsigned char Si2156_L1_DOWNLOAD_DATASET_CONTINUE (L1_Si2156_Context *api,
                                                   unsigned char   data0,
                                                   unsigned char   data1,
                                                   unsigned char   data2,
                                                   unsigned char   data3,
                                                   unsigned char   data4,
                                                   unsigned char   data5,
                                                   unsigned char   data6);
#endif /* Si2156_DOWNLOAD_DATASET_CONTINUE_CMD */
#ifdef    Si2156_DOWNLOAD_DATASET_START_CMD
unsigned char Si2156_L1_DOWNLOAD_DATASET_START    (L1_Si2156_Context *api,
                                                   unsigned char   dataset_id,
                                                   unsigned char   dataset_checksum,
                                                   unsigned char   data0,
                                                   unsigned char   data1,
                                                   unsigned char   data2,
                                                   unsigned char   data3,
                                                   unsigned char   data4);
#endif /* Si2156_DOWNLOAD_DATASET_START_CMD */
#ifdef    Si2156_DTV_RESTART_CMD
unsigned char Si2156_L1_DTV_RESTART               (L1_Si2156_Context *api);
#endif /* Si2156_DTV_RESTART_CMD */
#ifdef    Si2156_DTV_STATUS_CMD
unsigned char Si2156_L1_DTV_STATUS                (L1_Si2156_Context *api,
                                                   unsigned char   intack);
#endif /* Si2156_DTV_STATUS_CMD */
#ifdef    Si2156_EXIT_BOOTLOADER_CMD
unsigned char Si2156_L1_EXIT_BOOTLOADER           (L1_Si2156_Context *api,
                                                   unsigned char   func,
                                                   unsigned char   ctsien);
#endif /* Si2156_EXIT_BOOTLOADER_CMD */
#ifdef    Si2156_FINE_TUNE_CMD
unsigned char Si2156_L1_FINE_TUNE                 (L1_Si2156_Context *api,
                                                   unsigned char   persistence,
                                                             int   offset_500hz);
#endif /* Si2156_FINE_TUNE_CMD */
#ifdef    Si2156_GET_PROPERTY_CMD
unsigned char Si2156_L1_GET_PROPERTY              (L1_Si2156_Context *api,
                                                   unsigned char   reserved,
                                                   unsigned int    prop);
#endif /* Si2156_GET_PROPERTY_CMD */
#ifdef    Si2156_GET_REV_CMD
unsigned char Si2156_L1_GET_REV                   (L1_Si2156_Context *api);
#endif /* Si2156_GET_REV_CMD */
#ifdef    Si2156_PART_INFO_CMD
unsigned char Si2156_L1_PART_INFO                 (L1_Si2156_Context *api);
#endif /* Si2156_PART_INFO_CMD */
#ifdef    Si2156_POWER_DOWN_CMD
unsigned char Si2156_L1_POWER_DOWN                (L1_Si2156_Context *api);
#endif /* Si2156_POWER_DOWN_CMD */
#ifdef    Si2156_POWER_UP_CMD
unsigned char Si2156_L1_POWER_UP                  (L1_Si2156_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   reserved1,
                                                   unsigned char   reserved2,
                                                   unsigned char   reserved3,
                                                   unsigned char   clock_mode,
                                                   unsigned char   clock_freq,
                                                   unsigned char   addr_mode,
                                                   unsigned char   func,
                                                   unsigned char   ctsien,
                                                   unsigned char   wake_up);
#endif /* Si2156_POWER_UP_CMD */
#ifdef    Si2156_SET_PROPERTY_CMD
unsigned char Si2156_L1_SET_PROPERTY              (L1_Si2156_Context *api,
                                                   unsigned char   reserved,
                                                   unsigned int    prop,
                                                   unsigned int    data);
#endif /* Si2156_SET_PROPERTY_CMD */
#ifdef    Si2156_STANDBY_CMD
unsigned char Si2156_L1_STANDBY                   (L1_Si2156_Context *api,
                                                   unsigned char   type);
#endif /* Si2156_STANDBY_CMD */
#ifdef    Si2156_TUNER_STATUS_CMD
unsigned char Si2156_L1_TUNER_STATUS              (L1_Si2156_Context *api,
                                                   unsigned char   intack);
#endif /* Si2156_TUNER_STATUS_CMD */
#ifdef    Si2156_TUNER_TUNE_FREQ_CMD
unsigned char Si2156_L1_TUNER_TUNE_FREQ           (L1_Si2156_Context *api,
                                                   unsigned char   mode,
                                                   unsigned long   freq);
#endif /* Si2156_TUNER_TUNE_FREQ_CMD */
/* _commands_prototypes_insertion_point */

#endif /* Si2156_COMMANDS_PROTOTYPES_H */












