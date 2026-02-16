/*
 *
 * puma7_pmu.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2017 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */

/**************************************************************************/
/** \file   puma7_pmu.c
 *  \brief  PUMA7 PMU SRAM Control Driver.
 *
 *  \author     Intel
 **************************************************************************/


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs_macros.h>
#include <asm/uaccess.h>
#include <semaphore.h>
#include <puma7.h>
#include <puma7_pmu.h>

/* isspace */
#define ISSPACE(c)  ((c) == ' ' || ((c) >= '\t' && (c) <= '\r'))

/******************************************************************************************
 *   Power Management Unit (PMU) registers offset definitions 
 ******************************************************************************************/

#define    PPDISR            0x0       /* PMU Power Disable Request (not valid in CGM)  */
#define    PPDISS            0x4       /* PMU Power Disable Status(not valid in CGM)    */
#define    PMOCASPENB        0x8       /* PMU MOCA SRAM Power en_b                      */
#define    PMOCASPFWB        0xC       /* PMU MOCA SRAM Firewall                        */
#define    PMOCASPSTS        0x10      /* PMU MOCA SRAM Status                          */
#define    PCPUSPENB         0x14      /* PMU ARM11 SRAM Power en_b                     */
#define    PCPUSPFWB         0x18      /* PMU ARM11 SRAM Firewall                       */
#define    PCPUSPSTS         0x1C      /* PMU ARM11 SRAM Status                         */
#define    PDS12SPENB_1      0x20      /* PMU PHY DS12 1 SRAM Power en_b                */
#define    PDS12SPFWB_1      0x24      /* PMU PHY DS12 1 SRAM Firewall                  */
#define    PDS12SPSTS_1      0x28      /* PMU PHY DS12 1 SRAM Status                    */
#define    PDS12SPENB_2      0x2C      /* PMU PHY DS12 2 SRAM Power en_b                */
#define    PDS12SPFWB_2      0x30      /* PMU PHY DS12 2 SRAM Firewall                  */
#define    PDS12SPSTS_2      0x34      /* PMU PHY DS12 2 SRAM Status                    */
#define    PDS8SPENB         0x38      /* PMU PHY DS8 SRAM Power en_b                   */
#define    PDS8SPFWB         0x3C      /* PMU PHY DS8 SRAM Firewall                     */
#define    PDS8SPSTS         0x40      /* PMU PHY DS8 SRAM Status                       */
#define    PUSSPENB          0x44      /* PMU PHY US SRAM Power en_b                    */
#define    PUSSPFWB          0x48      /* PMU PHY US SRAM Firewall                      */
#define    PUSSPSTS          0x4C      /* PMU PHY US SRAM Status                        */
#define    POFDMGLBSPENB     0x50      /* PMU PHY OFDM GLOBAL SRAM Power en_b           */
#define    POFDMGLBSPFWB     0x54      /* PMU PHY OFDM GLOBAL SRAM Firewall             */
#define    POFDMGLBSPSTS     0x58      /* PMU PHY OFDM GLOBAL SRAM Status               */
#define    POFDMDEMOD0SPENB  0x5C      /* PMU PHY OFDM DEMOD0 SRAM Power en_b           */
#define    POFDMDEMOD0SPFWB  0x60      /* PMU PHY OFDM DEMOD0 SRAM Firewall             */
#define    POFDMDEMOD0SPSTS  0x64      /* PMU PHY OFDM DEMOD0 SRAM Status               */
#define    POFDMDEMOD1SPENB  0x68      /* PMU PHY OFDM DEMOD1 SRAM Power en_b           */
#define    POFDMDEMOD1SPFWB  0x6C      /* PMU PHY OFDM DEMOD1 SRAM Firewall             */
#define    POFDMDEMOD1SPSTS  0x70      /* PMU PHY OFDM DEMOD1 SRAM Status               */
#define    POFDMFEC0SPENB    0x74      /* PMU PHY OFDM FEC0 SRAM Power en_b             */
#define    POFDMFEC0SPFWB    0x78      /* PMU PHY OFDM FEC0 SRAM Firewall               */
#define    POFDMFEC0SPSTS    0x7C      /* PMU PHY OFDM FEC0 SRAM Status                 */
#define    POFDMFEC1SPENB    0x80      /* PMU PHY OFDM FEC1 SRAM Power en_b             */
#define    POFDMFEC1SPFWB    0x84      /* PMU PHY OFDM FEC1 SRAM Firewall               */
#define    POFDMFEC1SPSTS    0x88      /* PMU PHY OFDM FEC1 SRAM Status                 */
#define    POFDMUSSPENB      0x8C      /* PMU PHY OFDM US SRAM Power en_b               */
#define    POFDMUSSPFWB      0x90      /* PMU PHY OFDM US SRAM Firewall                 */
#define    POFDMUSSPSTS      0x94      /* PMU PHY OFDM US SRAM Status                   */
#define    PMACSPENBH        0x98      /* PMU MAC SRAM Power en_b High                  */
#define    PMACSPENBL        0x9C      /* PMU MAC SRAM Power en_b Low                   */
#define    PMACSPFWBH        0xA0      /* PMU MAC SRAM Firewall High                    */
#define    PMACSPFWBL        0xA4      /* PMU MAC SRAM Firewall Low                     */
#define    PMACSPSTSH        0xA8      /* PMU MAC SRAM Status High                      */
#define    PMACSPSTSL        0xAC      /* PMU MAC SRAM Status Low                       */
#define    PPPST1SPENBH      0xB0      /* PMU PP STAGE1 SRAM Power en_b High            */
#define    PPPST1SPENBL      0xB4      /* PMU PP STAGE1 SRAM Power en_b Low             */
#define    PPPST1SPFWBH      0xB8      /* PMU PP STAGE1 SRAM Firewall High              */
#define    PPPST1SPFWBL      0xBC      /* PMU PP STAGE1 SRAM Firewall Low               */
#define    PPPST1SPSTSH      0xC0      /* PMU PP STAGE1 SRAM Status High                */
#define    PPPST1SPSTSL      0xC4      /* PMU PP STAGE1 SRAM Status Low                 */
#define    PPPST2SPENBH      0xC8      /* PMU PP STAGE2 SRAM Power en_b High            */
#define    PPPST2SPENBL      0xCC      /* PMU PP STAGE2 SRAM Power en_b Low             */
#define    PPPST2SPFWBH      0xD0      /* PMU PP STAGE2 SRAM Firewall High              */
#define    PPPST2SPFWBL      0xD4      /* PMU PP STAGE2 SRAM Firewall Low               */
#define    PPPST2SPSTSH      0xD8      /* PMU PP STAGE2 SRAM Status High                */
#define    PPPST2SPSTSL      0xDC      /* PMU PP STAGE2 SRAM Status Low                 */


/* Registers bits definitions */
/*=============================/
/* PPDISR Register Bit Definitions */
/*  (not valid in CGM) 
*/
/* PPDISS Register Bit Definitions */
/*  (not valid in CGM) */


/* PMOCASPENB, PMOCASPFWB and PMOCASPSTS Registers Bit Definitions */
/* Description:*/
/*      For PMOCASPENB - 0: Power On    1: Power Off    */
/*      For PMOCASPFWB - 0: Isolation   1: No Isolation */
/*      For PMOCASPSTS - 0: Status On   1: Status Off   */
#define MOCA_MAC_SRAM_BIT                   0


/* PCPUSPENB, PCPUSPFWB and PCPUSPSTS Registers Bit Definitions */
/* Description:*/
/*      For PCPUSPENB - 0: Power On    1: Power Off    */
/*      For PCPUSPFWB - 0: Isolation   1: No Isolation */
/*      For PCPUSPSTS - 0: Status On   1: Status Off   */
#define ARM11_SRAM_BIT                      0


/* PDS12SPENB_1, PDS12SPFWB_1 and PDS12SPSTS_1 Registers Bit Definitions */
/* Description:*/
/*      For PDS12SPENB_1 - 0: Power On    1: Power Off    */
/*      For PDS12SPFWB_1 - 0: Isolation   1: No Isolation */
/*      For PDS12SPSTS_1 - 0: Status On   1: Status Off   */
#define PHY_DS1_CH1_SRAM_BIT                0
#define PHY_DS1_CH2_SRAM_BIT                1
#define PHY_DS1_CH3_SRAM_BIT                2
#define PHY_DS1_CH4_SRAM_BIT                3
#define PHY_DS1_CH5_SRAM_BIT                4
#define PHY_DS1_CH6_SRAM_BIT                5
#define PHY_DS1_CH7_SRAM_BIT                6
#define PHY_DS1_CH8_SRAM_BIT                7
#define PHY_DS1_CH9_SRAM_BIT                8
#define PHY_DS1_CH10_SRAM_BIT               9
#define PHY_DS1_CH11_SRAM_BIT               10
#define PHY_DS1_CH12_SRAM_BIT               11
#define PHY_DS1_MCR_SRAM_BIT                12


/* PDS22SPENB_2, PDS22SPFWB_2 and PDS22SPSTS_2 Registers Bit Definitions */
/* Description:*/
/*      For PDS22SPENB_2 - 0: Power On    1: Power Off    */
/*      For PDS22SPFWB_2 - 0: Isolation   1: No Isolation */
/*      For PDS22SPSTS_2 - 0: Status On   1: Status Off   */
#define PHY_DS2_CH1_SRAM_BIT                0
#define PHY_DS2_CH2_SRAM_BIT                1
#define PHY_DS2_CH3_SRAM_BIT                2
#define PHY_DS2_CH4_SRAM_BIT                3
#define PHY_DS2_CH5_SRAM_BIT                4
#define PHY_DS2_CH6_SRAM_BIT                5
#define PHY_DS2_CH7_SRAM_BIT                6
#define PHY_DS2_CH8_SRAM_BIT                7
#define PHY_DS2_CH9_SRAM_BIT                8
#define PHY_DS2_CH10_SRAM_BIT               9
#define PHY_DS2_CH11_SRAM_BIT               10
#define PHY_DS2_CH12_SRAM_BIT               11
#define PHY_DS2_MCR_SRAM_BIT                12


/* PDS8SPENB, PDS8SPFWB and PDS8SPSTS Registers Bit Definitions */
/* Description:*/
/*      For PDS8SPENB - 0: Power On    1: Power Off    */
/*      For PDS8SPFWB - 0: Isolation   1: No Isolation */
/*      For PDS8SPSTS - 0: Status On   1: Status Off   */
#define PHY_DS3_CH1_SRAM_BIT                0
#define PHY_DS3_CH2_SRAM_BIT                1
#define PHY_DS3_CH3_SRAM_BIT                2
#define PHY_DS3_CH4_SRAM_BIT                3
#define PHY_DS3_CH5_SRAM_BIT                4
#define PHY_DS3_CH6_SRAM_BIT                5
#define PHY_DS3_CH7_SRAM_BIT                6
#define PHY_DS3_CH8_SRAM_BIT                7
#define PHY_DS3_MCR_SRAM_BIT                8


/* PUSSPENB, PUSSPFWB and PUSSPSTS Registers Bit Definitions */
/* Description:*/
/*      For PUSSPENB - 0: Power On    1: Power Off    */
/*      For PUSSPFWB - 0: Isolation   1: No Isolation */
/*      For PUSSPSTS - 0: Status On   1: Status Off   */
#define PHY_US_CH1_SRAM_BIT                0
#define PHY_US_CH2_SRAM_BIT                1
#define PHY_US_CH3_SRAM_BIT                2
#define PHY_US_CH4_SRAM_BIT                3
#define PHY_US_CH5_SRAM_BIT                4
#define PHY_US_CH6_SRAM_BIT                5
#define PHY_US_CH7_SRAM_BIT                6
#define PHY_US_CH8_SRAM_BIT                7
#define PHY_US_MCR_SRAM_BIT                8


/* POFDMGLBSPENB, POFDMGLBSPFWB and POFDMGLBSPSTS Registers Bit Definitions */
/* Description:*/
/*      For POFDMGLBSPENB - 0: Power On    1: Power Off    */
/*      For POFDMGLBSPFWB - 0: Isolation   1: No Isolation */
/*      For POFDMGLBSPSTS - 0: Status On   1: Status Off   */
#define OFDM_GLOBAL_SRAM_BIT               0


/* POFDMDEMOD0SPENB, POFDMDEMOD0SPFWB and POFDMDEMOD0SPSTS Registers Bit Definitions */
/* Description:*/
/*      For PMACSPENBH - 0: Power On    1: Power Off    */
/*      For PMACSPENBH - 0: Isolation   1: No Isolation */
/*      For PMACSPSTSH - 0: Status On   1: Status Off   */
#define OFDM_DEMOD0_CONF_SRAM_BIT          0
#define OFDM_DEMOD0_OTHER_SRAM_BIT         1
#define OFDM_DEMOD0_INTR_SRAM_BIT          2
#define OFDM_DEMOD0_CMM_SRAM_BIT           3
#define OFDM_DEMOD0_MPNE_SRAM_BIT          4


/* POFDMDEMOD1SPENB, POFDMDEMOD1SPFWB and POFDMDEMOD1SPSTS Registers Bit Definitions */
/* Description:*/
/*      For PMACSPENBH - 0: Power On    1: Power Off    */
/*      For PMACSPENBH - 0: Isolation   1: No Isolation */
/*      For PMACSPSTSH - 0: Status On   1: Status Off   */
#define OFDM_DEMOD1_CONF_SRAM_BIT          0
#define OFDM_DEMOD1_OTHER_SRAM_BIT         1
#define OFDM_DEMOD1_INTR_SRAM_BIT          2
#define OFDM_DEMOD1_CMM_SRAM_BIT           3
#define OFDM_DEMOD1_MPNE_SRAM_BIT          4


/* POFDMFEC0SPENB, POFDMFEC0SPFWB and POFDMFEC0SPSTS Registers Bit Definitions */
/* Description:*/
/*      For PMACSPENBH - 0: Power On    1: Power Off    */
/*      For PMACSPENBH - 0: Isolation   1: No Isolation */
/*      For PMACSPSTSH - 0: Status On   1: Status Off   */
#define OFDM_FEC0_CONF_SRAM_BIT            0
#define OFDM_FEC0_NCP_SRAM_BIT             1
#define OFDM_FEC0_OTHER_SRAM_BIT           2


/* POFDMFEC1SPENB, POFDMFEC1SPFWB and POFDMFEC1SPSTS Registers Bit Definitions */
/* Description:*/
/*      For PMACSPENBH - 0: Power On    1: Power Off    */
/*      For PMACSPENBH - 0: Isolation   1: No Isolation */
/*      For PMACSPSTSH - 0: Status On   1: Status Off   */
#define OFDM_FEC1_CONF_SRAM_BIT            0
#define OFDM_FEC1_NCP_SRAM_BIT             1
#define OFDM_FEC1_OTHER_SRAM_BIT           2


/* POFDMUSSPENB, POFDMUSSPFWB and POFDMUSSPSTS Registers Bit Definitions */
/* Description:*/
/*      For PMACSPENBH - 0: Power On    1: Power Off    */
/*      For PMACSPENBH - 0: Isolation   1: No Isolation */
/*      For PMACSPSTSH - 0: Status On   1: Status Off   */
#define OFDMA_US_CONF_TRANS_0_SRAM_BIT     0
#define OFDMA_US_CONF_TRANS_1_SRAM_BIT     1
#define OFDMA_US_OTHER_TRANS_0_SRAM_BIT    2
#define OFDMA_US_OTHER_TRANS_1_SRAM_BIT    3
#define OFDMA_US_FRAMER_TRANS_0_SRAM_BIT   4
#define OFDMA_US_FRAMER_TRANS_1_SRAM_BIT   5


/* PMACSPENBH, PMACSPENBH and PMACSPSTSH Registers Bit Definitions */
/* Description:*/
/*      For PMACSPENBH - 0: Power On    1: Power Off    */
/*      For PMACSPENBH - 0: Isolation   1: No Isolation */
/*      For PMACSPSTSH - 0: Status On   1: Status Off   */
#define DOCSIS_MAC_SRAM_HIGH_0_BIT         0
#define DOCSIS_MAC_SRAM_HIGH_1_BIT         1
#define DOCSIS_MAC_SRAM_HIGH_2_BIT         2
#define DOCSIS_MAC_SRAM_HIGH_3_BIT         3
#define DOCSIS_MAC_SRAM_HIGH_4_BIT         4
#define DOCSIS_MAC_SRAM_HIGH_5_BIT         5
#define DOCSIS_MAC_SRAM_HIGH_6_BIT         6
#define DOCSIS_MAC_SRAM_HIGH_7_BIT         7
#define DOCSIS_MAC_SRAM_HIGH_8_BIT         8
#define DOCSIS_MAC_SRAM_HIGH_9_BIT         9
#define DOCSIS_MAC_SRAM_HIGH_10_BIT        10
#define DOCSIS_MAC_SRAM_HIGH_11_BIT        11
#define DOCSIS_MAC_SRAM_HIGH_12_BIT        12
#define DOCSIS_MAC_SRAM_HIGH_13_BIT        13
#define DOCSIS_MAC_SRAM_HIGH_14_BIT        14
#define DOCSIS_MAC_SRAM_HIGH_15_BIT        15
#define DOCSIS_MAC_SRAM_HIGH_16_BIT        16
#define DOCSIS_MAC_SRAM_HIGH_17_BIT        17
#define DOCSIS_MAC_SRAM_HIGH_18_BIT        18
#define DOCSIS_MAC_SRAM_HIGH_19_BIT        19
#define DOCSIS_MAC_SRAM_HIGH_20_BIT        20
#define DOCSIS_MAC_SRAM_HIGH_21_BIT        21
#define DOCSIS_MAC_SRAM_HIGH_22_BIT        22
#define DOCSIS_MAC_SRAM_HIGH_23_BIT        23
#define DOCSIS_MAC_SRAM_HIGH_24_BIT        24
#define DOCSIS_MAC_SRAM_HIGH_25_BIT        25
#define DOCSIS_MAC_SRAM_HIGH_26_BIT        26
#define DOCSIS_MAC_SRAM_HIGH_27_BIT        27
#define DOCSIS_MAC_SRAM_HIGH_28_BIT        28
#define DOCSIS_MAC_SRAM_HIGH_29_BIT        29
#define DOCSIS_MAC_SRAM_HIGH_30_BIT        30
#define DOCSIS_MAC_SRAM_HIGH_31_BIT        31


/* PMACSPFWBL, PMACSPSTSL and PMACSPSTSL Registers Bit Definitions */
/* Description:*/
/*      For PMACSPFWBL  - 0: Power On    1: Power Off    */
/*      For PMACSPENBL  - 0: Isolation   1: No Isolation */
/*      For PMACSPSTSL  - 0: Status On   1: Status Off   */
#define DOCSIS_MAC_SRAM_LOW_0_BIT         0
#define DOCSIS_MAC_SRAM_LOW_1_BIT         1
#define DOCSIS_MAC_SRAM_LOW_2_BIT         2
#define DOCSIS_MAC_SRAM_LOW_3_BIT         3
#define DOCSIS_MAC_SRAM_LOW_4_BIT         4
#define DOCSIS_MAC_SRAM_LOW_5_BIT         5
#define DOCSIS_MAC_SRAM_LOW_6_BIT         6
#define DOCSIS_MAC_SRAM_LOW_7_BIT         7
#define DOCSIS_MAC_SRAM_LOW_8_BIT         8
#define DOCSIS_MAC_SRAM_LOW_9_BIT         9
#define DOCSIS_MAC_SRAM_LOW_10_BIT        10
#define DOCSIS_MAC_SRAM_LOW_11_BIT        11
#define DOCSIS_MAC_SRAM_LOW_12_BIT        12
#define DOCSIS_MAC_SRAM_LOW_13_BIT        13
#define DOCSIS_MAC_SRAM_LOW_14_BIT        14
#define DOCSIS_MAC_SRAM_LOW_15_BIT        15
#define DOCSIS_MAC_SRAM_LOW_16_BIT        16
#define DOCSIS_MAC_SRAM_LOW_17_BIT        17
#define DOCSIS_MAC_SRAM_LOW_18_BIT        18
#define DOCSIS_MAC_SRAM_LOW_19_BIT        19
#define DOCSIS_MAC_SRAM_LOW_20_BIT        20
#define DOCSIS_MAC_SRAM_LOW_21_BIT        21
#define DOCSIS_MAC_SRAM_LOW_22_BIT        22
#define DOCSIS_MAC_SRAM_LOW_23_BIT        23
#define DOCSIS_MAC_SRAM_LOW_24_BIT        24
#define DOCSIS_MAC_SRAM_LOW_25_BIT        25
#define DOCSIS_MAC_SRAM_LOW_26_BIT        26
#define DOCSIS_MAC_SRAM_LOW_27_BIT        27
#define DOCSIS_MAC_SRAM_LOW_28_BIT        28
#define DOCSIS_MAC_SRAM_LOW_29_BIT        29
#define DOCSIS_MAC_SRAM_LOW_30_BIT        30
#define DOCSIS_MAC_SRAM_LOW_31_BIT        31


/* PPPST1SPENBH, PPPST1SPFWBH and PPPST1SPSTSH Registers Bit Definitions */
/* Description:*/
/*      For PPPST1SPENBH  - 0: Power On    1: Power Off    */
/*      For PPPST1SPFWBH  - 0: Isolation   1: No Isolation */
/*      For PPPST1SPSTSH  - 0: Status On   1: Status Off   */
#define PP_ST1_SRAM_HIGH_0_BIT         0
#define PP_ST1_SRAM_HIGH_1_BIT         1
#define PP_ST1_SRAM_HIGH_2_BIT         2
#define PP_ST1_SRAM_HIGH_3_BIT         3
#define PP_ST1_SRAM_HIGH_4_BIT         4
#define PP_ST1_SRAM_HIGH_5_BIT         5
#define PP_ST1_SRAM_HIGH_6_BIT         6
#define PP_ST1_SRAM_HIGH_7_BIT         7
#define PP_ST1_SRAM_HIGH_8_BIT         8
#define PP_ST1_SRAM_HIGH_9_BIT         9
#define PP_ST1_SRAM_HIGH_10_BIT        10
#define PP_ST1_SRAM_HIGH_11_BIT        11
#define PP_ST1_SRAM_HIGH_12_BIT        12
#define PP_ST1_SRAM_HIGH_13_BIT        13
#define PP_ST1_SRAM_HIGH_14_BIT        14
#define PP_ST1_SRAM_HIGH_15_BIT        15
#define PP_ST1_SRAM_HIGH_16_BIT        16
#define PP_ST1_SRAM_HIGH_17_BIT        17
#define PP_ST1_SRAM_HIGH_18_BIT        18
#define PP_ST1_SRAM_HIGH_19_BIT        19
#define PP_ST1_SRAM_HIGH_20_BIT        20
#define PP_ST1_SRAM_HIGH_21_BIT        21
#define PP_ST1_SRAM_HIGH_22_BIT        22
#define PP_ST1_SRAM_HIGH_23_BIT        23
#define PP_ST1_SRAM_HIGH_24_BIT        24
#define PP_ST1_SRAM_HIGH_25_BIT        25
#define PP_ST1_SRAM_HIGH_26_BIT        26
#define PP_ST1_SRAM_HIGH_27_BIT        27
#define PP_ST1_SRAM_HIGH_28_BIT        28
#define PP_ST1_SRAM_HIGH_29_BIT        29
#define PP_ST1_SRAM_HIGH_30_BIT        30
#define PP_ST1_SRAM_HIGH_31_BIT        31


/* PPPST1SPFWBL, PPPST1SPENBL and PPPST1SPSTSL Registers Bit Definitions */
/* Description:*/
/*      For  PPPST1SPFWBL  - 0: Power On    1: Power Off    */
/*      For  PPPST1SPENBL  - 0: Isolation   1: No Isolation */
/*      For  PPPST1SPSTSL  - 0: Status On   1: Status Off   */
#define PP_ST1_SRAM_LOW_0_BIT         0
#define PP_ST1_SRAM_LOW_1_BIT         1
#define PP_ST1_SRAM_LOW_2_BIT         2
#define PP_ST1_SRAM_LOW_3_BIT         3
#define PP_ST1_SRAM_LOW_4_BIT         4
#define PP_ST1_SRAM_LOW_5_BIT         5
#define PP_ST1_SRAM_LOW_6_BIT         6
#define PP_ST1_SRAM_LOW_7_BIT         7
#define PP_ST1_SRAM_LOW_8_BIT         8
#define PP_ST1_SRAM_LOW_9_BIT         9
#define PP_ST1_SRAM_LOW_10_BIT        10
#define PP_ST1_SRAM_LOW_11_BIT        11
#define PP_ST1_SRAM_LOW_12_BIT        12
#define PP_ST1_SRAM_LOW_13_BIT        13
#define PP_ST1_SRAM_LOW_14_BIT        14
#define PP_ST1_SRAM_LOW_15_BIT        15
#define PP_ST1_SRAM_LOW_16_BIT        16
#define PP_ST1_SRAM_LOW_17_BIT        17
#define PP_ST1_SRAM_LOW_18_BIT        18
#define PP_ST1_SRAM_LOW_19_BIT        19
#define PP_ST1_SRAM_LOW_20_BIT        20
#define PP_ST1_SRAM_LOW_21_BIT        21
#define PP_ST1_SRAM_LOW_22_BIT        22
#define PP_ST1_SRAM_LOW_23_BIT        23
#define PP_ST1_SRAM_LOW_24_BIT        24
#define PP_ST1_SRAM_LOW_25_BIT        25
#define PP_ST1_SRAM_LOW_26_BIT        26
#define PP_ST1_SRAM_LOW_27_BIT        27
#define PP_ST1_SRAM_LOW_28_BIT        28
#define PP_ST1_SRAM_LOW_29_BIT        29
#define PP_ST1_SRAM_LOW_30_BIT        30
#define PP_ST1_SRAM_LOW_31_BIT        31


/* PPPST2SPENBH, PPPST2SPFWBH and PPPST2SPSTSH Registers Bit Definitions */
/* Description:*/
/*      For  PPPST2SPENBH  - 0: Power On    1: Power Off    */
/*      For  PPPST2SPFWBH  - 0: Isolation   1: No Isolation */
/*      For  PPPST2SPSTSH  - 0: Status On   1: Status Off   */
#define PP_ST2_SRAM_HIGH_0_BIT         0
#define PP_ST2_SRAM_HIGH_1_BIT         1
#define PP_ST2_SRAM_HIGH_2_BIT         2
#define PP_ST2_SRAM_HIGH_3_BIT         3
#define PP_ST2_SRAM_HIGH_4_BIT         4
#define PP_ST2_SRAM_HIGH_5_BIT         5
#define PP_ST2_SRAM_HIGH_6_BIT         6
#define PP_ST2_SRAM_HIGH_7_BIT         7
#define PP_ST2_SRAM_HIGH_8_BIT         8
#define PP_ST2_SRAM_HIGH_9_BIT         9
#define PP_ST2_SRAM_HIGH_10_BIT        10
#define PP_ST2_SRAM_HIGH_11_BIT        11
#define PP_ST2_SRAM_HIGH_12_BIT        12
#define PP_ST2_SRAM_HIGH_13_BIT        13
#define PP_ST2_SRAM_HIGH_14_BIT        14
#define PP_ST2_SRAM_HIGH_15_BIT        15
#define PP_ST2_SRAM_HIGH_16_BIT        16
#define PP_ST2_SRAM_HIGH_17_BIT        17
#define PP_ST2_SRAM_HIGH_18_BIT        18
#define PP_ST2_SRAM_HIGH_19_BIT        19
#define PP_ST2_SRAM_HIGH_20_BIT        20
#define PP_ST2_SRAM_HIGH_21_BIT        21
#define PP_ST2_SRAM_HIGH_22_BIT        22
#define PP_ST2_SRAM_HIGH_23_BIT        23
#define PP_ST2_SRAM_HIGH_24_BIT        24
#define PP_ST2_SRAM_HIGH_25_BIT        25
#define PP_ST2_SRAM_HIGH_26_BIT        26
#define PP_ST2_SRAM_HIGH_27_BIT        27
#define PP_ST2_SRAM_HIGH_28_BIT        28
#define PP_ST2_SRAM_HIGH_29_BIT        29
#define PP_ST2_SRAM_HIGH_30_BIT        30
#define PP_ST2_SRAM_HIGH_31_BIT        31


/* PPPST2SPENBL, PPPST2SPFWBL and PPPST2SPSTSL Registers Bit Definitions */
/* Description:*/
/*      For PPPST2SPENBL - 0: Power On    1: Power Off    */
/*      For PPPST2SPFWBL - 0: Isolation   1: No Isolation */
/*      For PPPST2SPSTSL - 0: Status On   1: Status Off   */
#define PP_ST2_SRAM_LOW_0_BIT         0
#define PP_ST2_SRAM_LOW_1_BIT         1
#define PP_ST2_SRAM_LOW_2_BIT         2
#define PP_ST2_SRAM_LOW_3_BIT         3
#define PP_ST2_SRAM_LOW_4_BIT         4
#define PP_ST2_SRAM_LOW_5_BIT         5
#define PP_ST2_SRAM_LOW_6_BIT         6
#define PP_ST2_SRAM_LOW_7_BIT         7
#define PP_ST2_SRAM_LOW_8_BIT         8
#define PP_ST2_SRAM_LOW_9_BIT         9
#define PP_ST2_SRAM_LOW_10_BIT        10
#define PP_ST2_SRAM_LOW_11_BIT        11
#define PP_ST2_SRAM_LOW_12_BIT        12
#define PP_ST2_SRAM_LOW_13_BIT        13
#define PP_ST2_SRAM_LOW_14_BIT        14
#define PP_ST2_SRAM_LOW_15_BIT        15
#define PP_ST2_SRAM_LOW_16_BIT        16
#define PP_ST2_SRAM_LOW_17_BIT        17
#define PP_ST2_SRAM_LOW_18_BIT        18
#define PP_ST2_SRAM_LOW_19_BIT        19
#define PP_ST2_SRAM_LOW_20_BIT        20
#define PP_ST2_SRAM_LOW_21_BIT        21
#define PP_ST2_SRAM_LOW_22_BIT        22
#define PP_ST2_SRAM_LOW_23_BIT        23
#define PP_ST2_SRAM_LOW_24_BIT        24
#define PP_ST2_SRAM_LOW_25_BIT        25
#define PP_ST2_SRAM_LOW_26_BIT        26
#define PP_ST2_SRAM_LOW_27_BIT        27
#define PP_ST2_SRAM_LOW_28_BIT        28
#define PP_ST2_SRAM_LOW_29_BIT        29
#define PP_ST2_SRAM_LOW_30_BIT        30
#define PP_ST2_SRAM_LOW_31_BIT        31


/* SRAM Control operation macros */
#define PMU_SRAM_POWER_UP       1
#define PMU_SRAM_POWER_DOWN     0


/* Registers macros */
#define PMU_GET_BIT(reg, bit)            ((*((volatile unsigned int *)(AVALANCHE_PMU + (reg)))>>(bit))&0x00000001)
#define PMU_SET_BIT(reg, bit)            (*((volatile unsigned int *)(AVALANCHE_PMU + (reg))) = *((volatile unsigned int *)(AVALANCHE_PMU + (reg))) | (1 << (bit)))
#define PMU_CLR_BIT(reg, bit)            (*((volatile unsigned int *)(AVALANCHE_PMU + (reg))) = *((volatile unsigned int *)(AVALANCHE_PMU + (reg))) & ~(1 << (bit)))
#define PMU_GET_REG(reg)                 (*((volatile unsigned int *)(AVALANCHE_PMU + (reg))))


/* SRAM Name max size */
#define PMU_NAME_SIZE_MAX 32

/* Max length of proc name */
#define PMU_PROC_NAME_MAX   64

/* Proc write input buffer max size */
#define PMU_PROC_KBUF_MAX   16

/* SRAM Definition entry in SRAM Table */
typedef struct _pmu_sram_t
{ 
    char name[PMU_NAME_SIZE_MAX];       /* SRAM Name*/
    unsigned int enable_reg;            /* Power Enable Register offset */
    unsigned int firewall_reg;          /* Power Firewall Register offset */
    unsigned int status_reg;            /* Power Status Register offset */
    unsigned int bit;                   /* Bit offset */
} pmu_sram_t;


/* Global PMU register access semaphore */
static struct semaphore pmu_sem;

/* Global PMU SRAM Look Up Table.
   For each SRAM (enum) define Name, Regisers and bit definition
*/
static pmu_sram_t sram_table[PMU_SRAM_MAX] = 
{
/*  ============================================================================================================================================================ */
/*  | SRAM ID (Enum)                        | Name                          | Enable Reg      | Firewall Reg    | Status Reg      | Bit offset                  | */
/*  ============================================================================================================================================================ */
    [PMU_MOCA_MAC_SRAM_ID               ] = {"MoCA SRAM",                    PMOCASPENB,       PMOCASPFWB,       PMOCASPSTS,       MOCA_MAC_SRAM_BIT               },
    [PMU_ARM11_SRAM_ID                  ] = {"ARM11 SRAM",                   PCPUSPENB,        PCPUSPFWB,        PCPUSPSTS,        ARM11_SRAM_BIT                  },
    [PMU_PHY_DS1_CH1_SRAM_ID            ] = {"Down Stream 1 (Ch1) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH1_SRAM_BIT            },
    [PMU_PHY_DS1_CH2_SRAM_ID            ] = {"Down Stream 1 (Ch2) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH2_SRAM_BIT            },
    [PMU_PHY_DS1_CH3_SRAM_ID            ] = {"Down Stream 1 (Ch3) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH3_SRAM_BIT            },
    [PMU_PHY_DS1_CH4_SRAM_ID            ] = {"Down Stream 1 (Ch4) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH4_SRAM_BIT            },
    [PMU_PHY_DS1_CH5_SRAM_ID            ] = {"Down Stream 1 (Ch5) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH5_SRAM_BIT            },
    [PMU_PHY_DS1_CH6_SRAM_ID            ] = {"Down Stream 1 (Ch6) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH6_SRAM_BIT            },
    [PMU_PHY_DS1_CH7_SRAM_ID            ] = {"Down Stream 1 (Ch7) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH7_SRAM_BIT            },
    [PMU_PHY_DS1_CH8_SRAM_ID            ] = {"Down Stream 1 (Ch8) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH8_SRAM_BIT            },
    [PMU_PHY_DS1_CH9_SRAM_ID            ] = {"Down Stream 1 (Ch9) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH9_SRAM_BIT            },
    [PMU_PHY_DS1_CH10_SRAM_ID           ] = {"Down Stream 1 (Ch10) SRAM",    PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH10_SRAM_BIT           },
    [PMU_PHY_DS1_CH11_SRAM_ID           ] = {"Down Stream 1 (Ch11) SRAM",    PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH11_SRAM_BIT           },
    [PMU_PHY_DS1_CH12_SRAM_ID           ] = {"Down Stream 1 (Ch12) SRAM",    PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_CH12_SRAM_BIT           },
    [PMU_PHY_DS1_MCR_SRAM_ID            ] = {"Down Stream 1 (MCR) SRAM",     PDS12SPENB_1,     PDS12SPFWB_1,     PDS12SPSTS_1,     PHY_DS1_MCR_SRAM_BIT            },
    [PMU_PHY_DS2_CH1_SRAM_ID            ] = {"Down Stream 2 (Ch1) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH1_SRAM_BIT            },
    [PMU_PHY_DS2_CH2_SRAM_ID            ] = {"Down Stream 2 (Ch2) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH2_SRAM_BIT            },
    [PMU_PHY_DS2_CH3_SRAM_ID            ] = {"Down Stream 2 (Ch3) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH3_SRAM_BIT            },
    [PMU_PHY_DS2_CH4_SRAM_ID            ] = {"Down Stream 2 (Ch4) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH4_SRAM_BIT            },
    [PMU_PHY_DS2_CH5_SRAM_ID            ] = {"Down Stream 2 (Ch5) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH5_SRAM_BIT            },
    [PMU_PHY_DS2_CH6_SRAM_ID            ] = {"Down Stream 2 (Ch6) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH6_SRAM_BIT            },
    [PMU_PHY_DS2_CH7_SRAM_ID            ] = {"Down Stream 2 (Ch7) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH7_SRAM_BIT            },
    [PMU_PHY_DS2_CH8_SRAM_ID            ] = {"Down Stream 2 (Ch8) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH8_SRAM_BIT            },
    [PMU_PHY_DS2_CH9_SRAM_ID            ] = {"Down Stream 2 (Ch9) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH9_SRAM_BIT            },
    [PMU_PHY_DS2_CH10_SRAM_ID           ] = {"Down Stream 2 (Ch10) SRAM",    PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH10_SRAM_BIT           },
    [PMU_PHY_DS2_CH11_SRAM_ID           ] = {"Down Stream 2 (Ch11) SRAM",    PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH11_SRAM_BIT           },
    [PMU_PHY_DS2_CH12_SRAM_ID           ] = {"Down Stream 2 (Ch12) SRAM",    PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_CH12_SRAM_BIT           },
    [PMU_PHY_DS2_MCR_SRAM_ID            ] = {"Down Stream 2 (MCR) SRAM",     PDS12SPENB_2,     PDS12SPFWB_2,     PDS12SPSTS_2,     PHY_DS2_MCR_SRAM_BIT            },
    [PMU_PHY_DS3_CH1_SRAM_ID            ] = {"Down Stream 3 (Ch1) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH1_SRAM_BIT            },
    [PMU_PHY_DS3_CH2_SRAM_ID            ] = {"Down Stream 3 (Ch2) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH2_SRAM_BIT            },
    [PMU_PHY_DS3_CH3_SRAM_ID            ] = {"Down Stream 3 (Ch3) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH3_SRAM_BIT            },
    [PMU_PHY_DS3_CH4_SRAM_ID            ] = {"Down Stream 3 (Ch4) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH4_SRAM_BIT            },
    [PMU_PHY_DS3_CH5_SRAM_ID            ] = {"Down Stream 3 (Ch5) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH5_SRAM_BIT            },
    [PMU_PHY_DS3_CH6_SRAM_ID            ] = {"Down Stream 3 (Ch6) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH6_SRAM_BIT            },
    [PMU_PHY_DS3_CH7_SRAM_ID            ] = {"Down Stream 3 (Ch7) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH7_SRAM_BIT            },
    [PMU_PHY_DS3_CH8_SRAM_ID            ] = {"Down Stream 3 (Ch8) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_CH8_SRAM_BIT            },
    [PMU_PHY_DS3_MCR_SRAM_ID            ] = {"Down Stream 3 (MCR) SRAM",     PDS8SPENB,        PDS8SPFWB,        PDS8SPSTS,        PHY_DS3_MCR_SRAM_BIT            },
    [PMU_PHY_US_CH1_SRAM_ID             ] = {"Up Stream (Ch1) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH1_SRAM_BIT             },
    [PMU_PHY_US_CH2_SRAM_ID             ] = {"Up Stream (Ch2) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH2_SRAM_BIT             },
    [PMU_PHY_US_CH3_SRAM_ID             ] = {"Up Stream (Ch3) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH3_SRAM_BIT             },
    [PMU_PHY_US_CH4_SRAM_ID             ] = {"Up Stream (Ch4) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH4_SRAM_BIT             },
    [PMU_PHY_US_CH5_SRAM_ID             ] = {"Up Stream (Ch5) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH5_SRAM_BIT             },
    [PMU_PHY_US_CH6_SRAM_ID             ] = {"Up Stream (Ch6) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH6_SRAM_BIT             },
    [PMU_PHY_US_CH7_SRAM_ID             ] = {"Up Stream (Ch7) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH7_SRAM_BIT             },
    [PMU_PHY_US_CH8_SRAM_ID             ] = {"Up Stream (Ch8) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_CH8_SRAM_BIT             },
    [PMU_PHY_US_MCR_SRAM_ID             ] = {"Up Stream (MCR) SRAM",         PUSSPENB,         PUSSPFWB,         PUSSPSTS,         PHY_US_MCR_SRAM_BIT             },
    [PMU_OFDM_GLOBAL_SRAM_ID            ] = {"OFDM GLOBAL SRAM",             POFDMGLBSPENB,    POFDMGLBSPFWB,    POFDMGLBSPSTS,    OFDM_GLOBAL_SRAM_BIT            },
    [PMU_OFDM_DEMOD0_CONF_SRAM_ID       ] = {"OFDM DEMOD 0 (Conf) SRAM",     POFDMDEMOD0SPENB, POFDMDEMOD0SPFWB, POFDMDEMOD0SPSTS, OFDM_DEMOD0_CONF_SRAM_BIT       },
    [PMU_OFDM_DEMOD0_OTHER_SRAM_ID      ] = {"OFDM DEMOD 0 (Other) SRAM",    POFDMDEMOD0SPENB, POFDMDEMOD0SPFWB, POFDMDEMOD0SPSTS, OFDM_DEMOD0_OTHER_SRAM_BIT      },
    [PMU_OFDM_DEMOD0_INTR_SRAM_ID       ] = {"OFDM DEMOD 0 (Inter) SRAM",    POFDMDEMOD0SPENB, POFDMDEMOD0SPFWB, POFDMDEMOD0SPSTS, OFDM_DEMOD0_INTR_SRAM_BIT       },
    [PMU_OFDM_DEMOD0_CMM_SRAM_ID        ] = {"OFDM DEMOD 0 (CMM) SRAM",      POFDMDEMOD0SPENB, POFDMDEMOD0SPFWB, POFDMDEMOD0SPSTS, OFDM_DEMOD0_CMM_SRAM_BIT        },
    [PMU_OFDM_DEMOD0_MPNE_SRAM_ID       ] = {"OFDM DEMOD 0 (NPNE) SRAM",     POFDMDEMOD0SPENB, POFDMDEMOD0SPFWB, POFDMDEMOD0SPSTS, OFDM_DEMOD0_MPNE_SRAM_BIT       },
    [PMU_OFDM_DEMOD1_CONF_SRAM_ID       ] = {"OFDM DEMOD 1 (Conf) SRAM",     POFDMDEMOD1SPENB, POFDMDEMOD1SPFWB, POFDMDEMOD1SPSTS, OFDM_DEMOD1_CONF_SRAM_BIT       },
    [PMU_OFDM_DEMOD1_OTHER_SRAM_ID      ] = {"OFDM DEMOD 1 (Other) SRAM",    POFDMDEMOD1SPENB, POFDMDEMOD1SPFWB, POFDMDEMOD1SPSTS, OFDM_DEMOD1_OTHER_SRAM_BIT      },
    [PMU_OFDM_DEMOD1_INTR_SRAM_ID       ] = {"OFDM DEMOD 1 (Inter) SRAM",    POFDMDEMOD1SPENB, POFDMDEMOD1SPFWB, POFDMDEMOD1SPSTS, OFDM_DEMOD1_INTR_SRAM_BIT       },
    [PMU_OFDM_DEMOD1_CMM_SRAM_ID        ] = {"OFDM DEMOD 1 (CMM) SRAM",      POFDMDEMOD1SPENB, POFDMDEMOD1SPFWB, POFDMDEMOD1SPSTS, OFDM_DEMOD1_CMM_SRAM_BIT        },
    [PMU_OFDM_DEMOD1_MPNE_SRAM_ID       ] = {"OFDM DEMOD 1 (NPNE) SRAM",     POFDMDEMOD1SPENB, POFDMDEMOD1SPFWB, POFDMDEMOD1SPSTS, OFDM_DEMOD1_MPNE_SRAM_BIT       },
    [PMU_OFDM_FEC0_CONF_SRAM_ID         ] = {"OFDM FEC 0 SRAM (Conf)",       POFDMFEC0SPENB,   POFDMFEC0SPFWB,   POFDMFEC0SPSTS,   OFDM_FEC0_CONF_SRAM_BIT         },
    [PMU_OFDM_FEC0_NCP_SRAM_ID          ] = {"OFDM FEC 0 SRAM (Ncp)",        POFDMFEC0SPENB,   POFDMFEC0SPFWB,   POFDMFEC0SPSTS,   OFDM_FEC0_NCP_SRAM_BIT          },
    [PMU_OFDM_FEC0_OTHER_SRAM_ID        ] = {"OFDM FEC 0 SRAM (Other)",      POFDMFEC0SPENB,   POFDMFEC0SPFWB,   POFDMFEC0SPSTS,   OFDM_FEC0_OTHER_SRAM_BIT        },
    [PMU_OFDM_FEC1_CONF_SRAM_ID         ] = {"OFDM FEC 1 SRAM (Conf)",       POFDMFEC1SPENB,   POFDMFEC1SPFWB,   POFDMFEC1SPSTS,   OFDM_FEC1_CONF_SRAM_BIT         },
    [PMU_OFDM_FEC1_NCP_SRAM_ID          ] = {"OFDM FEC 1 SRAM (Ncp)",        POFDMFEC1SPENB,   POFDMFEC1SPFWB,   POFDMFEC1SPSTS,   OFDM_FEC1_NCP_SRAM_BIT          },
    [PMU_OFDM_FEC1_OTHER_SRAM_ID        ] = {"OFDM FEC 1 SRAM (Other)",      POFDMFEC1SPENB,   POFDMFEC1SPFWB,   POFDMFEC1SPSTS,   OFDM_FEC1_OTHER_SRAM_BIT        },
    [PMU_OFDMA_US_CONF_TRANS_0_SRAM_ID  ] = {"OFDM US Trans 0 (Conf) SRAM",  POFDMUSSPENB,     POFDMUSSPFWB,     POFDMUSSPSTS,     OFDMA_US_CONF_TRANS_0_SRAM_BIT  },
    [PMU_OFDMA_US_OTHER_TRANS_0_SRAM_ID ] = {"OFDM US Trans 0 (Other) SRAM", POFDMUSSPENB,     POFDMUSSPFWB,     POFDMUSSPSTS,     OFDMA_US_OTHER_TRANS_0_SRAM_BIT },
    [PMU_OFDMA_US_FRAMER_TRANS_0_SRAM_ID] = {"OFDM US Trans 1 (Framer) SRAM",POFDMUSSPENB,     POFDMUSSPFWB,     POFDMUSSPSTS,     OFDMA_US_FRAMER_TRANS_0_SRAM_BIT},
    [PMU_OFDMA_US_CONF_TRANS_1_SRAM_ID  ] = {"OFDM US Trans 1 (Conf) SRAM",  POFDMUSSPENB,     POFDMUSSPFWB,     POFDMUSSPSTS,     OFDMA_US_CONF_TRANS_1_SRAM_BIT  },
    [PMU_OFDMA_US_OTHER_TRANS_1_SRAM_ID ] = {"OFDM US Trans 1 (Other) SRAM", POFDMUSSPENB,     POFDMUSSPFWB,     POFDMUSSPSTS,     OFDMA_US_OTHER_TRANS_1_SRAM_BIT },
    [PMU_OFDMA_US_FRAMER_TRANS_1_SRAM_ID] = {"OFDM US Trans 1 (Framer) SRAM",POFDMUSSPENB,     POFDMUSSPFWB,     POFDMUSSPSTS,     OFDMA_US_FRAMER_TRANS_1_SRAM_BIT},
    [PMU_DOCSIS_MAC_SRAM_HIGH_0_ID      ] = {"DOCSIS MAC High 0 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_0_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_1_ID      ] = {"DOCSIS MAC High 1 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_1_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_2_ID      ] = {"DOCSIS MAC High 2 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_2_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_3_ID      ] = {"DOCSIS MAC High 3 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_3_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_4_ID      ] = {"DOCSIS MAC High 4 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_4_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_5_ID      ] = {"DOCSIS MAC High 5 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_5_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_6_ID      ] = {"DOCSIS MAC High 6 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_6_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_7_ID      ] = {"DOCSIS MAC High 7 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_7_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_8_ID      ] = {"DOCSIS MAC High 8 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_8_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_9_ID      ] = {"DOCSIS MAC High 9 SRAM ",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_9_BIT      },
    [PMU_DOCSIS_MAC_SRAM_HIGH_10_ID     ] = {"DOCSIS MAC High 10 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_10_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_11_ID     ] = {"DOCSIS MAC High 11 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_11_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_12_ID     ] = {"DOCSIS MAC High 12 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_12_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_13_ID     ] = {"DOCSIS MAC High 13 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_13_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_14_ID     ] = {"DOCSIS MAC High 14 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_14_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_15_ID     ] = {"DOCSIS MAC High 15 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_15_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_16_ID     ] = {"DOCSIS MAC High 16 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_16_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_17_ID     ] = {"DOCSIS MAC High 17 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_17_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_18_ID     ] = {"DOCSIS MAC High 18 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_18_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_19_ID     ] = {"DOCSIS MAC High 19 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_19_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_20_ID     ] = {"DOCSIS MAC High 20 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_20_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_21_ID     ] = {"DOCSIS MAC High 21 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_21_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_22_ID     ] = {"DOCSIS MAC High 22 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_22_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_23_ID     ] = {"DOCSIS MAC High 23 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_23_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_24_ID     ] = {"DOCSIS MAC High 24 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_24_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_25_ID     ] = {"DOCSIS MAC High 25 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_25_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_26_ID     ] = {"DOCSIS MAC High 26 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_26_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_27_ID     ] = {"DOCSIS MAC High 27 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_27_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_28_ID     ] = {"DOCSIS MAC High 28 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_28_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_29_ID     ] = {"DOCSIS MAC High 29 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_29_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_30_ID     ] = {"DOCSIS MAC High 30 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_30_BIT     },
    [PMU_DOCSIS_MAC_SRAM_HIGH_31_ID     ] = {"DOCSIS MAC High 31 SRAM",      PMACSPENBH,       PMACSPFWBH,       PMACSPSTSH,       DOCSIS_MAC_SRAM_HIGH_31_BIT     },
    [PMU_DOCSIS_MAC_SRAM_LOW_0_ID       ] = {"DOCSIS MAC Low 0 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_0_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_1_ID       ] = {"DOCSIS MAC Low 1 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_1_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_2_ID       ] = {"DOCSIS MAC Low 2 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_2_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_3_ID       ] = {"DOCSIS MAC Low 3 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_3_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_4_ID       ] = {"DOCSIS MAC Low 4 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_4_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_5_ID       ] = {"DOCSIS MAC Low 5 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_5_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_6_ID       ] = {"DOCSIS MAC Low 6 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_6_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_7_ID       ] = {"DOCSIS MAC Low 7 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_7_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_8_ID       ] = {"DOCSIS MAC Low 8 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_8_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_9_ID       ] = {"DOCSIS MAC Low 9 SRAM",        PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_9_BIT       },
    [PMU_DOCSIS_MAC_SRAM_LOW_10_ID      ] = {"DOCSIS MAC Low 10 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_10_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_11_ID      ] = {"DOCSIS MAC Low 11 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_11_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_12_ID      ] = {"DOCSIS MAC Low 12 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_12_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_13_ID      ] = {"DOCSIS MAC Low 13 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_13_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_14_ID      ] = {"DOCSIS MAC Low 14 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_14_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_15_ID      ] = {"DOCSIS MAC Low 15 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_15_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_16_ID      ] = {"DOCSIS MAC Low 16 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_16_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_17_ID      ] = {"DOCSIS MAC Low 17 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_17_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_18_ID      ] = {"DOCSIS MAC Low 18 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_18_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_19_ID      ] = {"DOCSIS MAC Low 19 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_19_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_20_ID      ] = {"DOCSIS MAC Low 20 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_20_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_21_ID      ] = {"DOCSIS MAC Low 21 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_21_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_22_ID      ] = {"DOCSIS MAC Low 22 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_22_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_23_ID      ] = {"DOCSIS MAC Low 23 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_23_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_24_ID      ] = {"DOCSIS MAC Low 24 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_24_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_25_ID      ] = {"DOCSIS MAC Low 25 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_25_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_26_ID      ] = {"DOCSIS MAC Low 26 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_26_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_27_ID      ] = {"DOCSIS MAC Low 27 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_27_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_28_ID      ] = {"DOCSIS MAC Low 28 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_28_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_29_ID      ] = {"DOCSIS MAC Low 29 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_29_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_30_ID      ] = {"DOCSIS MAC Low 30 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_30_BIT      },
    [PMU_DOCSIS_MAC_SRAM_LOW_31_ID      ] = {"DOCSIS MAC Low 31 SRAM",       PMACSPENBL,       PMACSPFWBL,       PMACSPSTSL,       DOCSIS_MAC_SRAM_LOW_31_BIT      },
    [PMU_PP_ST1_SRAM_HIGH_0_ID          ] = {"PP Stage1 High 0 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_0_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_1_ID          ] = {"PP Stage1 High 1 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_1_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_2_ID          ] = {"PP Stage1 High 2 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_2_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_3_ID          ] = {"PP Stage1 High 3 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_3_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_4_ID          ] = {"PP Stage1 High 4 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_4_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_5_ID          ] = {"PP Stage1 High 5 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_5_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_6_ID          ] = {"PP Stage1 High 6 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_6_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_7_ID          ] = {"PP Stage1 High 7 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_7_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_8_ID          ] = {"PP Stage1 High 8 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_8_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_9_ID          ] = {"PP Stage1 High 9 SRAM",        PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_9_BIT          },
    [PMU_PP_ST1_SRAM_HIGH_10_ID         ] = {"PP Stage1 High 10 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_10_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_11_ID         ] = {"PP Stage1 High 11 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_11_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_12_ID         ] = {"PP Stage1 High 12 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_12_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_13_ID         ] = {"PP Stage1 High 13 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_13_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_14_ID         ] = {"PP Stage1 High 14 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_14_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_15_ID         ] = {"PP Stage1 High 15 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_15_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_16_ID         ] = {"PP Stage1 High 16 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_16_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_17_ID         ] = {"PP Stage1 High 17 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_17_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_18_ID         ] = {"PP Stage1 High 18 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_18_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_19_ID         ] = {"PP Stage1 High 19 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_19_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_20_ID         ] = {"PP Stage1 High 20 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_20_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_21_ID         ] = {"PP Stage1 High 21 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_21_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_22_ID         ] = {"PP Stage1 High 22 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_22_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_23_ID         ] = {"PP Stage1 High 23 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_23_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_24_ID         ] = {"PP Stage1 High 24 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_24_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_25_ID         ] = {"PP Stage1 High 25 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_25_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_26_ID         ] = {"PP Stage1 High 26 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_26_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_27_ID         ] = {"PP Stage1 High 27 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_27_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_28_ID         ] = {"PP Stage1 High 28 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_28_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_29_ID         ] = {"PP Stage1 High 29 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_29_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_30_ID         ] = {"PP Stage1 High 30 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_30_BIT         },
    [PMU_PP_ST1_SRAM_HIGH_31_ID         ] = {"PP Stage1 High 31 SRAM",       PPPST1SPENBH,     PPPST1SPFWBH,     PPPST1SPSTSH,     PP_ST1_SRAM_HIGH_31_BIT         },
    [PMU_PP_ST1_SRAM_LOW_0_ID           ] = {"PP Stage1 Low 0 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_0_BIT           },
    [PMU_PP_ST1_SRAM_LOW_1_ID           ] = {"PP Stage1 Low 1 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_1_BIT           },
    [PMU_PP_ST1_SRAM_LOW_2_ID           ] = {"PP Stage1 Low 2 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_2_BIT           },
    [PMU_PP_ST1_SRAM_LOW_3_ID           ] = {"PP Stage1 Low 3 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_3_BIT           },
    [PMU_PP_ST1_SRAM_LOW_4_ID           ] = {"PP Stage1 Low 4 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_4_BIT           },
    [PMU_PP_ST1_SRAM_LOW_5_ID           ] = {"PP Stage1 Low 5 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_5_BIT           },
    [PMU_PP_ST1_SRAM_LOW_6_ID           ] = {"PP Stage1 Low 6 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_6_BIT           },
    [PMU_PP_ST1_SRAM_LOW_7_ID           ] = {"PP Stage1 Low 7 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_7_BIT           },
    [PMU_PP_ST1_SRAM_LOW_8_ID           ] = {"PP Stage1 Low 8 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_8_BIT           },
    [PMU_PP_ST1_SRAM_LOW_9_ID           ] = {"PP Stage1 Low 9 SRAM",         PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_9_BIT           },
    [PMU_PP_ST1_SRAM_LOW_10_ID          ] = {"PP Stage1 Low 10 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_10_BIT          },
    [PMU_PP_ST1_SRAM_LOW_11_ID          ] = {"PP Stage1 Low 11 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_11_BIT          },
    [PMU_PP_ST1_SRAM_LOW_12_ID          ] = {"PP Stage1 Low 12 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_12_BIT          },
    [PMU_PP_ST1_SRAM_LOW_13_ID          ] = {"PP Stage1 Low 13 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_13_BIT          },
    [PMU_PP_ST1_SRAM_LOW_14_ID          ] = {"PP Stage1 Low 14 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_14_BIT          },
    [PMU_PP_ST1_SRAM_LOW_15_ID          ] = {"PP Stage1 Low 15 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_15_BIT          },
    [PMU_PP_ST1_SRAM_LOW_16_ID          ] = {"PP Stage1 Low 16 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_16_BIT          },
    [PMU_PP_ST1_SRAM_LOW_17_ID          ] = {"PP Stage1 Low 17 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_17_BIT          },
    [PMU_PP_ST1_SRAM_LOW_18_ID          ] = {"PP Stage1 Low 18 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_18_BIT          },
    [PMU_PP_ST1_SRAM_LOW_19_ID          ] = {"PP Stage1 Low 19 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_19_BIT          },
    [PMU_PP_ST1_SRAM_LOW_20_ID          ] = {"PP Stage1 Low 20 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_20_BIT          },
    [PMU_PP_ST1_SRAM_LOW_21_ID          ] = {"PP Stage1 Low 21 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_21_BIT          },
    [PMU_PP_ST1_SRAM_LOW_22_ID          ] = {"PP Stage1 Low 22 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_22_BIT          },
    [PMU_PP_ST1_SRAM_LOW_23_ID          ] = {"PP Stage1 Low 23 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_23_BIT          },
    [PMU_PP_ST1_SRAM_LOW_24_ID          ] = {"PP Stage1 Low 24 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_24_BIT          },
    [PMU_PP_ST1_SRAM_LOW_25_ID          ] = {"PP Stage1 Low 25 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_25_BIT          },
    [PMU_PP_ST1_SRAM_LOW_26_ID          ] = {"PP Stage1 Low 26 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_26_BIT          },
    [PMU_PP_ST1_SRAM_LOW_27_ID          ] = {"PP Stage1 Low 27 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_27_BIT          },
    [PMU_PP_ST1_SRAM_LOW_28_ID          ] = {"PP Stage1 Low 28 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_28_BIT          },
    [PMU_PP_ST1_SRAM_LOW_29_ID          ] = {"PP Stage1 Low 29 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_29_BIT          },
    [PMU_PP_ST1_SRAM_LOW_30_ID          ] = {"PP Stage1 Low 30 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_30_BIT          },
    [PMU_PP_ST1_SRAM_LOW_31_ID          ] = {"PP Stage1 Low 31 SRAM",        PPPST1SPENBL,     PPPST1SPFWBL,     PPPST1SPSTSL,     PP_ST1_SRAM_LOW_31_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_0_ID          ] = {"PP Stage2 High 0 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_0_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_1_ID          ] = {"PP Stage2 High 1 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_1_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_2_ID          ] = {"PP Stage2 High 2 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_2_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_3_ID          ] = {"PP Stage2 High 3 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_3_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_4_ID          ] = {"PP Stage2 High 4 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_4_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_5_ID          ] = {"PP Stage2 High 5 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_5_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_6_ID          ] = {"PP Stage2 High 6 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_6_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_7_ID          ] = {"PP Stage2 High 7 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_7_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_8_ID          ] = {"PP Stage2 High 8 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_8_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_9_ID          ] = {"PP Stage2 High 9 SRAM",        PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_9_BIT          },
    [PMU_PP_ST2_SRAM_HIGH_10_ID         ] = {"PP Stage2 High 10 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_10_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_11_ID         ] = {"PP Stage2 High 11 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_11_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_12_ID         ] = {"PP Stage2 High 12 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_12_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_13_ID         ] = {"PP Stage2 High 13 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_13_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_14_ID         ] = {"PP Stage2 High 14 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_14_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_15_ID         ] = {"PP Stage2 High 15 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_15_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_16_ID         ] = {"PP Stage2 High 16 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_16_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_17_ID         ] = {"PP Stage2 High 17 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_17_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_18_ID         ] = {"PP Stage2 High 18 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_18_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_19_ID         ] = {"PP Stage2 High 19 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_19_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_20_ID         ] = {"PP Stage2 High 20 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_20_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_21_ID         ] = {"PP Stage2 High 21 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_21_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_22_ID         ] = {"PP Stage2 High 22 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_22_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_23_ID         ] = {"PP Stage2 High 23 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_23_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_24_ID         ] = {"PP Stage2 High 24 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_24_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_25_ID         ] = {"PP Stage2 High 25 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_25_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_26_ID         ] = {"PP Stage2 High 26 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_26_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_27_ID         ] = {"PP Stage2 High 27 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_27_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_28_ID         ] = {"PP Stage2 High 28 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_28_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_29_ID         ] = {"PP Stage2 High 29 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_29_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_30_ID         ] = {"PP Stage2 High 30 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_30_BIT         },
    [PMU_PP_ST2_SRAM_HIGH_31_ID         ] = {"PP Stage2 High 31 SRAM",       PPPST2SPENBH,     PPPST2SPFWBH,     PPPST2SPSTSH,     PP_ST2_SRAM_HIGH_31_BIT         },
    [PMU_PP_ST2_SRAM_LOW_0_ID           ] = {"PP Stage2 Low 0 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_0_BIT           },
    [PMU_PP_ST2_SRAM_LOW_1_ID           ] = {"PP Stage2 Low 1 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_1_BIT           },
    [PMU_PP_ST2_SRAM_LOW_2_ID           ] = {"PP Stage2 Low 2 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_2_BIT           },
    [PMU_PP_ST2_SRAM_LOW_3_ID           ] = {"PP Stage2 Low 3 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_3_BIT           },
    [PMU_PP_ST2_SRAM_LOW_4_ID           ] = {"PP Stage2 Low 4 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_4_BIT           },
    [PMU_PP_ST2_SRAM_LOW_5_ID           ] = {"PP Stage2 Low 5 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_5_BIT           },
    [PMU_PP_ST2_SRAM_LOW_6_ID           ] = {"PP Stage2 Low 6 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_6_BIT           },
    [PMU_PP_ST2_SRAM_LOW_7_ID           ] = {"PP Stage2 Low 7 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_7_BIT           },
    [PMU_PP_ST2_SRAM_LOW_8_ID           ] = {"PP Stage2 Low 8 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_8_BIT           },
    [PMU_PP_ST2_SRAM_LOW_9_ID           ] = {"PP Stage2 Low 9 SRAM",         PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_9_BIT           },
    [PMU_PP_ST2_SRAM_LOW_10_ID          ] = {"PP Stage2 Low 10 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_10_BIT          },
    [PMU_PP_ST2_SRAM_LOW_11_ID          ] = {"PP Stage2 Low 11 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_11_BIT          },
    [PMU_PP_ST2_SRAM_LOW_12_ID          ] = {"PP Stage2 Low 12 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_12_BIT          },
    [PMU_PP_ST2_SRAM_LOW_13_ID          ] = {"PP Stage2 Low 13 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_13_BIT          },
    [PMU_PP_ST2_SRAM_LOW_14_ID          ] = {"PP Stage2 Low 14 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_14_BIT          },
    [PMU_PP_ST2_SRAM_LOW_15_ID          ] = {"PP Stage2 Low 15 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_15_BIT          },
    [PMU_PP_ST2_SRAM_LOW_16_ID          ] = {"PP Stage2 Low 16 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_16_BIT          },
    [PMU_PP_ST2_SRAM_LOW_17_ID          ] = {"PP Stage2 Low 17 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_17_BIT          },
    [PMU_PP_ST2_SRAM_LOW_18_ID          ] = {"PP Stage2 Low 18 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_18_BIT          },
    [PMU_PP_ST2_SRAM_LOW_19_ID          ] = {"PP Stage2 Low 19 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_19_BIT          },
    [PMU_PP_ST2_SRAM_LOW_20_ID          ] = {"PP Stage2 Low 20 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_20_BIT          },
    [PMU_PP_ST2_SRAM_LOW_21_ID          ] = {"PP Stage2 Low 21 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_21_BIT          },
    [PMU_PP_ST2_SRAM_LOW_22_ID          ] = {"PP Stage2 Low 22 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_22_BIT          },
    [PMU_PP_ST2_SRAM_LOW_23_ID          ] = {"PP Stage2 Low 23 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_23_BIT          },
    [PMU_PP_ST2_SRAM_LOW_24_ID          ] = {"PP Stage2 Low 24 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_24_BIT          },
    [PMU_PP_ST2_SRAM_LOW_25_ID          ] = {"PP Stage2 Low 25 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_25_BIT          },
    [PMU_PP_ST2_SRAM_LOW_26_ID          ] = {"PP Stage2 Low 26 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_26_BIT          },
    [PMU_PP_ST2_SRAM_LOW_27_ID          ] = {"PP Stage2 Low 27 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_27_BIT          },
    [PMU_PP_ST2_SRAM_LOW_28_ID          ] = {"PP Stage2 Low 28 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_28_BIT          },
    [PMU_PP_ST2_SRAM_LOW_29_ID          ] = {"PP Stage2 Low 29 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_29_BIT          },
    [PMU_PP_ST2_SRAM_LOW_30_ID          ] = {"PP Stage2 Low 30 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_30_BIT          },
    [PMU_PP_ST2_SRAM_LOW_31_ID          ] = {"PP Stage2 Low 31 SRAM",        PPPST2SPENBL,     PPPST2SPFWBL,     PPPST2SPSTSL,     PP_ST2_SRAM_LOW_31_BIT          },
};


/**************************************************************************/
/*! \fn static int pmu_ctrl(Puma7PmuSram_e pmu_id, int ctrl_op)
 **************************************************************************
 *  \brief PMU SRAM Control function.
 *         Use to Power On/Off SRAM unit
 *  \param pmu_id:      SRAM id
 *  \param ctrl_op:     Operation Code: PMU_SRAM_POWER_DOWN or PMU_SRAM_POWER_UP
 *  \return 0 on success else negative number.
 **************************************************************************/
static int pmu_ctrl(Puma7PmuSram_e sram_id, unsigned int ctrl_op)
{
    unsigned int bit = 0;

    down(&pmu_sem);

    /* Set bit */
    bit = sram_table[sram_id].bit;

    if(ctrl_op == PMU_SRAM_POWER_UP)
    {
        printk(KERN_DEBUG "Power Up '%s'\n", sram_table[sram_id].name);

        /* Power Up: Clear bit in 'power enable register'. */
        PMU_CLR_BIT(sram_table[sram_id].enable_reg, bit); 

        /* Check status: Verify that 'power status register' equal to 'power enable register'. */
        if (PMU_GET_BIT(sram_table[sram_id].enable_reg, bit) != PMU_GET_BIT(sram_table[sram_id].status_reg, bit)) 
        {
            printk(KERN_ERR "Error: SRAM power on failed ('%s')\n", sram_table[sram_id].name);
            up(&pmu_sem);
            return -1;
        }
        
        /* Disable firewall: Set bit in 'power firewall register'. */
        PMU_SET_BIT(sram_table[sram_id].firewall_reg, bit); 
    }
    else if(ctrl_op == PMU_SRAM_POWER_DOWN)
    {
        printk(KERN_DEBUG "Power Down '%s\n", sram_table[sram_id].name);
 
        /* Enable firewall: Clear bit in 'power firewall register'. */
        PMU_CLR_BIT(sram_table[sram_id].firewall_reg, bit); 

        /* Power Down: Set bit in 'power enable register'. */
        PMU_SET_BIT(sram_table[sram_id].enable_reg, bit); 

        /* Check status: Verify that 'power status register' equal to 'power enable register'. */
        if (PMU_GET_BIT(sram_table[sram_id].enable_reg, bit) != PMU_GET_BIT(sram_table[sram_id].status_reg, bit)) 
        {
            printk(KERN_ERR "Error: SRAM power off failed ('%s')\n", sram_table[sram_id].name);
            up(&pmu_sem);
            return -1;
        }
    }
    else
    {
        printk(KERN_ERR "Error: SRAM Control failed ('%s') - unknoun operation (%d)\n", sram_table[sram_id].name, ctrl_op);
        up(&pmu_sem);
        return -1;
    }

    up(&pmu_sem);
    return 0;
}

/**************************************************************************/
/*! \fn static int pmu_status_proc_read(struct seq_file *m, void *v)
 **************************************************************************
 *  \brief PMU SRAM Status proc read function.
 *         Print all SRAMs status
 *  \return 0.
 **************************************************************************/
static int pmu_status_proc_read(struct seq_file *m, void *v)
{
    int i = 0;
    int pos = 0;
    char name[PMU_NAME_SIZE_MAX+1];

    down(&pmu_sem);

    seq_printf(m, "PMU Status:\n");

    for (i=0; i<PMU_SRAM_MAX; i++)
    {
        snprintf(name, PMU_NAME_SIZE_MAX, "%s", sram_table[i].name);
        pos = strlen(name);
        name[pos] = ' ';
        pos++;
        while (pos < PMU_NAME_SIZE_MAX)
        {
            name[pos] = '.';
            pos++;
        }
        name[pos] = '\0';

        seq_printf(m, "[%03d] %s %s\n", i, name, PMU_GET_BIT(sram_table[i].status_reg, sram_table[i].bit) ? "Off" : "On"); 
    }

    up(&pmu_sem);

    return 0;
}

DECLARE_PROCFS_READ_ENTRY(pmu_status, pmu_status_proc_read)


/**************************************************************************/
/*! \fn static int pmu_unit_proc_read(struct seq_file *m, void *v)
 **************************************************************************
 *  \brief PMU SRAM Unit proc read function.
 *         Use to read SRAM unit status.
 *  \return 0.
 **************************************************************************/
static int pmu_unit_proc_read(struct seq_file *m, void *v)
{
    Puma7PmuSram_e sram_id = (Puma7PmuSram_e)(m->private);

    down(&pmu_sem);
    /* Print PMU unit status*/
    seq_printf(m, "[%03d] %s: %s\n", sram_id, sram_table[sram_id].name, PMU_GET_BIT(sram_table[sram_id].status_reg, sram_table[sram_id].bit) ? "Off" : "On");

    up(&pmu_sem);
    
    return 0;
}


/**************************************************************************/
/*! \fn static int pmu_unit_proc_write(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
 **************************************************************************
 *  \brief PMU SRAM Unit proc write function.
 *         Use to turn On/Off SRAM unit.
 *  \return 0.
 **************************************************************************/
static int pmu_unit_proc_write(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    char kbuf[PMU_PROC_KBUF_MAX];
    int cnt = count;
    Puma7PmuSram_e sram_id = (Puma7PmuSram_e)PDE_DATA(file_inode(fp));   /* Get SRAM index */
    unsigned int ctrl_op;

    /* boundary check for input buffer */
    if (count > (PMU_PROC_KBUF_MAX - 1)) 
    {
        printk(KERN_ERR "Failed to write - count is too long (count=%d)\n", count);
        return -EINVAL;
    }

    /* Copy from user buffer to kernel buffer*/
    if (copy_from_user(&kbuf, buf, count) > 0)
    {
        printk(KERN_ERR "Failed to write - failed to copy from user\n");
        return -EFAULT;
    }

    /* Trim trailing white space */
    while (ISSPACE(kbuf[cnt-1]))
    {
        cnt--;
    }

    /* Set Null terminator */
    kbuf[cnt] = 0;

    /* Command length should be one charecter only. '1' or '0' */
    if (strlen(kbuf) != 1) 
    {
        printk(KERN_ERR "Invalid command.\n0 - Turn Off SRAM.\n1 - Turn On SRAM.\n");
        return -EINVAL;
    }

    /* Parse opcode in buffer */
    if (kbuf[0] == '0')
    {
        /* Turn off SRAM*/
        ctrl_op = PMU_SRAM_POWER_DOWN;
    }
    else if (kbuf[0] == '1')
    {
        /* Turn on SRAM*/
        ctrl_op = PMU_SRAM_POWER_UP;
    }
    else
    {
        printk(KERN_ERR "Invalid command.\n0 - Turn Off SRAM.\n1 - Turn On SRAM.\n");
        return -EINVAL;
    }

    /* Turn SRAM On/Off */
    pmu_ctrl(sram_id, ctrl_op);

    return count;
}

DECLARE_PROCFS_ENTRY(pmu_unit, pmu_unit_proc_read, pmu_unit_proc_write)


/**************************************************************************/
/*! \fn int PUMA7_SRAM_Ctrl(Puma7PmuSram_e sram_id, unsigned int power_on)
 **************************************************************************
 *  \brief PUMA7 PMU SRAM Control function.
 *         Use to Power On/Off SRAM unit
 *  \param  sram_id     - PUMA7 PMU SRAM id
 *  \param  power_on    - 0: Power Off, 1: Power On
 *  \return 0 on success else negative number.
 **************************************************************************/
int PUMA7_SRAM_Ctrl(Puma7PmuSram_e sram_id, unsigned int power_on)
{
    unsigned int ctrl_op; 

    if (sram_id >= PMU_SRAM_MAX)
    {
        printk(KERN_ERR "Invalid SRAM id (%d)\n", sram_id);
        return -1;
    }

    /* Parse command */
    if (power_on == 1)
    {
        ctrl_op = PMU_SRAM_POWER_UP;
    }
    else if (power_on == 0)
    {
        ctrl_op = PMU_SRAM_POWER_DOWN;
    }
    else
    {
        printk(KERN_ERR "Invalid SRAM Operation (%d)\n", power_on);
        return -1;
    }

    /* Power Up/Down the SRAM */
    if (pmu_ctrl(sram_id, ctrl_op) != 0)
    {
        printk(KERN_ERR "SRAM power control operation fail\n");
        return -1;
    }

    return 0;
}


/**************************************************************************/
/*! \fn static int __init p_unit_drv_init(void)
 **************************************************************************
 *  \brief Power Managment Unit device module init function.
 *  \return 0.
 **************************************************************************/
static int __init puma7_pmu_drv_init(void)
{
    int pmu_id = 0;
    char proc_name[PMU_PROC_NAME_MAX] = {0};
    printk(KERN_INFO "PMU: Initializing module\n");
    
    sema_init(&pmu_sem, 1);

    proc_mkdir("pmu", NULL);
    proc_mkdir("pmu/units", NULL);
    proc_create("pmu/status", 0644, NULL, &pmu_status_proc_fops);
    for (pmu_id=0; pmu_id<PMU_SRAM_MAX; pmu_id++)
    {
        snprintf(proc_name,PMU_PROC_NAME_MAX,"pmu/units/%d", pmu_id);
        proc_create_data(proc_name, 0644, NULL, &pmu_unit_proc_fops,pmu_id);
    }

    printk(KERN_INFO "PMU: /proc/pmu/ created\n");

    return 0;
}


/**************************************************************************/
/*! \fn static void __exit iosfsb_drv_exit(void)
 **************************************************************************
 *  \brief This function is the iosfsb device module exit function.
 **************************************************************************/
static void __exit puma7_pmu_drv_exit(void)
{
    printk(KERN_INFO "PMU: Terminating module\n");

    return;
}


module_init(puma7_pmu_drv_init);
module_exit(puma7_pmu_drv_exit);

/* Driver export API */
EXPORT_SYMBOL(PUMA7_SRAM_Ctrl);

/* Driver identification */
MODULE_DESCRIPTION("Power Managment Unit (PMU) Device Driver");
MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL");


