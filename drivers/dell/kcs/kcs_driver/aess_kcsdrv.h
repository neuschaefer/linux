/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 KCS driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
 
#ifndef AESSKCSDRV_H
#define AESSKCSDRV_H

#ifdef AESSKCSDRV_C 
 

/* IOCTL command */
#define AESS_KCSDRV_IOC_MAGIC    0xBA
#define AESS_KCSDRV_INIT         _IOWR(AESS_KCSDRV_IOC_MAGIC, 0, int)
#define AESS_KCSDRV_READ         _IOWR(AESS_KCSDRV_IOC_MAGIC, 1, int)
#define AESS_KCSDRV_WRITE        _IOWR(AESS_KCSDRV_IOC_MAGIC, 2, int)
#define AESS_KCSDRV_SWSMI        _IOWR(AESS_KCSDRV_IOC_MAGIC, 3, int)
#define AESS_KCSDRV_SETCBID                 _IOWR(AESS_KCSDRV_IOC_MAGIC, 4, int)

/* For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;
typedef u64                 UINT64;

typedef void (* HIFLRSTHookAPIPtr) (void);
/* init flag */
#define INIT_OK                 0x0
#define INIT_FAIL               0x1
#define OPEN_OK                 0x0
#define OPEN_FAIL               0x1


/*KCS Interrupt*/
#define KCS_HIB_INT 0x9
/*The number of hook functions*/
#define HIF_MAX_HOOK_FUNC_NUM 4

/*Global Control Register*/
#define GC_REG_BASE_ADDR        WPCM450_VA_GCR
#define GC_REG_MFSEL1            (GC_REG_BASE_ADDR + 0x0C)
#define GC_REG_INTCR            (GC_REG_BASE_ADDR + 0x3C)
  
/*KCS Register Basse Address*/
#define LPC_REG_BASE_ADDR       WPCM450_VA_KCS

/* KCS register offset*/
#define LPC_CH_ZERO_REG_STR       (LPC_REG_BASE_ADDR+0x0C)
#define LPC_CH_ZERO_REG_DOR       (LPC_REG_BASE_ADDR+0x0E)
#define LPC_CH_ZERO_REG_DIR       (LPC_REG_BASE_ADDR+0x10)
#define LPC_CH_ZERO_REG_CMR       (LPC_REG_BASE_ADDR+0x10)
#define LPC_CH_ZERO_REG_DOM       (LPC_REG_BASE_ADDR+0x14)
#define LPC_CH_ZERO_REG_CTL       (LPC_REG_BASE_ADDR+0x18)
#define LPC_CH_ZERO_REG_IC        (LPC_REG_BASE_ADDR+0x1A)
#define LPC_CH_ZERO_REG_IE        (LPC_REG_BASE_ADDR+0x1C)
#define LPC_CH_ONE_REG_STR        (LPC_REG_BASE_ADDR+0x1E)
#define LPC_CH_ONE_REG_DOR        (LPC_REG_BASE_ADDR+0x20)
#define LPC_CH_ONE_REG_DIR        (LPC_REG_BASE_ADDR+0x22)
#define LPC_CH_ONE_REG_CMR        (LPC_REG_BASE_ADDR+0x22)
#define LPC_CH_ONE_REG_DOM        (LPC_REG_BASE_ADDR+0x26)
#define LPC_CH_ONE_REG_CTL        (LPC_REG_BASE_ADDR+0x2A)
#define LPC_CH_ONE_REG_IC         (LPC_REG_BASE_ADDR+0x2C)
#define LPC_CH_ONE_REG_IE         (LPC_REG_BASE_ADDR+0x2E)
#define LPC_CH_TWO_REG_STR        (LPC_REG_BASE_ADDR+0x30)
#define LPC_CH_TWO_REG_DOR        (LPC_REG_BASE_ADDR+0x32)
#define LPC_CH_TWO_REG_DIR        (LPC_REG_BASE_ADDR+0x34)
#define LPC_CH_TWO_REG_CMR        (LPC_REG_BASE_ADDR+0x34)
#define LPC_CH_TWO_REG_DOM        (LPC_REG_BASE_ADDR+0x38)
#define LPC_CH_TWO_REG_CTL        (LPC_REG_BASE_ADDR+0x3C)
#define LPC_CH_TWO_REG_IC         (LPC_REG_BASE_ADDR+0x3E)
#define LPC_CH_TWO_REG_IE         (LPC_REG_BASE_ADDR+0x40)

/*BIOS POST Codes FIFO Registers*/
#define BIOS_POST_REG_BASE_ADDR         LPC_REG_BASE_ADDR
#define BIOS_PS_REG_FIFO_LADDR          (BIOS_POST_REG_BASE_ADDR+0x42)
#define BIOS_PS_REG_FIFO_MADDR          (BIOS_POST_REG_BASE_ADDR+0x44)
#define BIOS_PS_REG_FIFO_ENABLE         (BIOS_POST_REG_BASE_ADDR+0x46)
#define BIOS_PS_REG_FIFO_STATUS         (BIOS_POST_REG_BASE_ADDR+0x48)
#define BIOS_PS_REG_FIFO_DATA           (BIOS_POST_REG_BASE_ADDR+0x4A)
#define BIOS_PS_REG_FIFO_MISC_STATUS    (BIOS_POST_REG_BASE_ADDR+0x4C)



/*Field on STATUS Register*/
#define KCS_STATUS_OBF 0x1
#define KCS_STATUS_IBF 0x2
#define KCS_STATUS_F0  0x4
#define KCS_STATUS_CMD 0x8
#define KCS_STATUS_ST0 0x10
#define KCS_STATUS_ST1 0x20
#define KCS_STATUS_ST2 0x40
#define KCS_STATUS_ST3 0x80


/* MICS */
#define BIT_0                   0x00
#define BIT_4                   0x10
#define SHIFT_1_BITS            0x01
#define SHIFT_2_BITS            0x02
#define SHIFT_3_BITS            0x03
#define SHIFT_6_BITS            0x06
#define SHIFT_8_BITS            0x08
#define SHIFT_22_BITS           0x16
#define MASK_0X3F               0x3F
#define KCS_DUMMY_DATA          0x0
#define SET_SMA_BIT             0x01
#define INTERRUPT_OCCURRED      0x01
#define NO_INTERRUPT            0x0


/*Global Control Register Data*/
#define ENABLE_SMI_FUNCTION 0x1

/* KCS Register Definition */
#define KCS_REG_FOCTL                0x00
#define KCS_REG_ODR                  0x01
#define KCS_REG_IDR                  0x02
#define KCS_REG_STATUS               0x03
#define KCS_REG_IBF                  0x04
#define KCS_REG_CD                   0x05
#define KCS_REG_CMD                  0x06
#define KCS_REG_IC                   0x07
#define KCS_REG_IE                   0x08
#define KCS_REG_INT_STS              0x09
#define BIOS_REG_FIFO_INT            0x0A
#define BIOS_REG_FIFO_LPCRST_CHGSTS  0x0B
#define GC_REG_MFSEL_SMI             0x0C


/* KCS register data */
#define ENABLE_IBF_INT          0x01
#define DISABLE_IBF_INT         0xFE
#define ENABLE_SWSMI            0x04
#define DISABLE_HWSMI           0xDF
#define SET_SWIB                0x02
#define CLEAR_SWIB              0xFD


/* KCS interface package and structure definition */
#define KCS_PACKAGE_MAX     0x28
#define KCS_TOTAL_CHANNEL   0x03

/* KCS interface state definition */
#define KCS_STATUS_IDLE_STATE          0x0
#define KCS_STATUS_READ_STATE          0x1
#define KCS_STATUS_WRITE_STATE         0x2
#define KCS_STATUS_ERROR_STATE         0x3

/* KCS interface control codes definition */
#define KCS_ABORT_CMD           0x60
#define KCS_GET_STATUS          0x60
#define KCS_WRITE_START         0x61
#define KCS_WRITE_END           0x62
#define KCS_READ                0x68
#define KCS_ZERO_DATA           0

/* KCS interface status register bits definition */
#define KCS_DATA            0
#define KCS_COMMAND         1
#define SMS_ATN_SET         1
#define SMS_ATN_CLEAR       0

/* KCS interface status codes definition */
#define KCS_NO_ERROR                0x0
#define KCS_ABORTED_BY_COMMAND      0x01
#define KCS_ILLEGAL_CONTROL_CODE    0x02
#define KCS_LENGTH_ERROR            0x06
#define KCS_UNSPECIFIED_ERROR       0xFF

#define LPC_RST_INT_ENABLE               0x10
#define LPC_RST_INT_DISABLE              0xEF
#define LPC_RST_STS                      0x20
#define LPC_RST_CHG_STS                  0x40
#define LPC_RST_CHG_STS_CLEAR            0x40


/* KCS Seq No */
#define KCS_SEQ_NO_INIT         0

typedef enum
{
	KCS_CH_0 = 0,
	KCS_CH_1,
	KCS_CH_2,
	KCS_CH_NONE = 0xFF,
	KCS_CH_ALL = 0xFF
} eKCSChannelList;



/******************************************************************************
*   STRUCT      :   sLPCRegsPtrType
******************************************************************************/
/**
 *  @brief   structure of LPC Register pointers
 *
 *****************************************************************************/
typedef struct
{
	volatile UINT8          *const pIDR;

	/** Pointer to ODR */
	volatile UINT8          *const pODR;

	/** Pointer to CMR */
	volatile UINT8          *const pCMR;
	
	/** Pointer to STR */
	volatile UINT8          *const pSTR;

	/** Pointer to CTL*/
	volatile UINT8          *const pCTL;
	
	/** Pointer to IC*/
	volatile UINT8          *const pIC;
	
	/** Pointer to IE*/
	volatile UINT8          *const pIE;
} sLPCRegsPtrType;


/******************************************************************************
*   ENUM        :   eHIFStateTag
******************************************************************************/
/**
 *  @brief    The type state of KCS state machine
 *
 *****************************************************************************/
typedef enum
{
	/** The KCS driver has been initialized and is ready to process any KCS
		interface interrupt. */
	KCS_IDLE_STATE,

	/** When receive WRITE_START command then transition to this state. To store
		the receive data into local buffer in this state. */
	KCS_RECEIVE_DATA_STATE,

	/** When receive WRITE_END command and already in the RECEIVE_DATA state
		then transition to this state. To store the last one receives data
		in this state. */
	KCS_RECEIVE_DATA_END_STATE,

	/** When KCSWriteStart() API be called then transition to this state.
		Or already in this state and receive READ command, it still stay
		in this state. To put transfer data into ODR register in this state. */
	KCS_TRANSFER_DATA_STATE,

	/** When data length counter count down to zero and already
		in the TRANSFER_DATA state then transition to this state.
		To put the dummy data to ODR register in this state. */
	KCS_TRANSFER_DATA_END_STATE,

	/** When receive GET_STATUS/ABORT command then transition to this state. */
	KCS_RESPONSE_STATUS_PHASE1_STATE,

	/** When get the dummy data H'00 and already in the
		RESPONSE_STATUS_PHASE1 state then transition to this state.
		At the same time write HIF status to ODR register. */
	KCS_RESPONSE_STATUS_PHASE2_STATE,

	/** Error state */
	KCS_ERROR_STATE

} eKCSStateTag;

/******************************************************************************
*   STRUCT      :    sKCSData
******************************************************************************/
/**
 *  @brief    KCS Parameters definition from User space
 *
 *****************************************************************************/
typedef struct 
{
	/** Channel number */
	UINT8 u8Channel;	    
	
	/** Set for SMS_ATTEN Bit */
	UINT8 u8Control;
	
	/** KCS base address */
	UINT16 u16BaseAddress;
	
	/** Write data length */
	UINT8 u8WriteLength;
	
	/** Read data size */
	UINT8 *pu8ReadLength;    
	
	/** Data buffer, IPMI command length only 40 bytes */
	UINT8 *pu8Data;
	
	/** Receive complete event */
	UINT32 u32KCSRxOKEvent;
	
	/** Transmit done event */
	UINT32 u32KCSTxOKEvent;
	
	/** Transmit fail event */
	UINT32 u32KCSTxFailEvent;
	
	/** Event group Driver ID */
	UINT16  u16DriverID;
	
    /** Callback Function event group Driver ID*/
    UINT16  u16CBFunDriverID;
    
    /** Callback Function event ID*/
    UINT32 u32CBFunEventID;
} sKCSInfo;

typedef struct
{ 
    /** Callback Function event group Driver ID*/
    UINT16  u16CBFunDriverID;
    
    /** Callback Function event ID*/
    UINT32 u32CBFunEventID;
} sLPCResetCBIDStruct;


/******************************************************************************
*   STRUCT      :   sKCSStruct
******************************************************************************/
/**
 *  @brief   Structure to KCS driver related data parameter, 
 *           used by linux driver.
 *
 *****************************************************************************/
typedef struct
{
	/** Channel number */
	UINT8 u8Channel;	
		
	/** ISR state  */
	UINT8   u8HIFISRState;

	/** Receive buffer */
	UINT8   au8HIFReceiveBuf[KCS_PACKAGE_MAX];

	/** Transmit buffer */
	UINT8   au8HIFTransferBuf[KCS_PACKAGE_MAX];

	/** Receive data counter */
	UINT8   u8HIFInDataSize;

	/** Transmit data pointer */
	UINT8   *pu8HIFOutDataPtr;

	/** Transmit data counter */
	UINT8   u8HIFOutDataSize;

	/** KCS error status code */
	UINT8   u8HIFStatusCode;

	/** Address base */
	UINT16  u16AddrBase;

	/** Receive complete event */
	UINT32  u32KCSRxOKEvent;

	/** Transmit done event */
	UINT32  u32KCSTxOKEvent;

	/** Transmit fail event */
	UINT32  u32KCSTxFailEvent;
  
	/**KCS sequence No*/
	UINT8 u8KCSSeqNo;

	/** Event group Driver ID */
	UINT16 u16DriverID;
	
} sKCSStruct;


/* Declear KCS data structure */
static sKCSStruct S_aKCSData[KCS_TOTAL_CHANNEL];
static sLPCResetCBIDStruct S_sLPCResetCBID;

/* Set the address for KCS related register, 
   input data register as same as output data register */

static const sLPCRegsPtrType LPCRegs[KCS_TOTAL_CHANNEL] =
{
  {
		(volatile UINT8 *) LPC_CH_ZERO_REG_DIR,
		(volatile UINT8 *) LPC_CH_ZERO_REG_DOR,
		(volatile UINT8 *) LPC_CH_ZERO_REG_CMR,
		(volatile UINT8 *) LPC_CH_ZERO_REG_STR,
		(volatile UINT8 *) LPC_CH_ZERO_REG_CTL,
		(volatile UINT8 *) LPC_CH_ZERO_REG_IC,
		(volatile UINT8 *) LPC_CH_ZERO_REG_IE
	},
	{
		(volatile UINT8 *) LPC_CH_ONE_REG_DIR,
		(volatile UINT8 *) LPC_CH_ONE_REG_DOR,
		(volatile UINT8 *) LPC_CH_ONE_REG_CMR,
		(volatile UINT8 *) LPC_CH_ONE_REG_STR,
		(volatile UINT8 *) LPC_CH_ONE_REG_CTL,
		(volatile UINT8 *) LPC_CH_ONE_REG_IC,
		(volatile UINT8 *) LPC_CH_ONE_REG_IE
	},
	{
		(volatile UINT8 *) LPC_CH_TWO_REG_DIR,
		(volatile UINT8 *) LPC_CH_TWO_REG_DOR,
		(volatile UINT8 *) LPC_CH_TWO_REG_CMR,
		(volatile UINT8 *) LPC_CH_TWO_REG_STR,
		(volatile UINT8 *) LPC_CH_TWO_REG_CTL,
		(volatile UINT8 *) LPC_CH_TWO_REG_IC,
		(volatile UINT8 *) LPC_CH_TWO_REG_IE
	},   
};

/* KCS Register */
#define REG_IDR  (*(LPCRegs[u8Channel].pIDR))
#define REG_ODR  (*(LPCRegs[u8Channel].pODR))
#define REG_CMR  (*(LPCRegs[u8Channel].pCMR))
#define REG_STR  (*(LPCRegs[u8Channel].pSTR))
#define REG_CTL  (*(LPCRegs[u8Channel].pCTL))
#define REG_IC  (*(LPCRegs[u8Channel].pIC))
#define REG_IE  (*(LPCRegs[u8Channel].pIE))


/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
static int aess_kcs_write(sKCSInfo *psdata);

static int aess_kcs_read(sKCSInfo *psdata);

static int aess_kcs_channel_init(sKCSInfo *psdata);

static int aess_kcs_clearall(void);

static void aess_kcs_isr_handler (UINT8 u8Channel);

static void aess_lpc_rst_isr_handler(void); 

static void aess_kcsdrv_bh(UINT32 u32Data);

static void aess_kcs_setreg(
								   /** KCS interface channel */
								   UINT8 u8Channel,
							   
								   /** KCS parameters definition */
								   UINT8 u8Register ,
							   
								   /** Data for setting KCS parameters */
								   UINT8 u8Data
								  )
{            
    UINT8 u8Tmp = 0;
    UINT32 u32Tmp = 0;  
    if ((u8Channel < KCS_TOTAL_CHANNEL) || (KCS_CH_NONE == u8Channel))
    {
        switch (u8Register)
		    {
            case KCS_REG_IBF:
                if(ENABLE_IBF_INT==u8Data)
                {
                    /*Enable IBF Interrupt*/
                    u8Tmp = REG_CTL | ENABLE_IBF_INT;
                    REG_CTL = u8Tmp;

                }else if(DISABLE_IBF_INT==u8Data)
                {
                    /*Disable IBF Interrupt*/
                    u8Tmp = REG_CTL & DISABLE_IBF_INT;
                    REG_CTL = u8Tmp;
                }
                break;
            case KCS_REG_FOCTL:
                u8Tmp = REG_STR;
                u8Tmp |= (u8Data << SHIFT_2_BITS);
                REG_STR = u8Tmp;
                break;
			      case KCS_REG_ODR:
                REG_ODR = u8Data;
                break;
			      case KCS_REG_STATUS:
                u8Tmp = REG_STR;
                u8Tmp = ((u8Tmp) & MASK_0X3F)	| (u8Data << SHIFT_6_BITS);
                REG_STR = u8Tmp;
                break;
			      case BIOS_REG_FIFO_INT:
                if (LPC_RST_INT_ENABLE == u8Data)
                {
                    /* Enable LPC reset interrupt*/
                    u8Tmp = (*((UINT8 *)BIOS_PS_REG_FIFO_ENABLE)) | LPC_RST_INT_ENABLE;
                    *((UINT8 *)BIOS_PS_REG_FIFO_ENABLE) = u8Tmp;
               	}else if (LPC_RST_INT_DISABLE == u8Data)
               	{
                    /* Disable LPC reset interrupt*/
                    u8Tmp = (*((UINT8 *)BIOS_PS_REG_FIFO_ENABLE)) & LPC_RST_INT_DISABLE;
                    *((UINT8 *)BIOS_PS_REG_FIFO_ENABLE) = u8Tmp;
                }
                break;
           case BIOS_REG_FIFO_LPCRST_CHGSTS:
                u8Tmp = (*((UINT8 *)BIOS_PS_REG_FIFO_MISC_STATUS)) | LPC_RST_CHG_STS_CLEAR;
               *((UINT8 *)BIOS_PS_REG_FIFO_MISC_STATUS) = u8Tmp;
               break;      
           case GC_REG_MFSEL_SMI:

                /*Enable pin function to SMI*/
                u32Tmp = (*((UINT32 *)GC_REG_MFSEL1))|(u8Data<<SHIFT_22_BITS);
                *((UINT32 *)GC_REG_MFSEL1) = u32Tmp;
                break;
           case KCS_REG_IC:
                if(SET_SWIB==u8Data)
                {
                  u8Tmp = REG_IC|SET_SWIB;/*Host SMI Request Control Bit*/
      
                }else if(CLEAR_SWIB==u8Data)
                {
                   u8Tmp = REG_IC&CLEAR_SWIB;
                }
                REG_IC = u8Tmp;
                break;     
           case KCS_REG_IE:
                if (DISABLE_HWSMI==u8Data)
                {  
                    u8Tmp = REG_IE&DISABLE_HWSMI;
                }else if(ENABLE_SWSMI==u8Data)
                {
                    u8Tmp = REG_IE|ENABLE_SWSMI;
                }

                REG_IE = u8Tmp;
               break;     
           default:
                break;
        }
    } 
    return;
}

static UINT8 aess_kcs_getreg(
									/** KCS interface channel */
									UINT8 u8Channel,
					 
									/** KCS parameters definition */
									UINT8 u8Register
								   )
{
    UINT8 u8Data = 0;

    if ((u8Channel < KCS_TOTAL_CHANNEL) || (KCS_CH_NONE == u8Channel)||(KCS_CH_ALL == u8Channel))
    {
        switch (u8Register)
        {
            case KCS_REG_IBF:
                u8Data = ((REG_STR & KCS_STATUS_IBF) >> SHIFT_1_BITS);//REG_STR.EQU.BIT.IBF;
                break;
            case KCS_REG_CD:
                u8Data = ((REG_STR & KCS_STATUS_CMD) >> SHIFT_3_BITS);//REG_STR.EQU.BIT.CD;
                break;
            case KCS_REG_CMD:
                u8Data = (UINT8) REG_CMR;
                break;
            case KCS_REG_IDR:
                u8Data = (UINT8) REG_IDR;
                break;
            case BIOS_REG_FIFO_LPCRST_CHGSTS:
		            u8Data = ((*((UINT8 *)BIOS_PS_REG_FIFO_MISC_STATUS))&LPC_RST_CHG_STS) >> SHIFT_6_BITS;
                break;
            case KCS_REG_INT_STS:
                /*If one of KCS0~2, LPC reset interrupts, then return 1*/
                u8Data  = (*((UINT8 *)LPC_CH_ZERO_REG_STR)& KCS_STATUS_IBF);
                u8Data |= (*((UINT8 *)LPC_CH_ONE_REG_STR)& KCS_STATUS_IBF); 
                u8Data |= (*((UINT8 *)LPC_CH_TWO_REG_STR)& KCS_STATUS_IBF); 
                u8Data |= ((*((UINT8 *)BIOS_PS_REG_FIFO_MISC_STATUS))&LPC_RST_CHG_STS);
                if(u8Data)
                {
                   u8Data = INTERRUPT_OCCURRED;
                }else
                {
                  u8Data = NO_INTERRUPT;
                }    
                
                break;
            default:
                break;
        }
        return (u8Data);            
    }
	return STATUS_FAIL;

}

static void aess_kcs_setstate(
							  UINT8 u8Channel,
							  UINT8 u8State
							 );

static void aess_kcs_errorhandler(UINT8 u8Channel);

#if LINUX_VERSION_CODE  == KERNEL_VERSION(2,6,23)
static irqreturn_t aess_kcs_isr(int irq, void *dev_id);
#else
static irqreturn_t aess_kcs_isr(int irq, void *dev_id, struct pt_regs *regs);
#endif



#endif   /* AESSKCSDRV_C */

#endif   /* AESSKCSDRV_H */
