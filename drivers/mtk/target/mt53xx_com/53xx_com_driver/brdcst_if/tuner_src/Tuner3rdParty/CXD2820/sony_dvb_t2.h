/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2009 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**

 @file    sony_dvb_t2.h

          This file provides DVB-T2 related type definitions.

*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_T2_H_
#define SONY_DVB_T2_H_

#include <sony_dvb.h>

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/

#define SONY_DVB_DVBT2_BW_5_MHZ							(5)     /**< DVBT2 BW 5MHZ in MHz. */
#define SONY_DVB_DVBT2_BW_6_MHZ							(6)     /**< DVBT2 BW 6MHZ in MHz. */
#define SONY_DVB_DVBT2_BW_7_MHZ							(7)     /**< DVBT2 BW 7MHZ in MHz. */
#define SONY_DVB_DVBT2_BW_8_MHZ							(8)     /**< DVBT2 BW 8MHZ in MHz. */

#define SONY_DVB_DVBT2_UNKNOWN_CELLID					(0)     /**< DVBT2 Unknown CellId */

/*------------------------------------------------------------------------------
 Globals
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Types
------------------------------------------------------------------------------*/

/**
 @brief DVBT2 S1 signalling definition.
*/
typedef enum {
    SONY_DVB_DVBT2_S1_SISO = 0x00,                              /**< DVBT2 SISO. */
    SONY_DVB_DVBT2_S1_MISO = 0x01,                              /**< DVBT2 MISO. */
    SONY_DVB_DVBT2_S1_NON_DVBT2 = 0x02,                         /**< Non DVBT2. */
	SONY_DVB_DVBT2_S1_RSVD1	= 0x03,	                            /**< Reserved. */
	SONY_DVB_DVBT2_S1_RSVD2	= 0x04,	                            /**< Reserved. */
	SONY_DVB_DVBT2_S1_RSVD3 = 0x05,	                            /**< Reserved. */
	SONY_DVB_DVBT2_S1_RSVD4	= 0x06,	                            /**< Reserved. */
	SONY_DVB_DVBT2_S1_RSVD5	= 0x07,	                            /**< Reserved. */
    SONY_DVB_DVBT2_S1_UNKNOWN = 0xFF                            /**< UNKNOWN DVBT2 MISO/SISO. */
} sony_dvb_dvbt2_s1_t;

/**
 @brief DVBT2 S2 Signalling definition.	
        Actual values are S2 >> 1.
*/
typedef enum {
    SONY_DVB_DVBT2_S2_M2K_G_ANY = 0x00,                         /**< DVBT2 2K mode with any guard. */
    SONY_DVB_DVBT2_S2_M8K_G_DVBT = 0x01,                        /**< DVBT2 8K mode with DVBT guard. */
    SONY_DVB_DVBT2_S2_M4K_G_ANY = 0x02,                         /**< DVBT2 4K mode with any guard. */
    SONY_DVB_DVBT2_S2_M1K_G_ANY = 0x03,                         /**< DVBT2 1K mode with any guard. */
    SONY_DVB_DVBT2_S2_M16K_G_ANY = 0x04,                        /**< DVBT2 16K mode with any guard. */
    SONY_DVB_DVBT2_S2_M32K_G_DVBT = 0x05,                       /**< DVBT2 32K mode with DVBT guard. */
    SONY_DVB_DVBT2_S2_M8K_G_DVBT2 = 0x06,                       /**< DVBT2 8K mode with DVBT2 guard. */
    SONY_DVB_DVBT2_S2_M32K_G_DVBT2 = 0x07,                      /**< DVBT2 32K mode with DVBT2 guard. */
    SONY_DVB_DVBT2_S2_UNKNOWN = 0xFF                            /**< UNKNOWN DVBT2 mode/guard. */
} sony_dvb_dvbt2_s2_t;

/**
 @brief DVBT2 guard intervals. 
        Same encoding as used for L1-pre signalling Guard Interval.
*/
typedef enum {
    SONY_DVB_DVBT2_G1_32 = 0x00,                                /**< DVBT2 1/32 GI. */
    SONY_DVB_DVBT2_G1_16 = 0x01,                                /**< DVBT2 1/16 GI. */
    SONY_DVB_DVBT2_G1_8 = 0x02,                                 /**< DVBT2 1/8 GI. */
    SONY_DVB_DVBT2_G1_4 = 0x03,                                 /**< DVBT2 1/4 GI. */
    SONY_DVB_DVBT2_G1_128 = 0x04,                               /**< DVBT2 1/128 GI. */
    SONY_DVB_DVBT2_G19_128 = 0x05,                              /**< DVBT2 19/128 GI. */
    SONY_DVB_DVBT2_G19_256 = 0x06,                              /**< DVBT2 19/256 GI. */
    SONY_DVB_DVBT2_G_RSVD1 = 0x07,                              /**< Reserved. */
    SONY_DVB_DVBT2_G_UNKNOWN = 0xFF                             /**< UNKNOWN DVBT2 GI. */
} sony_dvb_dvbt2_guard_t;

/**
 @brief The DVBT FFT modes.
*/
typedef enum {
    SONY_DVB_DVBT2_M2K = 0x00,                                  /**< DVBT2 2K mode. */
    SONY_DVB_DVBT2_M8K = 0x01,                                  /**< DVBT2 8K mode. */
    SONY_DVB_DVBT2_M4K = 0x02,                                  /**< DVBT2 4K mode. */
    SONY_DVB_DVBT2_M1K = 0x03,                                  /**< DVBT2 1K mode. */
    SONY_DVB_DVBT2_M16K = 0x04,                                 /**< DVBT2 16K mode. */
    SONY_DVB_DVBT2_M32K = 0x05,                                 /**< DVBT2 32K mode. */
    SONY_DVB_DVBT2_M_RSVD1 = 0x06,                              /**< Reserved. */
    SONY_DVB_DVBT2_M_RSVD2 = 0x07                               /**< Reserved. */
} sony_dvb_dvbt2_mode_t;

/**
 @brief The DVBT2 bandwidth.
*/
typedef enum {
    SONY_DVB_DVBT2_BW_8 = 0x00,                                 /**< DVBT2 8MHz. */
    SONY_DVB_DVBT2_BW_7 = 0x01,                                 /**< DVBT2 7MHz. */
    SONY_DVB_DVBT2_BW_6 = 0x02,                                 /**< DVBT2 6MHz. */
    SONY_DVB_DVBT2_BW_5 = 0x03,                                 /**< DVBT2 5MHz. */
    SONY_DVB_DVBT2_BW_10 = 0x04,                                /**< DVBT2 10MHz. Professional/Test Receivers Only */
    SONY_DVB_DVBT2_BW_1_7 = 0x05,                               /**< DVBT2 1.7MHz. */
    SONY_DVB_DVBT2_BW_RSVD1 = 0x06,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD2 = 0x07,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD3 = 0x08,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD4 = 0x09,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD5 = 0x0A,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD6 = 0x0B,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD7 = 0x0C,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD8 = 0x0D,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD9 = 0x0E,                             /**< Reserved. */
    SONY_DVB_DVBT2_BW_RSVD10 = 0x0F,                            /**< Reserved. */
    SONY_DVB_DVBT2_BW_UNKNOWN = 0xFF                            /**< DVBT2 Unknown Bandwidth. */
} sony_dvb_dvbt2_bw_t;

/**
 @brief The DVBT2 current T2 super-frame stream type.
*/
typedef enum {
    SONY_DVB_DVBT2_L1PRE_TYPE_TS = 0x00,                        /**< DVBT2 Streams: Transport Stream (TS) Only. */
    SONY_DVB_DVBT2_L1PRE_TYPE_GS = 0x01,                        /**< DVBT2 Streams: Generic Streams (GS) Only. */
    SONY_DVB_DVBT2_L1PRE_TYPE_TS_GS = 0x02                      /**< DVBT2 Streams: Mixed GS and TS. */
} sony_dvb_dvbt2_l1pre_type_t;

/**
 @brief The DVBT2 L1-pre PAPR (Peak to Average Power Ratio) indicator.
*/
typedef enum {
    SONY_DVB_DVBT2_PAPR_NONE = 0x00,                            /**< DVBT2 PAPR none. */
    SONY_DVB_DVBT2_PAPR_ACE = 0x01,                             /**< DVBT2 PAPR ACE used. */
    SONY_DVB_DVBT2_PAPR_TR = 0x02,                              /**< DVBT2 PAPR TR used. */
    SONY_DVB_DVBT2_PAPR_TR_ACE = 0x03,                          /**< DVBT2 PAPR TR and ACE used. */
    SONY_DVB_DVBT2_PAPR_RSVD1 = 0x04,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD2 = 0x05,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD3 = 0x06,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD4 = 0x07,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD5 = 0x08,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD6 = 0x09,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD7 = 0x0A,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD8 = 0x0B,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD9 = 0x0C,                           /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD10 = 0x0D,                          /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD11 = 0x0E,                          /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_RSVD12 = 0x0F,                          /**< Reseved. */
    SONY_DVB_DVBT2_PAPR_UNKNOWN = 0xFF                          /**< DVBT2 PAPR Unknown. */
} sony_dvb_dvbt2_papr_t;

/**
 @brief The DVBT2 L1-post constellation/modulation.
*/
typedef enum {
    SONY_DVB_DVBT2_L1POST_BPSK = 0x00,                          /**< DVBT2 L1-post uses BPSK. */
    SONY_DVB_DVBT2_L1POST_QPSK = 0x01,                          /**< DVBT2 L1-post uses QPSK. */
    SONY_DVB_DVBT2_L1POST_QAM16 = 0x02,                         /**< DVBT2 L1-post uses 16-QAM. */
    SONY_DVB_DVBT2_L1POST_QAM64 = 0x03,                         /**< DVBT2 L1-post uses 64-QAM. */
    SONY_DVB_DVBT2_L1POST_C_RSVD1 = 0x04,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD2 = 0x05,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD3 = 0x06,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD4 = 0x07,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD5 = 0x08,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD6 = 0x09,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD7 = 0x0A,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD8 = 0x0B,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD9 = 0x0C,                       /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD10 = 0x0D,                      /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD11 = 0x0E,                      /**< Reserved */
    SONY_DVB_DVBT2_L1POST_C_RSVD12 = 0x0F,                      /**< Reserved */
    SONY_DVB_DVBT2_L1POST_CONSTELL_UNKNOWN = 0xFF               /**< DVBT2 L1-post uses UNKNOWN. */
} sony_dvb_dvbt2_l1post_constell_t;

/**
 @brief The DVBT2 L1-post code rate.
*/
typedef enum {
    SONY_DVB_DVBT2_L1POST_R1_2 = 0x00,                          /**< DVBT2 L1-post uses 1/2 code rate. */
    SONY_DVB_DVBT2_L1POST_R_RSVD1 = 0x01,                       /**< Reserved  */
    SONY_DVB_DVBT2_L1POST_R_RSVD2 = 0x02,                       /**< Reserved  */
    SONY_DVB_DVBT2_L1POST_R_RSVD3 = 0x03,                       /**< Reserved  */
    SONY_DVB_DVBT2_L1POST_R_UNKNOWN = 0xFF                      /**< DVBT2 L1-post uses UNKNOWN code rate. */
} sony_dvb_dvbt2_l1post_cr_t;

/**
 @brief The L1-post FEC type.
*/
typedef enum {
    SONY_DVB_DVBT2_L1POST_FEC_LDPC16K = 0x00,                   /**< DVBT2 L1-post uses 16K LDPC FEC. */
    SONY_DVB_DVBT2_L1POST_FEC_RSVD1 = 0x01,                     /**< Reserved  */
    SONY_DVB_DVBT2_L1POST_FEC_RSVD2 = 0x02,                     /**< Reserved  */
    SONY_DVB_DVBT2_L1POST_FEC_RSVD3 = 0x03,                     /**< Reserved  */
    SONY_DVB_DVBT2_L1POST_FEC_UNKNOWN = 0xFF                    /**< DVBT2 L1-post uses UNKNOWN FEC. */
} sony_dvb_dvbt2_l1post_fec_type_t;

/**
 @brief The L1-pre pilot pattern signalling.
*/
typedef enum {
    SONY_DVB_DVBT2_PP1 = 0x00,                                  /**< DVBT2 pilot pattern 1. */
    SONY_DVB_DVBT2_PP2 = 0x01,                                  /**< DVBT2 pilot pattern 2. */
    SONY_DVB_DVBT2_PP3 = 0x02,                                  /**< DVBT2 pilot pattern 3. */
    SONY_DVB_DVBT2_PP4 = 0x03,                                  /**< DVBT2 pilot pattern 4. */
    SONY_DVB_DVBT2_PP5 = 0x04,                                  /**< DVBT2 pilot pattern 5. */
    SONY_DVB_DVBT2_PP6 = 0x05,                                  /**< DVBT2 pilot pattern 6. */
    SONY_DVB_DVBT2_PP7 = 0x06,                                  /**< DVBT2 pilot pattern 7. */
    SONY_DVB_DVBT2_PP8 = 0x07,                                  /**< DVBT2 pilot pattern 8. */
	SONY_DVB_DVBT2_PP_RSVD1 = 0x08,	                            /**< Reserved */
	SONY_DVB_DVBT2_PP_RSVD2 = 0x09,	                            /**< Reserved */
	SONY_DVB_DVBT2_PP_RSVD3 = 0x0A,	                            /**< Reserved */
	SONY_DVB_DVBT2_PP_RSVD4 = 0x0B,	                            /**< Reserved */
	SONY_DVB_DVBT2_PP_RSVD5 = 0x0C,	                            /**< Reserved */
	SONY_DVB_DVBT2_PP_RSVD6 = 0x0D,	                            /**< Reserved */
	SONY_DVB_DVBT2_PP_RSVD7 = 0x0E,	                            /**< Reserved */
	SONY_DVB_DVBT2_PP_RSVD8 = 0x0F,	                            /**< Reserved */
    SONY_DVB_DVBT2_PP_UNKNOWN = 0xFF                            /**< DVBT2 pilot pattern UNKNOWN. */
} sony_dvb_dvbt2_pp_t;

/**
 @brief L1-pre signalling information structure definition.
*/
typedef struct sony_dvb_dvbt2_l1pre_t {
    sony_dvb_dvbt2_l1pre_type_t type;                           /**< The stream type contained within the current T2 super-frame. */
    uint8 bwExt;                                              /**< BW extension indicator (8K/16K/32K only). */
    sony_dvb_dvbt2_s1_t s1;                                     /**< S1 signalling. P1 S1. */
    uint8 s2;                                                 /**< S2 signalling. P1 S2. */
    uint8 mixed ;                                             /**< Mixed indicator; 0: Not mixed, 1: Mixed (from S2 field). */
    sony_dvb_dvbt2_mode_t fftMode ;                             /**< FFT mode. (from S2) */
    uint8 l1Rep;                                              /**< L1 repeat enable flag. */
    sony_dvb_dvbt2_guard_t gi;                                  /**< The guard interval used for the current super-frame. */
    sony_dvb_dvbt2_papr_t papr;                                 /**< The PAPR indicator. */
    sony_dvb_dvbt2_l1post_constell_t mod;                       /**< The L1-post modulation in this frame. */
    sony_dvb_dvbt2_l1post_cr_t cr;                              /**< The L1-post code rate in this frame. */
    sony_dvb_dvbt2_l1post_fec_type_t fec;                       /**< The L1-post FEC type. */
    uint32 l1PostSize;                                        /**< Size of the L1-post in OFDM cells. */
    uint32 l1PostInfoSize;                                    /**< L1-post info size = L1-post configurable+dynamic+extension. */
    sony_dvb_dvbt2_pp_t pp;                                     /**< The pilot pattern for the OFDM symbols in this frame. */
    uint8 txIdAvailability;                                   /**< The TX Id. */
    uint16 cellId;                                            /**< The T2 cell Id. */
    uint16 networkId;                                         /**< The T2 network Id. */
    uint16 systemId;                                          /**< T2 system Id. */
    uint8 numFrames;                                          /**< Number of T2-frames per T2 super-frame. */
    uint16 numSymbols;                                        /**< Numnber of OFDM symbols per T2-frame. */
    uint8 regen;                                              /**< Regeneration count indicator. */
    uint8 postExt;                                            /**< L1-post extensions enabled. */
    uint8 numRfFreqs;                                         /**< The number of RF frequencies in use. */
    uint8 rfIdx;                                              /**< The current RF index. */
    uint16 resvd;                                             /**< Reserved. */
    uint32 crc32;                                             /**< CRC-32 of L1-pre data. */
} sony_dvb_dvbt2_l1pre_t;

/**
 @brief DVBT2 PLP code rate definition.
*/
typedef enum {
    SONY_DVB_DVBT2_R1_2 = 0x00,                                 /**< DVBT2 1/2 code rate. */
    SONY_DVB_DVBT2_R3_5 = 0x01,                                 /**< DVBT2 3/5 code rate. */
    SONY_DVB_DVBT2_R2_3 = 0x02,                                 /**< DVBT2 2/3 code rate. */
    SONY_DVB_DVBT2_R3_4 = 0x03,                                 /**< DVBT2 3/4 code rate. */
    SONY_DVB_DVBT2_R4_5 = 0x04,                                 /**< DVBT2 4/5 code rate. */
    SONY_DVB_DVBT2_R5_6 = 0x05,                                 /**< DVBT2 5/6 code rate. */
    SONY_DVB_DVBT2_R_RSVD1 = 0x06,                              /**< Reserved. */
    SONY_DVB_DVBT2_R_RSVD2 = 0x07,                              /**< Reserved. */
    SONY_DVB_DVBT2_PLP_CR_UNKNOWN = 0xFF                        /**< UNKNOWN DVBT2 PLP code rate. */
} sony_dvb_dvbt2_plp_code_rate_t;

/**
 @brief DVBT2 PLP constellation.
*/
typedef enum {
    SONY_DVB_DVBT2_QPSK = 0x00,                                 /**< DVBT2 QPSK. */
    SONY_DVB_DVBT2_QAM16 = 0x01,                                /**< DVBT2 QAM16. */
    SONY_DVB_DVBT2_QAM64 = 0x02,                                /**< DVBT2 QAM64. */
    SONY_DVB_DVBT2_QAM256 = 0x03,                               /**< DVBT2 QAM256. */
    SONY_DVB_DVBT2_CON_RSVD1 = 0x04,                            /**< Reserved */
    SONY_DVB_DVBT2_CON_RSVD2 = 0x05,                            /**< Reserved */
    SONY_DVB_DVBT2_CON_RSVD3 = 0x06,                            /**< Reserved */
    SONY_DVB_DVBT2_CON_RSVD4 = 0x07,                            /**< Reserved */
    SONY_DVB_DVBT2_CONSTELL_UNKNOWN = 0xFF                      /**< UNKNOWN DVBT2 constellation. */
} sony_dvb_dvbt2_plp_constell_t;

/**
 @brief DVBT2 PLP type definition.
*/
typedef enum {
    SONY_DVB_DVBT2_PLP_TYPE_COMMON = 0x00,                      /**< DVBT2 PLP Common Type. */
    SONY_DVB_DVBT2_PLP_TYPE_DATA1 = 0x01,                       /**< DVBT2 PLP Data Type 1. */
    SONY_DVB_DVBT2_PLP_TYPE_DATA2 = 0x02,                       /**< DVBT2 PLP Data Type 2. */
    SONY_DVB_DVBT2_PLP_TYPE_RSVD1 = 0x03,                       /**< Reserved */
    SONY_DVB_DVBT2_PLP_TYPE_RSVD2 = 0x04,                       /**< Reserved */
    SONY_DVB_DVBT2_PLP_TYPE_RSVD3 = 0x05,                       /**< Reserved */
    SONY_DVB_DVBT2_PLP_TYPE_RSVD4 = 0x06,                       /**< Reserved */
    SONY_DVB_DVBT2_PLP_TYPE_RSVD5 = 0x07,                       /**< Reserved */
    SONY_DVB_DVBT2_PLP_TYPE_UNKNOWN = 0xFF                      /**< DVBT2 PLP type UNKNOWN. */
} sony_dvb_dvbt2_plp_type_t;

/**
 @brief DVBT2 PLP payload type definition.
*/
typedef enum {
    SONY_DVB_DVBT2_PLP_PAYLOAD_GFPS = 0x00,                     /**< DVBT2 GFPS payload type. */
    SONY_DVB_DVBT2_PLP_PAYLOAD_GCS = 0x01,                      /**< DVBT2 GCS payload type. */
    SONY_DVB_DVBT2_PLP_PAYLOAD_GSE = 0x02,                      /**< DVBT2 GSE payload type. */
    SONY_DVB_DVBT2_PLP_PAYLOAD_TS = 0x03,                       /**< DVBT2 TS payload type. */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD1 = 0x04,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD2 = 0x05,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD3 = 0x06,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD4 = 0x07,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD5 = 0x08,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD6 = 0x09,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD7 = 0x0A,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD8 = 0x0B,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD9 = 0x0C,                    /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD10 = 0x0D,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD11 = 0x0E,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD12 = 0x0F,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD13 = 0x10,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD14 = 0x11,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD15 = 0x12,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD16 = 0x13,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD17 = 0x14,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD18 = 0x15,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD19 = 0x16,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD20 = 0x17,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD21 = 0x18,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD22 = 0x19,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD23 = 0x1A,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD24 = 0x1B,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD25 = 0x1C,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD26 = 0x1D,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD27 = 0x1E,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_RSVD28 = 0x1F,                   /**< Reserved */
    SONY_DVB_DVBT2_PLP_PAYLOAD_UNKNOWN = 0xFF                   /**< DVBT2 UNKNOWN payload type. */
} sony_dvb_dvbt2_plp_payload_t;

/**
 @brief The DVBT2 FEC type definition.
*/
typedef enum {
    SONY_DVB_DVBT2_FEC_LDPC_16K = 0x00,                         /**< DVBT2 LDPC 16K (Short FECFRAME). */
    SONY_DVB_DVBT2_FEC_LDPC_64K = 0x01,                         /**< DVBT2 LDPC 64K (Long FECFRAME). */
    SONY_DVB_DVBT2_FEC_RSVD1 = 0x02,                            /**< Reserved. */
    SONY_DVB_DVBT2_FEC_RSVD2 = 0x03,                            /**< Reserved. */
    SONY_DVB_DVBT2_FEC_UNKNOWN = 0xFF                           /**< DVBT2 FEC Type Unknown. */
} sony_dvb_dvbt2_plp_fec_t;

/**
 @brief DVBT2 basic types of PLP available.
*/
typedef enum {
    SONY_DVB_DVBT2_PLP_COMMON,                                  /**< DVBT2 basic PLP common type. */
    SONY_DVB_DVBT2_PLP_DATA                                     /**< DVBT2 basic PLP data type. */
} sony_dvb_dvbt2_plp_btype_t;

/**
 @brief DVBT2 PLP information structure definition.
*/
typedef struct sony_dvb_dvbt2_plp_t {

    /**
     @brief The PLP ID.
    */
    uint8 id;

    /**
     @brief The type of the PLP. 
    */
    sony_dvb_dvbt2_plp_type_t type;

    /**
     @brief The payload carried by the PLP. 
    */
    sony_dvb_dvbt2_plp_payload_t payload;

    /**
     @brief TFS not supported, no meaning. 
    */
    uint8 ff;

    /**
     @brief TFS not supported, no meaning.
    */
    uint8 firstRfIdx;

    /**
     @brief TFS not supported, no meaning.
    */
    uint8 firstFrmIdx;

    /**
     @brief The group of PLPs that this PLP belongs too.
    */
    uint8 groupId;

    /**
     @brief The constellation of this PLP. 
    */
    sony_dvb_dvbt2_plp_constell_t constell;

    /**
     @brief The code rate of this PLP.
    */
    sony_dvb_dvbt2_plp_code_rate_t plpCr;

    /**
     @brief DVBT2 rotated constellation indicator. 
    */
    uint8 rot;

    /**
     @brief The FEC type used on this PLP.
    */
    sony_dvb_dvbt2_plp_fec_t fec;

    /**
     @brief Maximum number of PLP blocks.
    */
    uint16 numBlocksMax;

    /**
     @brief The T2 frame interval within the superframe of this PLP. 
    */
    uint8 frmInt;

    /**
     @brief Time Interleaver length. 
    */
    uint8 tilLen;

    /**
     @brief Time Interleaver type indicator.
    */
    uint8 tilType;

    /**
     @brief In-band flag. Indicates whether PLP carries in-band signalling. 
    */
    uint8 inBandFlag;

    /**
     @brief Reserved field. 
    */
    uint16 rsvd;

} sony_dvb_dvbt2_plp_t;


/**
 @brief L1-post information excluding PLPs.
*/
typedef struct sony_dvb_dvbt2_l1post_t  {

    /**
     @brief The number of sub-slices per T2 frame.
    */
    uint16 subSlicesPerFrame ;

    /**
     @brief The number of PLPs in the current superframe.
            Minimum value of 1.
    */
    uint8 numPLPs ;

    /**
     @brief Number of auxillary streams.
    */
    uint8 numAux ;

    /**
     @brief Auxillary stream config (Reserved for Future Use)
    */
    uint8 auxConfigRFU ;

    /**
     @brief The RF index. 
            TFS only so not supported.
    */
    uint8 rfIdx ;    
    
    /**
     @brief The frequency in Hz whose index is rfIdx. 
            TFS only so not supported.
    */
    uint32 freq ;

    /**
     @brief Indicates the type of FEF part. Undefined.
            Only valid if S2 LSB is 1.
    */
    uint8 fefType ;

    /**
     @brief The length of the FEF as part of the elementary 
            period.
            Only valid if S2 LSB is 1.
    */
    uint32 fefLength ;
    
    /**
     @brief The number of T2-frames between two FEF parts. 
    */
    uint8 fefInterval ;

} sony_dvb_dvbt2_l1post_t ;

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

#endif /* SONY_DVB_T2_H_ */
