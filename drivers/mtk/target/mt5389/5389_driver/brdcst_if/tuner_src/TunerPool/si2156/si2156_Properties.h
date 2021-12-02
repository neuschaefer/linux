/*************************************************************************************
                  Silicon Laboratories Broadcast Si2156 Layer 1 API
   API properties structures and functions definitions
   FILE: Si2156_Properties.h
   Supported IC : Si2156
   Compiled for ROM 12 firmware 1_E_build_6
   Revision: 0.1
   Date: May 31 2011
  (C) Copyright 2011, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   _Si2156_PROPERTIES_H_
#define   _Si2156_PROPERTIES_H_

/* _properties_defines_insertion_start */
/* Si2156 ATV_AFC_RANGE property definition */
#define   Si2156_ATV_AFC_RANGE_PROP 0x0610

#ifdef    Si2156_ATV_AFC_RANGE_PROP
  #define Si2156_ATV_AFC_RANGE_PROP_CODE 0x010610


    typedef struct { /* Si2156_ATV_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2156_ATV_AFC_RANGE_PROP_struct;

   /* ATV_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2156_ATV_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2156_ATV_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2156_ATV_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    1000
    #define Si2156_ATV_AFC_RANGE_PROP_RANGE_KHZ_RANGE_KHZ_MIN  0
    #define Si2156_ATV_AFC_RANGE_PROP_RANGE_KHZ_RANGE_KHZ_MAX  65535

#endif /* Si2156_ATV_AFC_RANGE_PROP */

/* Si2156 ATV_AGC_SPEED property definition */
#define   Si2156_ATV_AGC_SPEED_PROP 0x0611

#ifdef    Si2156_ATV_AGC_SPEED_PROP
  #define Si2156_ATV_AGC_SPEED_PROP_CODE 0x010611


    typedef struct { /* Si2156_ATV_AGC_SPEED_PROP_struct */
      unsigned char   if_agc_speed;
   } Si2156_ATV_AGC_SPEED_PROP_struct;

   /* ATV_AGC_SPEED property, IF_AGC_SPEED field definition (NO TITLE)*/
   #define  Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB         0
   #define  Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK        0xff
   #define  Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_DEFAULT    0
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO    0
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_89      89
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_105     105
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_121     121
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_137     137
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_158     158
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_172     172
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_185     185
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_196     196
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_206     206
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_216     216
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_219     219
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_222     222
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_248     248
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_250     250
    #define Si2156_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_251     251

#endif /* Si2156_ATV_AGC_SPEED_PROP */

/* Si2156 ATV_CONFIG_IF_PORT property definition */
#define   Si2156_ATV_CONFIG_IF_PORT_PROP 0x0603

#ifdef    Si2156_ATV_CONFIG_IF_PORT_PROP
  #define Si2156_ATV_CONFIG_IF_PORT_PROP_CODE 0x010603


    typedef struct { /* Si2156_ATV_CONFIG_IF_PORT_PROP_struct */
      unsigned char   atv_agc_source;
      unsigned char   atv_out_type;
   } Si2156_ATV_CONFIG_IF_PORT_PROP_struct;

   /* ATV_CONFIG_IF_PORT property, ATV_AGC_SOURCE field definition (NO TITLE)*/
   #define  Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_LSB         8
   #define  Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_MASK        0x07
   #define  Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_DEFAULT    0
    #define Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_INTERNAL      0
    #define Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_DLIF_AGC_3DB  1
    #define Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_ALIF_AGC_3DB  2

   /* ATV_CONFIG_IF_PORT property, ATV_OUT_TYPE field definition (NO TITLE)*/
   #define  Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LSB         0
   #define  Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_MASK        0x0f
   #define  Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_DEFAULT    10
    #define Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LIF_DIFF_IF1  8
    #define Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LIF_DIFF_IF2  10
    #define Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LIF_SE_IF1A   12
    #define Si2156_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LIF_SE_IF2A   14

#endif /* Si2156_ATV_CONFIG_IF_PORT_PROP */

/* Si2156 ATV_EXT_AGC property definition */
#define   Si2156_ATV_EXT_AGC_PROP 0x0607

#ifdef    Si2156_ATV_EXT_AGC_PROP
  #define Si2156_ATV_EXT_AGC_PROP_CODE 0x010607


    typedef struct { /* Si2156_ATV_EXT_AGC_PROP_struct */
      unsigned char   max_10mv;
      unsigned char   min_10mv;
   } Si2156_ATV_EXT_AGC_PROP_struct;

   /* ATV_EXT_AGC property, MAX_10MV field definition (NO TITLE)*/
   #define  Si2156_ATV_EXT_AGC_PROP_MAX_10MV_LSB         8
   #define  Si2156_ATV_EXT_AGC_PROP_MAX_10MV_MASK        0xff
   #define  Si2156_ATV_EXT_AGC_PROP_MAX_10MV_DEFAULT    200
    #define Si2156_ATV_EXT_AGC_PROP_MAX_10MV_MAX_10MV_MIN  0
    #define Si2156_ATV_EXT_AGC_PROP_MAX_10MV_MAX_10MV_MAX  215

   /* ATV_EXT_AGC property, MIN_10MV field definition (NO TITLE)*/
   #define  Si2156_ATV_EXT_AGC_PROP_MIN_10MV_LSB         0
   #define  Si2156_ATV_EXT_AGC_PROP_MIN_10MV_MASK        0xff
   #define  Si2156_ATV_EXT_AGC_PROP_MIN_10MV_DEFAULT    50
    #define Si2156_ATV_EXT_AGC_PROP_MIN_10MV_MIN_10MV_MIN  0
    #define Si2156_ATV_EXT_AGC_PROP_MIN_10MV_MIN_10MV_MAX  215

#endif /* Si2156_ATV_EXT_AGC_PROP */

/* Si2156 ATV_IEN property definition */
#define   Si2156_ATV_IEN_PROP 0x0601

#ifdef    Si2156_ATV_IEN_PROP
  #define Si2156_ATV_IEN_PROP_CODE 0x010601


    typedef struct { /* Si2156_ATV_IEN_PROP_struct */
      unsigned char   chlien;
      unsigned char   pclien;
   } Si2156_ATV_IEN_PROP_struct;

   /* ATV_IEN property, CHLIEN field definition (NO TITLE)*/
   #define  Si2156_ATV_IEN_PROP_CHLIEN_LSB         0
   #define  Si2156_ATV_IEN_PROP_CHLIEN_MASK        0x01
   #define  Si2156_ATV_IEN_PROP_CHLIEN_DEFAULT    0
    #define Si2156_ATV_IEN_PROP_CHLIEN_DISABLE  0
    #define Si2156_ATV_IEN_PROP_CHLIEN_ENABLE   1

   /* ATV_IEN property, PCLIEN field definition (NO TITLE)*/
   #define  Si2156_ATV_IEN_PROP_PCLIEN_LSB         1
   #define  Si2156_ATV_IEN_PROP_PCLIEN_MASK        0x01
   #define  Si2156_ATV_IEN_PROP_PCLIEN_DEFAULT    0
    #define Si2156_ATV_IEN_PROP_PCLIEN_DISABLE  0
    #define Si2156_ATV_IEN_PROP_PCLIEN_ENABLE   1

#endif /* Si2156_ATV_IEN_PROP */

/* Si2156 ATV_INT_SENSE property definition */
#define   Si2156_ATV_INT_SENSE_PROP 0x0613

#ifdef    Si2156_ATV_INT_SENSE_PROP
  #define Si2156_ATV_INT_SENSE_PROP_CODE 0x010613


    typedef struct { /* Si2156_ATV_INT_SENSE_PROP_struct */
      unsigned char   chlnegen;
      unsigned char   chlposen;
      unsigned char   pclnegen;
      unsigned char   pclposen;
   } Si2156_ATV_INT_SENSE_PROP_struct;

   /* ATV_INT_SENSE property, CHLNEGEN field definition (NO TITLE)*/
   #define  Si2156_ATV_INT_SENSE_PROP_CHLNEGEN_LSB         0
   #define  Si2156_ATV_INT_SENSE_PROP_CHLNEGEN_MASK        0x01
   #define  Si2156_ATV_INT_SENSE_PROP_CHLNEGEN_DEFAULT    0
    #define Si2156_ATV_INT_SENSE_PROP_CHLNEGEN_DISABLE  0
    #define Si2156_ATV_INT_SENSE_PROP_CHLNEGEN_ENABLE   1

   /* ATV_INT_SENSE property, CHLPOSEN field definition (NO TITLE)*/
   #define  Si2156_ATV_INT_SENSE_PROP_CHLPOSEN_LSB         8
   #define  Si2156_ATV_INT_SENSE_PROP_CHLPOSEN_MASK        0x01
   #define  Si2156_ATV_INT_SENSE_PROP_CHLPOSEN_DEFAULT    1
    #define Si2156_ATV_INT_SENSE_PROP_CHLPOSEN_DISABLE  0
    #define Si2156_ATV_INT_SENSE_PROP_CHLPOSEN_ENABLE   1

   /* ATV_INT_SENSE property, PCLNEGEN field definition (NO TITLE)*/
   #define  Si2156_ATV_INT_SENSE_PROP_PCLNEGEN_LSB         1
   #define  Si2156_ATV_INT_SENSE_PROP_PCLNEGEN_MASK        0x01
   #define  Si2156_ATV_INT_SENSE_PROP_PCLNEGEN_DEFAULT    0
    #define Si2156_ATV_INT_SENSE_PROP_PCLNEGEN_DISABLE  0
    #define Si2156_ATV_INT_SENSE_PROP_PCLNEGEN_ENABLE   1

   /* ATV_INT_SENSE property, PCLPOSEN field definition (NO TITLE)*/
   #define  Si2156_ATV_INT_SENSE_PROP_PCLPOSEN_LSB         9
   #define  Si2156_ATV_INT_SENSE_PROP_PCLPOSEN_MASK        0x01
   #define  Si2156_ATV_INT_SENSE_PROP_PCLPOSEN_DEFAULT    1
    #define Si2156_ATV_INT_SENSE_PROP_PCLPOSEN_DISABLE  0
    #define Si2156_ATV_INT_SENSE_PROP_PCLPOSEN_ENABLE   1

#endif /* Si2156_ATV_INT_SENSE_PROP */

/* Si2156 ATV_LIF_FREQ property definition */
#define   Si2156_ATV_LIF_FREQ_PROP 0x060c

#ifdef    Si2156_ATV_LIF_FREQ_PROP
  #define Si2156_ATV_LIF_FREQ_PROP_CODE 0x01060c


    typedef struct { /* Si2156_ATV_LIF_FREQ_PROP_struct */
      unsigned int    offset;
   } Si2156_ATV_LIF_FREQ_PROP_struct;

   /* ATV_LIF_FREQ property, OFFSET field definition (NO TITLE)*/
   #define  Si2156_ATV_LIF_FREQ_PROP_OFFSET_LSB         0
   #define  Si2156_ATV_LIF_FREQ_PROP_OFFSET_MASK        0xffff
   #define  Si2156_ATV_LIF_FREQ_PROP_OFFSET_DEFAULT    5000
    #define Si2156_ATV_LIF_FREQ_PROP_OFFSET_OFFSET_MIN  0
    #define Si2156_ATV_LIF_FREQ_PROP_OFFSET_OFFSET_MAX  7000

#endif /* Si2156_ATV_LIF_FREQ_PROP */

/* Si2156 ATV_LIF_OUT property definition */
#define   Si2156_ATV_LIF_OUT_PROP 0x060d

#ifdef    Si2156_ATV_LIF_OUT_PROP
  #define Si2156_ATV_LIF_OUT_PROP_CODE 0x01060d


    typedef struct { /* Si2156_ATV_LIF_OUT_PROP_struct */
      unsigned char   amp;
      unsigned char   offset;
   } Si2156_ATV_LIF_OUT_PROP_struct;

   /* ATV_LIF_OUT property, AMP field definition (NO TITLE)*/
   #define  Si2156_ATV_LIF_OUT_PROP_AMP_LSB         8
   #define  Si2156_ATV_LIF_OUT_PROP_AMP_MASK        0xff
   #define  Si2156_ATV_LIF_OUT_PROP_AMP_DEFAULT    100
    #define Si2156_ATV_LIF_OUT_PROP_AMP_AMP_MIN  0
    #define Si2156_ATV_LIF_OUT_PROP_AMP_AMP_MAX  255

   /* ATV_LIF_OUT property, OFFSET field definition (NO TITLE)*/
   #define  Si2156_ATV_LIF_OUT_PROP_OFFSET_LSB         0
   #define  Si2156_ATV_LIF_OUT_PROP_OFFSET_MASK        0xff
   #define  Si2156_ATV_LIF_OUT_PROP_OFFSET_DEFAULT    148
    #define Si2156_ATV_LIF_OUT_PROP_OFFSET_OFFSET_MIN  0
    #define Si2156_ATV_LIF_OUT_PROP_OFFSET_OFFSET_MAX  255

#endif /* Si2156_ATV_LIF_OUT_PROP */

/* Si2156 ATV_RF_TOP property definition */
#define   Si2156_ATV_RF_TOP_PROP 0x0612

#ifdef    Si2156_ATV_RF_TOP_PROP
  #define Si2156_ATV_RF_TOP_PROP_CODE 0x010612


    typedef struct { /* Si2156_ATV_RF_TOP_PROP_struct */
      unsigned char   atv_rf_top;
   } Si2156_ATV_RF_TOP_PROP_struct;

   /* ATV_RF_TOP property, ATV_RF_TOP field definition (NO TITLE)*/
   #define  Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_LSB         0
   #define  Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_MASK        0xff
   #define  Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_DEFAULT    0
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_AUTO   0
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_0DB    6
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M1DB   7
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M2DB   8
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M4DB   10
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M5DB   11
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB   12
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M7DB   13
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M8DB   14
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M9DB   15
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M10DB  16
    #define Si2156_ATV_RF_TOP_PROP_ATV_RF_TOP_M11DB  17

#endif /* Si2156_ATV_RF_TOP_PROP */

/* Si2156 ATV_RSQ_RSSI_THRESHOLD property definition */
#define   Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP 0x0605

#ifdef    Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP
  #define Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_CODE 0x010605


    typedef struct { /* Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_struct */
               char   hi;
               char   lo;
   } Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_struct;

   /* ATV_RSQ_RSSI_THRESHOLD property, HI field definition (NO TITLE)*/
   #define  Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_LSB         8
   #define  Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_MASK        0xff
   #define  Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_DEFAULT    0
    #define Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_HI_MIN  -128
    #define Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_HI_HI_MAX  127

   /* ATV_RSQ_RSSI_THRESHOLD property, LO field definition (NO TITLE)*/
   #define  Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB         0
   #define  Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK        0xff
   #define  Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_DEFAULT    -70
    #define Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_LO_MIN  -128
    #define Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_LO_LO_MAX  127

#endif /* Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP */

/* Si2156 ATV_VIDEO_MODE property definition */
#define   Si2156_ATV_VIDEO_MODE_PROP 0x0604

#ifdef    Si2156_ATV_VIDEO_MODE_PROP
  #define Si2156_ATV_VIDEO_MODE_PROP_CODE 0x010604


    typedef struct { /* Si2156_ATV_VIDEO_MODE_PROP_struct */
      unsigned char   color;
      unsigned char   invert_spectrum;
      unsigned char   trans;
      unsigned char   video_sys;
   } Si2156_ATV_VIDEO_MODE_PROP_struct;

   /* ATV_VIDEO_MODE property, COLOR field definition (NO TITLE)*/
   #define  Si2156_ATV_VIDEO_MODE_PROP_COLOR_LSB         4
   #define  Si2156_ATV_VIDEO_MODE_PROP_COLOR_MASK        0x01
   #define  Si2156_ATV_VIDEO_MODE_PROP_COLOR_DEFAULT    0
    #define Si2156_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC  0
    #define Si2156_ATV_VIDEO_MODE_PROP_COLOR_SECAM     1

   /* ATV_VIDEO_MODE property, INVERT_SPECTRUM field definition (NO TITLE)*/
   #define  Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_LSB         9
   #define  Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_MASK        0x01
   #define  Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_DEFAULT    0
    #define Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_NORMAL    0
    #define Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED  1

   /* ATV_VIDEO_MODE property, TRANS field definition (NO TITLE)*/
   #define  Si2156_ATV_VIDEO_MODE_PROP_TRANS_LSB         8
   #define  Si2156_ATV_VIDEO_MODE_PROP_TRANS_MASK        0x01
   #define  Si2156_ATV_VIDEO_MODE_PROP_TRANS_DEFAULT    0
    #define Si2156_ATV_VIDEO_MODE_PROP_TRANS_TERRESTRIAL  0
    #define Si2156_ATV_VIDEO_MODE_PROP_TRANS_CABLE        1

   /* ATV_VIDEO_MODE property, VIDEO_SYS field definition (NO TITLE)*/
   #define  Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LSB         0
   #define  Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_MASK        0x07
   #define  Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DEFAULT    0
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B   0
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH  1
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M   2
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_N   3
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I   4
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK  5
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L   6
    #define Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LP  7

#endif /* Si2156_ATV_VIDEO_MODE_PROP */

/* Si2156 ATV_VSNR_CAP property definition */
#define   Si2156_ATV_VSNR_CAP_PROP 0x0616

#ifdef    Si2156_ATV_VSNR_CAP_PROP
  #define Si2156_ATV_VSNR_CAP_PROP_CODE 0x010616


    typedef struct { /* Si2156_ATV_VSNR_CAP_PROP_struct */
      unsigned char   atv_vsnr_cap;
      unsigned char   backend_noise;
      unsigned char   frontend_noise;
   } Si2156_ATV_VSNR_CAP_PROP_struct;

   /* ATV_VSNR_CAP property, ATV_VSNR_CAP field definition (NO TITLE)*/
   #define  Si2156_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_LSB         0
   #define  Si2156_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_MASK        0xff
   #define  Si2156_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_DEFAULT    0
    #define Si2156_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_ATV_VSNR_CAP_MIN  0
    #define Si2156_ATV_VSNR_CAP_PROP_ATV_VSNR_CAP_ATV_VSNR_CAP_MAX  127

   /* ATV_VSNR_CAP property, BACKEND_NOISE field definition (NO TITLE)*/
   #define  Si2156_ATV_VSNR_CAP_PROP_BACKEND_NOISE_LSB         12
   #define  Si2156_ATV_VSNR_CAP_PROP_BACKEND_NOISE_MASK        0x0f
   #define  Si2156_ATV_VSNR_CAP_PROP_BACKEND_NOISE_DEFAULT    0
    #define Si2156_ATV_VSNR_CAP_PROP_BACKEND_NOISE_BACKEND_NOISE_MIN  0
    #define Si2156_ATV_VSNR_CAP_PROP_BACKEND_NOISE_BACKEND_NOISE_MAX  7

   /* ATV_VSNR_CAP property, FRONTEND_NOISE field definition (NO TITLE)*/
   #define  Si2156_ATV_VSNR_CAP_PROP_FRONTEND_NOISE_LSB         8
   #define  Si2156_ATV_VSNR_CAP_PROP_FRONTEND_NOISE_MASK        0x0f
   #define  Si2156_ATV_VSNR_CAP_PROP_FRONTEND_NOISE_DEFAULT    0
    #define Si2156_ATV_VSNR_CAP_PROP_FRONTEND_NOISE_FRONTEND_NOISE_MIN  0
    #define Si2156_ATV_VSNR_CAP_PROP_FRONTEND_NOISE_FRONTEND_NOISE_MAX  15

#endif /* Si2156_ATV_VSNR_CAP_PROP */

/* Si2156 CRYSTAL_TRIM property definition */
#define   Si2156_CRYSTAL_TRIM_PROP 0x0402

#ifdef    Si2156_CRYSTAL_TRIM_PROP
  #define Si2156_CRYSTAL_TRIM_PROP_CODE 0x010402


    typedef struct { /* Si2156_CRYSTAL_TRIM_PROP_struct */
      unsigned char   xo_cap;
   } Si2156_CRYSTAL_TRIM_PROP_struct;

   /* CRYSTAL_TRIM property, XO_CAP field definition (NO TITLE)*/
   #define  Si2156_CRYSTAL_TRIM_PROP_XO_CAP_LSB         0
   #define  Si2156_CRYSTAL_TRIM_PROP_XO_CAP_MASK        0x0f
   #define  Si2156_CRYSTAL_TRIM_PROP_XO_CAP_DEFAULT    8
    #define Si2156_CRYSTAL_TRIM_PROP_XO_CAP_XO_CAP_MIN  0
    #define Si2156_CRYSTAL_TRIM_PROP_XO_CAP_XO_CAP_MAX  15

#endif /* Si2156_CRYSTAL_TRIM_PROP */

/* Si2156 DTV_AGC_SPEED property definition */
#define   Si2156_DTV_AGC_SPEED_PROP 0x0708

#ifdef    Si2156_DTV_AGC_SPEED_PROP
  #define Si2156_DTV_AGC_SPEED_PROP_CODE 0x010708


    typedef struct { /* Si2156_DTV_AGC_SPEED_PROP_struct */
      unsigned char   agc_decim;
      unsigned char   if_agc_speed;
   } Si2156_DTV_AGC_SPEED_PROP_struct;

   /* DTV_AGC_SPEED property, AGC_DECIM field definition (NO TITLE)*/
   #define  Si2156_DTV_AGC_SPEED_PROP_AGC_DECIM_LSB         8
   #define  Si2156_DTV_AGC_SPEED_PROP_AGC_DECIM_MASK        0x03
   #define  Si2156_DTV_AGC_SPEED_PROP_AGC_DECIM_DEFAULT    0
    #define Si2156_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF  0
    #define Si2156_DTV_AGC_SPEED_PROP_AGC_DECIM_2    1
    #define Si2156_DTV_AGC_SPEED_PROP_AGC_DECIM_4    2
    #define Si2156_DTV_AGC_SPEED_PROP_AGC_DECIM_8    3

   /* DTV_AGC_SPEED property, IF_AGC_SPEED field definition (NO TITLE)*/
   #define  Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_LSB         0
   #define  Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_MASK        0xff
   #define  Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_DEFAULT    0
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO  0
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_39    39
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_54    54
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_63    63
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_89    89
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_105   105
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_121   121
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_137   137
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_158   158
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_172   172
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_185   185
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_196   196
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_206   206
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_216   216
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_219   219
    #define Si2156_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_222   222

#endif /* Si2156_DTV_AGC_SPEED_PROP */

/* Si2156 DTV_CONFIG_IF_PORT property definition */
#define   Si2156_DTV_CONFIG_IF_PORT_PROP 0x0702

#ifdef    Si2156_DTV_CONFIG_IF_PORT_PROP
  #define Si2156_DTV_CONFIG_IF_PORT_PROP_CODE 0x010702


    typedef struct { /* Si2156_DTV_CONFIG_IF_PORT_PROP_struct */
      unsigned char   dtv_agc_source;
      unsigned char   dtv_out_type;
   } Si2156_DTV_CONFIG_IF_PORT_PROP_struct;

   /* DTV_CONFIG_IF_PORT property, DTV_AGC_SOURCE field definition (NO TITLE)*/
   #define  Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_LSB         8
   #define  Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_MASK        0x07
   #define  Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_DEFAULT    0
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_INTERNAL       0
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_DLIF_AGC_3DB   1
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_ALIF_AGC_3DB   2
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_DLIF_AGC_FULL  3
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_ALIF_AGC_FULL  4

   /* DTV_CONFIG_IF_PORT property, DTV_OUT_TYPE field definition (NO TITLE)*/
   #define  Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LSB         0
   #define  Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_MASK        0x0f
   #define  Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_DEFAULT    0
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_IF1      0
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_IF2      1
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_SE_IF1A  4
    #define Si2156_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_SE_IF2A  5

#endif /* Si2156_DTV_CONFIG_IF_PORT_PROP */

/* Si2156 DTV_EXT_AGC property definition */
#define   Si2156_DTV_EXT_AGC_PROP 0x0705

#ifdef    Si2156_DTV_EXT_AGC_PROP
  #define Si2156_DTV_EXT_AGC_PROP_CODE 0x010705


    typedef struct { /* Si2156_DTV_EXT_AGC_PROP_struct */
      unsigned char   max_10mv;
      unsigned char   min_10mv;
   } Si2156_DTV_EXT_AGC_PROP_struct;

   /* DTV_EXT_AGC property, MAX_10MV field definition (NO TITLE)*/
   #define  Si2156_DTV_EXT_AGC_PROP_MAX_10MV_LSB         8
   #define  Si2156_DTV_EXT_AGC_PROP_MAX_10MV_MASK        0xff
   #define  Si2156_DTV_EXT_AGC_PROP_MAX_10MV_DEFAULT    200
    #define Si2156_DTV_EXT_AGC_PROP_MAX_10MV_MAX_10MV_MIN  0
    #define Si2156_DTV_EXT_AGC_PROP_MAX_10MV_MAX_10MV_MAX  215

   /* DTV_EXT_AGC property, MIN_10MV field definition (NO TITLE)*/
   #define  Si2156_DTV_EXT_AGC_PROP_MIN_10MV_LSB         0
   #define  Si2156_DTV_EXT_AGC_PROP_MIN_10MV_MASK        0xff
   #define  Si2156_DTV_EXT_AGC_PROP_MIN_10MV_DEFAULT    50
    #define Si2156_DTV_EXT_AGC_PROP_MIN_10MV_MIN_10MV_MIN  0
    #define Si2156_DTV_EXT_AGC_PROP_MIN_10MV_MIN_10MV_MAX  215

#endif /* Si2156_DTV_EXT_AGC_PROP */

/* Si2156 DTV_FILTER_SELECT property definition */
#define   Si2156_DTV_FILTER_SELECT_PROP 0x070c

#ifdef    Si2156_DTV_FILTER_SELECT_PROP
  #define Si2156_DTV_FILTER_SELECT_PROP_CODE 0x01070c


    typedef struct { /* Si2156_DTV_FILTER_SELECT_PROP_struct */
      unsigned char   filter;
   } Si2156_DTV_FILTER_SELECT_PROP_struct;

   /* DTV_FILTER_SELECT property, FILTER field definition (NO TITLE)*/
   #define  Si2156_DTV_FILTER_SELECT_PROP_FILTER_LSB         0
   #define  Si2156_DTV_FILTER_SELECT_PROP_FILTER_MASK        0x03
   #define  Si2156_DTV_FILTER_SELECT_PROP_FILTER_DEFAULT    0
    #define Si2156_DTV_FILTER_SELECT_PROP_FILTER_DEFAULT  0
    #define Si2156_DTV_FILTER_SELECT_PROP_FILTER_CUSTOM1  1
    #define Si2156_DTV_FILTER_SELECT_PROP_FILTER_CUSTOM2  2

#endif /* Si2156_DTV_FILTER_SELECT_PROP */

/* Si2156 DTV_IEN property definition */
#define   Si2156_DTV_IEN_PROP 0x0701

#ifdef    Si2156_DTV_IEN_PROP
  #define Si2156_DTV_IEN_PROP_CODE 0x010701


    typedef struct { /* Si2156_DTV_IEN_PROP_struct */
      unsigned char   chlien;
   } Si2156_DTV_IEN_PROP_struct;

   /* DTV_IEN property, CHLIEN field definition (NO TITLE)*/
   #define  Si2156_DTV_IEN_PROP_CHLIEN_LSB         0
   #define  Si2156_DTV_IEN_PROP_CHLIEN_MASK        0x01
   #define  Si2156_DTV_IEN_PROP_CHLIEN_DEFAULT    0
    #define Si2156_DTV_IEN_PROP_CHLIEN_DISABLE  0
    #define Si2156_DTV_IEN_PROP_CHLIEN_ENABLE   1

#endif /* Si2156_DTV_IEN_PROP */

/* Si2156 DTV_INITIAL_AGC_SPEED property definition */
#define   Si2156_DTV_INITIAL_AGC_SPEED_PROP 0x070d

#ifdef    Si2156_DTV_INITIAL_AGC_SPEED_PROP
  #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_CODE 0x01070d


    typedef struct { /* Si2156_DTV_INITIAL_AGC_SPEED_PROP_struct */
      unsigned char   agc_decim;
      unsigned char   if_agc_speed;
   } Si2156_DTV_INITIAL_AGC_SPEED_PROP_struct;

   /* DTV_INITIAL_AGC_SPEED property, AGC_DECIM field definition (NO TITLE)*/
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_LSB         8
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_MASK        0x03
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_DEFAULT    0
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF  0
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_2    1
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_4    2
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_8    3

   /* DTV_INITIAL_AGC_SPEED property, IF_AGC_SPEED field definition (NO TITLE)*/
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_LSB         0
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_MASK        0xff
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_DEFAULT    0
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO  0
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_39    39
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_54    54
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_63    63
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_89    89
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_105   105
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_121   121
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_137   137
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_158   158
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_172   172
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_185   185
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_196   196
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_206   206
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_216   216
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_219   219
    #define Si2156_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_222   222

#endif /* Si2156_DTV_INITIAL_AGC_SPEED_PROP */

/* Si2156 DTV_INITIAL_AGC_SPEED_PERIOD property definition */
#define   Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP 0x070e

#ifdef    Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
  #define Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE 0x01070e


    typedef struct { /* Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_struct */
      unsigned int    period;
   } Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_struct;

   /* DTV_INITIAL_AGC_SPEED_PERIOD property, PERIOD field definition (NO TITLE)*/
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_LSB         0
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_MASK        0xffff
   #define  Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_DEFAULT    0
    #define Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_PERIOD_MIN  0
    #define Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_PERIOD_PERIOD_MAX  65535

#endif /* Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */

/* Si2156 DTV_INT_SENSE property definition */
#define   Si2156_DTV_INT_SENSE_PROP 0x070a

#ifdef    Si2156_DTV_INT_SENSE_PROP
  #define Si2156_DTV_INT_SENSE_PROP_CODE 0x01070a


    typedef struct { /* Si2156_DTV_INT_SENSE_PROP_struct */
      unsigned char   chlnegen;
      unsigned char   chlposen;
   } Si2156_DTV_INT_SENSE_PROP_struct;

   /* DTV_INT_SENSE property, CHLNEGEN field definition (NO TITLE)*/
   #define  Si2156_DTV_INT_SENSE_PROP_CHLNEGEN_LSB         0
   #define  Si2156_DTV_INT_SENSE_PROP_CHLNEGEN_MASK        0x01
   #define  Si2156_DTV_INT_SENSE_PROP_CHLNEGEN_DEFAULT    0
    #define Si2156_DTV_INT_SENSE_PROP_CHLNEGEN_DISABLE  0
    #define Si2156_DTV_INT_SENSE_PROP_CHLNEGEN_ENABLE   1

   /* DTV_INT_SENSE property, CHLPOSEN field definition (NO TITLE)*/
   #define  Si2156_DTV_INT_SENSE_PROP_CHLPOSEN_LSB         8
   #define  Si2156_DTV_INT_SENSE_PROP_CHLPOSEN_MASK        0x01
   #define  Si2156_DTV_INT_SENSE_PROP_CHLPOSEN_DEFAULT    1
    #define Si2156_DTV_INT_SENSE_PROP_CHLPOSEN_DISABLE  0
    #define Si2156_DTV_INT_SENSE_PROP_CHLPOSEN_ENABLE   1

#endif /* Si2156_DTV_INT_SENSE_PROP */

/* Si2156 DTV_LIF_FREQ property definition */
#define   Si2156_DTV_LIF_FREQ_PROP 0x0706

#ifdef    Si2156_DTV_LIF_FREQ_PROP
  #define Si2156_DTV_LIF_FREQ_PROP_CODE 0x010706


    typedef struct { /* Si2156_DTV_LIF_FREQ_PROP_struct */
      unsigned int    offset;
   } Si2156_DTV_LIF_FREQ_PROP_struct;

   /* DTV_LIF_FREQ property, OFFSET field definition (NO TITLE)*/
   #define  Si2156_DTV_LIF_FREQ_PROP_OFFSET_LSB         0
   #define  Si2156_DTV_LIF_FREQ_PROP_OFFSET_MASK        0xffff
   #define  Si2156_DTV_LIF_FREQ_PROP_OFFSET_DEFAULT    5000
    #define Si2156_DTV_LIF_FREQ_PROP_OFFSET_OFFSET_MIN  0
    #define Si2156_DTV_LIF_FREQ_PROP_OFFSET_OFFSET_MAX  7000

#endif /* Si2156_DTV_LIF_FREQ_PROP */

/* Si2156 DTV_LIF_OUT property definition */
#define   Si2156_DTV_LIF_OUT_PROP 0x0707

#ifdef    Si2156_DTV_LIF_OUT_PROP
  #define Si2156_DTV_LIF_OUT_PROP_CODE 0x010707


    typedef struct { /* Si2156_DTV_LIF_OUT_PROP_struct */
      unsigned char   amp;
      unsigned char   offset;
   } Si2156_DTV_LIF_OUT_PROP_struct;

   /* DTV_LIF_OUT property, AMP field definition (NO TITLE)*/
   #define  Si2156_DTV_LIF_OUT_PROP_AMP_LSB         8
   #define  Si2156_DTV_LIF_OUT_PROP_AMP_MASK        0xff
   #define  Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT    27
    #define Si2156_DTV_LIF_OUT_PROP_AMP_AMP_MIN  0
    #define Si2156_DTV_LIF_OUT_PROP_AMP_AMP_MAX  255

   /* DTV_LIF_OUT property, OFFSET field definition (NO TITLE)*/
   #define  Si2156_DTV_LIF_OUT_PROP_OFFSET_LSB         0
   #define  Si2156_DTV_LIF_OUT_PROP_OFFSET_MASK        0xff
   #define  Si2156_DTV_LIF_OUT_PROP_OFFSET_DEFAULT    148
    #define Si2156_DTV_LIF_OUT_PROP_OFFSET_OFFSET_MIN  0
    #define Si2156_DTV_LIF_OUT_PROP_OFFSET_OFFSET_MAX  255

#endif /* Si2156_DTV_LIF_OUT_PROP */

/* Si2156 DTV_MODE property definition */
#define   Si2156_DTV_MODE_PROP 0x0703

#ifdef    Si2156_DTV_MODE_PROP
  #define Si2156_DTV_MODE_PROP_CODE 0x010703


    typedef struct { /* Si2156_DTV_MODE_PROP_struct */
      unsigned char   bw;
      unsigned char   invert_spectrum;
      unsigned char   modulation;
   } Si2156_DTV_MODE_PROP_struct;

   /* DTV_MODE property, BW field definition (NO TITLE)*/
   #define  Si2156_DTV_MODE_PROP_BW_LSB         0
   #define  Si2156_DTV_MODE_PROP_BW_MASK        0x0f
   #define  Si2156_DTV_MODE_PROP_BW_DEFAULT    8
    #define Si2156_DTV_MODE_PROP_BW_BW_6MHZ  6
    #define Si2156_DTV_MODE_PROP_BW_BW_7MHZ  7
    #define Si2156_DTV_MODE_PROP_BW_BW_8MHZ  8

   /* DTV_MODE property, INVERT_SPECTRUM field definition (NO TITLE)*/
   #define  Si2156_DTV_MODE_PROP_INVERT_SPECTRUM_LSB         8
   #define  Si2156_DTV_MODE_PROP_INVERT_SPECTRUM_MASK        0x01
   #define  Si2156_DTV_MODE_PROP_INVERT_SPECTRUM_DEFAULT    0
    #define Si2156_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL    0
    #define Si2156_DTV_MODE_PROP_INVERT_SPECTRUM_INVERTED  1

   /* DTV_MODE property, MODULATION field definition (NO TITLE)*/
   #define  Si2156_DTV_MODE_PROP_MODULATION_LSB         4
   #define  Si2156_DTV_MODE_PROP_MODULATION_MASK        0x0f
   #define  Si2156_DTV_MODE_PROP_MODULATION_DEFAULT    2
    #define Si2156_DTV_MODE_PROP_MODULATION_ATSC    0
    #define Si2156_DTV_MODE_PROP_MODULATION_QAM_US  1
    #define Si2156_DTV_MODE_PROP_MODULATION_DVBT    2
    #define Si2156_DTV_MODE_PROP_MODULATION_DVBC    3
    #define Si2156_DTV_MODE_PROP_MODULATION_ISDBT   4
    #define Si2156_DTV_MODE_PROP_MODULATION_ISDBC   5
    #define Si2156_DTV_MODE_PROP_MODULATION_DTMB    6

#endif /* Si2156_DTV_MODE_PROP */

/* Si2156 DTV_RF_TOP property definition */
#define   Si2156_DTV_RF_TOP_PROP 0x0709

#ifdef    Si2156_DTV_RF_TOP_PROP
  #define Si2156_DTV_RF_TOP_PROP_CODE 0x010709


    typedef struct { /* Si2156_DTV_RF_TOP_PROP_struct */
      unsigned char   dtv_rf_top;
   } Si2156_DTV_RF_TOP_PROP_struct;

   /* DTV_RF_TOP property, DTV_RF_TOP field definition (NO TITLE)*/
   #define  Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_LSB         0
   #define  Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_MASK        0xff
   #define  Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_DEFAULT    0
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO   0
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_0DB    6
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M1DB   7
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M2DB   8
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M4DB   10
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M5DB   11
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M6DB   12
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M7DB   13
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M8DB   14
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M9DB   15
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M10DB  16
    #define Si2156_DTV_RF_TOP_PROP_DTV_RF_TOP_M11DB  17

#endif /* Si2156_DTV_RF_TOP_PROP */

/* Si2156 DTV_RSQ_RSSI_THRESHOLD property definition */
#define   Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP 0x0704

#ifdef    Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP
  #define Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE 0x010704


    typedef struct { /* Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_struct */
               char   hi;
               char   lo;
   } Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_struct;

   /* DTV_RSQ_RSSI_THRESHOLD property, HI field definition (NO TITLE)*/
   #define  Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_LSB         8
   #define  Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_MASK        0xff
   #define  Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_DEFAULT    0
    #define Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_HI_MIN  -128
    #define Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_HI_HI_MAX  127

   /* DTV_RSQ_RSSI_THRESHOLD property, LO field definition (NO TITLE)*/
   #define  Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_LSB         0
   #define  Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_MASK        0xff
   #define  Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_DEFAULT    -80
    #define Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_LO_MIN  -128
    #define Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_LO_LO_MAX  127

#endif /* Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP */

/* Si2156 MASTER_IEN property definition */
#define   Si2156_MASTER_IEN_PROP 0x0401

#ifdef    Si2156_MASTER_IEN_PROP
  #define Si2156_MASTER_IEN_PROP_CODE 0x010401


    typedef struct { /* Si2156_MASTER_IEN_PROP_struct */
      unsigned char   atvien;
      unsigned char   ctsien;
      unsigned char   dtvien;
      unsigned char   errien;
      unsigned char   tunien;
   } Si2156_MASTER_IEN_PROP_struct;

   /* MASTER_IEN property, ATVIEN field definition (NO TITLE)*/
   #define  Si2156_MASTER_IEN_PROP_ATVIEN_LSB         1
   #define  Si2156_MASTER_IEN_PROP_ATVIEN_MASK        0x01
   #define  Si2156_MASTER_IEN_PROP_ATVIEN_DEFAULT    0
    #define Si2156_MASTER_IEN_PROP_ATVIEN_OFF  0
    #define Si2156_MASTER_IEN_PROP_ATVIEN_ON   1

   /* MASTER_IEN property, CTSIEN field definition (NO TITLE)*/
   #define  Si2156_MASTER_IEN_PROP_CTSIEN_LSB         7
   #define  Si2156_MASTER_IEN_PROP_CTSIEN_MASK        0x01
   #define  Si2156_MASTER_IEN_PROP_CTSIEN_DEFAULT    0
    #define Si2156_MASTER_IEN_PROP_CTSIEN_OFF  0
    #define Si2156_MASTER_IEN_PROP_CTSIEN_ON   1

   /* MASTER_IEN property, DTVIEN field definition (NO TITLE)*/
   #define  Si2156_MASTER_IEN_PROP_DTVIEN_LSB         2
   #define  Si2156_MASTER_IEN_PROP_DTVIEN_MASK        0x01
   #define  Si2156_MASTER_IEN_PROP_DTVIEN_DEFAULT    0
    #define Si2156_MASTER_IEN_PROP_DTVIEN_OFF  0
    #define Si2156_MASTER_IEN_PROP_DTVIEN_ON   1

   /* MASTER_IEN property, ERRIEN field definition (NO TITLE)*/
   #define  Si2156_MASTER_IEN_PROP_ERRIEN_LSB         6
   #define  Si2156_MASTER_IEN_PROP_ERRIEN_MASK        0x01
   #define  Si2156_MASTER_IEN_PROP_ERRIEN_DEFAULT    0
    #define Si2156_MASTER_IEN_PROP_ERRIEN_OFF  0
    #define Si2156_MASTER_IEN_PROP_ERRIEN_ON   1

   /* MASTER_IEN property, TUNIEN field definition (NO TITLE)*/
   #define  Si2156_MASTER_IEN_PROP_TUNIEN_LSB         0
   #define  Si2156_MASTER_IEN_PROP_TUNIEN_MASK        0x01
   #define  Si2156_MASTER_IEN_PROP_TUNIEN_DEFAULT    0
    #define Si2156_MASTER_IEN_PROP_TUNIEN_OFF  0
    #define Si2156_MASTER_IEN_PROP_TUNIEN_ON   1

#endif /* Si2156_MASTER_IEN_PROP */

/* Si2156 TUNER_BLOCKED_VCO property definition */
#define   Si2156_TUNER_BLOCKED_VCO_PROP 0x0504

#ifdef    Si2156_TUNER_BLOCKED_VCO_PROP
  #define Si2156_TUNER_BLOCKED_VCO_PROP_CODE 0x010504


    typedef struct { /* Si2156_TUNER_BLOCKED_VCO_PROP_struct */
               int    vco_code;
   } Si2156_TUNER_BLOCKED_VCO_PROP_struct;

   /* TUNER_BLOCKED_VCO property, VCO_CODE field definition (NO TITLE)*/
   #define  Si2156_TUNER_BLOCKED_VCO_PROP_VCO_CODE_LSB         0
   #define  Si2156_TUNER_BLOCKED_VCO_PROP_VCO_CODE_MASK        0xffff
   #define  Si2156_TUNER_BLOCKED_VCO_PROP_VCO_CODE_DEFAULT    0x8000
    #define Si2156_TUNER_BLOCKED_VCO_PROP_VCO_CODE_VCO_CODE_MIN  -32768
    #define Si2156_TUNER_BLOCKED_VCO_PROP_VCO_CODE_VCO_CODE_MAX  32767

#endif /* Si2156_TUNER_BLOCKED_VCO_PROP */

/* Si2156 TUNER_IEN property definition */
#define   Si2156_TUNER_IEN_PROP 0x0501

#ifdef    Si2156_TUNER_IEN_PROP
  #define Si2156_TUNER_IEN_PROP_CODE 0x010501


    typedef struct { /* Si2156_TUNER_IEN_PROP_struct */
      unsigned char   rssihien;
      unsigned char   rssilien;
      unsigned char   tcien;
   } Si2156_TUNER_IEN_PROP_struct;

   /* TUNER_IEN property, RSSIHIEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_IEN_PROP_RSSIHIEN_LSB         2
   #define  Si2156_TUNER_IEN_PROP_RSSIHIEN_MASK        0x01
   #define  Si2156_TUNER_IEN_PROP_RSSIHIEN_DEFAULT    0
    #define Si2156_TUNER_IEN_PROP_RSSIHIEN_DISABLE  0
    #define Si2156_TUNER_IEN_PROP_RSSIHIEN_ENABLE   1

   /* TUNER_IEN property, RSSILIEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_IEN_PROP_RSSILIEN_LSB         1
   #define  Si2156_TUNER_IEN_PROP_RSSILIEN_MASK        0x01
   #define  Si2156_TUNER_IEN_PROP_RSSILIEN_DEFAULT    0
    #define Si2156_TUNER_IEN_PROP_RSSILIEN_DISABLE  0
    #define Si2156_TUNER_IEN_PROP_RSSILIEN_ENABLE   1

   /* TUNER_IEN property, TCIEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_IEN_PROP_TCIEN_LSB         0
   #define  Si2156_TUNER_IEN_PROP_TCIEN_MASK        0x01
   #define  Si2156_TUNER_IEN_PROP_TCIEN_DEFAULT    0
    #define Si2156_TUNER_IEN_PROP_TCIEN_DISABLE  0
    #define Si2156_TUNER_IEN_PROP_TCIEN_ENABLE   1

#endif /* Si2156_TUNER_IEN_PROP */

/* Si2156 TUNER_INT_SENSE property definition */
#define   Si2156_TUNER_INT_SENSE_PROP 0x0505

#ifdef    Si2156_TUNER_INT_SENSE_PROP
  #define Si2156_TUNER_INT_SENSE_PROP_CODE 0x010505


    typedef struct { /* Si2156_TUNER_INT_SENSE_PROP_struct */
      unsigned char   rssihnegen;
      unsigned char   rssihposen;
      unsigned char   rssilnegen;
      unsigned char   rssilposen;
      unsigned char   tcnegen;
      unsigned char   tcposen;
   } Si2156_TUNER_INT_SENSE_PROP_struct;

   /* TUNER_INT_SENSE property, RSSIHNEGEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSIHNEGEN_LSB         2
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSIHNEGEN_MASK        0x01
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSIHNEGEN_DEFAULT    0
    #define Si2156_TUNER_INT_SENSE_PROP_RSSIHNEGEN_DISABLE  0
    #define Si2156_TUNER_INT_SENSE_PROP_RSSIHNEGEN_ENABLE   1

   /* TUNER_INT_SENSE property, RSSIHPOSEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSIHPOSEN_LSB         10
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSIHPOSEN_MASK        0x01
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSIHPOSEN_DEFAULT    1
    #define Si2156_TUNER_INT_SENSE_PROP_RSSIHPOSEN_DISABLE  0
    #define Si2156_TUNER_INT_SENSE_PROP_RSSIHPOSEN_ENABLE   1

   /* TUNER_INT_SENSE property, RSSILNEGEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSILNEGEN_LSB         1
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSILNEGEN_MASK        0x01
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSILNEGEN_DEFAULT    0
    #define Si2156_TUNER_INT_SENSE_PROP_RSSILNEGEN_DISABLE  0
    #define Si2156_TUNER_INT_SENSE_PROP_RSSILNEGEN_ENABLE   1

   /* TUNER_INT_SENSE property, RSSILPOSEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSILPOSEN_LSB         9
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSILPOSEN_MASK        0x01
   #define  Si2156_TUNER_INT_SENSE_PROP_RSSILPOSEN_DEFAULT    1
    #define Si2156_TUNER_INT_SENSE_PROP_RSSILPOSEN_DISABLE  0
    #define Si2156_TUNER_INT_SENSE_PROP_RSSILPOSEN_ENABLE   1

   /* TUNER_INT_SENSE property, TCNEGEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_INT_SENSE_PROP_TCNEGEN_LSB         0
   #define  Si2156_TUNER_INT_SENSE_PROP_TCNEGEN_MASK        0x01
   #define  Si2156_TUNER_INT_SENSE_PROP_TCNEGEN_DEFAULT    0
    #define Si2156_TUNER_INT_SENSE_PROP_TCNEGEN_DISABLE  0
    #define Si2156_TUNER_INT_SENSE_PROP_TCNEGEN_ENABLE   1

   /* TUNER_INT_SENSE property, TCPOSEN field definition (NO TITLE)*/
   #define  Si2156_TUNER_INT_SENSE_PROP_TCPOSEN_LSB         8
   #define  Si2156_TUNER_INT_SENSE_PROP_TCPOSEN_MASK        0x01
   #define  Si2156_TUNER_INT_SENSE_PROP_TCPOSEN_DEFAULT    1
    #define Si2156_TUNER_INT_SENSE_PROP_TCPOSEN_DISABLE  0
    #define Si2156_TUNER_INT_SENSE_PROP_TCPOSEN_ENABLE   1

#endif /* Si2156_TUNER_INT_SENSE_PROP */

/* Si2156 TUNER_LO_INJECTION property definition */
#define   Si2156_TUNER_LO_INJECTION_PROP 0x0506

#ifdef    Si2156_TUNER_LO_INJECTION_PROP
  #define Si2156_TUNER_LO_INJECTION_PROP_CODE 0x010506


    typedef struct { /* Si2156_TUNER_LO_INJECTION_PROP_struct */
      unsigned char   band_1;
      unsigned char   band_2;
      unsigned char   band_3;
      unsigned char   band_4;
      unsigned char   band_5;
   } Si2156_TUNER_LO_INJECTION_PROP_struct;

   /* TUNER_LO_INJECTION property, BAND_1 field definition (NO TITLE)*/
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_1_LSB         0
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_1_MASK        0x01
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_1_DEFAULT    1
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_1_LOW_SIDE   0
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE  1

   /* TUNER_LO_INJECTION property, BAND_2 field definition (NO TITLE)*/
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_2_LSB         1
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_2_MASK        0x01
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_2_DEFAULT    1
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE   0
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_2_HIGH_SIDE  1

   /* TUNER_LO_INJECTION property, BAND_3 field definition (NO TITLE)*/
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_3_LSB         2
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_3_MASK        0x01
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_3_DEFAULT    1
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE   0
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_3_HIGH_SIDE  1

   /* TUNER_LO_INJECTION property, BAND_4 field definition (NO TITLE)*/
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_4_LSB         3
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_4_MASK        0x01
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_4_DEFAULT    1
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_4_LOW_SIDE   0
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_4_HIGH_SIDE  1

   /* TUNER_LO_INJECTION property, BAND_5 field definition (NO TITLE)*/
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_5_LSB         4
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_5_MASK        0x01
   #define  Si2156_TUNER_LO_INJECTION_PROP_BAND_5_DEFAULT    1
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_5_LOW_SIDE   0
    #define Si2156_TUNER_LO_INJECTION_PROP_BAND_5_HIGH_SIDE  1

#endif /* Si2156_TUNER_LO_INJECTION_PROP */

/* _properties_defines_insertion_point */

/* _properties_struct_insertion_start */

  /* --------------------------------------------*/
  /* PROPERTIES STRUCT                           */
  /* This stores all property fields             */
  /* --------------------------------------------*/
  typedef struct {
    #ifdef    Si2156_ATV_AFC_RANGE_PROP
              Si2156_ATV_AFC_RANGE_PROP_struct                 atv_afc_range;
    #endif /* Si2156_ATV_AFC_RANGE_PROP */
    #ifdef    Si2156_ATV_AGC_SPEED_PROP
              Si2156_ATV_AGC_SPEED_PROP_struct                 atv_agc_speed;
    #endif /* Si2156_ATV_AGC_SPEED_PROP */
    #ifdef    Si2156_ATV_CONFIG_IF_PORT_PROP
              Si2156_ATV_CONFIG_IF_PORT_PROP_struct            atv_config_if_port;
    #endif /* Si2156_ATV_CONFIG_IF_PORT_PROP */
    #ifdef    Si2156_ATV_EXT_AGC_PROP
              Si2156_ATV_EXT_AGC_PROP_struct                   atv_ext_agc;
    #endif /* Si2156_ATV_EXT_AGC_PROP */
    #ifdef    Si2156_ATV_IEN_PROP
              Si2156_ATV_IEN_PROP_struct                       atv_ien;
    #endif /* Si2156_ATV_IEN_PROP */
    #ifdef    Si2156_ATV_INT_SENSE_PROP
              Si2156_ATV_INT_SENSE_PROP_struct                 atv_int_sense;
    #endif /* Si2156_ATV_INT_SENSE_PROP */
    #ifdef    Si2156_ATV_LIF_FREQ_PROP
              Si2156_ATV_LIF_FREQ_PROP_struct                  atv_lif_freq;
    #endif /* Si2156_ATV_LIF_FREQ_PROP */
    #ifdef    Si2156_ATV_LIF_OUT_PROP
              Si2156_ATV_LIF_OUT_PROP_struct                   atv_lif_out;
    #endif /* Si2156_ATV_LIF_OUT_PROP */
    #ifdef    Si2156_ATV_RF_TOP_PROP
              Si2156_ATV_RF_TOP_PROP_struct                    atv_rf_top;
    #endif /* Si2156_ATV_RF_TOP_PROP */
    #ifdef    Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP
              Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP_struct        atv_rsq_rssi_threshold;
    #endif /* Si2156_ATV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef    Si2156_ATV_VIDEO_MODE_PROP
              Si2156_ATV_VIDEO_MODE_PROP_struct                atv_video_mode;
    #endif /* Si2156_ATV_VIDEO_MODE_PROP */
    #ifdef    Si2156_ATV_VSNR_CAP_PROP
              Si2156_ATV_VSNR_CAP_PROP_struct                  atv_vsnr_cap;
    #endif /* Si2156_ATV_VSNR_CAP_PROP */
    #ifdef    Si2156_CRYSTAL_TRIM_PROP
              Si2156_CRYSTAL_TRIM_PROP_struct                  crystal_trim;
    #endif /* Si2156_CRYSTAL_TRIM_PROP */
    #ifdef    Si2156_DTV_AGC_SPEED_PROP
              Si2156_DTV_AGC_SPEED_PROP_struct                 dtv_agc_speed;
    #endif /* Si2156_DTV_AGC_SPEED_PROP */
    #ifdef    Si2156_DTV_CONFIG_IF_PORT_PROP
              Si2156_DTV_CONFIG_IF_PORT_PROP_struct            dtv_config_if_port;
    #endif /* Si2156_DTV_CONFIG_IF_PORT_PROP */
    #ifdef    Si2156_DTV_EXT_AGC_PROP
              Si2156_DTV_EXT_AGC_PROP_struct                   dtv_ext_agc;
    #endif /* Si2156_DTV_EXT_AGC_PROP */
    #ifdef    Si2156_DTV_FILTER_SELECT_PROP
              Si2156_DTV_FILTER_SELECT_PROP_struct             dtv_filter_select;
    #endif /* Si2156_DTV_FILTER_SELECT_PROP */
    #ifdef    Si2156_DTV_IEN_PROP
              Si2156_DTV_IEN_PROP_struct                       dtv_ien;
    #endif /* Si2156_DTV_IEN_PROP */
    #ifdef    Si2156_DTV_INITIAL_AGC_SPEED_PROP
              Si2156_DTV_INITIAL_AGC_SPEED_PROP_struct         dtv_initial_agc_speed;
    #endif /* Si2156_DTV_INITIAL_AGC_SPEED_PROP */
    #ifdef    Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP
              Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_struct  dtv_initial_agc_speed_period;
    #endif /* Si2156_DTV_INITIAL_AGC_SPEED_PERIOD_PROP */
    #ifdef    Si2156_DTV_INT_SENSE_PROP
              Si2156_DTV_INT_SENSE_PROP_struct                 dtv_int_sense;
    #endif /* Si2156_DTV_INT_SENSE_PROP */
    #ifdef    Si2156_DTV_LIF_FREQ_PROP
              Si2156_DTV_LIF_FREQ_PROP_struct                  dtv_lif_freq;
    #endif /* Si2156_DTV_LIF_FREQ_PROP */
    #ifdef    Si2156_DTV_LIF_OUT_PROP
              Si2156_DTV_LIF_OUT_PROP_struct                   dtv_lif_out;
    #endif /* Si2156_DTV_LIF_OUT_PROP */
    #ifdef    Si2156_DTV_MODE_PROP
              Si2156_DTV_MODE_PROP_struct                      dtv_mode;
    #endif /* Si2156_DTV_MODE_PROP */
    #ifdef    Si2156_DTV_RF_TOP_PROP
              Si2156_DTV_RF_TOP_PROP_struct                    dtv_rf_top;
    #endif /* Si2156_DTV_RF_TOP_PROP */
    #ifdef    Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP
              Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP_struct        dtv_rsq_rssi_threshold;
    #endif /* Si2156_DTV_RSQ_RSSI_THRESHOLD_PROP */
    #ifdef    Si2156_MASTER_IEN_PROP
              Si2156_MASTER_IEN_PROP_struct                    master_ien;
    #endif /* Si2156_MASTER_IEN_PROP */
    #ifdef    Si2156_TUNER_BLOCKED_VCO_PROP
              Si2156_TUNER_BLOCKED_VCO_PROP_struct             tuner_blocked_vco;
    #endif /* Si2156_TUNER_BLOCKED_VCO_PROP */
    #ifdef    Si2156_TUNER_IEN_PROP
              Si2156_TUNER_IEN_PROP_struct                     tuner_ien;
    #endif /* Si2156_TUNER_IEN_PROP */
    #ifdef    Si2156_TUNER_INT_SENSE_PROP
              Si2156_TUNER_INT_SENSE_PROP_struct               tuner_int_sense;
    #endif /* Si2156_TUNER_INT_SENSE_PROP */
    #ifdef    Si2156_TUNER_LO_INJECTION_PROP
              Si2156_TUNER_LO_INJECTION_PROP_struct            tuner_lo_injection;
    #endif /* Si2156_TUNER_LO_INJECTION_PROP */
  } Si2156_PropObj;
/* _properties_struct_insertion_point */

#define   Si2156_GET_PROPERTY_STRING

#endif /* _Si2156_PROPERTIES_H_ */







