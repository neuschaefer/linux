/*
 * kernel/puma7_io_ctrl.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015-2017 Intel Corporation. All rights reserved.
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
#include <linux/kernel.h>
#include "iosfsb_api.h"
#include "gpio_southeast_registers.h"
#include "gpio_southwest_registers.h"
#include "gpio_northeast_registers.h"
#include "gpio_northwest_registers.h"
#include "gpio_west_registers.h"
#include "puma7_io_ctrl.h"
#include "docsis_ip_boot_params.h"

/****************************************************************************/
/*                          DEFINES & TYPEDEFS                              */
/****************************************************************************/

/*Pad Control Register 0*/
#define PCR0_GPIOCFG_MASK   (0x700)     /* Bits 10:8  */
#define PCR0_GPIOEN_MASK    (0x8000)    /* Bit 15     */
#define PCR0_GPIOPM_MASK    (0xF0000)   /* Bits 19:16 */
#define PCR0_TERM_MASK      (0xF00000)  /* Bits 23:20 */
#define PCR0_RXTXENCFG_MASK (0x3000000) /* Bits 24:24 */
/*Pad Control Register 1*/
#define PCR1_INVRXTX_MASK   (0xF0)      /* Bits 7:4   */


/* To open debug print use this macro */     
//#define IO_CTRL_DEBUG_OUTPUT_ON
/* Debug */
#ifdef  IO_CTRL_DEBUG_OUTPUT_ON
    #define IO_CTRL_DEBUG_OUTPUT(fmt, args...) printk("Puma7 IO CTRL Debug (%s): " fmt, __FUNCTION__ , ## args)
#else
    #define IO_CTRL_DEBUG_OUTPUT(fmt, args...)
#endif

#define IO_CTRL_ERROR_OUTPUT(fmt, args...) printk(KERN_ERR,"Puma7 IO CTRL ERROR (%s): " fmt, __FUNCTION__ , ## args)

#define IO_CTRL_ENABLE_VAL2STR(_enable) ((_enable) ? "enable" : "disable")

typedef union
{
    struct  
    {
#ifdef LITTLE_ENDIEN
        int GPIORXState : 1; /* GPIO RX State, this is the  current state of the PAD              */
        int GPIOTXState : 1; /* GPIO TX State is register  that controls the output driven by the 
                                pad. This only applies when GPIO Config is set to enable the     
                                TX buffer.                                                        */
        int GFDebSelect : 1; /* Chooses between filtering using glitch filter                    
                                (pulses < ~120nS filtered)and the debounce logic
                                0 = Glitch Filter Logic
                                1 = Debounce Filter                               */
        int PSlewVal    : 2; /* This field is only used for the following Cherryview pins:       
                                CX_PRDY_B,CX_PRDY_B_2, TCK, TRST_B, TMS, TDI, TDO, 
                                TDO_2,SVID0_ALERT_B, SVID0_DATA,SVID0_CLK, PROCHOT_B              */
        int NStrVal     : 2; /* Termination strength setting for the MSMV CFIO buffer Family.
                                00 = 150 ohm Nominal
                                01 = 75 ohm Nominal
                                10 = 50 ohm Nominal
                                11 = 35 ohm Nominal
                                This field is only used for the following Cherryview pins:
                                CX_PRDY_B, CX_PRDY_B_2, TCK, TRST_B, TMS, TDI, TDO, TDO_2,
                                SVID0_ALERT_B, SVID0_DATA, SVID0_CLK, PROCHOT_B                   */
        int GPIO_Light  : 1; /* This mode disables the following fields and muxes function 1 
                                directly out of the pad.
                                (Pmode, GPIOCfg, GPIOEn,XTXEnCfg, GFCfg, IOSState, InvRXTX, ODEn)
                                0 = Light Mode Disabled
                                1 = Light Mode Enabled                                            */
        int GPIOCfg     : 3; /* 0 = GPIO (TX Enabled and RX Enabled)
                                1 = GPO (TX Enabled and RX Disabled)
                                2 = GPI (TX Disabled and RX Enabled) 
                                3 = Hi-Z (TX Disabled and RX Disabled) Others = Reserved          */
        int NSlewVal    : 2; /* This field is only used for the following Cherryview pins:
                                CX_PRDY_B, CX_PRDY_B_2, TCK, TRST_B, TMS, TDI, TDO, TDO_2,
                                SVID0_ALERT_B, SVID0_DATA, SVID0_CLK, PROCHOT_B                   */
        int PStrVal     : 2; /* Termination strength setting for the MSMV CFIO buffer Family.
                                00 = 150 ohm Nominal
                                01 = 75 ohm Nominal
                                10 = 50 ohm Nominal
                                11 = 35 ohm Nominal
                                This field is only used for the following Cherryview pins:
                                CX_PRDY_B, CX_PRDY_B_2,TCK, TRST_B, TMS, TDI, TDO, TDO_2, 
                                SVID0_ALERT_B, SVID0_DATA, SVID0_CLK, PROCHOT_B                   */
        int GPIOEn      : 1; /* This enables GPIO mode and function in Pad Mode no longer controls
                                the Pad and the GPIO fields control the Pad.
                                0 = GPIO disabled and and function defined in Pad Mode controls
                                    the pad.
                                1 = GPIO enabled and Pad Register controls the pad                */
        int GPIOPM      : 4; /* This controls which function controls this particular Pad when
                                GPIO Enable = 0.                                                  */
        int TERM        : 4; /* The Pad Termination state defines the different weak pull-up 
                                and pull-down settings that are supported by the buffer.
                                [3] 0 = Pulldown, 1 = Pullup.
                                [2] = 1k pull up only. MSMV only.
                                [1] = 5k
                                [0] = 20k                                                         */
        int RxTxEnCfg   : 2; /* This field controls the Rx and Tx Buffer enables when GPIO mode
                                is disabled
                                0 =  Function defined in Pad Mode controls TX and RX Enables
                                1 = Function controls TX Enable and RX Always Disabled
                                2 = Function controls TX Enable and RX Always Enabled
                                3 = Both TX and RX Always Enabled                                 */
        int GFCfg       : 2; /* Enable the Glitch Filter on the RX path to deglitch the
                                incoming signal. This only makes sense when the buffer is
                                configured as an input.
                                0 = Disable (Bypass the glitch filter)
                                1 = Enable for Edge Detect Only
                                2 = Enable for RX Data Only
                                3 = Enable for Edge Detect and RX Data                            */
        int IntSel      : 4; /* The Interrupt Select defines which interrupt line driven 
                                from the GPIO Controller toggles when an interrupt is detected
                                on this pad.
                                Only applicable when parallel wires  are used for Interrupt Wire
                                Logic with the Interrupt Steering Logic.
                                0 = Interrupt Line 0
                                1 = Interrupt Line 1
                                ... 15 = Interrupt Line 15     */ 
#else
        int IntSel      : 4; /* The Interrupt Select defines which interrupt line driven 
                                from the GPIO Controller toggles when an interrupt is detected
                                on this pad.
                                Only applicable when parallel wires  are used for Interrupt Wire
                                Logic with the Interrupt Steering Logic.
                                0 = Interrupt Line 0
                                1 = Interrupt Line 1
                                ... 15 = Interrupt Line 15     */ 
        int GFCfg       : 2; /* Enable the Glitch Filter on the RX path to deglitch the
                                incoming signal. This only makes sense when the buffer is
                                configured as an input.
                                0 = Disable (Bypass the glitch filter)
                                1 = Enable for Edge Detect Only
                                2 = Enable for RX Data Only
                                3 = Enable for Edge Detect and RX Data                            */
        int RxTxEnCfg   : 2; /* This field controls the Rx and Tx Buffer enables when GPIO mode
                                is disabled
                                0 =  Function defined in Pad Mode controls TX and RX Enables
                                1 = Function controls TX Enable and RX Always Disabled
                                2 = Function controls TX Enable and RX Always Enabled
                                3 = Both TX and RX Always Enabled                                 */
        int TERM        : 4; /* The Pad Termination state defines the different weak pull-up 
                                and pull-down settings that are supported by the buffer.
                                [3] 0 = Pulldown, 1 = Pullup.
                                [2] = 1k pull up only. MSMV only.
                                [1] = 5k
                                [0] = 20k                                                         */
        int GPIOPM      : 4; /* This controls which function controls this particular Pad when
                                GPIO Enable = 0.                                                  */
        int GPIOEn      : 1; /* This enables GPIO mode and function in Pad Mode no longer controls
                                the Pad and the GPIO fields control the Pad.
                                0 = GPIO disabled and and function defined in Pad Mode controls
                                    the pad.
                                1 = GPIO enabled and Pad Register controls the pad                */
        int PStrVal     : 2; /* Termination strength setting for the MSMV CFIO buffer Family.
                                00 = 150 ohm Nominal
                                01 = 75 ohm Nominal
                                10 = 50 ohm Nominal
                                11 = 35 ohm Nominal
                                This field is only used for the following Cherryview pins:
                                CX_PRDY_B, CX_PRDY_B_2,TCK, TRST_B, TMS, TDI, TDO, TDO_2, 
                                SVID0_ALERT_B, SVID0_DATA, SVID0_CLK, PROCHOT_B                   */
        int NSlewVal    : 2; /* This field is only used for the following Cherryview pins:
                                CX_PRDY_B, CX_PRDY_B_2, TCK, TRST_B, TMS, TDI, TDO, TDO_2,
                                SVID0_ALERT_B, SVID0_DATA, SVID0_CLK, PROCHOT_B                   */
        int GPIOCfg     : 3; /* 0 = GPIO (TX Enabled and RX Enabled)
                                1 = GPO (TX Enabled and RX Disabled)
                                2 = GPI (TX Disabled and RX Enabled) 
                                3 = Hi-Z (TX Disabled and RX Disabled) Others = Reserved          */
        int GPIO_Light  : 1; /* This mode disables the following fields and muxes function 1 
                                directly out of the pad.
                                (Pmode, GPIOCfg, GPIOEn,XTXEnCfg, GFCfg, IOSState, InvRXTX, ODEn)
                                0 = Light Mode Disabled
                                1 = Light Mode Enabled                                            */
        int NStrVal     : 2; /* Termination strength setting for the MSMV CFIO buffer Family.
                                00 = 150 ohm Nominal
                                01 = 75 ohm Nominal
                                10 = 50 ohm Nominal
                                11 = 35 ohm Nominal
                                This field is only used for the following Cherryview pins:
                                CX_PRDY_B, CX_PRDY_B_2, TCK, TRST_B, TMS, TDI, TDO, TDO_2,
                                SVID0_ALERT_B, SVID0_DATA, SVID0_CLK, PROCHOT_B                   */
        int PSlewVal    : 2; /* This field is only used for the following Cherryview pins:       
                                CX_PRDY_B,CX_PRDY_B_2, TCK, TRST_B, TMS, TDI, TDO, 
                                TDO_2,SVID0_ALERT_B, SVID0_DATA,SVID0_CLK, PROCHOT_B              */
        int GFDebSelect : 1; /* Chooses between filtering using glitch filter                    
                                (pulses < ~120nS filtered)and the debounce logic
								0 = Glitch Filter Logic
								1 = Debounce Filter                               */
        int GPIOTXState : 1; /* GPIO TX State is register  that controls the output driven by the 
                                pad. This only applies when GPIO Config is set to enable the     
                                TX buffer.                                                        */
        int GPIORXState : 1; /* GPIO RX State, this is the  current state of the PAD              */

#endif                                                                  
    }field;
    int val;
}IO_CFG0_t;


typedef union 
{
    struct 
    {
#ifdef LITTLE_ENDIEN
        int IntWakeCfg       : 3; /* Configure the edge detection logic that is used to trigger
                                     interrupts and wakes. 
                                     Pad must be configured as an input. For active low level
                                     interrupts then invert the RX Data (i.e. InvRXTX).
                                     0 = Interrupt/Wake Disable (Disabled and no Pad State toggles
                                         trigger the Wake/Interrupt logic)
                                     1 = Falling Edge Detect Interrupt/Wake
                                     2 = Rising Edge Detect Interrupt/Wake
                                     3 = Falling or Rising Edge Detect Interrupt/Wake 
                                     4 = Level Interrupt/Wake (Bypass edge detect logic and Pad
                                         state is directly toggling the Wake/Interrupt logic)
                                     Others = Reserved                                            */
        int ODEn             : 1; /* Open Drain Enable muxes TX Data to output 0 and TX Enable
                                     is connected to TX Data. This allows only zero to be driven
                                     on the output buffer and ones are pulled up with either an
                                     external or internal pullup.
                                     0 = Disabled
                                     1 = Enabled                                                  */
        int  InvRXTX         : 4; /* These bits invert the RX/TX Data and RX/TX Enables to the
                                     CFIO buffer. Typically this is used to indicate an active
                                     high or low signal. This setting affects all modes.
                                     0 = No Inversion
                                     1 = Inversion Enabled
                                         [0] RX Enable
                                         [1] TX Enable
                                         [2] RX Data
                                         [3] TX Data                                              */
        int  reserved5       : 2;
        int  IOSTerm         : 2; /* IOSTerm defines the behaviour of the termination logic when
                                     the IO Standby state has been riggered. The value of the
                                     termination is determined in Term[2:0].
                                     0 = Same as state specified in Term.
                                     1 = Disable Pullup and Pulldown 
                                     2 = Enable Pulldown
                                     3 = Enable Pullup                                            */
        int  IOSState        : 4; /* The I/O Standby State defines which state the pin should be
                                     parked in when the I/O is in a standby state.
                                     0 = Latch last value driven on TX, TX   Enable and RX Enable
                                     1 = Drive 0 with RX disabled
                                     2 = Drive 1 with RX disabled
                                     3 = Drive 0 with RX enabled
                                     4 = Drive 1 with RX enabled
                                     5 = Hi-Z
                                     6 = TX Disabled and RX Enabled (i.e. wake or interrupt)
                                     Others = Reserved                                            */
        int  analogmuxen     : 1; /* Enables the analog mux at the Pad SIP                        */
        int  odten           : 1; /* Controlled by Family Register.                               */
        int  odtupdn         : 1; /* Controlled by Family Register.                               */
        int  hsmode          : 1; /* Controlled by Family Register.                               */
        int  vp18Mode        : 1; /* Controlled by Family Register.                               */
        int  CurrSrcStr      : 3; /* Current Source Strength configuration bits for I2C High
                                     Speed Mode. Only supported on ULPMSMV CFIO Type.
                                     Connected to crtstr[2:0] CFIO signal.                        */
        int  hysctl          : 2; /* Controls VIH to VIL hysteresis. See Description in CFIO HIP
                                     for values. This control is only relevant to GPIO_SUS,
                                     PREQ, JTAG and SVID pins. All other pins use the
                                     family control register.                                     */
        int  parkmodeen_b    : 1; /* Controlled by Family Register.                               */
        int  csen            : 1; /* MSMV Buffers Only Enables the Current Source for I2C HS Mode.*/
        int  svid_1v_od_en   : 1; /* MSMV Buffers Only Changes VIH level to support 1V open
                                     drain mode for sVID.                                         */
        int  reserved6       : 1; /* reserved6                                                    */
        int  PadRstCfg       : 1; /* This register controls which reset is used to reset the
                                     specified Pad Register fields.
                                     0 = Powergood (i.e. sticky reset)
                                     1 = GPIO Reset (i.e. warm reset)                             */
        int  CfgLock         : 1; /* CfgLock locks the pad from being configured for the 
                                     specified fields. The following register fields will be RO 
                                     when this bit is set.
                                     0 = Unlocked
                                     1 = Lock the following fields as RO PadRstCfg, CFIOPadCfg,
                                         IOState, IOSTerm, InvRXTX, , IntWakeCfg, IntSel, GFCfg,
                                         RXTXEnCfg, Term, Pmode,  GPIOEn, GPIOCfg                 */
#else
        int  CfgLock         : 1; /* CfgLock locks the pad from being configured for the 
                                     specified fields. The following register fields will be RO 
                                     when this bit is set.
                                     0 = Unlocked
                                     1 = Lock the following fields as RO PadRstCfg, CFIOPadCfg,
                                         IOState, IOSTerm, InvRXTX, , IntWakeCfg, IntSel, GFCfg,
                                         RXTXEnCfg, Term, Pmode,  GPIOEn, GPIOCfg                 */
        int  PadRstCfg       : 1; /* This register controls which reset is used to reset the
                                     specified Pad Register fields.
                                     0 = Powergood (i.e. sticky reset)
                                     1 = GPIO Reset (i.e. warm reset)                             */
        int  reserved6       : 1; /* reserved6                                                    */
        int  svid_1v_od_en   : 1; /* MSMV Buffers Only Changes VIH level to support 1V open
                                     drain mode for sVID.                                         */
        int  csen            : 1; /* MSMV Buffers Only Enables the Current Source for I2C HS Mode.*/
        int  parkmodeen_b    : 1; /* Controlled by Family Register.                               */
        int  hysctl          : 2; /* Controls VIH to VIL hysteresis. See Description in CFIO HIP
                                     for values. This control is only relevant to GPIO_SUS,
                                     PREQ, JTAG and SVID pins. All other pins use the
                                     family control register.                                     */
        int  CurrSrcStr      : 3; /* Current Source Strength configuration bits for I2C High
                                     Speed Mode. Only supported on ULPMSMV CFIO Type.
                                     Connected to crtstr[2:0] CFIO signal.                        */
        int  vp18Mode        : 1; /* Controlled by Family Register.                               */
        int  hsmode          : 1; /* Controlled by Family Register.                               */
        int  odtupdn         : 1; /* Controlled by Family Register.                               */
        int  odten           : 1; /* Controlled by Family Register.                               */
        int  analogmuxen     : 1; /* Enables the analog mux at the Pad SIP                        */
        int  IOSState        : 4; /* The I/O Standby State defines which state the pin should be
                                     parked in when the I/O is in a standby state.
                                     0 = Latch last value driven on TX, TX   Enable and RX Enable
                                     1 = Drive 0 with RX disabled
                                     2 = Drive 1 with RX disabled
                                     3 = Drive 0 with RX enabled
                                     4 = Drive 1 with RX enabled
                                     5 = Hi-Z
                                     6 = TX Disabled and RX Enabled (i.e. wake or interrupt)
                                     Others = Reserved                                            */
        int  IOSTerm         : 2; /* IOSTerm defines the behaviour of the termination logic when
                                     the IO Standby state has been riggered. The value of the
                                     termination is determined in Term[2:0].
                                     0 = Same as state specified in Term.
                                     1 = Disable Pullup and Pulldown 
                                     2 = Enable Pulldown
                                     3 = Enable Pullup                                            */
        int  reserved5       : 2;
        int  InvRXTX         : 4; /* These bits invert the RX/TX Data and RX/TX Enables to the
                                     CFIO buffer. Typically this is used to indicate an active
                                     high or low signal. This setting affects all modes.
                                     0 = No Inversion
                                     1 = Inversion Enabled
                                         [0] RX Enable
                                         [1] TX Enable
                                         [2] RX Data
                                         [3] TX Data                                              */
        int ODEn             : 1; /* Open Drain Enable muxes TX Data to output 0 and TX Enable
                                     is connected to TX Data. This allows only zero to be driven
                                     on the output buffer and ones are pulled up with either an
                                     external or internal pullup.
                                     0 = Disabled
                                     1 = Enabled                                                  */
        int IntWakeCfg       : 3; /* Configure the edge detection logic that is used to trigger
                                     interrupts and wakes. 
                                     Pad must be configured as an input. For active low level
                                     interrupts then invert the RX Data (i.e. InvRXTX).
                                     0 = Interrupt/Wake Disable (Disabled and no Pad State toggles
                                         trigger the Wake/Interrupt logic)
                                     1 = Falling Edge Detect Interrupt/Wake
                                     2 = Rising Edge Detect Interrupt/Wake
                                     3 = Falling or Rising Edge Detect Interrupt/Wake 
                                     4 = Level Interrupt/Wake (Bypass edge detect logic and Pad
                                         state is directly toggling the Wake/Interrupt logic)
                                     Others = Reserved                                            */
#endif
    }field;
    int val;
} IO_CFG1_t;

/*define list of MPEG addresses  */
typedef struct
{
    unsigned int clkAddr;
    unsigned int dataAddr;
    unsigned int syncAddr;
    unsigned int davAddr;
}mpegPadAddress_t;

/*define list of LED addresses  */
typedef struct
{
    unsigned int ledCnfg0;
    unsigned int ledCnfg1;
}ledsPadAddress_t;

/*define list of GBE addresses  */
typedef struct
{
    unsigned int intr;
    unsigned int txData0;
    unsigned int txData1;
    unsigned int txData2;
    unsigned int txData3;
    unsigned int txClk;
    unsigned int txCtl;
    unsigned int rxData0;
    unsigned int rxData1;
    unsigned int rxData2;
    unsigned int rxData3;
    unsigned int rxClk;
    unsigned int rxCtl;
    unsigned int mdio_cnfg0;
    unsigned int mdio_cnfg1;
    unsigned int mdc;
}gbePadAddress_t;

/****************************************************************************/
/*                          LOCAL DECLERATIONS                              */
/****************************************************************************/
/*This structure hold all TS relevant address for configuring the pads*/
static mpegPadAddress_t mpegPadAddress []= {
    /*TS0*/
    { GPIO_SOUTHEAST_REGISTERS_TS0_CLK_PAD_CFG0_MSGREGADDR ,       
      GPIO_SOUTHEAST_REGISTERS_TS0_DATA_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS0_SYNC_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS0_DAV_PAD_CFG0_MSGREGADDR  },
    /*TS1*/
    { GPIO_SOUTHEAST_REGISTERS_TS1_CLK_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS1_DATA_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS1_SYNC_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS1_DAV_PAD_CFG0_MSGREGADDR  },
    /*TS2*/
    { GPIO_SOUTHEAST_REGISTERS_TS2_CLK_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS2_DATA_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS2_SYNC_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS2_DAV_PAD_CFG0_MSGREGADDR  },
    /*TS3*/
    { GPIO_SOUTHEAST_REGISTERS_TS3_CLK_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS3_DATA_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS3_SYNC_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS3_DAV_PAD_CFG0_MSGREGADDR  },
    /*TS4*/
    { GPIO_SOUTHEAST_REGISTERS_TS4_CLK_CCARD_MICLK_PAD_CFG0_MSGREGADDR  ,
      GPIO_SOUTHEAST_REGISTERS_TS4_DATA_CCARD_MDO_0_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS4_SYNC_CCARD_MISTRT_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS4_DAV_CCARD_MDI_0_PAD_CFG0_MSGREGADDR   },
    /*TS5*/
    { GPIO_SOUTHEAST_REGISTERS_TS5_CLK_CCARD_MOCLK_PAD_CFG0_MSGREGADDR  ,
      GPIO_SOUTHEAST_REGISTERS_TS5_DATA_CCARD_MDO_1_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS5_SYNC_CCARD_MOSTRT_PAD_CFG0_MSGREGADDR,
      GPIO_SOUTHEAST_REGISTERS_TS5_DAV_CCARD_MDI_1_PAD_CFG0_MSGREGADDR   },
    /*TS6*/
    { GPIO_SOUTHEAST_REGISTERS_TS6_CLK_CCARD_MDI_2_PAD_CFG0_MSGREGADDR  ,
      GPIO_SOUTHEAST_REGISTERS_TS6_DATA_CCARD_MDO_3_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS6_SYNC_CCARD_MDO_2_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS6_DAV_CCARD_MDI_3_PAD_CFG0_MSGREGADDR   },
    /*TS7*/
    { GPIO_SOUTHEAST_REGISTERS_TS7_CLK_CCARD_MDI_4_PAD_CFG0_MSGREGADDR  ,
      GPIO_SOUTHEAST_REGISTERS_TS7_DATA_CCARD_MDO_5_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS7_SYNC_CCARD_MDO_4_PAD_CFG0_MSGREGADDR ,
      GPIO_SOUTHEAST_REGISTERS_TS7_DAV_CCARD_MDI_5_PAD_CFG0_MSGREGADDR   },
};

/*This structure hold all GBE relevant address for configuring the pads*/
static gbePadAddress_t gbePadAddress []= 
{
    {
     GPIO_WEST_REGISTERS_GBE_0_TXDATA_0_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_TXDATA_1_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_TXDATA_2_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_TXDATA_3_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_TXCLK_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_0_TXCTL_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_0_RXDATA_0_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_RXDATA_1_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_RXDATA_2_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_RXDATA_3_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_0_RXCLK_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_0_RXCTL_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_0_MDIO_PAD_CFG0_MSGREGADDR    ,
     GPIO_WEST_REGISTERS_GBE_0_MDIO_PAD_CFG1_MSGREGADDR    ,
     GPIO_WEST_REGISTERS_GBE_0_MDC_PAD_CFG0_MSGREGADDR     },
    {
     GPIO_WEST_REGISTERS_GBE_1_INT_PAD_CFG0_MSGREGADDR     ,
     GPIO_WEST_REGISTERS_GBE_1_TXDATA_0_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_TXDATA_1_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_TXDATA_2_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_TXDATA_3_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_TXCLK_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_1_TXCTL_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_1_RXDATA_0_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_RXDATA_1_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_RXDATA_2_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_RXDATA_3_PAD_CFG0_MSGREGADDR,
     GPIO_WEST_REGISTERS_GBE_1_RXCLK_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_1_RXCTL_PAD_CFG0_MSGREGADDR   ,
     GPIO_WEST_REGISTERS_GBE_1_MDIO_PAD_CFG0_MSGREGADDR    ,
     GPIO_WEST_REGISTERS_GBE_1_MDIO_PAD_CFG1_MSGREGADDR    ,
     GPIO_WEST_REGISTERS_GBE_1_MDC_PAD_CFG0_MSGREGADDR     ,
    }
};
/*This structure hold all LEDs relevant address for configuring the pads*/
static ledsPadAddress_t ledsPadAddress []=
{
    {GPIO_NORTHEAST_REGISTERS_LED_1_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_1_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_2_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_2_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_3_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_3_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_4_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_4_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_5_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_5_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_6_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_6_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_7_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_7_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_8_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_8_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_9_PAD_CFG0_MSGREGADDR ,
     GPIO_NORTHEAST_REGISTERS_LED_9_PAD_CFG1_MSGREGADDR  },
    {GPIO_NORTHEAST_REGISTERS_LED_10_PAD_CFG0_MSGREGADDR,
     GPIO_NORTHEAST_REGISTERS_LED_10_PAD_CFG1_MSGREGADDR },
    {GPIO_NORTHEAST_REGISTERS_LED_11_PAD_CFG0_MSGREGADDR,
     GPIO_NORTHEAST_REGISTERS_LED_11_PAD_CFG1_MSGREGADDR },
    {GPIO_NORTHEAST_REGISTERS_LED_12_PAD_CFG0_MSGREGADDR,
     GPIO_NORTHEAST_REGISTERS_LED_12_PAD_CFG1_MSGREGADDR }
};

/****************************************************************************/
/*                          GLOBAL FUNCTIONS                                */
/****************************************************************************/

/**************************************************************************
 * int IO_CTRL_configMPEG(int tsNumber , IO_CTRL_mpegDir_e inOut)         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure requested TS for      *
 *               out                                                      *
 *  INPUT: tsNumber = TS number to configure                              * 
 *         inOut    = MPEG out or in                                      * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configMPEG(int tsNumber, IO_CTRL_mpegDir_e inOut)
{
    int ret = IOCNFG_OK;
    IO_CFG0_t ioPadCnfg0;

    IO_CTRL_DEBUG_OUTPUT("Request to configure TS[%d] as MPEG %s\n",tsNumber,inOut?"IN":"OUT");
    /*Enable relevant PAD */
    if (inOut == IO_MPEG_OUT)
    {
        ioPadCnfg0.field.GPIOPM = 2;
    }
    else //IN
    {
        ioPadCnfg0.field.GPIOPM = 1;
    }
    /*Configure clock*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT, mpegPadAddress[tsNumber].clkAddr, 
                                                    ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) <0 )
    {
        IO_CTRL_ERROR_OUTPUT("fail to modify TS[%d] clock\n",tsNumber);
        ret = IOCNFG_FAIL;
    }
    /*Configure data*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT, mpegPadAddress[tsNumber].dataAddr, 
                                                    ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) <0 )
    {
        IO_CTRL_ERROR_OUTPUT("fail to modify TS[%d] data\n",tsNumber);
        ret = IOCNFG_FAIL;
    }
    /*Configure sync*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT, mpegPadAddress[tsNumber].syncAddr, 
                                                    ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) <0 )
    {
        IO_CTRL_ERROR_OUTPUT("fail to modify TS[%d] sync\n",tsNumber);
        ret = IOCNFG_FAIL;
    }
    /*Configure DAV*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT, mpegPadAddress[tsNumber].davAddr, 
                                                    ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) <0 )
    {
        IO_CTRL_ERROR_OUTPUT("fail to modify TS[%d] DAV\n",tsNumber);
        ret = IOCNFG_FAIL;
    }
    return ret;
}
EXPORT_SYMBOL(IO_CTRL_configMPEG);
/**************************************************************************
 * int IO_CTRL_configMPEGoutAllTS(IO_CTRL_mpegDir_e inOut)                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure all TS for            *
 *               out/ in                                                  * 
 *  INPUT: inOut    = MPEG out or in                                      * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configMPEGoutAllTS(IO_CTRL_mpegDir_e inOut)
{
    int tsID;

    IO_CTRL_DEBUG_OUTPUT("Request to configure all TS as MPEG %s\n",inOut?"IN":"OUT");

    /*For all TS*/
    for (tsID = 0; tsID <= PUAM7_TS_MAX_NUMBER ; tsID++)
    {
        if (IO_CTRL_configMPEG(tsID, inOut) != IOCNFG_OK)
        {
            return IOCNFG_FAIL;
        }
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_configMPEGoutAllTS);
/**************************************************************************
 * int IO_CTRL_configDocsisUart(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the Docsis Uart       *
 *               (UART - 2)                                               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configDocsisUart(void)
{
    IO_CFG0_t ioPadCnfg0;

    IO_CTRL_DEBUG_OUTPUT("Request to configure Docsis UART(2)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    ioPadCnfg0.field.GPIOCfg = 3;
    /*Config Rx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DOCSIS_UART_RXD_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DOCSIS UART Rx pad\n");
        return IOCNFG_FAIL;
    }
    /*Config Tx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DOCSIS_UART_TXD_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DOCSIS UART Rx pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configDocsisUart);

#ifdef CONFIG_INTEL_UART_ENABLE_CONTROL
/**************************************************************************
 * int IO_CTRL_configDocsisUartCtrl(int enable)                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to enable/disable the Docsis Uart  *
 *               (UART - 2)                                               *
 *                                                                        *
 *  INPUT: enable - 0(disable) / 1(enable)                                *
 *                                                                        *
 *  RETURN: fail / success                                                *
 **************************************************************************/
int IO_CTRL_configDocsisUartCtrl(int enable)
{
    IO_CFG0_t ioPadCnfg0;

    IO_CTRL_DEBUG_OUTPUT("Request controlDocsisUart enable=%d\n", enable);

    /*configure cnfg0 according to input parameter*/
    if (enable == 0)
    {
        /*Disable UART*/
        ioPadCnfg0.field.GPIOEn = 1;
        ioPadCnfg0.field.GPIOCfg = 3;
    }
    else
    {
        /*Enable UART*/
        ioPadCnfg0.field.GPIOEn = 0;
        ioPadCnfg0.field.GPIOCfg = 0;
    }
   
     /*Config Tx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                  GPIO_SOUTHEAST_REGISTERS_DOCSIS_UART_TXD_PAD_CFG0_MSGREGADDR,
                                  ioPadCnfg0.val, 0, PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to %s DOCSIS UART Tx\n", IO_CTRL_ENABLE_VAL2STR(enable));
        return IOCNFG_FAIL;
    }
    /*Config Rx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                  GPIO_SOUTHEAST_REGISTERS_DOCSIS_UART_RXD_PAD_CFG0_MSGREGADDR,
                                  ioPadCnfg0.val, 0, PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to %s DOCSIS UART Rx\n", IO_CTRL_ENABLE_VAL2STR(enable));
        return IOCNFG_FAIL;
    }
   
    return IOCNFG_OK;
}

/**************************************************************************
 * int IO_CTRL_readDocsisUartCtrl(int *enable)                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to read Docsis Uart control        *
 *  configuration                                                         *
 *                                                                        *
 *  OUTPUT: enable - 0(disable) / 1(enable)                               *
 *                                                                        *
 *  RETURN: fail / success                                                *
 **************************************************************************/
int IO_CTRL_readDocsisUartCtrl(int *enable)
{
    IO_CFG0_t ioPadCnfg0;

    if (enable == NULL)
        return IOCNFG_FAIL;

    *enable = 0;

    if (iosfsb_read(IOSFSB_GPIOSE_PORT, IOSFSB_OP_CRREAD,
                    GPIO_SOUTHEAST_REGISTERS_DOCSIS_UART_TXD_PAD_CFG0_MSGREGADDR, (unsigned int *)&ioPadCnfg0, 0))
    {
        IO_CTRL_ERROR_OUTPUT("fail to read DOCSIS UART Tx control status\n");
        return IOCNFG_FAIL;
    }

    if (!((ioPadCnfg0.field.GPIOEn == 1) && (ioPadCnfg0.field.GPIOCfg == 3)))
    {
        /* Tx is enabled */
        *enable |= 1;
    }

    if (iosfsb_read(IOSFSB_GPIOSE_PORT, IOSFSB_OP_CRREAD,
                    GPIO_SOUTHEAST_REGISTERS_DOCSIS_UART_RXD_PAD_CFG0_MSGREGADDR, (unsigned int *)&ioPadCnfg0, 0))
    {
        IO_CTRL_ERROR_OUTPUT("fail to read DOCSIS UART Rx control status\n");
        return IOCNFG_FAIL;
    }

    if (!((ioPadCnfg0.field.GPIOEn == 1) && (ioPadCnfg0.field.GPIOCfg == 3)))
    {
        /* Rx is enabled */
        *enable |= 1;
    }

    return IOCNFG_OK;
}
#endif

/**************************************************************************
 * int IO_CTRL_configDectUart(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the DECT Uart         *
 *               (UART - 0)                                               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configDectUart(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure DECT UART(0)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    ioPadCnfg0.field.GPIOCfg = 3;
    /*Config CTS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_CTS_B_DECT_SPI_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT UART CTS pad\n");
        return IOCNFG_FAIL;
    }
    /*Config RTS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_RTS_B_DECT_SPI_CS_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT UART RTS pad\n");
        return IOCNFG_FAIL;
    }
    /*Config Rx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_RX_DECT_SPI_MISO_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT UART Rx pad\n");
        return IOCNFG_FAIL;
    }
    /*Config Tx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_TX_DECT_SPI_MOSI_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT UART Tx pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configDectUart);
/**************************************************************************
 * int IO_CTRL_configBBUUart(void)                                        *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the BBU Uart          *
 *               (UART - 1)                                               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configBBUUart(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure BBU UART(1)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    ioPadCnfg0.field.GPIOCfg = 3;
    /*Config CTS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_CTS_B_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU UART CTS pad\n");
        return IOCNFG_FAIL;
    }
    /*Config RTS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_RTS_B_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU UART RTS pad\n");
        return IOCNFG_FAIL;
    }
    /*Config Rx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_RX_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU UART Rx pad\n");
        return IOCNFG_FAIL;
    }
    /*Config Tx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_TX_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU UART Tx pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configBBUUart);
/**************************************************************************
 * int IO_CTRL_configUART3(void)                                          *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the Uart3             *
 *               (Pinout UART2)                                           *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configUART3(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;
    IO_CTRL_DEBUG_OUTPUT("Request to configure UART3\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 0;
    /*Config CTS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_CTS_B_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 CTS (cnfg0) pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_CTS_B_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 CTS (cnfg1) pad\n");
        return IOCNFG_FAIL;
    }
    /*Config RTS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_RTS_B_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 RTS (cnfg0) pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_RTS_B_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 RTS (cnfg1) pad\n");
        return IOCNFG_FAIL;
    }
    /*Config Rx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_RXD_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 Rx (cnfg0) pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_RXD_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 Rx (cnfg1) pad\n");
        return IOCNFG_FAIL;
    }
    /*Config Tx*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_TXD_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 Tx (cnfg0) pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_UART2_TXD_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config UART3 Tx (cnfg1) pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configUART3);
/**************************************************************************
 * int IO_CTRL_configI2C0(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the I2C0              *
 *               (Legacy D3.0 Tuners)                                     *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configI2C0(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;
    IO_CTRL_DEBUG_OUTPUT("Request to configure I2C0(Legacy D3.0 Tuners)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg1.field.InvRXTX = 0;
    /*Config SCL*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SCL_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C0 SCL cfg0 pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SCL_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C0 SCL cfg1 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config SDA*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SDA_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C0 SDA cfg0 pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SDA_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C0 SDA cfg1 pad\n");
        return IOCNFG_FAIL;
    }
    
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_configI2C0);
/**************************************************************************
 * int IO_CTRL_configI2C1(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the I2C1              *
 *               (Odin Tuner BBU)                                         *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configI2C1(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;
    IO_CTRL_DEBUG_OUTPUT("Request to configure I2C1 (Odin Tuner BBU)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg1.field.InvRXTX = 0;
    /*Config SCL*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C1_SCL_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C1 SCL cfg0 pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C1_SCL_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C1 SCL cfg1 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config SDA*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C1_SDA_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C1 SDA cfg0 pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C1_SDA_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C1 SDA cfg1 pad\n");
        return IOCNFG_FAIL;
    }
    
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_configI2C1);
/**************************************************************************
 * int IO_CTRL_configI2C6(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the I2C6              *
 *               (not defined muxed with ATOM I2C3)                       *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configI2C6(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;
    IO_CTRL_DEBUG_OUTPUT("Request to configure I2C6\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg1.field.InvRXTX = 0;
    /*Config SCL*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_I2C3_SCL_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C6 SCL cfg0 pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_I2C3_SCL_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C6 SCL cfg1 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config SDA*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_I2C3_SDA_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C6 SDA cfg0 pad\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_I2C3_SDA_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config  I2C6 SDA cfg1 pad\n");
        return IOCNFG_FAIL;
    }
    
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_configI2C6);
/**************************************************************************
 * int IO_CTRL_configDectSPI(void)                                        *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the DECT SPI          *
 *               (Codec 0)                                                *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configDectSPI(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure DECT SPI\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_CTS_B_DECT_SPI_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT SPI CLK pad\n");
        return IOCNFG_FAIL;
    }
    /*Config CS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_RTS_B_DECT_SPI_CS_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT SPI CS pad\n");
        return IOCNFG_FAIL;
    }
    /*Config MISO*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_RX_DECT_SPI_MISO_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT SPI MISO pad\n");
        return IOCNFG_FAIL;
    }
    /*Config MOSI*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_DECT_UART_TX_DECT_SPI_MOSI_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config DECT SPI MOSI pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configDectSPI);
/**************************************************************************
 * int IO_CTRL_configCodec1(void)                                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the Codec1            *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configCodec1(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure CODEC 1\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 CLK pad\n");
        return IOCNFG_FAIL;
    }
    /*Config CS0*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_CS0_N_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 CS0 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config CS1*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_CS1_N_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 CS1 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config data in */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_DATA_IN_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 data in pad\n");
        return IOCNFG_FAIL;
    }
    /*Config data out*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_DATA_OUT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 data out pad\n");
        return IOCNFG_FAIL;
    }
    /*Config int*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_INT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 int pad\n");
        return IOCNFG_FAIL;
    }
    ioPadCnfg0.field.GPIOEn = 1;
    /*Config reset0 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_RESET0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 reaset 0 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config reset 1*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_RESET1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config CODEC 1 reset 1 pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configCodec1);

/**************************************************************************
 * int IO_CTRL_configSSI(void)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the SSI               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configSSI(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure SSI\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;

    /*Config data out */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_DATA_OUT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SSI data out pad\n");
        return IOCNFG_FAIL;
    }

    /*Config data in */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_DATA_IN_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SSI data in pad\n");
        return IOCNFG_FAIL;
    }
    /*Config slave clock*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_RESET1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SSI slave clock pad\n");
        return IOCNFG_FAIL;
    }

    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SSI CLK pad\n");
        return IOCNFG_FAIL;
    }
    ioPadCnfg0.field.GPIOEn = 1;
    /*Config reset 0*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_RESET0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SSI reaset 0 pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configSSI);

/**************************************************************************
 * int IO_CTRL_configZDS(void)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the ZDS               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configZDS(int reset_reason)
{
    IO_CFG0_t ioPadCnfg0;
    printk("Request to configure ZDS, reset reason is =%d\n", reset_reason);
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;

    if ((reset_reason == RESET_TYPE_COLD_BOOT) || (reset_reason == RESET_TYPE_COLD_RESET))
    {
        /* Power cycle, we need to set the clock to enable*/
        ioPadCnfg0.field.GPIOEn = 0;
    }
    else
    {
        /* Soft reset, we need to set the clock to disable*/
        ioPadCnfg0.field.GPIOEn = 1;
    }
    
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config ZDS CLK pad\n");
        return IOCNFG_FAIL;
    }

    ioPadCnfg0.field.GPIOEn = 0;
    /*Config Zsync*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_CS0_N_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config ZDS Zsync pad\n");
        return IOCNFG_FAIL;
    }
    /*Config data in */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_DATA_IN_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config ZDS data in pad\n");
        return IOCNFG_FAIL;
    }
    /*Config data out */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_CODEC_DATA_OUT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config ZDS data out pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configZDS);
/**************************************************************************
 * int IO_CTRL_configTDM(IO_CTRL_numTDM_e numTDM)                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the requested TDM     * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configTDM(IO_CTRL_numTDM_e numTDM)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;
    IO_CTRL_DEBUG_OUTPUT("Request to configure TDM%d\n",numTDM);
    /*configure cnfg0*/
    if (numTDM == IO_TDM1)
    {
        ioPadCnfg0.field.GPIOPM = 2;
    }
    else
    {
        ioPadCnfg0.field.GPIOPM = 1;
    }
    ioPadCnfg1.field.InvRXTX = 0;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg0 CLK pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_CLK_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg1 CLK pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    /*Config FS*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_FS_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg0 FS pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_FS_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg1 FS pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    /*Config RX */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_RX_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg0 RX pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_RX_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg1 RX pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    /*Config TX */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_TX_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg0 TX pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TDM_TX_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config TDM%d cnfg1 TX pad\n",numTDM);
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configTDM);
/**************************************************************************
 * int IO_CTRL_configGBE(IO_CTRL_numGBE_e numGBE)                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the requested GBE     * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configGBE(IO_CTRL_numGBE_e numGBE)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure GBE%d\n",numGBE);
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    ioPadCnfg0.field.GPIOCfg = 3;
    /*Config int*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].txData0,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d int pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config tx data0 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].txData0,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d tx data0 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config tx data1 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].txData1,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d tx data1 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config tx data2 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].txData2,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d tx data2 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config tx data3 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].txData3,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d tx data3 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config tx clk */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].txClk,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d tx clk pad\n",numGBE);
        return IOCNFG_FAIL;
    }
        /*Config tx ctl */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].txCtl,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d tx ctl pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config rx data0 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].rxData0,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d rx data0 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config rx data1 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].rxData1,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d rx data1 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config rx data2 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].rxData2,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d tx data2 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config rx data3 */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].rxData3,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d rx data3 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config rx clk */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].rxClk,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d rx clk pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config rx ctl */
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].rxCtl,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK| PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d rx ctl pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOCfg = 0;
    /*configure cnfg0*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config mdio*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].mdio_cnfg0,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d mdio cnfg0 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].mdio_cnfg1,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d mdio cnfg1 pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    /*Config mdc*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   gbePadAddress[numGBE].mdc,
                                   ioPadCnfg0.val, 0,PCR0_GPIOPM_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE%d mdc pad\n",numGBE);
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configGBE);
/**************************************************************************
 * int IO_CTRL_configTDM(IO_CTRL_numTDM_e numTDM)                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the RF FE SPI         * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configRfFeSpi(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure RF FE SPI\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    ioPadCnfg0.field.GPIOCfg = 3;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SPI_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE SPI CLK pad\n");
        return IOCNFG_FAIL;
    }
    /*Config CS0*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SPI_CS0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE SPI CS0 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config CS1*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SPI_CS1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE SPI CS1 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config MISO*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SPI_MISO_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE SPI MISO pad\n");
        return IOCNFG_FAIL;
    }
    /*Config MOSI*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SPI_MOSI_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE SPI MOSI pad\n");
        return IOCNFG_FAIL;
    }
    /*Config INT*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SPI_INT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE SPI INT pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configRfFeSpi);
/**************************************************************************
 * int IO_CTRL_configEPGA(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to coinfigure the EPGA             * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configEPGA(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure EPGA\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    ioPadCnfg0.field.GPIOCfg = 3;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_EPGA_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config EPGA CLK pad\n");
        return IOCNFG_FAIL;
    }
    /*Config PWDN*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_EPGA_PWDN_N_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config EPGA PWDN pad\n");
        return IOCNFG_FAIL;
    }
    /*Config DATA*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_EPGA_DATA_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config EPGA DATA pad\n");
        return IOCNFG_FAIL;
    }
    /*Config WE_N*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_EPGA_WE_N_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config EPGA WE_N pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configEPGA);
/**************************************************************************
 * int IO_CTRL_configBBUSpi(IO_CTRL_numTDM_e numTDM)                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the BBU SPI           * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configBBUSpi(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure BBU SPI\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_LED_1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU SPI CLK pad\n");
        return IOCNFG_FAIL;
    }
    /*Config CS0*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_LED_2_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK| PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU SPI CS0 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config MISO*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_LED_3_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU SPI MISO pad\n");
        return IOCNFG_FAIL;
    }
    /*Config MOSI*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_LED_4_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU SPI MOSI pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configBBUSpi);
/**************************************************************************
 * int IO_CTRL_configBBUSpi(IO_CTRL_numTDM_e numTDM)                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure all BBU elements      * 
 *               (PWM, BBU ENABLE 0,1, EPROM1,2)                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configBBU(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CTRL_DEBUG_OUTPUT("Request to configure BBU ALL\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    ioPadCnfg0.field.GPIOCfg = 3;
    /*Config PWM*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BATTERY_PWM_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU PWM pad\n");
        return IOCNFG_FAIL;
    }
    /*Config ENABLE 1*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_ENABLE_1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU ENABLE 1 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config ENABLE 2*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_ENABLE_1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU ENABLE 2 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config EPROM 1*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_ONE_WIRE_EPROM1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU EPROM 1 pad\n");
        return IOCNFG_FAIL;
    }
    /*Config EPROM 2*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_ONE_WIRE_EPROM2_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK | PCR0_GPIOCFG_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config BBU EPROM 2 pad\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;

}
EXPORT_SYMBOL(IO_CTRL_configBBU);
/**************************************************************************
 * int IO_CTRL_SGMII1Int(void)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure SGMII 1 interrupt     *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_SGMII1Int(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure SGMII 1 interrupt\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 0;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MPEG_RESET_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SGMII 1 interrupt cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MPEG_RESET_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SGMII 1 interrupt cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_SGMII1Int);
/**************************************************************************
 * int IO_CTRL_SGMII0Int(void)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure SGMII 0 interrupt     *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_SGMII0Int(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure SGMII 0 interrupt\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 0;
    /*Config CLK*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_SATA_DEVSLP0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SGMII 0 interrupt cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_SATA_DEVSLP0_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config SGMII 0 interrupt cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_SGMII0Int);
/**************************************************************************
 * int IO_CTRL_netGpio0FfFeSwitch0(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure FR FE switch 0        * 
 *               (Net GPIO 0)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio0FfFeSwitch0(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure RF FE switch 0 (net GPIO 0)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SWITCH0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE switch 0 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SWITCH0_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE switch 0 cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio0FfFeSwitch0);
/**************************************************************************
 * int IO_CTRL_netGpio1FfFeSwitch1(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure FR FE switch 1        * 
 *               (Net GPIO 1)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio1FfFeSwitch1(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure RF FE switch 1 (net GPIO 1)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SWITCH1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE switch 1 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_SWITCH1_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config RF FE switch 1 cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio1FfFeSwitch1);
/**************************************************************************
 * int IO_CTRL_netGpioMocaGpio(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure MOCA GPIO             * 
 *               (Net GPIO 2)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio2MocaGpio(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure MOCA GPIO (net GPIO 2)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MOCA_GPIO_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config MOCA GPIO cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MOCA_GPIO_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config MOCA GPIO cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio2MocaGpio);
/**************************************************************************
 * int IO_CTRL_netGpio3mpegReset(void)                                    *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure MPEG reset            * 
 *               (Net GPIO 3)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio3mpegReset(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure MPEG reset (net GPIO 3)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MPEG_RESET_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config MPEG reset cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MPEG_RESET_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config MPEG reset cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio3mpegReset);
/**************************************************************************
 * int IO_CTRL_netGpio4RfFeResetPad(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 4            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio4RfFeResetPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net GPIO 4\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_RESET_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 4 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_RF_FE_RESET_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 4cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio4RfFeResetPad);
/**************************************************************************
 * int IO_CTRL_netGpio5USMonitoring(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 5            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio5USMonitoring(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net GPIO 5\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MOCA_RESET_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 5 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MOCA_RESET_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 5 cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio5USMonitoring);
/**************************************************************************
 * int IO_CTRL_netGpio6MocaIntPad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (Moca interrupt)                                         * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6MocaIntPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net GPIO 6 (Moca interrupt)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MOCA_INT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (Moca interrupt) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MOCA_INT_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (Moca interrupt) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio6MocaIntPad);
/**************************************************************************
 * int IO_CTRL_netGpio6Led14(void)                                        *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (LED14)                                                  * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6Led14(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net GPIO 6 (LED14)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_LED_14_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (LED14) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_LED_14_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (LED14) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio6Led14);
/**************************************************************************
 * int IO_CTRL_netGpio6MMC2Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (MMC2 CLK)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6MMC2Clk(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net GPIO 6 (MMC2 CLK)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (MMC2 CLK) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_CLK_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (MMC2 CLK) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio6MMC2Clk);
/**************************************************************************
 * int IO_CTRL_netGpio6PcieClkReq2B(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (PCIE_CLKREQ2B)                                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6PcieClkReq2B(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net GPIO 6 (PCIE_CLKREQ2B)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_PCIE_CLKREQ2B_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (PCIE_CLKREQ2B) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_PCIE_CLKREQ2B_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 6 (PCIE_CLKREQ2B) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio6PcieClkReq2B);
/**************************************************************************
 * int IO_CTRL_netGpio7MpegIntPad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net PGIO 7            * 
 *               (MPEG interrupt)                                         * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio7MpegIntPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 7 (MPEG interrupt)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MPEG_INT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 7 (MPEG interrupt) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_MPEG_INT_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 7 (MPEG interrupt) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio7MpegIntPad);
/**************************************************************************
 * int IO_CTRL_netGpio7Ts4Clk(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net PGIO 7            * 
 *               (TS4 CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio7Ts4Clk(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 7 (TS4 CLK) \n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_CLK_CCARD_MICLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 7 (TS4 CLK) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_CLK_CCARD_MICLK_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 7 (TS4 CLK) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio7Ts4Clk);
/**************************************************************************
 * int IO_CTRL_netGpio7Ts0Clk(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net PGIO 7            * 
 *               (TS0 CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio7Ts0Clk(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 7 (TS0 CLK) \n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 7 (TS0 CLK) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_CLK_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 7 (TS0 CLK) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio7Ts0Clk);
/**************************************************************************
 * int IO_CTRL_netGpio8ExtIrq0Pad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 8            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio8ExtIrq0Pad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 8\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_EXT_IRQ_0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 8 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_EXT_IRQ_0_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 8 cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio8ExtIrq0Pad);
/**************************************************************************
 * int IO_CTRL_netGpio8Ts4Sync(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 8            * 
 *               (TS4 SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio8Ts4Sync(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 8 (TS4 SYNC)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_SYNC_CCARD_MISTRT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 8 (TS4 SYNC) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_SYNC_CCARD_MISTRT_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 8 (TS4 SYNC) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio8Ts4Sync);
/**************************************************************************
 * int IO_CTRL_netGpio8Ts0Sync(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 8            * 
 *               (TS0 SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio8Ts0Sync(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 8 (TS0 SYNC)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_SYNC_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 8 (TS0 SYNC) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_SYNC_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 8 (TS0 SYNC) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio8Ts0Sync);
/**************************************************************************
 * int IO_CTRL_netGpio9ExtIrq1Pad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9ExtIrq1Pad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 9\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_EXT_IRQ_1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 9 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_EXT_IRQ_1_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 9 cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio9ExtIrq1Pad);
/**************************************************************************
 * int IO_CTRL_netGpio9Ts4Dav(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *               (TS4 DEV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9Ts4Dav(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 9 (TS4 DEV)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_DAV_CCARD_MDI_0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 9 (TS4 DEV) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_DAV_CCARD_MDI_0_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 9 (TS4 DEV) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio9Ts4Dav);
/**************************************************************************
 * int IO_CTRL_netGpio9Ts0Dav(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *               (TS0 DEV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9Ts0Dav(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 9 (TS0 DEV)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_DAV_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 9 (TS0 DEV) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_DAV_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 9 (TS0 DEV) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio9Ts0Dav);
/**************************************************************************
 * int IO_CTRL_netGpio9PcieClkReq2B(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *               (PCIE_CLKREQ2B)                                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9PcieClkReq2B(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net GPIO 9 (PCIE_CLKREQ2B)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_PCIE_CLKREQ2B_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 9 (PCIE_CLKREQ2B) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_PCIE_CLKREQ2B_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net GPIO 9 (PCIE_CLKREQ2B) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio9PcieClkReq2B);
/**************************************************************************
 * int IO_CTRL_netGpio10Ts0Data(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (TS0_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10Ts0Data(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 10 (TS0_DATA)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_DATA_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (TS0_DATA) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS0_DATA_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (TS0_DATA) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio10Ts0Data);
/**************************************************************************
 * int IO_CTRL_netGpio10MMC2Cmd(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (MMC2_CMD)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10MMC2Cmd(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 10 (MMC2_CMD)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_CMD_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (MMC2_CMD) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_CMD_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (MMC2_CMD) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio10MMC2Cmd);
/**************************************************************************
 * int IO_CTRL_netGpio10PcieClkReq3B(void)                                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (PCIE_CLKREQ3B)                                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10PcieClkReq3B(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 10 (PCIE_CLKREQ3B)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_PCIE_CLKREQ3B_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (PCIE_CLKREQ3B) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_PCIE_CLKREQ3B_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (PCIE_CLKREQ3B) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio10PcieClkReq3B);
/**************************************************************************
 * int IO_CTRL_netGpio10T4Data(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (TS4_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10T4Data(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 10 (TS4_DATA)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_DATA_CCARD_MDO_0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (TS4_DATA) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS4_DATA_CCARD_MDO_0_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 10 (TS4_DATA) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio10T4Data);
/**************************************************************************
 * int IO_CTRL_netGpio11I2c3Pad(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (I2C3_SC)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11I2c3Pad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 11 (I2C3_SC)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_I2C3_SCL_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (I2C3_SC) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_WEST_REGISTERS_I2C3_SCL_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (I2C3_SC) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio11I2c3Pad);
/**************************************************************************
 * int IO_CTRL_netGpio11MMC2D0(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (MMC2_D0)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11MMC2D0(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 11 (MMC2_D0)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D0_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (MMC2_D0) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D0_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (MMC2_D0) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio11MMC2D0);
/**************************************************************************
 * int IO_CTRL_netGpio11Ts5Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (TS5_CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11Ts5Clk(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 11 (TS5_CLK)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_CLK_CCARD_MOCLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (TS5_CLK) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_CLK_CCARD_MOCLK_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (TS5_CLK) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio11Ts5Clk);
/**************************************************************************
 * int IO_CTRL_netGpio11Ts1Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (TS1_CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11Ts1Clk(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 11 (TS1_CLK)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (TS1_CLK) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_CLK_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 11 (TS1_CLK) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio11Ts1Clk);
/**************************************************************************
 * int IO_CTRL_netGpio12I2c0SdaPad(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (I2C0_SDA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12I2c0SdaPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 12 (I2C0_SDA)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SDA_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (I2C0_SDA) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SDA_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (I2C0_SDA) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio12I2c0SdaPad);
/**************************************************************************
 * int IO_CTRL_netGpio12MMC2D1(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (MMC2_D1)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12MMC2D1(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 12 (MMC2_D1)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (MMC2_D1) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D1_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (MMC2_D1) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio12MMC2D1);
/**************************************************************************
 * int IO_CTRL_netGpio12TS5Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (TS5_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12TS5Sync(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 12 (TS5_SYNC)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_SYNC_CCARD_MOSTRT_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (TS5_SYNC) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_SYNC_CCARD_MOSTRT_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (TS5_SYNC) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio12TS5Sync);
/**************************************************************************
 * int IO_CTRL_netGpio12TS1Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (TS1_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12TS1Sync(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 12 (TS1_SYNC)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_SYNC_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (TS1_SYNC) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_SYNC_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 12 (TS1_SYNC) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio12TS1Sync);
/**************************************************************************
 * int IO_CTRL_netGpio13I2c0SclPad(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13I2c0SclPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 13\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SCL_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_I2C0_SCL_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio13I2c0SclPad);
/**************************************************************************
 * int IO_CTRL_netGpio13MMC2D2(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *               (MMC2_D2)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13MMC2D2(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 13 (MMC2_D2)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D2_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 (MMC2_D2) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D2_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 (MMC2_D2) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio13MMC2D2);
/**************************************************************************
 * int IO_CTRL_netGpio13Ts5Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *               (TS5_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13Ts5Dav(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 13 (TS5_DAV)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_DAV_CCARD_MDI_1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 (TS5_DAV) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_DAV_CCARD_MDI_1_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 (TS5_DAV) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio13Ts5Dav);
/**************************************************************************
 * int IO_CTRL_netGpio13Ts1Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *               (TS1_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13Ts1Dav(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 13 (TS1_DAV)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_DAV_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 (TS1_DAV) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_DAV_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 13 (TS1_DAV) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio13Ts1Dav);
/**************************************************************************
 * int IO_CTRL_netGpio14ZibeeResetPad(void)                               *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14ZibeeResetPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 14\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_ZIGBEE_RESET_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_ZIGBEE_RESET_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio14ZibeeResetPad);
/**************************************************************************
 * int IO_CTRL_netGpio14Ts5Data(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *               (TS5_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14Ts5Data(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 14 (TS5_DATA)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_DATA_CCARD_MDO_1_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 (TS5_DATA) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS5_DATA_CCARD_MDO_1_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 (TS5_DATA) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio14Ts5Data);
/**************************************************************************
 * int IO_CTRL_netGpio14Ts1Data(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *               (TS1_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14Ts1Data(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 14 (TS1_DATA)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_DATA_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 (TS1_DATA) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS1_DATA_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 (TS1_DATA) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio14Ts1Data);
/**************************************************************************
 * int IO_CTRL_netGpio14MMC2D3(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *               (MMC2_D3)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14MMC2D3(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 14 (MMC2_D3)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D3_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 (MMC2_D3) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D3_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 14 (MMC2_D3) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}

EXPORT_SYMBOL(IO_CTRL_netGpio14MMC2D3);
/**************************************************************************
 * int IO_CTRL_netGpio15ZibeeIntPad(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (ZIGBEE_INT_STBY)                                         * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15ZibeeIntPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 15 (ZIGBEE_INT_STBY)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_ZIGBEE_INT_STBY_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg0 (ZIGBEE_INT_STBY)\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   GPIO_NORTHEAST_REGISTERS_ZIGBEE_INT_STBY_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg1 (ZIGBEE_INT_STBY)\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio15ZibeeIntPad);
/**************************************************************************
 * int IO_CTRL_netGpio15MMC2D4(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (MMC2_D4)                                                 * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15MMC2D4(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 15 (MMC2_D4)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D4_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg0 (MMC2_D4)\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D4_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg1 (MMC2_D4)\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio15MMC2D4);
/**************************************************************************
 * int IO_CTRL_netGpio15Ts6Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (TS6_CLK)                                                 * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15Ts6Clk(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 15 (TS6_CLK)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS6_CLK_CCARD_MDI_2_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg0 (TS6_CLK)\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS6_CLK_CCARD_MDI_2_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg1 (TS6_CLK)\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio15Ts6Clk);
/**************************************************************************
 * int IO_CTRL_netGpio15Ts2Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (TS2_CLK)                                                 * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15Ts2Clk(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 15 (TS2_CLK)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS2_CLK_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg0 (TS2_CLK)\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS2_CLK_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 15 cnfg1 (TS2_CLK)\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio15Ts2Clk);
/**************************************************************************
 * int IO_CTRL_netGpio16BbuUartTxPad(void)                                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (BBU_UART_TX)                                            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16BbuUartTxPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 16 (BBU_UART_TX)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_TX_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (BBU_UART_TX) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_TX_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (BBU_UART_TX) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio16BbuUartTxPad);
/**************************************************************************
 * int IO_CTRL_netGpio16MMC2D5(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (MMC2_D5)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16MMC2D5(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 16 (MMC2_D5)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D5_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (MMC2_D5) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D5_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (MMC2_D5) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio16MMC2D5);
/**************************************************************************
 * int IO_CTRL_netGpio16Ts6Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (TS6_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16Ts6Sync(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 16 (TS6_SYNC)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS6_SYNC_CCARD_MDO_2_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (TS6_SYNC) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS6_SYNC_CCARD_MDO_2_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (TS6_SYNC) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio16Ts6Sync);
/**************************************************************************
 * int IO_CTRL_netGpio16Ts2Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (TS2_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16Ts2Sync(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 16 (TS2_SYNC)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS2_SYNC_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (TS2_SYNC) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS2_SYNC_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 16 (TS2_SYNC) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio16Ts2Sync);
/**************************************************************************
 * int IO_CTRL_netGpio17BbuUartRxPad(void)                                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (BBU_UART_RX)                                            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17BbuUartRxPad(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 17 (BBU_UART_RX)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 2;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_RX_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (BBU_UART_RX) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_NORTHWEST_REGISTERS_BBU_UART_RX_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (BBU_UART_RX) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio17BbuUartRxPad);
/**************************************************************************
 * int IO_CTRL_netGpio17MMC2D6(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (MMC2_D6)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17MMC2D6(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 17 (MMC2_D6)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 3;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D6_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (MMC2_D6) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSW_PORT,
                                   GPIO_SOUTHWEST_REGISTERS_MMC2_D6_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (MMC2_D6) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio17MMC2D6);
/**************************************************************************
 * int IO_CTRL_netGpio17Ts6Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (TS6_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17Ts6Dav(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 17 (TS6_DAV)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS6_DAV_CCARD_MDI_3_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (TS6_DAV) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS6_DAV_CCARD_MDI_3_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (TS6_DAV) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio17Ts6Dav);
/**************************************************************************
 * int IO_CTRL_netGpio17Ts2Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (TS2_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17Ts2Dav(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure net PGIO 17 (TS2_DAV)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 4;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS2_DAV_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (TS2_DAV) cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIOSE_PORT,
                                   GPIO_SOUTHEAST_REGISTERS_TS2_DAV_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config net PGIO 17 (TS2_DAV) cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio17Ts2Dav);
/**************************************************************************
 * int IO_CTRL_netGpio18Gbe0Reset(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure GBE 0 reset           * 
 *               (GPIO 18)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio18Gbe0Reset(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure GBE 0 reset (net GPIO 18)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_WEST_REGISTERS_GBE_0_RESET_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE 0 reset cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_WEST_REGISTERS_GBE_0_RESET_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE 0 reset cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio18Gbe0Reset);
/**************************************************************************
 * int IO_CTRL_netGpio19Gbe1Reset(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure GBE 1 reset           * 
 *               (GPIO 19)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio19Gbe1Reset(void)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure GBE 1 reset (net GPIO 19)\n");
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_WEST_REGISTERS_GBE_1_RESET_PAD_CFG0_MSGREGADDR,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE 1 reset cnfg0\n");
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONW_PORT,
                                   GPIO_WEST_REGISTERS_GBE_1_RESET_PAD_CFG1_MSGREGADDR,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config GBE 1 reset cnfg1\n");
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_netGpio19Gbe1Reset);
/**************************************************************************
 * IO_CTRL_gpioLeds(int ledId)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the board LEDs        * 
 *  INPUT: ledId - Id of the requested led                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_gpioLeds(int ledId)
{
    IO_CFG0_t ioPadCnfg0;
    IO_CFG1_t ioPadCnfg1;

    IO_CTRL_DEBUG_OUTPUT("Request to configure LED %d (net GPIO %d)\n", ledId, ledId+19);
    /*configure cnfg0*/
    ioPadCnfg0.field.GPIOPM = 1;
    ioPadCnfg0.field.GPIOEn = 0;
    /*configure cnfg1*/
    ioPadCnfg1.field.InvRXTX = 2;
    /*Config pad*/
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   ledsPadAddress[ledId-1].ledCnfg0,
                                   ioPadCnfg0.val, 0, PCR0_GPIOPM_MASK | PCR0_GPIOEN_MASK) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config LED %d cnfg0\n",ledId+1);
        return IOCNFG_FAIL;
    }
    if (iosfsb_memReadModifyWrite(IOSFSB_GPIONE_PORT,
                                   ledsPadAddress[ledId-1].ledCnfg1,
                                   ioPadCnfg1.val, 0, PCR1_INVRXTX_MASK ) < 0)
    {
        IO_CTRL_ERROR_OUTPUT("fail to config LED %d cnfg1\n",ledId+1);
        return IOCNFG_FAIL;
    }
    return IOCNFG_OK;
}
EXPORT_SYMBOL(IO_CTRL_gpioLeds);
/**************************************************************************
 * IO_CTRL_configPad(unsigned int opcode, unsigned int value)             *
 **************************************************************************
 *  DESCRIPTION: This function is used call pad configuration upon        *
 *               request                                                  *
 *  INPUT: opcode - Pad configuration function represented by ENUM        * 
 *         value1 - First value to path function if needed                * 
 *         value2 - Second value to path function if needed               * 
 *                                                                        * 
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configPad(unsigned int opcode, unsigned int value1, unsigned int value2 )
{
    switch (opcode)
    {
        case(IO_CTRL_CONFIG_SINGLE_MPEG_2VAR):
        {
            return(IO_CTRL_configMPEG(value1, value2));
        }
        case(IO_CTRL_CONFIG_ALL_MPEG_1VAR):
        {
            return(IO_CTRL_configMPEGoutAllTS(value1));
        }
        case(IO_CTRL_CONFIG_DOCSIS_UART):
        {
            return(IO_CTRL_configDocsisUart());
        }
        case(IO_CTRL_CONFIG_DECT_UART):
        {           
            return(IO_CTRL_configDectUart());
        }
        case(IO_CTRL_CONFIG_BBU_UART):
        {
            return(IO_CTRL_configBBUUart());
        }
        case(IO_CTRL_CONFIG_UART3):
        {
            return(IO_CTRL_configUART3());
        }
        case(IO_CTRL_CONFIG_I2C0):
        {
            return(IO_CTRL_configI2C0());
        }
        case(IO_CTRL_CONFIG_I2C1):
        {
            return(IO_CTRL_configI2C1());
        }
        case(IO_CTRL_CONFIG_I2C6):
        {
            return(IO_CTRL_configI2C6());
        }
        case(IO_CTRL_CONFIG_DECT_SPI):
        {
            return(IO_CTRL_configDectSPI());
        }
        case(IO_CTRL_CONFIG_CODEC1):
        {
            return(IO_CTRL_configCodec1());
        }
        case(IO_CTRL_CONFIG_ZDS):
        {  
            return(IO_CTRL_configZDS(value1));
        }
        case(IO_CTRL_CONFIG_TDM_1VAR):
        {
            return(IO_CTRL_configTDM(value1));
        }
        case(IO_CTRL_CONFIG_GBE_1VAR):
        {
            return(IO_CTRL_configGBE(value1));
        }
        case(IO_CTRL_CONFIG_RF_FE_SPI):
        {
            return(IO_CTRL_configRfFeSpi());
        }
        case(IO_CTRL_CONFIG_EPGA):
        {
            return(IO_CTRL_configEPGA());
        }
        case(IO_CTRL_CONFIG_BBU_SPI):
        {
            return(IO_CTRL_configBBUSpi());
        }
        case(IO_CTRL_CONFIG_ALL_BBU):
        {
            return(IO_CTRL_configBBU());
        }
        case(IO_CTRL_CONFIG_SGMII1_INT):
        {
            return(IO_CTRL_SGMII1Int());
        }
        case(IO_CTRL_CONFIG_SGMII0_INT):
        {
            return(IO_CTRL_SGMII0Int());
        }
        case(IO_CTRL_CONFIG_NGIO0_RF_FE_SWITCH0):
        {
            return(IO_CTRL_netGpio0FfFeSwitch0());
        }
        case(IO_CTRL_CONFIG_NGIO1_RF_FE_SWITCH1):
        {
            return(IO_CTRL_netGpio1FfFeSwitch1());
        }
        case(IO_CTRL_CONFIG_NGIO2_MOCA_GPIO):
        {
            return(IO_CTRL_netGpio2MocaGpio());
        }
        case(IO_CTRL_CONFIG_NGIO3_MPEG_RESET):
        {
            return(IO_CTRL_netGpio3mpegReset());
        }
        case(IO_CTRL_CONFIG_NGIO4_RF_FE_RESET_PAD):
        {
            return(IO_CTRL_netGpio4RfFeResetPad());
        }
        case(IO_CTRL_CONFIG_NGIO5_US_MONITORING):
        {
            return(IO_CTRL_netGpio5USMonitoring());
        }
        case(IO_CTRL_CONFIG_NGIO6_MOCA_INT_PAD):
        {
            return(IO_CTRL_netGpio6MocaIntPad());
        }
        case(IO_CTRL_CONFIG_NGIO6_LED14):
        {
            return(IO_CTRL_netGpio6Led14());
        }
        case(IO_CTRL_CONFIG_NGIO6_PCIE_CLK_REQ2B):
        {
            return(IO_CTRL_netGpio6PcieClkReq2B());
        }
        case(IO_CTRL_CONFIG_NGIO6_MMC2_CLK):
        {
            return(IO_CTRL_netGpio6MMC2Clk());
        }
        case(IO_CTRL_CONFIG_NGIO7_MPEG_INT_PAD):
        {
            return(IO_CTRL_netGpio7MpegIntPad());
        }
        case(IO_CTRL_CONFIG_NGIO7_TS4_CLK):
        {
            return(IO_CTRL_netGpio7Ts4Clk());
        }
        case(IO_CTRL_CONFIG_NGIO7_TS0_CLK):
        {
            return(IO_CTRL_netGpio7Ts0Clk());
        }
        case(IO_CTRL_CONFIG_NGIO8_EXT_IRQ0_PAD):
        {
            return(IO_CTRL_netGpio8ExtIrq0Pad());
        }
        case(IO_CTRL_CONFIG_NGIO8_TS4_SYNC):
        {
            return(IO_CTRL_netGpio8Ts4Sync());
        }
        case(IO_CTRL_CONFIG_NGIO8_TS0_SYNC):
        {
            return(IO_CTRL_netGpio8Ts0Sync());
        }
        case(IO_CTRL_CONFIG_NGIO9_EXT_IRQ1_PAD):
        {
            return(IO_CTRL_netGpio9ExtIrq1Pad());
        }
        case(IO_CTRL_CONFIG_NGIO9_TS4_DAV):
        {
            return(IO_CTRL_netGpio9Ts4Dav());
        }
        case(IO_CTRL_CONFIG_NGIO9_TS0_DAV):
        {
            return(IO_CTRL_netGpio9Ts0Dav());
        }
        case(IO_CTRL_CONFIG_NGIO9_PCIE_CLK_REQ2B):
        {
            return(IO_CTRL_netGpio9PcieClkReq2B());
        }
        case(IO_CTRL_CONFIG_NGIO10_TS0_DATA):
        {
            return(IO_CTRL_netGpio10Ts0Data());
        }
        case(IO_CTRL_CONFIG_NGIO10_MMC2_CMD):
        {
            return(IO_CTRL_netGpio10MMC2Cmd());
        }
        case(IO_CTRL_CONFIG_NGIO10_PCIE_CLK_REQ3B):
        {
            return(IO_CTRL_netGpio10PcieClkReq3B());
        }
        case(IO_CTRL_CONFIG_NGIO10_TS4_DATA):
        {
            return(IO_CTRL_netGpio10T4Data());
        }
        case(IO_CTRL_CONFIG_NGIO11_I2C3_PAD):
        {
            return(IO_CTRL_netGpio11I2c3Pad());
        }
        case(IO_CTRL_CONFIG_NGIO11_MMC2_D0):
        {
            return(IO_CTRL_netGpio11MMC2D0());
        }
        case(IO_CTRL_CONFIG_NGIO11_TS5_CLK):
        {
            return(IO_CTRL_netGpio11Ts5Clk());
        }
        case(IO_CTRL_CONFIG_NGIO11_TS1_CLK):
        {
            return(IO_CTRL_netGpio11Ts1Clk());
        }
        case(IO_CTRL_CONFIG_NGIO12_I2C0_SDA_PAD):
        {
            return(IO_CTRL_netGpio12I2c0SdaPad());
        }
        case(IO_CTRL_CONFIG_NGIO12_MMC2_D1):
        {
            return(IO_CTRL_netGpio12MMC2D1());
        }
        case(IO_CTRL_CONFIG_NGIO12_TS1_SYNC):
        {
            return(IO_CTRL_netGpio12TS1Sync());
        }
        case(IO_CTRL_CONFIG_NGIO12_TS5_SYNC):
        {
            return(IO_CTRL_netGpio12TS5Sync());
        }
        case(IO_CTRL_CONFIG_NGIO13_I2C0_SCL_PAD):
        {
            return(IO_CTRL_netGpio13I2c0SclPad());
        }
        case(IO_CTRL_CONFIG_NGIO13_MMC2_D2):
        {
            return(IO_CTRL_netGpio13MMC2D2());
        }
        case(IO_CTRL_CONFIG_NGIO13_TS5_DAV):
        {
            return(IO_CTRL_netGpio13Ts5Dav());
        }
        case(IO_CTRL_CONFIG_NGIO13_TS1_DAV):
        {
            return(IO_CTRL_netGpio13Ts1Dav());
        }
        case(IO_CTRL_CONFIG_NGIO14_ZIBEE_REASET_PAD):
        {
            return(IO_CTRL_netGpio14ZibeeResetPad());
        }
        case(IO_CTRL_CONFIG_NGIO14_TS5_DATA):
        {
            return(IO_CTRL_netGpio14Ts5Data());
        }
        case(IO_CTRL_CONFIG_NGIO14_TS1_DATA):
        {
            return(IO_CTRL_netGpio14Ts1Data());
        }
        case(IO_CTRL_CONFIG_NGIO14_MMC2_D3):
        {
            return(IO_CTRL_netGpio14MMC2D3());
        }
        case(IO_CTRL_CONFIG_NGIO15_ZIBEE_INT_PAD):
        {
            return(IO_CTRL_netGpio15ZibeeIntPad());
        }
        case(IO_CTRL_CONFIG_NGIO15_MMC2_D4):
        {
            return(IO_CTRL_netGpio15MMC2D4());
        }
        case(IO_CTRL_CONFIG_NGIO15_TS6_CLK):
        {
            return(IO_CTRL_netGpio15Ts6Clk());
        }
        case(IO_CTRL_CONFIG_NGIO15_TS2_CLK):
        {
            return(IO_CTRL_netGpio15Ts2Clk());
        }
        case(IO_CTRL_CONFIG_NGIO16_BBU_UART_TX_PAD):
        {
            return(IO_CTRL_netGpio16BbuUartTxPad());
        }
        case(IO_CTRL_CONFIG_NGIO16_MMC2_D5):
        {
            return(IO_CTRL_netGpio16MMC2D5());
        }
        case(IO_CTRL_CONFIG_NGIO16_TS6_SYNC):
        {
            return(IO_CTRL_netGpio16Ts6Sync());
        }
        case(IO_CTRL_CONFIG_NGIO16_TS2_SYNC):
        {
            return(IO_CTRL_netGpio16Ts2Sync());
        }
        case(IO_CTRL_CONFIG_NGIO17_BBU_UART_RX_PAD):
        {
            return(IO_CTRL_netGpio17BbuUartRxPad());
        }
        case(IO_CTRL_CONFIG_NGIO17_MMC2_D6):
        {
            return(IO_CTRL_netGpio17MMC2D6());
        }
        case(IO_CTRL_CONFIG_NGIO17_TS6_DAV):
        {
            return(IO_CTRL_netGpio17Ts6Dav());
        }
        case(IO_CTRL_CONFIG_NGIO17_TS2_DAV):
        {
            return(IO_CTRL_netGpio17Ts2Dav());
        }
        case(IO_CTRL_CONFIG_NGIO18_GBE0_RESET):
        {
            return(IO_CTRL_netGpio18Gbe0Reset());
        }
        case(IO_CTRL_CONFIG_NGIO19_GBE1_RESET):
        {
            return(IO_CTRL_netGpio19Gbe1Reset());
        }
        case(IO_CTRL_CONFIG_NGIO_GPIO_LEDS_1VAR):
        {
            return(IO_CTRL_gpioLeds(value1));
        }
        default:
        {
            IO_CTRL_ERROR_OUTPUT("Received non valid opcode %d\n",opcode);
            break;
        }

    }
    return IOCNFG_FAIL;
}
EXPORT_SYMBOL(IO_CTRL_configPad);


