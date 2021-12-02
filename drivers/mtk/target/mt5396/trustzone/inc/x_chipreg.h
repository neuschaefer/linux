#ifndef CHIP_REG_H
#define CHIP_REG_H

#include "x_typedef.h"
#include "x_hal_io.h"

//----------------------------------------------------------------------------
#define IO_VIRT                         (0xf0000000U)
#define IO_READ32(base, offset)		    HAL_READ32((base) + (offset))
#define IO_WRITE32(base, offset, value)	HAL_WRITE32((base) + (offset), (value))

#define BIM_BASE                    (IO_VIRT + 0x08000)
#define RS232_BASE                  (IO_VIRT + 0x0c000)
#define CKGEN_BASE                  (IO_VIRT + 0x0d000)
#define PDWNC_BASE                  (IO_VIRT + 0x28000)

//----------------------------------------------------------------------------
#define CKGEN_READ32(offset)              IO_READ32(CKGEN_BASE, (offset))
#define CKGEN_WRITE32(offset, value)      IO_WRITE32(CKGEN_BASE, (offset), (value))

#define REG_STRAPPING           (0x0)

//----------------------------------------------------------------------------
#define BIM_READ32(off)         IO_READ32(BIM_BASE, (off))
#define BIM_WRITE32(off, val)   IO_WRITE32(BIM_BASE, (off), (val))

#define REG_RW_REMAP            (0x001c)        // Remap Register
    #define REMAP_ENABLE        (1)
#define REG_RW_BUSTIME          (0x0024)        // BUS Access Timing Register
    #define BUS_RT_EN           (1U << 21)      // RISC timeout counter enable bit
#define REG_RW_IRQEN            (0x0034)        // RISC IRQ Enable Register
#define REG_RW_IRQCLR           (0x0038)        // RISC IRQ Clear Register
#define REG_RW_FIQEN            (0x0040)        // RISC IRQ Enable Register
#define REG_RW_FIQCLR           (0x0044)        // RISC IRQ Clear Register
#define REG_RW_TIMER2_LLMT      (0x0070)        // RISC Timer 2 Low Limit Register
#define REG_RW_TIMER2_LOW       (0x0074)        // RISC Timer 2 Low Register
#define REG_RW_TIMER2_HLMT      (0x0190)        // RISC Timer 2 High Limit Register
#define REG_RW_TIMER2_HIGH      (0x0194)        // RISC Timer 2 High Register

#define REG_RW_TIMER_CTRL   0x0078      // RISC Timer Control Register
    #define TMR0_CNTDWN_EN  (1U << 0)   // Timer 0 enable to count down
    #define TMR0_AUTOLD_EN  (1U << 1)   // Timer 0 auto-load enable
    #define TMR1_CNTDWN_EN  (1U << 8)   // Timer 1 enable to count down
    #define TMR1_CNTDWN_EN  (1U << 8)   // Timer 1 enable to count down
    #define TMR1_AUTOLD_EN  (1U << 9)   // Timer 1 auto-load enable
    #define TMR2_CNTDWN_EN  (1U << 16)  // Timer 2 enable to count down
    #define TMR2_AUTOLD_EN  (1U << 17)  // Timer 2 auto-load enable

#define REG_RW_GPRB0        0x00e0      // RISC Byte General Purpose Register 0

#define BIM_SetTimeLog(x)       BIM_WRITE32((REG_RW_GPRB0 + (x << 2)), BIM_READ32(REG_RW_TIMER2_LOW))


//----------------------------------------------------------------------------
#define RS232_READ32(off)       IO_READ32(RS232_BASE, (off))
#define RS232_WRITE32(off, val) IO_WRITE32(RS232_BASE, (off), (val))    

#define REG_U0_OUTPORT          (0x0000)
#define REG_CMD_STATUS          (0x0004)
    #define CMD_MODE            (0x00)
    #define SER_UR_ACTIVE       (0)
    #define SER_ADSP_ACTIVE     (1)
    #define SER_RISC_ACTIVE     (2)
    #define REG_SER_TRANSMODE   (0x40)
#define REG_RS232_MODE          (0x0018)
    #define RISC_ACTIVE         (0x02)
#define REG_BUFFER_SIZE         (0x001c)
    #define SINGLE_BYTE         (0x00)
#define REG_U0_STATUS           (0x004c)
    #define STATUS_TXBUF        (0x1f00)
    #define STATUS_RXBUF        (0x001f)
#define REG_U0_BUFCTL           (0x0050)
    #define DEF_BUFCTL          (0xff01)


//----------------------------------------------------------------------------
#define PDWNC_READ32(off)       IO_READ32(PDWNC_BASE, (off))
#define PDWNC_WRITE32(off, val) IO_WRITE32(PDWNC_BASE, (off), (val))    

#define REG_RW_IOPADEN          (0x0080)        // IO Pad Output Enable Register
    #define PAD_UR0_TX_EN       (1U << 15)      // UART 0 TX PAD output enable
#define REG_RW_WATCHDOG_EN      0x0100          // Watchdog Timer Control Register
#define REG_RW_WATCHDOG_TMR     0x0104          // Watchdog Timer Register


#endif /* CHIP_REG_H */
