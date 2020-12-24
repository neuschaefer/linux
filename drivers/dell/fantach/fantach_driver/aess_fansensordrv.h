/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 On chip fan tach sensor driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#include <asm/ioctl.h>
#include <asm/arch/map.h>

#if !defined (AESSFANSENSORDRV_H)
#define AESSFANSENSORDRV_H

#ifdef AESSFANSENSORDRV_C

/* For temporary use, because no type.h now */
/* ============================================ */
#define    hw_char         *(volatile unsigned char *)
#define    hw_short        *(volatile unsigned short *)

/* type define */
typedef    unsigned long   uint32;
typedef    unsigned short  uint16;
typedef    unsigned char   uint8;

typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;

#define TRUE                      1
#define FALSE                     0

#define STATUS_OK     0
/* ============================================= */

/* ioctl definitions */
#define AESS_FANSENSORDRV_IOC_MAGIC    0xBD
#define AESS_FANTACH_CONFIG     _IOWR(AESS_FANSENSORDRV_IOC_MAGIC, 0, sFanTachData)
#define AESS_FANTACH_READ       _IOWR(AESS_FANSENSORDRV_IOC_MAGIC, 1, sFanTachData)

/* init flag */
#define AESSFANSENSOR_NOT_INIT    0
#define AESSFANSENSOR_INIT_OK     1

/* APB clock is 55MHz */
#define WPCM450_APB_CLOCK   55000000
#define WPCM450_MFT_CLKPS   255		

/* FAN TACH Input (214843.75Hz) */
#define WPCM450_FAN_TACH_INPUT_CLOCK   (WPCM450_APB_CLOCK / (WPCM450_MFT_CLKPS + 1))

/* Get Fan Tach Timeout (base on clock 214843.75Hz, 1 cnt = 4.654us)
   Timeout 94ms ~= 0x5000 
   (The minimum FAN speed could to support ~640RPM/pulse 1, 320RPM/pulse 2, ...-- 10.6Hz)
 */
 #define FAN_TACH_TIMEOUT   ((UINT16) 0x5000)

/* enable a background timer to poll fan tach value, (200ms * 4) to polling all fan) */
#define FAN_TACH_POLLING_INTERVAL 20  /* 1 = 1 jiffies = 10 ms */

#define FAN_TACH_TO_RPM(cnt, pulse) \
    ((WPCM450_FAN_TACH_INPUT_CLOCK * 60) / (cnt * pulse))

/* MFT General Defintion */
#define WPCM450_MFT0	0
#define WPCM450_MFT1	1
#define WPCM450_CMPA	0
#define WPCM450_CMPB	1

#define WPCM450_MFT_MODE_1	0      // PWM and Counter Mode (Not support)
#define WPCM450_MFT_MODE_2	1     // Dual Input Capture Mode (Not support)
#define WPCM450_MFT_MODE_3	2     // Dual Independent Timer Mode (Not support)
#define WPCM450_MFT_MODE_4	3     // Input Capture and Timer Mode (Not support)
#define WPCM450_MFT_MODE_5	4     // Dual Independent Input Capture (support)

#define WPCM450_MFT_TCNT    ((UINT16) 0xFFFF)
#define WPCM450_MFT_TCPA    ((UINT16) (WPCM450_MFT_TCNT - FAN_TACH_TIMEOUT))
#define WPCM450_MFT_TCPB    ((UINT16) (WPCM450_MFT_TCNT - FAN_TACH_TIMEOUT))

/* Dell Thidwick platform FAN 4 speed is not stable
	if the count is lower than 200,
	we drop this capture, and re-capture again 
	
	55MHz / 256(div) * 60(second) / 200 / 2 (pulse) = 32226 (rpm)
	the max RPM can't over than 32226rpm
*/
#define FAN_TACH_CNT_LIMIT (WPCM450_MFT_TCNT-200)



#define WPCM450_MFT_NO_CLOCK_MODE		0
#define WPCM450_MFT_APB_CLOCK_MODE		1

#define FAN_PULSE_REVOLUTION_1      0x01
#define FAN_PULSE_REVOLUTION_2      0x02
#define FAN_PULSE_REVOLUTION_4      0x04
#define FAN_PULSE_REVOLUTION_8      0x08
#define FAN_PULSE_REVOLUTION_16    0x10

#define DEFAULT_PULSE_PER_REVOLUTION	2

/* MFT base address */
#define MFT_REGS_BASE(n)    ((n == 0) ? (WPCM450_VA_MFT0) : (WPCM450_VA_MFT1))

#define WPCM450_MFSEL2      (WPCM450_VA_GCR+0x010)

#define WPCM450_MFSEL2_FLSEL(n) (1<<n)


/* MFT registers */
#define WPCM450_TCNT1(n)    ((void *) (MFT_REGS_BASE(n) + 0x00))
#define WPCM450_TCRA(n)     ((void *) (MFT_REGS_BASE(n) + 0x02))
#define WPCM450_TCRB(n)     ((void *) (MFT_REGS_BASE(n) + 0x04))
#define WPCM450_TCNT2(n)    ((void *) (MFT_REGS_BASE(n) + 0x06))
#define WPCM450_TPRSC(n)    ((void *) (MFT_REGS_BASE(n) + 0x08))
#define WPCM450_TCKC(n)     ((void *) (MFT_REGS_BASE(n) + 0x0A))
#define WPCM450_TMCTRL(n)   ((void *) (MFT_REGS_BASE(n) + 0x0C))
#define WPCM450_TICTRL(n)   ((void *) (MFT_REGS_BASE(n) + 0x0E))
#define WPCM450_TICLR(n)    ((void *) (MFT_REGS_BASE(n) + 0x10))
#define WPCM450_TIEN(n)     ((void *) (MFT_REGS_BASE(n) + 0x12))
#define WPCM450_TCPA(n)     ((void *) (MFT_REGS_BASE(n) + 0x14))
#define WPCM450_TCPB(n)     ((void *) (MFT_REGS_BASE(n) + 0x16))
#define WPCM450_TCPCFG(n)   ((void *) (MFT_REGS_BASE(n) + 0x18))
#define WPCM450_TINASEL(n)  ((void *) (MFT_REGS_BASE(n) + 0x1A))
#define WPCM450_TINBSEL(n)  ((void *) (MFT_REGS_BASE(n) + 0x1C))


#define WPCM450_TCKC_C2CSEL(mode)	(((mode) & 0x7) << 3)
#define WPCM450_TCKC_C1CSEL(mode)	((mode) & 0x7)

#define WPCM450_TMCTRL_TBEN	(1<<6)
#define WPCM450_TMCTRL_TAEN	(1<<5)
#define WPCM450_TMCTRL_TBEDG	(1<<4)
#define WPCM450_TMCTRL_TAEDG	(1<<3)
#define WPCM450_TMCTRL_MDSEL(mode)	((mode) & 0x7)

#define WPCM450_TICLR_CLEAR_ALL (0x3F)
#define WPCM450_TICLR_TFCLR     (1<<5)
#define WPCM450_TICLR_TECLR     (1<<4)
#define WPCM450_TICLR_TDCLR     (1<<3)
#define WPCM450_TICLR_TCCLR     (1<<2)
#define WPCM450_TICLR_TBCLR     (1<<1)
#define WPCM450_TICLR_TACLR     (1<<0)

#define WPCM450_TIEN_ENABLE_ALL (0x3F)
#define WPCM450_TIEN_TFIEN     (1<<5)       /* Compare Match for TCNT2 or TCRB */
#define WPCM450_TIEN_TEIEN     (1<<4)       /* Compare Match for TCNT1 or TCRA */
#define WPCM450_TIEN_TDIEN     (1<<3)       /* TCNT2 underflow */
#define WPCM450_TIEN_TCIEN     (1<<2)       /* TCNT1 underflow */       
#define WPCM450_TIEN_TBIEN     (1<<1)       /* Input Capture on TBn Transition */
#define WPCM450_TIEN_TAIEN     (1<<0)       /* Input Capture on TAn Transition */

#define WPCM450_TICTRL_TFPND     (1<<5) 
#define WPCM450_TICTRL_TEPND     (1<<4) 
#define WPCM450_TICTRL_TDPND     (1<<3) 
#define WPCM450_TICTRL_TCPND     (1<<2)       
#define WPCM450_TICTRL_TBPND     (1<<1)
#define WPCM450_TICTRL_TAPND     (1<<0)

#define WPCM450_TCPCFG_HIBEN    (1<<7)
#define WPCM450_TCPCFG_EQBEN    (1<<6)
#define WPCM450_TCPCFG_LOBEN    (1<<5)
#define WPCM450_TCPCFG_CPBSEL   (1<<4)
#define WPCM450_TCPCFG_HIAEN    (1<<3)
#define WPCM450_TCPCFG_EQAEN    (1<<2)
#define WPCM450_TCPCFG_LOAEN    (1<<1)
#define WPCM450_TCPCFG_CPASEL   (1<<0)

#define WPCM450_TINASEL_FANIN0 (0x0)
#define WPCM450_TINASEL_FANIN1 (0x1)
#define WPCM450_TINASEL_FANIN2 (0x2)
#define WPCM450_TINASEL_FANIN3 (0x3)

#define WPCM450_TINBSEL_FANIN4 (0x0)
#define WPCM450_TINBSEL_FANIN5 (0x1)
#define WPCM450_TINBSEL_FANIN6 (0x2)
#define WPCM450_TINBSEL_FANIN7 (0x3)

#define WPCM450_TINASEL_FANIN8 (0x0)
#define WPCM450_TINASEL_FANIN9 (0x1)
#define WPCM450_TINASEL_FANIN10 (0x2)
#define WPCM450_TINASEL_FANIN11 (0x3)

#define WPCM450_TINBSEL_FANIN12 (0x0)
#define WPCM450_TINBSEL_FANIN13 (0x1)
#define WPCM450_TINBSEL_FANIN14 (0x2)
#define WPCM450_TINBSEL_FANIN15 (0x3)


/******************************************************************************
*   STRUCT      :   sFanTachData
******************************************************************************/
/**
 *  @brief   Structure to FanSensor driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
    /* Set Fan Channel no. (zero-based) */
    UINT8 u8ChannelNum;     

    /* Pulse Revolution */
    UINT8 u8FanPulsePerRev;   

    /* Fan Speed Reading (unit: RPM) */
    UINT16 u16FanSpeedReading;       
} sFanTachData;

#define FAN_TACH_DISABLE    0xFF
#define FAN_TACH_INIT       0x00
#define FAN_TACH_PREPARE_TO_GET_FIRST_CAPTURE       0x01
#define FAN_TACH_ENOUGH_SAMPLE                      0x02

/******************************************************************************
*   STRUCT      :   sFanTachData
******************************************************************************/
/**
 *  @brief   Structure to FanSensor driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
    /* FAN_TACH_DISABLE    0xFF
       FAN_TACH_INIT        0x00 
       FAN_TACH_PREPARE_TO_GET_FIRST_CAPTURE  0x01
       ... */
    UINT8 u8FanStatusFlag;     

    /* Pulse Revolution */
    UINT8 u8FanPulsePerRev;     

    /* Fan Tach Count 
       (unit: Base on the APB freq. [default: 55Mhz]
       and prescaler TPRSC   [default: 256]) */
    UINT16 u16FanTachCnt;        

    /* Fan Tach Count Temp */
    UINT32 u32FanTachCntTemp;        
} sFanTachDev;

/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
 
static int aess_fantach_read(sFanTachData *pFanTachData);
static int aess_fantach_config(sFanTachData *pFanTachData);
static int wpcm450_fan_tach_init(void);

#endif   /* AESSFANSENSORDRV_C */

#endif   /* AESSFANSENSORDRV_H */
