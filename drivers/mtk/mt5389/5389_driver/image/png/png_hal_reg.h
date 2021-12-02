/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/

#ifndef __PNG_HAL_REG_H
#define __PNG_HAL_REG_H

#define PNG_REG_OW_ADDR_MASK        0x3FFFFFF   //ow addr
#define PNG_REG_B_ADDR_MASK         0x3FFFFFFF  //byte addr
#define PNG_REG_B_LEN_MASK          0x7FFFFFFF  //byte addr
// IOMMU
#define REG_RW_IOMMU_CFG0               0x000        // basic setting
#define REG_RW_IOMMU_CFG1               0x004        // page table index
#define REG_RW_IOMMU_CFG2               0x008        // agnet_0~1 setting
#define REG_RW_IOMMU_CFG3               0x00C        // agnet_2~3 setting
#define REG_RW_IOMMU_CFG4               0x010        // interrupt, monitor and debug
#define REG_RW_IOMMU_CFG5               0x014        // perfomance meter
#define REG_RW_IOMMU_CFG6               0x018        // monitor result
#define REG_RW_IOMMU_CFG7               0x01C        // monitor result
#define REG_RW_IOMMU_CFG8               0x020        // monitor result
// special setting *********************
#define REG_RW_IOMMU_CFG9               0x024        // over read protection
#define REG_RW_IOMMU_CFGA               0x028        // over read protection
#define REG_RW_IOMMU_CFGB               0x02C        // over read protection
#define REG_RW_IOMMU_CFGC               0x030        // over read protection
// special setting &&&&&&&&&&&&&&&&&&&&&
#define IO_MMU_BASE                   0xF0068000
#define PNG_MMU_BASE                0x180
#define PNG_REG_BASE1               0x6E000
#define PNG_REG_BASE2               0x6E100
#define IMAGE_BASE 0xF0000000
#define IMAGE2_BASE 0xF0000000


///PNG decode register define
#define PNG_DEC_RESET_REG                   0x00
    #define PNG_REG_RESET_START         0xF
    #define PNG_REG_RESET_END           0x0
#define PNG_DEC_ENABLE_REG                  0x04
    #define PNG_REG_ENABLE              0x1
    #define PNG_MMU_ENABLE      0x2
#define PNG_DEC_SRAM_CHIP_SELECT_REG        0x08
    #define PNG_REG_SRAM_OFF            0x0
    #define PNG_REG_TRAM_SRAM_ON        0x1
    #define PNG_REG_SRAM_ON             0x2     //designer's suggestion
#define PNG_DEC_DECODE_START_REG            0x0C
    #define PNG_REG_DEC_START0          0x0
    #define PNG_REG_DEC_START1          0x1
#define PNG_DEC_SLICE_RESUME_REG            0x10
    #define PNG_REG_DST_RESUME0         0x0
    #define PNG_REG_DST_RESUME1         0x1
#define PNG_DEC_BITSTRM_RESUME_REG          0x14
    #define PNG_REG_SRC_RESUME0         0x0
    #define PNG_REG_SRC_RESUME1         0x1
#define PNG_DEC_READ_PLTT_CHUNK_REG         0x18
    #define PNG_REG_READ_PLTE           0x1
#define PNG_DEC_READ_TRNS_CHUNK_REG         0x1C
    #define PNG_REG_READ_TRNS           0x1
#define PNG_DEC_PLTT_ENTRY_NUM_REG          0x20
    #define PNG_REG_PLTE_ENTRY_NUM_MASK 0xFF    //must minus 1
#define PNG_DEC_TRNS_ENTRY_NUM_REG          0x24
    #define PNG_REG_TRNS_ENTRY_NUM_MASK 0xFF    //must minus 1
#define PNG_DEC_LAST_IDAT_GROUP_REG         0x28
    #define PNG_REG_LAST_IDAT_GRP       0x1
    #define PNG_REG_NOTLAST_IDAT_GRP    0x0
#define PNG_DEC_LAST_SLICE_REG              0x2C
    #define PNG_REG_LAST_SLICE_REG      0x1
    #define PNG_REG_NOTLAST_SLICE_REG   0x0
#define PNG_DEC_DRAM_REQUEST_SET_REG        0x30
    #define PNG_REG_DRAM_REQ_1          0x0
    #define PNG_REG_DRAM_REQ_2          0x1
    #define PNG_REG_DRAM_REQ_3          0x2
    #define PNG_REG_DRAM_REQ_4          0x3     //designer's suggestion
#define PNG_DEC_BITSTRM_SRT_ADDR_REG        0x34
#define PNG_DEC_BITSTRM_SRT_ADDR_WR_REG     0x38
    #define PNG_REG_SRT_ADDR_CHG        0x1
#define PNG_DEC_BITSTRM_LENGTH_REG          0x3C
#define PNG_DEC_BITSTRM_FIFO_SRT_OWADDR_REG 0x40
#define PNG_DEC_BITSTRM_FIFO_END_OWADDR_REG 0x44 //after OW, need minus 1
#define PNG_DEC_PIXEL_OUT_XY_ADDR_REG       0x48
#define PNG_DEC_PIXEL_OUT_SRT_OWADDR_REG    0x4C
#define PNG_DEC_PIXEL_OUT_END_OWADDR_REG    0x50
#define PNG_DEC_LZ77_STR_OWADDR_REG         0x54
#define PNG_DEC_LZ77_END_OWADDR_REG         0x58
#define PNG_DEC_LINE_BUF_STR_OWADDR_REG     0x5C
#define PNG_DEC_LINE_BUF_END_OWADDR_REG     0x60
#define PNG_DEC_DISP_WIDTH_SLICE_HEGIHT_REG 0x64
        #define PNG_REG_DISP_W_SHIFT            16
        #define PNG_REG_SLICE_H_SHIFT           0
        #define PNG_REG_DISP_W_MASK             0xFFFF      //must minus 1
        #define PNG_REG_SLICE_H_MASK            0xFFFF      //must minus 1
#define PNG_DEC_SRC_WIDTH_HEIGHT_REG        0x68
        #define PNG_REG_SRC_W_SHIFT             16
        #define PNG_REG_SRC_H_SHIFT             0
        #define PNG_REG_SRC_W_MASK              0xFFFF      //must minus 1
        #define PNG_REG_SRC_H_MASK              0xFFFF      //must minus 1
#define PNG_DEC_CROP_ORG_XY_REG             0x6C
        #define PNG_REG_CROP_X_SHIFT            16
        #define PNG_REG_CROP_Y_SHIFT            0
        #define PNG_REG_CROP_X_MASK             0xFFFF
        #define PNG_REG_CROP_Y_MASK             0xFFFF
#define PNG_DEC_CROP_WIDTH_HEIGHT_REG       0x70
        #define PNG_REG_CROP_W_SHIFT            16
        #define PNG_REG_CROP_H_SHIFT            0
        #define PNG_REG_CROP_W_MASK             0xFFFF      //must minus 1
        #define PNG_REG_CROP_H_MASK             0xFFFF      //must minus 1
#define PNG_DEC_RESIZE_RATIO_REG            0x74
    #define PNG_REG_INTERLACE_7         (7U << 6)
#define PNG_DEC_SRC_FORMAT_REG              0x78
        #define PNG_REG_BIT_DEPTH_SHIFT         0x4
        #define PNG_REG_CLR_TYPE_SHIFT          0x1
        #define PNG_REG_BIT_DEPTH_MASK          0x1F
        #define PNG_REG_CLR_TYPE_MASK           0x7
    #define PNG_REG_INTERLACE_ON        0x1
    #define PNG_REG_INTERLACE_OFF       0x0
#define PNG_DEC_OUTPUT_FORMAT_REG           0x7C
    #define PNG_REG_OUT_FORMAT_PALETTE  0x0
    #define PNG_REG_OUT_FORMAT_ARGB1555 0x1
    #define PNG_REG_OUT_FORMAT_RGB565   0x2
    #define PNG_REG_OUT_FORMAT_ARGB4444 0x3
    #define PNG_REG_OUT_FORMAT_ARGB8888 0x4
#define PNG_DEC_ALPHA_MATCHED_PIXEL_REG     0x80
#define PNG_DEC_ALPHA_UNMATCHED_PIXEL_REG   0x84
        #define PNG_REG_ALPHA_VAL_MASK          0xFF
#define PNG_DEC_TRANSPARENCY_CTRL_REG       0x88
    #define PNG_REG_TRNS_EN             (1U << 3)
    #define PNG_REG_TRNS_OUT            (1U << 2)
    #define PNG_REG_TRNS_ORG_ALPHA      (1U << 1)
    #define PNG_REG_TRNS_MATCH_16BIT    (1U << 0)
#define PNG_DEC_TRANSPARENCY_KEY0_REG       0x8C
        #define PNG_REG_TRNS_GREY_SHIFT     16
        #define PNG_REG_TRNS_R_SHIFT        0
#define PNG_DEC_TRANSPARENCY_KEY1_REG       0x90
        #define PNG_REG_TRNS_G_SHIFT        16
        #define PNG_REG_TRNS_B_SHIFT        0
#define PNG_DEC_TRANSPARENCY_BACKGROUND_REG 0x94
        #define PNG_REG_IRQ0_BIT_MASK       0xFEFFFFFF
        #define PNG_REG_IRQ1_BIT_MASK       0xFDFFFFFF
        #define PNG_REG_IRQ2_BIT_MASK       0xFBFFFFFF
        #define PNG_REG_DSTIRQ_CLEAN_EN     0x4000000
        #define PNG_REG_SRCIRQ_CLEAN_EN     0x2000000
        #define PNG_REG_DECIRQ_CLEAN_EN     0x1000000
        #define PNG_REG_TRNS_BG_GREY_SHIFT  24
        #define PNG_REG_TRNS_BG_R_SHIFT     16
        #define PNG_REG_TRNS_BG_G_SHIFT     8
        #define PNG_REG_TRNS_BG_B_SHIFT     0
#define PNG_DEC_CHUNK_TYPE_REG              0x98
#define PNG_DEC_CHUNK_TYPE_WR_REG           0x9C
    #define PNG_REG_CHUNK_WR            0x1
#define PNG_DEC_CRC_OUT_REG                 0xA0
#define PNG_DEC_MONITOR_REG                 0xA4
    #define PNG_REG_MONITOR_SLICE_DONE      (0x1 << 6)
    #define PNG_REG_MONITOR_BITSTRM_EMPTY   (0x1 << 5)
    #define PNG_REG_MONITOR_PLTT_FIN        (0x1 << 4)
    #define PNG_REG_MONITOR_TRNS_FIN        (0x1 << 3)
    #define PNG_REG_MONITOR_DEC_DONE        (0x1 << 2)
    #define PNG_REG_MONITOR_ALDER_ERR       (0x1 << 1)
    #define PNG_REG_MONITOR_DEC_ERR         (0x1)
#define PNG_DEC_ADLER32_OUT_REG             0xA8
#define PNG_DEC_DECODE_ERR_CODE_REG         0xAC
#define PNG_DEC_DRAM_WR_OUT_OF_RANGE_REG    0xB0
#define PNG_DEC_PELOUT_WDLT_EN_REG    0x104
#define PNG_DEC_PELOUT_WDLT_EN_0        0x1
#define PNG_DEC_PELOUT_WDLT_EN_1        0x2



#endif // __PNG_HAL_REG_H
