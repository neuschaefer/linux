/*****************************************************************************
*    Copyright (C)2009 Ali Corporation. All Rights Reserved.
*
*    File:    
*    	
*
*    Description:    This file contains definition depend on paticular platform .
*    History:
*     		Date            		Athor       		Version   		Reason
*     		========		======		=======   	======= =================
*  1.  	Oct.12.2009  		Douglass     	Ver 0.1   		Create file for porting demodulator driver
*    
*
*****************************************************************************/

#ifndef __PORTING_S3501_H__
#define __PORTING_S3501_H__

#include "u_common.h"
#include "x_printf.h"
#include "PD_Def.h"
#include "PD_Def_dbg.h"

/*
#ifndef INT8
typedef char			INT8;
#endif

#ifndef UINT8
typedef unsigned char	UINT8;
#endif
#ifndef INT16
typedef short			INT16;
#endif
#ifndef UINT16
typedef unsigned short	UINT16;
#endif
#ifndef INT32
typedef long			INT32;
#endif
#ifndef UINT32
typedef unsigned long	UINT32;
#endif
*/
typedef long               	ER;

#undef NULL
#define NULL 					((void *)0)

#define BLIND_SCAN 1
#define NO_USE_FUNCTION 0
#define NO_BYPASS_I2C 1
#define NIM_TS_PORT_CAP

#define NIM_SIGNAL_INPUT_OPEN		0x01
#define NIM_SIGNAL_INPUT_CLOSE		0x02



//#define NIM_PRINTF(...)  
#define ERROR_PRINTF mcPRN_USER_MSG
#define DATA_PRINTF  mcPRN_DBG_MSG2
#define NIM_PRINTF mcPRN_DBG_MSG1
#define DOUG_PRINTF  mcPRN_DBG_MSG1
#define DBG_PRINTF  mcPRN_DBG_MSG


/* Return value of function calls and error define */
#define SUCCESS         			0       					/* Success return */
#define ERR_NO_MEM      		-1      					/* Not enough memory error */
#define ERR_DEV_ERROR   		-5      					/* Device work status error */
#define ERR_DEV_CLASH   		-6      					/* Device clash for same device in queue */
#define ERR_QUEUE_FULL  		-7      					/* Queue node count reached the max. val*/
#define ERR_NO_DEV      		-8      					/* Device not exist on PCI */
#define ERR_FAILUE			-9						/* Common error, operation not success */
#define ERR_TIME_OUT    		-34    	 				/* Waiting time out */
#define ERR_FAILED			-40
#define ERR_ADDRESS			-42

#define S3501_I2C_ERROR_BASE		-200
#define S3501_ERR_I2C_NO_ACK		(S3501_I2C_ERROR_BASE - 3)	/* I2C slave no ack */

//LNB porting  lib_diseqc.h
//Lnb Polarity defines
#define	LNB_POL_AUTO 	 		0x00	 				/* LNB POL AUTO */
#define	LNB_POL_H		 		0x01 				/* LNB POL H	   */
#define	LNB_POL_V		 		0x02 				/* LNB POL V	   */
#define	LNB_POL_POFF	 		0x03 				/* LNB POWER OFF  */


/****************************************************************************
*	Hld dev header
*
****************************************************************************/
#define HLD_DEV_TYPE_NIM				0x01050000	/* NIM (Demodulator + tuner) */
#define HLD_DEV_TYPE_MASK			0xFFFF0000	/* Device type mask */
#define HLD_DEV_ID_MASK				0x0000FFFF	/* Device id mask */
#define HLD_DEV_STATS_UP				0x01000001	/* Device is up */
#define HLD_DEV_STATS_RUNNING		0x01000002	/* Device is running */
#define HLD_MAX_DEV_NUMBER			32			/* Max device number */
#define HLD_MAX_NAME_SIZE			16			/* Max device name length */


struct hld_device
{
	struct hld_device   *next;						/* Next device structure */
	UINT32				type;						/* Device type */
	INT8				name[HLD_MAX_NAME_SIZE];	/* Device name */
};

/*
  * this struct is used to save device IO parameter
  */
struct io_param
{
     UINT8 *io_buff_in;
     UINT32 buff_in_len;
     UINT8 *io_buff_out;
     UINT32 buff_out_len;
};

struct io_param_ex
{
     UINT8 *io_buff_in;
     UINT32 buff_in_len;
     UINT8 *io_buff_out;
     UINT32 buff_out_len;
	 UINT32 hnd;
	 UINT8 h264_flag;
	 UINT8 is_scrambled;
     UINT8 record_all;
};

#ifdef __cplusplus
extern "C"
{
#endif

void *dev_get_by_name(INT8 *name);
void *dev_get_by_type(void *sdev, UINT32 type);
void *dev_get_by_id(UINT32 type, UINT16 id);
void dev_list_all(void *sdev);

void *dev_alloc(INT8 *name, UINT32 type, UINT32 size);
INT32 dev_register(void *dev);
void  dev_free(void *dev);

#ifdef __cplusplus
}
#endif


/****************************************************************************
*	Nim dev header
*
****************************************************************************/

#define EXT_QPSK_MODE_SPI 		0
#define EXT_QPSK_MODE_SSI 		1
#define NIM_CHIP_ID_M3501A		0x350100C0
#define NIM_CHIP_ID_M3501B		0x350100D0
#define NIM_CHIP_SUB_ID_S3501D	0x00
#define NIM_CHIP_SUB_ID_M3501B	0xC0
#define NIM_FREQ_RETURN_REAL	0
#define NIM_FREQ_RETURN_SET		1


/* NIM Device I/O control command */
enum nim_device_ioctrl_command
{
	NIM_DRIVER_READ_TUNER_STATUS,	/* Read tuner lock status */
	NIM_DRIVER_READ_QPSK_STATUS,  	/* Read QPSK  lock status */
	NIM_DRIVER_READ_FEC_STATUS,   	/* Read FEC lcok status */

	NIM_DRIVER_READ_QPSK_BER,     		/* Read QPSK Bit Error Rate */
	NIM_DRIVER_READ_VIT_BER,      		/* Read Viterbi Bit Error Rate */
	NIM_DRIVER_READ_RSUB,         		/* Read Reed Solomon Uncorrected block */
	NIM_DRIVER_STOP_ATUOSCAN,		/* Stop autoscan */
	NIM_DRIVER_UPDATE_PARAM,			/* Reset current parameters */
	NIM_DRIVER_TS_OUTPUT,				/* Enable NIM output TS*/
	NIM_DRIVER_FFT,						/* Autoscan Internal use */					
	NIM_DRIVER_GET_CR_NUM,			/* Autoscan internal use*/
	NIM_DRIVER_GET_CUR_FREQ,			/* Get the Current Frequency */
	NIM_DRIVER_FFT_PARA,				/* Reserved */
	NIM_DRIVER_GET_SPECTRUM,			/* Reserved */
	NIM_FFT_JUMP_STEP,               		/* Get AutoScan FFT frequency jump step */ //huang hao add for m3327 qpsk
	NIM_DRIVER_READ_COFFSET,	   		/* Read Carrier Offset state */ //joey add for stv0297.
	NIM_DRIVER_SEARCH_1ST_CHANNEL,	/* Search channel spot*/ //joey add for stv0297.
	NIM_DRIVER_SET_TS_MODE,			/*Set ts output mode:*/ 
										/*bit0: 1 serial out, 	0 parallel out*/
										/*bit1: 1 clk rising, 	0 clk falling*/
										/*bit2: 1 valid gap,		0 valid no gap*/
	NIM_DRIVER_SET_PARAMETERS,		/* Set the parameters of nim,add by Roman at 060321 */
	NIM_DRIVER_SET_RESET_CALLBACK,	/* When nim device need to be reset, call an callback to notice app */
	NIM_DRIVER_ENABLE_DVBS2_HBCD,	/* For DVB-S2, enable/disable HBCD mode */
	NIM_DRIVER_STOP_CHANSCAN,		/* Stop channel change because some low symbol rate TP too long to be locked */
	NIM_DRIVER_RESET_PRE_CHCHG,	    /* Reset nim device before channel change */

        NIM_ENABLE_IC_SORTING,			/*Enable IC sorting, set IC sorting param*/
	NIM_DRIVER_GET_OSD_FREQ_OFFSET, /*get OSD freq offset. */
	NIM_DRIVER_SET_RF_AD_GAIN,		/* Set RF ad table for RSSI display.*/ 
	NIM_DRIVER_SET_IF_AD_GAIN,		/* Set IF ad table for RSSI display. */ 
	NIM_DRIVER_GET_RF_IF_AD_VAL,        /* get RF IF ad value. */ 
	NIM_DRIVER_GET_REC_PERFORMANCE_INFO,	/* get receiver performance info. */ 
	NIM_DRIVER_ENABLE_DEBUG_LOG,			/* enable nim driver debug infor out.*/
	NIM_DRIVER_DISABLE_DEBUG_LOG,			/* disable nim driver debug infor out.*/
	NIM_DRIVER_GET_FRONTEND_STATE,			/* Read front end state.*/
	NIM_DRIVER_SET_FRONTEND_LOCK_CHECK,			/* Set front end lock check flag.*/
};

/* NIM Device I/O control command  extension */
enum nim_device_ioctrl_command_ext
{
	NIM_DRIVER_AUTO_SCAN			= 0x00FC0000,	/* Do AutoScan Procedure */
	NIM_DRIVER_CHANNEL_CHANGE	= 0x00FC0001,	/* Do Channel Change */
	NIM_DRIVER_CHANNEL_SEARCH	= 0x00FC0002,	/* Do Channel Search */
	NIM_DRIVER_GET_RF_LEVEL		= 0x00FC0003,	/* Get RF level */
	NIM_DRIVER_GET_CN_VALUE		= 0x00FC0004,	/* Get CN value */
	NIM_DRIVER_GET_BER_VALUE		= 0x00FC0005,		/* Get BER value */
	NIM_DRIVER_QUICK_CHANNEL_CHANGE = 0x00FC0006,/* Do Quick Channel Change without waiting lock */
	NIM_DRIVER_GET_ID				= 0x00FC0007  /*Get 3501 type: M3501A/M3501B */
};

/* FrontEnd Type */
enum fe_type {
	FE_QPSK	= 0x00, 	/* DVB-S */
	FE_QAM 	= 0x01,	/* DVB-C */
	FE_OFDM	= 0x02 	/* DVB-T */
};


/* Structure for NIM Device Status */
struct nim_device_stats
{
	UINT16	temp_num;			/* blank */
};

/* Structure for NIM DiSEqC Device Information parameters */
struct t_diseqc_info
{
	UINT8 sat_or_tp;			/* 0:sat, 1:tp*/
	UINT8 diseqc_type;
	UINT8 diseqc_port;
	UINT8 diseqc11_type;
	UINT8 diseqc11_port;
	UINT8 diseqc_k22;
	UINT8 diseqc_polar;		/* 0: auto,1: H,2: V */
	UINT8 diseqc_toneburst;	/* 0: off, 1: A, 2: B */	

	UINT8 positioner_type;	/*0-no positioner 1-1.2 positioner support, 2-1.3 USALS*/
	UINT8 position;			/*use for DiSEqC1.2 only*/	
	UINT16 wxyz;			/*use for USALS only*/
};

/* Structure for Channel Change parameters */
struct NIM_Channel_Change
{
	UINT32 freq;				/* Channel Center Frequency: in MHz unit */
	UINT32 sym;				/* Channel Symbol Rate: in KHz unit */
	UINT8 fec;				/* Channel FEC rate */
	UINT32 bandwidth;		/* Channel Symbol Rate: same as Channel Symbol Rate ? -- for DVB-T */
	UINT8 guard_interval;	/* Guard Interval -- for DVB-T */
	UINT8 fft_mode;			/* -- for DVB-T */
	UINT8 modulation;		/* -- for DVB-T */
	UINT8 usage_type;		/* -- for DVB-T */
	UINT8 inverse;			/* -- for DVB-T */
};


/* Structure for Auto Scan parameters */
struct NIM_Auto_Scan
{
  	//>>> Unicable begin
	UINT8 unicable;
//	UINT8 invert;	/* Fub INVERT? */
	UINT16 Fub;	/* Unicable: UB slots centre freq (MHz) */
	//<<< Unicable end
 
	UINT32 sfreq;			/* Start Frequency of the Scan procedure: in MHz unit */
	UINT32 efreq;			/* End Frequency of the Scan procedure: in MHz unit */
	INT32 (*callback)(UINT8 status, UINT8 polar, UINT32 freq, UINT32 sym, UINT8 fec);	/* Callback Function pointer */
};


/* for NIM_DRIVER_SET_RESET_CALLBACK */
typedef INT32 (*pfn_nim_reset_callback)(UINT32 param);


/* Structure nim_device, the basic structure between HLD and LLD of demomulator device */
typedef struct nim_device
{
	struct nim_device   *next;				/* Next nim device structure */
	UINT32	type;						/* Interface hardware type */
	INT8		name[HLD_MAX_NAME_SIZE];	/* Device name */
	UINT32	base_addr;					/* Demodulator address */
	UINT32 Tuner_select;					/* I2C TYPE for  TUNER select  */
	UINT16	flags;						/* Interface flags, status and ability */
	
	/* Hardware privative structure */
	void		*priv;					/* pointer to private data */
	/* Functions of this dem device */
	INT32	(*init)(void);									/* NIM Device Initialization */
	INT32	(*open)(struct nim_device *dev);			/* NIM Device Open */
	INT32	(*stop)(struct nim_device *dev);			/* NIM Device Stop */
	INT32	(*do_ioctl)(struct nim_device *dev, INT32 cmd, UINT32 param);		/* NIM Device I/O Control */
	INT32 	(*do_ioctl_ext)(struct nim_device *dev, INT32 cmd, void * param_list);		/* NIM Device I/O Control Extension */
	INT32	(*get_lock)(struct nim_device *dev, UINT8 *lock);					/* Get Current NIM Device Channel Lock Status */
	INT32	(*get_freq)(struct nim_device *dev, UINT32 *freq);				/* Get Current NIM Device Channel Frequency */
	INT32	(*get_FEC)(struct nim_device *dev, UINT8 *fec);					/* Get Current NIM Device Channel FEC Rate */
	INT32	(*get_SNR)(struct nim_device *dev, UINT8 *snr);	                /* Get Current NIM Device Channel SNR Value */
    INT32	(*get_map_type)(struct nim_device *dev, UINT8 *map_type);

	UINT8		diseqc_typex;				/* NIM DiSEqC Device Type */
	UINT8		diseqc_portx;				/* NIM DiSEqC Device Port */
	struct t_diseqc_info diseqc_info;			/* NIM DiSEqC Device Information Structure */
	INT32	(*set_polar)(struct nim_device *dev, UINT8 polar);		/*DVB-S NIM Device set LNB polarization */
	INT32	(*set_12v)(struct nim_device *dev, UINT8 flag);			/*DVB-S NIM Device set LNB votage 12V enable or not */
	INT32   (*DiSEqC_operate)(struct nim_device *dev, UINT32 mode, UINT8* cmd, UINT8 cnt);	/* NIM DiSEqC Device Opearation */
	INT32   (*DiSEqC2X_operate)(struct nim_device *dev, UINT32 mode, UINT8* cmd, UINT8 cnt, UINT8 *rt_value, UINT8 *rt_cnt);	/* NIM DiSEqC2X Device Opearation */
	INT32	(*get_sym)(struct nim_device *dev, UINT32 *sym);		/* Get Current NIM Device Channel Symbol Rate */
	INT32	(*get_BER)(struct nim_device *dev, UINT32 *ebr);			/* Get Current NIM Device Channel Bit-Error Rate */
	INT32	(*get_AGC)(struct nim_device *dev, UINT8 *agc);			/* Get Current NIM Device Channel AGC Value */
	INT32	(*get_fft_result)(struct nim_device *dev, UINT32 freq, UINT32 *start_addr);	/* Get Current NIM Device Channel FFT spectrum result */
	INT32	(*channel_search)(struct nim_device *dev, UINT32 freq);				//should be opened in DVBS
        const char *(*get_ver_infor)(struct nim_device *dev);				/* Get Current NIM Device Version Number */

}nim_device;

/****************************************************************************
*	Nim tuner header
*
****************************************************************************/

#define MAX_TUNER_SUPPORT_NUM		2

/*external demodulator config parameter*/
struct EXT_DM_CONFIG{
	UINT32 i2c_base_addr;
	UINT32 i2c_type_id;
	UINT32 dm_crystal;
	UINT32 dm_clock;
	UINT32 polar_gpio_num;
    UINT32 lock_polar_reverse;
};


#define LNB_CMD_BASE		0xf0
#define LNB_CMD_ALLOC_ID	(LNB_CMD_BASE+1)
#define LNB_CMD_INIT_CHIP	(LNB_CMD_BASE+2)
#define LNB_CMD_SET_POLAR	(LNB_CMD_BASE+3)
#define LNB_CMD_POWER_EN	(LNB_CMD_BASE+4)

/*external lnb controller config parameter*/
struct EXT_LNB_CTRL_CONFIG{
	UINT32 param_check_sum; //ext_lnb_control+i2c_base_addr+i2c_type_id = param_check_sum
	INT32 	(*ext_lnb_control) (UINT32, UINT32, UINT32);
	UINT32 i2c_base_addr;
	UINT32 i2c_type_id;
	UINT8 int_gpio_en;
	UINT8 int_gpio_polar;
	UINT8 int_gpio_num;
};

struct COFDM_TUNER_CONFIG_DATA
{
	UINT8 *ptMT352;
	UINT8 *ptMT353;
	UINT8 *ptST0360;	
	UINT8 *ptST0361;
	UINT8 *ptST0362;
	UINT8 *ptAF9003;
	UINT8  *ptNXP10048;
	UINT8  *ptSH1432;
	UINT16 *ptSH1409;

//for ddk and normal design.
	//for I/Q conncetion config. bit2: I/Q swap. bit1: I_Diff swap. bit0: Q_Diff swap.< 0: no, 1: swap>; 
	UINT8 Connection_config;
	//bit0: IF-AGC enable <0: disable, 1: enalbe>;bit1: IF-AGC slop <0: negtive, 1: positive>
	//bit2: RF-AGC enable <0: disable, 1: enalbe>;bit3: RF-AGC slop <0: negtive, 1: positive>
	//bit4: Low-if/Zero-if.<0: Low-if, 1: Zero-if>
	UINT16 Cofdm_Config;

	UINT8 AGC_REF;
	UINT8 RF_AGC_MAX;
	UINT8 RF_AGC_MIN;
	UINT8 IF_AGC_MAX;
	UINT8 IF_AGC_MIN;
	UINT32 i2c_type_sel;
	UINT32 i2c_type_sel_1;//for I2C_SUPPORT_MUTI_DEMOD	
	UINT8 demod_chip_addr;
	UINT8 demod_chip_addr1;
	UINT8 demod_chip_ver;
	UINT8 tnuer_id;
	UINT8  cTuner_Tsi_Setting_0;
	UINT8  cTuner_Tsi_Setting_1;
};

struct COFDM_TUNER_CONFIG_EXT
{
	UINT8  cTuner_Crystal;
	UINT8  cTuner_Base_Addr;		/* Tuner BaseAddress for Write Operation: (BaseAddress + 1) for Read */	
	UINT8  cChip;
	UINT8  cTuner_Ref_DivRatio;
	UINT16 wTuner_IF_Freq;
	UINT8  cTuner_AGC_TOP;
	UINT16 cTuner_Step_Freq;
	INT32  (*Tuner_Write)(UINT32 id, UINT8 slv_addr, UINT8 *data, int len);		/* Write Tuner Program Register */
	INT32  (*Tuner_Read)(UINT32 id, UINT8 slv_addr, UINT8 *data, int len);		/* Read Tuner Status Register */	
	INT32  (*Tuner_Write_Read)(UINT32 id, UINT8 slv_addr, UINT8 *data, UINT8 wlen,int len);
	UINT32 i2c_type_id;	/*i2c type and dev id select. bit16~bit31: type, I2C_TYPE_SCB/I2C_TYPE_GPIO. bit0~bit15:dev id, 0/1.*/				
};


struct COFDM_TUNER_CONFIG_API
{	
	struct COFDM_TUNER_CONFIG_DATA config_data;	
	INT32 (*nim_Tuner_Init) (struct COFDM_TUNER_CONFIG_EXT * ptrTuner_Config);		
	INT32 (*nim_Tuner_Control) (UINT32 freq, UINT8 bandwidth,UINT8 AGC_Time_Const,UINT8 *data,UINT8 cmd_type);	
	INT32 (*nim_Tuner_Status) (UINT8 *lock);				
	INT32 (*nim_Tuner_Cal_Agc) (UINT8 flag, UINT16 rf_val, UINT16 if_val, UINT8 *data);

    void (*nim_lock_cb) (UINT8 lock);				
	struct COFDM_TUNER_CONFIG_EXT tuner_config;
	struct EXT_DM_CONFIG ext_dm_config;
};


typedef enum//For  TSI  select
{
	NIM_0_SPI_0	  = 0,
	NIM_0_SPI_1	 ,
	NIM_1_SPI_0	 ,
	NIM_1_SPI_1	 ,
	NIM_0_SSI_0	 ,
	NIM_0_SSI_1	 ,
	NIM_1_SSI_0	 ,
	NIM_1_SSI_1	
} NIM_TSI_Setting ;

struct QPSK_TUNER_CONFIG_DATA
{
	UINT16 Recv_Freq_Low;
	UINT16 Recv_Freq_High;
	UINT16 Ana_Filter_BW;
	UINT8 Connection_config;
	UINT8 Reserved_byte;
	UINT8 AGC_Threshold_1;
	UINT8 AGC_Threshold_2;
	UINT16 QPSK_Config;/*bit0:QPSK_FREQ_OFFSET,bit1:EXT_ADC,bit2:IQ_AD_SWAP,bit3:I2C_THROUGH,bit4:polar revert bit5:NEW_AGC1,bit6bit7:QPSK bitmode:
	                                00:1bit,01:2bit,10:4bit,11:8bit*/
};

struct QPSK_TUNER_CONFIG_EXT
{
	UINT16 wTuner_Crystal;			/* Tuner Used Crystal: in KHz unit */
	UINT8  cTuner_Base_Addr;		/* Tuner BaseAddress for Write Operation: (BaseAddress + 1) for Read */
	UINT8  cTuner_Out_S_D_Sel;		/* Tuner Output mode Select: 1 --> Single end, 0 --> Differential */
	UINT32 i2c_type_id;	/*i2c type and dev id select. bit16~bit31: type, I2C_TYPE_SCB/I2C_TYPE_GPIO. bit0~bit15:dev id, 0/1.*/	
};

struct QPSK_TUNER_CONFIG_API
{
	/* struct for QPSK Configuration */
	struct   QPSK_TUNER_CONFIG_DATA config_data;

	/* Tuner Initialization Function */
	INT32 (*nim_Tuner_Init) (UINT32* tuner_id, struct QPSK_TUNER_CONFIG_EXT * ptrTuner_Config);

	/* Tuner Parameter Configuration Function */
	INT32 (*nim_Tuner_Control) (UINT32 tuner_id, UINT32 freq, UINT32 sym);

	/* Get Tuner Status Function */
	INT32 (*nim_Tuner_Status) (UINT32 tuner_id, UINT8 *lock);

	/* Extension struct for Tuner Configuration */
	struct QPSK_TUNER_CONFIG_EXT tuner_config;
	struct EXT_DM_CONFIG ext_dm_config;
	struct EXT_LNB_CTRL_CONFIG ext_lnb_config;
};

extern INT32 nim_hz6306_init(UINT32* tuner_id, struct QPSK_TUNER_CONFIG_EXT * ptrTuner_Config);
extern INT32 nim_hz6306_control(UINT32 tuner_id, UINT32 freq, UINT32 sym);
extern INT32 nim_hz6306_status(UINT32 tuner_id, UINT8 *lock);

extern INT32 nim_vz7306_init(UINT32* tuner_id, struct QPSK_TUNER_CONFIG_EXT * ptrTuner_Config);
extern INT32 nim_vz7306_control(UINT32 tuner_id, UINT32 freq, UINT32 sym);
extern INT32 nim_vz7306_status(UINT32 tuner_id, UINT8 *lock);

extern INT32 nim_s3501_attach (struct QPSK_TUNER_CONFIG_API * ptrQPSK_Tuner);

struct QAM_TUNER_CONFIG_DATA
{
	UINT8 RF_AGC_MAX;//x.y V to xy value, 5.0v to 50v(3.3v to 33v)Qam then use it configue register.
	UINT8 RF_AGC_MIN;//x.y V to xy value, 5.0v to 50v(3.3v to 33v)Qam then use it configue register.
	UINT8 IF_AGC_MAX;//x.y V to xy value, 5.0v to 50v(3.3v to 33v)Qam then use it configue register.
	UINT8 IF_AGC_MIN;//x.y V to xy value, 5.0v to 50v(3.3v to 33v)Qam then use it configue register.
	UINT8 AGC_REF; //the average amplitude to full scale of A/D. % percentage rate.
};

struct QAM_TUNER_CONFIG_EXT
{
	UINT8  cTuner_Crystal;
	UINT8  cTuner_Base_Addr;		/* Tuner BaseAddress for Write Operation: (BaseAddress + 1) for Read */
	UINT8  cChip;
	UINT8  cTuner_special_config;		/*0x01, RF AGC is disabled*/
	UINT8  cTuner_Ref_DivRatio;
	UINT16 wTuner_IF_Freq;
	UINT8  cTuner_AGC_TOP;
	UINT8  cTuner_Step_Freq;
//	INT32  (*Tuner_Write)(UINT32 id, UINT8 slv_addr, UINT8 *data, int len);		/* Write Tuner Program Register */
//	INT32  (*Tuner_Read)(UINT32 id, UINT8 slv_addr, UINT8 *data, int len);		/* Read Tuner Status Register */	
	UINT32 i2c_type_id;	/*i2c type and dev id select. bit16~bit31: type, I2C_TYPE_SCB/I2C_TYPE_GPIO. bit0~bit15:dev id, 0/1.*/	
};

struct QAM_TUNER_CONFIG_API
{
	/* struct for QAM Configuration */
	struct QAM_TUNER_CONFIG_DATA tuner_config_data;

	/* Tuner Initialization Function */
	INT32 (*nim_Tuner_Init)(UINT32 * ptrTun_id, struct QAM_TUNER_CONFIG_EXT * ptrTuner_Config);

	/* Tuner Parameter Configuration Function */
	INT32 (*nim_Tuner_Control)(UINT32 Tun_id, UINT32 freq, UINT32 sym, UINT8 AGC_Time_Const, UINT8 _i2c_cmd);//since there will no bandwidth demand, so pass "sym" for later use.

	/* Get Tuner Status Function */
	INT32 (*nim_Tuner_Status)(UINT32 Tun_id, UINT8 *lock);

	/* Extension struct for Tuner Configuration */
	struct QAM_TUNER_CONFIG_EXT tuner_config_ext;
	
	struct EXT_DM_CONFIG ext_dem_config;

};

/********************************************************************
*					OS Definition 
* 
*   Update the definition for your system
********************************************************************/
typedef unsigned short 				ID_THREAD;
typedef unsigned short 				ID;
typedef short						PRI;
//typedef void (*FP)(DWORD , DWORD );
typedef void (*FP)(UINT32 param1, UINT32 param2);


#define OSAL_INVALID_ID  			0xFFFF
#define OSAL_E_FAIL				(-1)
#define NIM_S3501_STACKSIZE		0xC00

/* Task structure */
typedef struct t_ctsk
{
    FP      task;
    PRI     itskpri;
    INT32   stksz;
    UINT32  quantum;
    UINT32  para1;
    UINT32  para2;
    char    name[3];    //added by jeff wu 2003-05-27
} T_CTSK;

/* Type define for task priority */
enum
{
	OSAL_PRI_LOW		= 31,			/* Lowest,  for idle task */
	OSAL_PRI_NORMAL	= 20,			/* Normal,  for user task */
	OSAL_PRI_HIGH		= 17,				/* High,    for system task */
	OSAL_PRI_CRITICL	= 10,			/* Highest, for HSR scheduler */
};

/*********************************************************************
*    Nim header
*
*********************************************************************/
// DiSEqC mode 
#define NIM_DISEQC_MODE_22KOFF			0	/* 22kHz off */
#define	NIM_DISEQC_MODE_22KON		1	/* 22kHz on */
#define	NIM_DISEQC_MODE_BURST0		2	/* Burst mode, on for 12.5mS = 0 */
#define	NIM_DISEQC_MODE_BURST1		3	/* Burst mode, modulated 1:2 for 12.5mS = 1 */
#define	NIM_DISEQC_MODE_BYTES			4	/* Modulated with bytes from DISEQC INSTR */
#define	NIM_DISEQC_MODE_ENVELOP_ON	5	/* Envelop enable*/
#define	NIM_DISEQC_MODE_ENVELOP_OFF	6	/* Envelop disable, out put 22K wave form*/
#define	NIM_DISEQC_MODE_OTHERS		7	/* Undefined mode */
#define 	DISEQC2X_ERR_NO_REPLY			0x01
#define 	DISEQC2X_ERR_REPLY_PARITY		0x02
#define 	DISEQC2X_ERR_REPLY_UNKNOWN	0x03
#define 	DISEQC2X_ERR_REPLY_BUF_FUL	0x04

// Polarization
#define NIM_PORLAR_HORIZONTAL	0x00
#define NIM_PORLAR_VERTICAL		0x01
#define NIM_PORLAR_LEFT			0x02
#define NIM_PORLAR_RIGHT		0x03

#define NIM_PORLAR_REVERSE		0x01
#define NIM_PORLAR_SET_BY_22K	0x02

#ifdef __cplusplus
extern "C"
{
#endif

/* Socket management functions */
INT32 nim_open(struct nim_device *dev);
INT32 nim_close(struct nim_device *dev);
INT32 nim_io_control(struct nim_device *dev, INT32 cmd, UINT32 param);
INT32 nim_get_lock(struct nim_device *dev, UINT8 *lock);
INT32 nim_get_freq(struct nim_device *dev, UINT32 *freq);
INT32 nim_get_FEC(struct nim_device *dev, UINT8 *fec);
INT32 nim_get_SNR(struct nim_device *dev, UINT8 *snr);
INT32 nim_ioctl_ext(struct nim_device *dev, INT32 cmd, void * param_list);
INT32 nim_set_polar(struct nim_device *dev, UINT8 polar);
INT32 nim_set_12v(struct nim_device *dev, UINT8 flag);
INT32 nim_channel_change(struct nim_device *dev, UINT32 freq, UINT32 sym, UINT8 fec);
INT32 nim_channel_search(struct nim_device *dev, UINT32 freq);
INT32 nim_DiSEqC_operate(struct nim_device *dev, UINT32 mode, UINT8* cmd, UINT8 cnt);
INT32 nim_DiSEqC2X_operate(struct nim_device *dev, UINT32 mode, UINT8* cmd, UINT8 cnt, UINT8 *rt_value, UINT8 *rt_cnt);

INT32 nim_get_sym(struct nim_device *dev, UINT32 *sym);
INT32 nim_get_BER(struct nim_device *dev, UINT32 *ber);
INT32 nim_get_fft_result(struct nim_device *dev, UINT32 freq, UINT32* start_adr);
INT32 nim_get_AGC(struct nim_device *dev, UINT8 *agc);
INT32 nim_get_map_type(struct nim_device *dev, UINT8 *agc);



#ifdef __cplusplus
}
#endif


/*********************************************************************
*	    Implement Code
*  Please implement these function in Porting.c
*
**********************************************************************/

void comm_delay(UINT32 n);

//I2C implement
#define I2C_FOR_NIM	 0xC0//TUNER HZ6303 I2C ADDRESS 

INT32 i2c_write(UINT32 id, UINT8 sa, UINT8 *p, int wl);
INT32 i2c_read(UINT32 id, UINT8 sa, UINT8 *p, int rl);
INT32 i2c_write_read(UINT32 id, UINT8 sa, UINT8 *p, int wl, int rl);

S32 DVBSBreak(void);

// System alloc implement
void * comm_malloc(unsigned int size);
void comm_free(void * ptr);
void* comm_memset(void *dest, int c, unsigned int  len);
void* comm_memcpy(void *dest, const void *src, unsigned int len);
int comm_strcmp(const char *dest, const char *src);
int comm_strcpy(char *dest, const char *src);

//OS Implementation function
ID_THREAD osal_task_create(T_CTSK * pk_cthd);

#define NIM_MUTEX_ENTER(...)		
#define NIM_MUTEX_LEAVE(...)
#endif
