/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */

#ifndef _DT_BINDINGS_CLOCK_NUVOTON_WPCM450_CLK_H
#define _DT_BINDINGS_CLOCK_NUVOTON_WPCM450_CLK_H

/* Clocks based on CLKEN bits */
#define WPCM450_CLK_FIU            0
#define WPCM450_CLK_XBUS           1
#define WPCM450_CLK_KCS            2
#define WPCM450_CLK_SHM            4
#define WPCM450_CLK_USB1           5
#define WPCM450_CLK_EMC0           6
#define WPCM450_CLK_EMC1           7
#define WPCM450_CLK_USB0           8
#define WPCM450_CLK_PECI           9
#define WPCM450_CLK_AES           10
#define WPCM450_CLK_UART0         11
#define WPCM450_CLK_UART1         12
#define WPCM450_CLK_SMB2          13
#define WPCM450_CLK_SMB3          14
#define WPCM450_CLK_SMB4          15
#define WPCM450_CLK_SMB5          16
#define WPCM450_CLK_HUART         17
#define WPCM450_CLK_PWM           18
#define WPCM450_CLK_TIMER0        19
#define WPCM450_CLK_TIMER1        20
#define WPCM450_CLK_TIMER2        21
#define WPCM450_CLK_TIMER3        22
#define WPCM450_CLK_TIMER4        23
#define WPCM450_CLK_MFT0          24
#define WPCM450_CLK_MFT1          25
#define WPCM450_CLK_WDT           26
#define WPCM450_CLK_ADC           27
#define WPCM450_CLK_SDIO          28
#define WPCM450_CLK_SSPI          29
#define WPCM450_CLK_SMB0          30
#define WPCM450_CLK_SMB1          31

/* Other clocks */
#define WPCM450_CLK_USBPHY        32

#define WPCM450_NUM_CLKS          33

/* Resets based on IPSRST bits */
#define WPCM450_RESET_FIU          0
#define WPCM450_RESET_EMC0         6
#define WPCM450_RESET_EMC1         7
#define WPCM450_RESET_USB0         8
#define WPCM450_RESET_USB1         9
#define WPCM450_RESET_AES_PECI    10
#define WPCM450_RESET_UART        11
#define WPCM450_RESET_MC          12
#define WPCM450_RESET_SMB2        13
#define WPCM450_RESET_SMB3        14
#define WPCM450_RESET_SMB4        15
#define WPCM450_RESET_SMB5        16
#define WPCM450_RESET_PWM         18
#define WPCM450_RESET_TIMER       19
#define WPCM450_RESET_ADC         27
#define WPCM450_RESET_SDIO        28
#define WPCM450_RESET_SSPI        29
#define WPCM450_RESET_SMB0        30
#define WPCM450_RESET_SMB1        31

#define WPCM450_NUM_RESETS        32

#endif /* _DT_BINDINGS_CLOCK_NUVOTON_WPCM450_CLK_H */
