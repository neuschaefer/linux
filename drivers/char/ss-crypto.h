#ifndef __SUNXI_SS__
#define __SUNXI_SS__

#define SUNXI_DMA_REG_BASE         0xF1C02000
#define SUNXI_DDMA_REG_BASE        0xF1C02300

#define DMA_IRQ_EN_REG       *((volatile u32 *)(SUNXI_DMA_REG_BASE+0x00))
#define DMA_IRQ_PEND_REG     *((volatile u32 *)(SUNXI_DMA_REG_BASE+0x04))

#define DDMA0_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+0*0x20+0x00))
#define DDMA0_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+0*0x20+0x04))
#define DDMA0_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+0*0x20+0x08))
#define DDMA0_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+0*0x20+0x0c))
#define DDMA0_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+0*0x20+0x18))

#define DDMA1_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+1*0x20+0x00))
#define DDMA1_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+1*0x20+0x04))
#define DDMA1_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+1*0x20+0x08))
#define DDMA1_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+1*0x20+0x0c))
#define DDMA1_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+1*0x20+0x18))

#define DDMA2_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+2*0x20+0x00))
#define DDMA2_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+2*0x20+0x04))
#define DDMA2_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+2*0x20+0x08))
#define DDMA2_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+2*0x20+0x0c))
#define DDMA2_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+2*0x20+0x18))

#define DDMA3_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+3*0x20+0x00))
#define DDMA3_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+3*0x20+0x04))
#define DDMA3_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+3*0x20+0x08))
#define DDMA3_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+3*0x20+0x0c))
#define DDMA3_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+3*0x20+0x18))

#define DDMA4_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+4*0x20+0x00))
#define DDMA4_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+4*0x20+0x04))
#define DDMA4_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+4*0x20+0x08))
#define DDMA4_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+4*0x20+0x0c))
#define DDMA4_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+4*0x20+0x18))

#define DDMA5_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+5*0x20+0x00))
#define DDMA5_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+5*0x20+0x04))
#define DDMA5_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+5*0x20+0x08))
#define DDMA5_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+5*0x20+0x0c))
#define DDMA5_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+5*0x20+0x18))

#define DDMA6_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+6*0x20+0x00))
#define DDMA6_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+6*0x20+0x04))
#define DDMA6_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+6*0x20+0x08))
#define DDMA6_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+6*0x20+0x0c))
#define DDMA6_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+6*0x20+0x18))

#define DDMA7_CFG_REG        *((volatile u32 *)(SUNXI_DDMA_REG_BASE+7*0x20+0x00))
#define DDMA7_SRC_ADDR_REG   *((volatile u32 *)(SUNXI_DDMA_REG_BASE+7*0x20+0x04))
#define DDMA7_DEST_ADDR_REG  *((volatile u32 *)(SUNXI_DDMA_REG_BASE+7*0x20+0x08))
#define DDMA7_BC_REG         *((volatile u32 *)(SUNXI_DDMA_REG_BASE+7*0x20+0x0c))
#define DDMA7_PARA_REG       *((volatile u32 *)(SUNXI_DDMA_REG_BASE+7*0x20+0x18))

#define SUNXI_SS_REG_BASE        0xF1C15000
#define SUNXI_SS_IRQ                86

#define SS_CTL_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x00))
#define SS_KEY0_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x04))
#define SS_KEY1_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x08))
#define SS_KEY2_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x0c))
#define SS_KEY3_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x10))
#define SS_KEY4_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x14))
#define SS_KEY5_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x18))
#define SS_KEY6_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x1c))
#define SS_KEY7_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x20))

#define SS_IV0_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x24))
#define SS_IV1_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x28))
#define SS_IV2_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x2c))
#define SS_IV3_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x30))

#define SS_CNT0_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x34))
#define SS_CNT1_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x38))
#define SS_CNT2_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x3c))
#define SS_CNT3_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x40))

#define SS_FCSR_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x44))
#define SS_ICSR_REG          *((volatile u32 *)(SUNXI_SS_REG_BASE+0x48))

#define SS_MD0_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x4c))
#define SS_MD1_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x50))
#define SS_MD2_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x54))
#define SS_MD3_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x58))
#define SS_MD4_REG           *((volatile u32 *)(SUNXI_SS_REG_BASE+0x5c))

#define SS_RXFIFO_REG        *((volatile u32 *)(SUNXI_SS_REG_BASE+0x200))
#define SS_TXFIFO_REG        *((volatile u32 *)(SUNXI_SS_REG_BASE+0x204))

/* SUNXI_SS_CTL configuration values */

/* AES/DES/3DES key select - bits 24-27 */
#define SUNXI_KEYSELECT_KEYN            (0 << 24)
#define SUNXI_KEYSELECT_SID_RKEYN       (1 << 24)
#define SUNXI_KEYSELECT_IKEY(n)         ((3 + n) << 24)

/* PRNG generator mode - bit 15 */
#define SUNXI_PRNG_ONESHOT              (0 << 15)
#define SUNXI_PRNG_CONTINUE             (1 << 15)

/* IV Steady of SHA-1/MD5 constants - bit 14 */
#define SUNXI_IV_CONSTANTS              (0 << 14)
#define SUNXI_IV_ARBITRARY              (1 << 14)

/* SS operation mode - bits 12-13 */
#define SUNXI_SS_ECB                    (0 << 12)
#define SUNXI_SS_CBC                    (1 << 12)
#define SUNXI_SS_CNT                    (2 << 12)

/* Counter width for CNT mode - bits 10-11 */
#define SUNXI_CNT_16BITS                (0 << 10)
#define SUNXI_CNT_32BITS                (1 << 10)
#define SUNXI_CNT_64BITS                (2 << 10)

/* Key size for AES - bits 8-9 */
#define SUNXI_AES_128BITS               (0 << 8)
#define SUNXI_AES_192BITS               (1 << 8)
#define SUNXI_AES_256BITS               (2 << 8)

/* Operation direction - bit 7 */
#define SUNXI_SS_ENCRYPTION             (0 << 7)
#define SUNXI_SS_DECRYPTION             (1 << 7)

/* SS Method - bits 4-6 */
#define SUNXI_OP_AES                    (0 << 4)
#define SUNXI_OP_DES                    (1 << 4)
#define SUNXI_OP_3DES                   (2 << 4)
#define SUNXI_OP_SHA1                   (3 << 4)
#define SUNXI_OP_MD5                    (4 << 4)
#define SUNXI_OP_PRNG                   (5 << 4)

/* Data end bit - bit 2 */
#define SUNXI_SS_DATA_END                BIT(2)

/* PRNG start bit - bit 1 */
#define SUNXI_PRNG_START                BIT(1)

/* SS Enable bit - bit 0 */
#define SUNXI_SS_DISABLED               (0 << 0)
#define SUNXI_SS_ENABLED                (1 << 0)

/* RX FIFO status - bit 30 */
#define SUNXI_RXFIFO_FREE               BIT(30)

/* RX FIFO empty spaces - bits 24-29 */
#define SUNXI_RXFIFO_SPACES(val)        (((val) >> 24) & 0x3f)

/* TX FIFO status - bit 22 */
#define SUNXI_TXFIFO_AVAILABLE          BIT(22)

/* TX FIFO available spaces - bits 16-21 */
#define SUNXI_TXFIFO_SPACES(val)        (((val) >> 16) & 0x3f)

#define SUNXI_RXFIFO_EMP_INT_PENDING        BIT(2)
#define SUNXI_TXFIFO_AVA_INT_PENDING        BIT(1)

#define SUNXI_SS_ICS_DRA_ENABLE                BIT(4)

#endif
 
