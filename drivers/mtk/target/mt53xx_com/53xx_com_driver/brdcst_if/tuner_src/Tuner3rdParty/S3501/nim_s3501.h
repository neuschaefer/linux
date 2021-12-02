/*****************************************************************************
*    Copyright (C)2003 Ali Corporation. All Rights Reserved.
*
*    File:    nim_s3501.h
*
*    Description:    Header file in LLD. MTK sdk release
*    History:
*           Date            	Athor        	Version          		Reason
*	    ========		=======	==========	==========================
*   	1.  Oct.12.2009  	Douglass         	Ver 0.1                   	Create file for S3501 DVBS2 project
*
*****************************************************************************/

#ifndef __LLD_NIM_S3501_H__
#define __LLD_NIM_S3501_H__

#include "Porting.h"


#define		FFT_BITWIDTH			10

#define		STATISTIC_LENGTH		2
//average length of data to determine threshold
//0:2;1:4;2:8

#define		MAX_CH_NUMBER			32//maximum number of channels that can be stored

#define s3501_LOACL_FREQ   		5150
#define s3501_DEBUG_FLAG			0
#define QPSK_TUNER_FREQ_OFFSET	4
#define M3501_IQ_AD_SWAP   		0x04
#define M3501_EXT_ADC          		0x02
#define M3501_QPSK_FREQ_OFFSET 	0x01
#define M3501_I2C_THROUGH           	0x08
#define M3501_NEW_AGC1                	0x20
#define M3501_POLAR_REVERT          	0x10
#define M3501_1BIT_MODE                	0x00 
#define M3501_2BIT_MODE                	0x40
#define M3501_4BIT_MODE                	0x80
#define M3501_8BIT_MODE                	0xc0
#define M3501_AGC_INVERT			0x100
//#define M3501_SPI_SSI_MODE		0x200     8bit mode(SPI) 1bit mode(SSI)
#define M3501_USE_188_MODE		0x400
#define M3501_DVBS_MODE			0x00	
#define M3501_DVBS2_MODE		0x01
#define M3501_SIGNAL_DISPLAY_LIN	0x800

#define NIM_S3501_BASE_IO_ADR  0xB8003000


#define NIM_GET_DWORD(i)           (*(volatile UINT32 *)(i))
#define NIM_SET_DWORD(i,d)        (*(volatile UINT32 *)(i)) = (d)

#define NIM_GET_WORD(i)             (*(volatile UINT16 *)(i))
#define NIM_SET_WORD(i,d)          (*(volatile UINT16 *)(i)) = (d)

#define NIM_GET_BYTE(i)             (*(volatile UINT8 *)(i))
#define NIM_SET_BYTE(i,d)          (*(volatile UINT8 *)(i)) = (d)


#define S3501_FREQ_OFFSET 1
#define LNB_LOACL_FREQ   5150

#define AS_FREQ_MIN     900
#define AS_FREQ_MAX     2200


#define NIM_OPTR_CHL_CHANGE0		0x70
#define NIM_OPTR_CHL_CHANGE			0x00
#define NIM_OPTR_SOFT_SEARCH		0x01
#define NIM_OPTR_FFT_RESULT			0x02
#define NIM_OPTR_DYNAMIC_POW		0x03
#define NIM_OPTR_DYNAMIC_POW0		0x73
#define NIM_OPTR_IOCTL				0x04
#define NIM_OPTR_HW_OPEN			0x05
#define NIM_OPTR_HW_CLOSE			0x06

#define NIM_DEMOD_CTRL_0X50			0x50
#define NIM_DEMOD_CTRL_0X51			0x51
#define NIM_DEMOD_CTRL_0X90			0x90
#define NIM_DEMOD_CTRL_0X91			0x91
#define NIM_DEMOD_CTRL_0X02			0x02
#define NIM_DEMOD_CTRL_0X52			0x52



#define NIM_LOCK_STUS_NORMAL				0x00
#define NIM_LOCK_STUS_SETTING				0x01
#define NIM_LOCK_STUS_CLEAR					0x02
#define NIM_SWITCH_TR_CR			0x01
#define NIM_SWITCH_RS				0x02
#define NIM_SWITCH_FC				0x04
#define NIM_SWITCH_HBCD				0x08

#define TS_DYM_HEAD0 				0x47
#define TS_DYM_HEAD1 				0x1f
#define TS_DYM_HEAD2 				0xff
#define TS_DYM_HEAD3 				0x10
#define TS_DYM_HEAD4 				0x00

/* 3501 register define */
enum NIM3501_REGISTER_ADDRESS{	
		R00_CTRL				= 0x00,		/* 		NIM3501 control register 				*/
		R01_ADC				= 0x01,		/* 		ADC Configuration Register 			*/
		R02_IERR				= 0x02,		/* 		Interrupt Events Register 				*/
		R03_IMASK				= 0x03,		/* 		Interrupt Mask Register 				*/
		R04_STATUS				= 0x04,		/* 		Status Register 						*/
		R05_TIMEOUT_TRH		= 0x05,		/* 		HW Timeout Threshold Register(LSB) 	*/
		R07_AGC1_CTRL			= 0x07,		/* 		AGC1 reference value register 			*/
		R0A_AGC1_LCK_CMD		= 0x0a,		/* 		AGC1 lock command register 			*/
		R10_DCC_CFG			= 0x10,		/* 		DCC Configure Register 				XXX*/
		R11_DCC_OF_I			= 0x11,		/* 		DCC Offset I monitor Register 			XXX*/
		R12_DCC_OF_Q			= 0x12,		/* 		DCC Offset Q monitor Register 			XXX*/
		R13_IQ_BAL_CTRL		= 0x13,		/* 		IQ Balance Configure Register 			XXX*/
		R15_FLT_ROMINDX		= 0x15,		/* 		Filter Bank Rom Index Register 			XXX*/
		R16_AGC2_REFVAL		= 0x16,		/* 		AGC2 Reference Value Register 			XXX*/
		R17_AGC2_CFG			= 0x17,		/* 		AGC2 configure register 				XXX*/
		R18_TR_CTRL			= 0x18,		/* 		TR acquisition gain register 			XXX*/
		R1B_TR_TIMEOUT_BAND	= 0x1b,		/*		TR Time out band register 				*/
		R21_BEQ_CTRL			= 0x21,		/* 		BEQ Control REgister 				XXX*/
		R22_BEQ_CMA_POW		= 0x22,		/* 		BEQ CMA power register 				XXX*/
		R24_MATCH_FILTER		= 0x24,		/* 		Match Filter Register 					*/
		R25_BEQ_MASK			= 0x25,		/* 		BEQ Mask Register 					*/
		R26_TR_LD_LPF_OPT		= 0x26,		/* 		TR LD LPF Output register 				XXX*/
		R28_PL_TIMEOUT_BND	= 0x28,		/* 		PL Time out Band REgister 			*/
		R2A_PL_BND_CTRL		= 0x2a,		/* 		PL Time Band Control */
		R2E_PL_ANGLE_UPDATE	= 0x2e,		/* 		PL Angle Update High/Low limit register 	XXX*/
		R30_AGC3_CTRL			= 0x30,		/* 		AGC3  Control Register 				XXX*/
		R33_CR_CTRL			= 0x33,		/* 		CR DVB-S/DVBS-S2  CONTROL register */
		R45_CR_LCK_DETECT		= 0x45,		/* 		CR lock detecter lpf monitor register */
		R47_HBCD_TIMEOUT		= 0x47,		/* 		HBCD Time out band register */
		R48_VITERBI_CTRL		= 0x48,		/* 		Viterbi module control register 			XXX*/
		R57_LDPC_CTRL			= 0x57,		/*		LDPC control register */
		R5B_ACQ_WORK_MODE	= 0x5b,		/*		Acquiescent work mode register */
		R5C_ACQ_CARRIER		= 0x5c,		/*		Acquiescent carrier control register */
		R5F_ACQ_SYM_RATE		= 0x5f,		/* 		Acquiescent symbol rate register */
		R62_FC_SEARCH			= 0x62,		/* 		FC Search Range Register */
		R64_RS_SEARCH			= 0x64,		/* 		RS Search Range Register */
		R66_TR_SEARCH			= 0x66,		/* 		TR Search Step register */
		R67_VB_CR_RETRY		= 0x67,		/* 		VB&CR Maximum Retry Number Register */
		R68_WORK_MODE			= 0x68,		/* 		Work Mode Report Register */
		R69_RPT_CARRIER		= 0x69,		/* 		Report carrier register */
		R6C_RPT_SYM_RATE		= 0x6c,		/* 		report symbol rate register */
		R6F_FSM_STATE			= 0x6f,		/* 		FSM State Moniter Register 			XXX*/
		R70_CAP_REG			= 0x70,		/* 		Capture Param register */
		R74_PKT_STA_NUM		= 0x74,		/* 		Packet Statistic Number Register */
		R76_BIT_ERR				= 0x76,		/* 		Bit Error Register */
		R79_PKT_ERR			= 0x79,		/* 		Packet Error Register */
		R7B_TEST_MUX			= 0x7b,		/* 		Test Mux Select REgister 				XXX*/
		R7C_DISEQC_CTRL		= 0x7c,		/* 		DISEQC Control Register */
		R86_DISEQC_RDATA		= 0x86,		/* 		Diseqc data for read  */
		R8E_DISEQC_TIME		= 0x8e,		/*		Diseqc time register 	*/
		R90_DISEQC_CLK_RATIO	= 0x90,		/*		Diseqc clock ratio register */
		R97_S2_FEC_THR			= 0x97,		/* 		S2 FEC Threshold register 				XXX*/
		R99_H8PSK_THR			= 0x99,		/* 		H8PSK CR Lock Detect threshold register 	XXX*/
		R9C_DEMAP_BETA			= 0x9c,		/* 		Demap Beta register */
		RA0_RXADC_REG			= 0xa0,		/* 		RXADC ANATST/POWER register */
		RA3_CHIP_ID				= 0xa3,		/* 		Chip ID REgister */
		RA5_VER_ID				= 0xa5,		/* 		version ID register					XXX*/
		RA7_I2C_ENHANCE		= 0xa7,		/* 		I2C Enhance Register 				XXX*/
		RA8_M90_CLK_DCHAN		= 0xa8,		/* 		M90 clock delay chain register 			XXX*/
		RA9_M180_CLK_DCHAN	= 0xa9,		/* 		M180 Clock delay chain register 		XXX*/
		RAA_S2_FEC_ITER		= 0xaa,		/* 		S2 FEC iteration counter register 		XXX*/
		RAD_TSOUT_SYMB		= 0xad,		/* 		ts  out setting SYMB_PRD_FORM_REG */
		RAF_TSOUT_PAD			= 0xaf,		/* 		TS out setting and pad driving register */
		RB0_PLL_CONFIG			= 0xb0,		/* 		PLL configure REgister 				XXX*/
		RB1_TSOUT_SMT			= 0xb1,		/* 		TS output Setting and Pad driving */
		RB3_PIN_SHARE_CTRL		= 0xb3,		/* 		Pin Share Control register */
		RB5_CR_PRS_TRA			= 0xb5,		/* 		CR DVB-S/S2 PRS in Tracking State */
		RB6_H8PSK_CETA			= 0xb6,		/* 		H8PSK COS/SIN Ceta Value Register 		XXX*/
		RB8_LOW_RS_CLIP		= 0xb8,		/* 		Low RS Clip Value REgister 			XXX*/
		RBA_AGC1_REPORT		= 0xba,		/* 		AGC1 report register 				XXX*/
		RBD_CAP_PRM			= 0xbd,		/* 		Capture Config/Block register */
		RBF_S2_FEC_DBG			= 0xbf,		/* 		DVB-S2 FEC Debug REgister 			XXX*/
		RC0_BIST_LDPC_REG		= 0xc0,		/* 		LDPC Average Iteration counter register 	XXX*/
		RC1_DVBS2_FEC_LDPC		= 0xc1,		/* 		DVBS2 FEC LDPC Register */
		RC8_BIST_TOLERATOR		= 0xc8,		/* 0xc0	Tolerator MBIST register */
		RC9_CR_OUT_IO_RPT		= 0xc9,		/* 		Report CR OUT I Q */
		// for s3501B
		RCB_I2C_CFG				= 0xcb,		/* 		I2C Slave Configure Register */
		RCC_STRAP_PIN_CLOCK	= 0xcc,		/* 		strap pin and clock enable register 		XXX*/
		RCD_I2C_CLK			= 0xcd,		/* 		I2C AND CLOCK ENABLE REGISTER 		XXX*/
		RCE_TS_FMT_CLK			= 0xce,		/* 		TS Format and clock enable register */
		RD0_DEMAP_NOISE_RPT	= 0xd0,		/* 		demap noise rtp register 				XXX*/
		RD3_BER_REG			= 0xd3,		/* 		BER register */
		RD6_LDPC_REG			= 0xd6,		/* 		LDPC register 						XXX*/
		RD7_EQ_REG				= 0xd7,		/* 		EQ register */
		RD8_TS_OUT_SETTING		= 0xd8,		/* 		TS output setting register */
		RD9_TS_OUT_CFG			= 0xd9,		/* 		BYPASS register 					XXX*/
		RDA_EQ_DBG				= 0xda,		/* 		EQ Debug Register */
		RDC_EQ_DBG_TS_CFG		= 0xdc,		/* 		EQ debug and ts config register */
		RDD_TS_OUT_DVBS		= 0xdd,		/* 		TS output dvbs mode setting  */
		RDF_TS_OUT_DVBS2		= 0xdf,		/* 		TS output dvbs2 mode setting  */
			
};

struct nim_s3501_TskStatus
{
	UINT32   	m_lock_flag;
	ID 		m_task_id;
	UINT32 m_sym_rate;
	UINT8 	m_work_mode;
	UINT8 	m_map_type;
	UINT8 	m_code_rate;
	UINT8 	m_info_data;

};
struct nim_s3501_TParam
{
	int t_last_snr;		
	int t_last_iter;
	int t_aver_snr;
	int t_snr_state;
	int t_snr_thre1;
	int t_snr_thre2;
	int t_snr_thre3;
	INT32 t_phase_noise_detected;
	INT32 t_dynamic_power_en;
	UINT32 phase_noise_detect_finish;
	UINT32 t_reg_setting_switch;
	UINT8 t_i2c_err_flag;

};
struct nim_s3501_LStatus
{
	ID nim_s3501_sema;
	ER ret;
	UINT8 s3501_autoscan_stop_flag;
	UINT8 s3501_chanscan_stop_flag;
	UINT32 old_ber ;
	UINT32 old_per ;
	UINT32 old_ldpc_ite_num;
	UINT8  *ADCdata;// = (unsigned char *)__MM_DMX_FFT_START_BUFFER;//[2048];
	UINT8  *ADCdata_raw_addr;
	INT32	m_Freq[256];
	UINT32   m_Rs[256];
	INT32    FFT_I_1024[1024];
	INT32	FFT_Q_1024[1024];
	UINT8   m_CRNum;
	UINT32  m_CurFreq;
	UINT8    c_RS ;
	UINT32   m_StepFreq;
	pfn_nim_reset_callback m_pfn_reset_s3501;
	UINT8 m_enable_dvbs2_hbcd_mode;
	UINT8 m_dvbs2_hbcd_enable_value;
	UINT8   s3501d_lock_status;
	//UINT8   phase_err_check_status;
	UINT32   phase_err_check_status;
	UINT32   m_s3501_type;
	UINT32   m_s3501_sub_type;
	UINT32 m_setting_freq;
	UINT32   m_Err_Cnts;
    	UINT8    m_hw_timeout_thr;
};

struct nim_s3501_private
{
	INT32 	(*nim_Tuner_Init) (UINT32*, struct QPSK_TUNER_CONFIG_EXT *);	/* Tuner Initialization Function */
	INT32 	(*nim_Tuner_Control) (UINT32, UINT32, UINT32);	/* Tuner Parameter Configuration Function */
	INT32 	(*nim_Tuner_Status) (UINT32, UINT8 *);	
	struct   QPSK_TUNER_CONFIG_DATA Tuner_Config_Data;
	UINT32 tuner_id;
	UINT32 i2c_type_id;
	UINT32 polar_gpio_num;
	UINT32 sys_crystal;
	UINT32 sys_clock;
	UINT16 pre_freq ;
	UINT16 pre_sym ;
	INT8 autoscan_stop_flag ;
	struct nim_device_stats stats;
	UINT8 chip_id;
	struct EXT_DM_CONFIG ext_dm_config;
	struct nim_s3501_LStatus ul_status;
	INT32	ext_lnb_id;
	INT32 	(*ext_lnb_control) (UINT32, UINT32, UINT32);
	struct nim_s3501_TskStatus tsk_status;
	struct nim_s3501_TParam t_Param;
};
#if 1


 const char *nim_s3501_get_ver_infor(struct nim_device *dev);
 INT32 nim_s3501_reg_get_map_type(struct nim_device*dev,UINT8*map_type);
#endif
INT32 nim_s3501_FFT( struct nim_device *dev, UINT32 startFreq );


UINT32 nim_s3501_get_CURFreq(struct nim_device *dev);
UINT8 nim_s3501_get_CRNum(struct nim_device *dev);
void nim_s3501_set_FFT_para( struct nim_device *dev );
INT32 nim_s3501_reg_get_freq( struct nim_device *dev, UINT32 *freq);
INT32 nim_s3501_reg_get_symbol_rate(struct nim_device *dev, UINT32 *sym_rate);
INT32 nim_s3501_reg_get_code_rate(struct nim_device *dev, UINT8* code_rate);
void nim_s3501_clear_int(struct nim_device *dev );

void nim_s3501_set_CodeRate(struct nim_device *dev, UINT8 coderate );
void nim_s3501_set_RS(struct nim_device *dev, UINT32 rs );
void nim_s3501_set_freq_offset(struct nim_device *dev, INT32 delfreq );

INT32 nim_s3501_soft_search(struct nim_device *dev, UINT32 *rs, UINT32 *freq, INT32 delfreq );

INT32 nim_s3501_fft_find_channel( struct nim_device *dev,UINT32* tune_freq );
void nim_s3501_calculate_energy(struct nim_device *dev);
void nim_s3501_fft_result_read(struct nim_device *dev);
INT32 nim_S3501_cap(struct nim_device *dev, UINT32 startFreq,UINT32 sym);
INT32 nim_s3501_get_bitmode(struct nim_device*dev,UINT8 * bitMode);
INT32 nim_reg_read(struct nim_device* dev,UINT8 bMemAdr, UINT8 *pData, UINT8 bLen);//liuyuan add
INT32 nim_reg_write(struct nim_device* dev, UINT8 bMemAdr, UINT8 *pData, UINT8 bLen);//liuyuan add

#if BLIND_SCAN
//Nim Autosearch,R2FFT 
extern void nim_s3501_smoothfilter(void);
extern INT32 nim_s3501_autosearch(INT32 *success, INT32 *delta_fc_est, INT32 *SymbolRate_est, INT32 * m_IF_freq, INT32* m_ch_number);
extern void	R2FFT(INT32 *FFT_I_1024, INT32 *FFT_Q_1024);
extern INT32 nim_s3501_autoscan_signal_input(struct nim_device * dev, UINT8 s_Case);
#endif
#endif	/* __LLD_NIM_S3501_H__ */


