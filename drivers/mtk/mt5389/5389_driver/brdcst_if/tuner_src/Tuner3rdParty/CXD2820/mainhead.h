/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
#include "sony_dvb.h"
#include "sony_dvb_demod.h"
#include "sony_dvb_cxd2820.h"
#include "x_tuner.h"


/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 * type definition (struct / enum):
 ******************************************************************************
 ******************************************************************************
 *****************************************************************************/
/*************************************************
 Block ID definition for I2C access
**************************************************/
typedef enum {
    TUNER_BLOCK,       /* Tag for specifing Tuner block at I2C function */
    DEMOD_DVBT_BLOCK,   /* Tag for specifing Demod block at I2C function */
    DEMOD_DVBC_BLOCK   /* Tag for specifing Demod DVB-C block at I2C function */
} BLOCKID;

/*************************************************
 TS Lock Status
**************************************************/
typedef enum {
    TS_UNKNOWN,     /* Unknown */
    TS_LOCK,        /* TS Lock */
    TS_UNLOCK       /* Unlock ( Unlock_Flag==1 or Time out) */
} T_TS_LOCK;

/*************************************************
 TPS Lock Status
**************************************************/
typedef enum {
    TPS_UNKNOWN,
    TPS_LOCK,
    TPS_UNLOCK
} T_TPS_LOCK;


/*************************************************
 Demod Lock Status
**************************************************/
typedef enum {
    DEM_UNKNOWN,
    DEM_LOCK,
    DEM_UNLOCK
} T2_DEM_LOCK;
typedef enum {
    AR_UNKNOWN,     /* unknown */
    AR_LOCK,        /* AR Lock */
    AR_UNLOCK,      /* AR Unlock (no DVB-C signal/polling time out) */
} C_AR_LOCK;

/*************************************************
 DVB-T tuning parameter struct
**************************************************/
typedef struct {
    unsigned char  m_mode;          /* mode 0:zapping 1:scannig */
    INT32  m_frequency;     /* center frequency */
    unsigned char  m_BandWidth;     /* 0:N/A 1:6MHzBandWidth 2:7MHzBandWidth 3:8MHzBandWidth */
    unsigned char  m_hierarchy;     /* 0:High Priority 1:Low Priority */
    unsigned char  m_CoexistL;      /* 0:not coexistence of System-L/L' , 1:coexistence of System-L/L' */
} T_DigitalTuningParam;

/* 2010-05-14 add */
/*************************************************
 PLP Sel Error Flag
**************************************************/
typedef enum {
    PLPSEL_OK,
    PLPSEL_ERROR
} PLPSEL_FLAG;

/* 2010-05-14 add */
/*************************************************
 PLP Sel Error Flag
**************************************************/
typedef enum {
    NO_CHANGE,
    CHANGE_AND_SRST
} L1CHG_RECOVERY_FLAG;
/*************************************************
 DVB-T2 tuning parameter struct
**************************************************/
typedef struct {
     INT32  m_frequency;     /* center frequency                                                */
    unsigned char  m_BandWidth;     /* 0:N/A 1:6MHzBandWidth 2:7MHzBandWidth 3:8MHzBandWidth           */
    unsigned char  m_CoexistL;      /* 0:not coexistence of System-L/L' , 1:coexistence of System-L/L' */
    unsigned char  m_MplpSelect;    /* 0:DefaultPLP  1:PLP selection                                   */
    unsigned char  m_MplpId;        /* PLP ID                                                          */

} T2_DigitalTuningParam;
/*************************************************
 DVB-C tuning parameter struct
**************************************************/
typedef struct {
    unsigned char  m_mode;          /* mode 0:zapping 1:scannig */
    unsigned long  m_frequency;     /* center frequency */
    unsigned char  m_region;        /* 0:not coexistence of System-L/L' , 1:coexistence of System-L/L' */
} C_TuningParam;

/*************************************************
 DVB-T TPS signaling information struct
**************************************************/
typedef struct {
    unsigned int    constellation;
    unsigned int    hierarchy;
    unsigned int    rateHP;
    unsigned int    rateLP;
    unsigned int    guard;
    unsigned int    mode;
    unsigned int    cellID;
} T_TPSInfo;
/*****************************
 LockState
*****************************/
typedef struct {

    enum {
    	DVBT_LOCKSTAT_START,
    	DVBT_LOCKSTAT_WAIT_TPS_LOCK,
    	DVBT_LOCKSTAT_WAIT_TS_LOCK,
    	DVBT_LOCKSTAT_END,
    } m_state;                      /* Lock Sequence State */
	
    T_TS_LOCK     m_tslock;         /* TS Lock Result      */
    T_TPS_LOCK    m_tpslock;        /* TPS Lock Result     */
     INT32   m_offset;         /* CarrierOffset [kHz] */
    signed short  m_waittime;       /* Wait time           */
    unsigned char m_seq_count;      /* Time-Out Counter    */
    unsigned char m_bandwidth;      /* bandwidth           */
} T_LOCKSTAT;
/*****************************
 LockState
*****************************/
typedef struct {
    enum {
        DVBC_LOCKSTAT_START,
        DVBC_LOCKSTAT_WAIT_AR_LOCK,
        DVBC_LOCKSTAT_WAIT_TS_LOCK,
        DVBC_LOCKSTAT_END,
    } m_state;                      /* Lock Sequence State */
    T_TS_LOCK     m_tslock;         /* TS Lock Result      */
    C_AR_LOCK     m_arlock;         /* AR Lock Result     */
    signed short  m_waittime;       /* Wait time           */
    unsigned char m_seq_count;      /* Time-Out Counter    */
} C_LOCKSTAT;
/*************************************************
 DVB-T2 L1 signaling information struct
**************************************************/
typedef struct {
    unsigned int    type;
    unsigned int    bwt_ext;
    unsigned int    mixed;
    unsigned int    miso;
    unsigned int    mode;
    unsigned int    guard;
    unsigned int    pp;
    unsigned int    papr;
    unsigned int    ndsym;
} T2_DemodInfo;

/*****************************
 Demod & TSlock  State
*****************************/
typedef struct {
    enum {
        DEMOD_SEQ_START,               /* 0 */
        DEMOD_SEQ_WAIT_DEM_LOCK,       /* 1 */
        DEMOD_SEQ_WAIT_TS_LOCK,        /* 2 */
        DEMOD_SEQ_END                  /* 3 */
    } m_state;
    T2_DEM_LOCK   m_dem_lock;
    T_TS_LOCK    m_ts_lock;
    int           m_waittime;         /* Wait time                                             */
    unsigned char m_seq_count;        /* Sequence_counter                                      */
    unsigned char m_spectrum_flag;    /* 0: normal, 1:iversion                                 */
    unsigned long m_frequency;        /* Current frequency                                     */
    unsigned char m_BandWidth;        /* 0:N/A 1:6MHzBandWidth 2:7MHzBandWidth 3:8MHzBandWidth */
     INT32 m_offset; //add by SONY AEC 20100419	
} T2_DemodSeqState;

/*************************************************
 Demod setting array struct
**************************************************/
struct CoreSettingInfo
{
    unsigned char m_address;                   /* I2C SubAddress */
    unsigned char m_data;                      /* I2C Data */
};

/************************************************
demod status
*************************************************/
typedef enum {
	UNKNOWN_SYSTEM,
 	DVBT_SYSTEM,
 	DVBC_SYSTEM,
 	DVBT2_SYSTEM
} DEM_Status;



#define PORTING_TEMP     1


#define cMAX_READ_NUM           8


enum
{
    SONY_DVBT2_BW_5MHz = 5,
    SONY_DVBT2_BW_6MHz,
    SONY_DVBT2_BW_7MHz,
    SONY_DVBT2_BW_8MHz
};
/*************************************************
 DVB-C  information struct
**************************************************/
typedef struct {
    UINT32    SymbolRate;
    INT32     QAMSize;
} C_Info;

typedef struct {
	T_TPSInfo TPS_info;
	UINT32 hier;
	UINT32 bandwidth;
} T_Info;

typedef struct {
	T2_DemodInfo demod_info;
} T2_Info;

// add SONY DEMOD CTX
typedef struct _DEMOD_CXD2820_CTX_T DEMOD_CXD2820_CTX_T;
struct _DEMOD_CXD2820_CTX_T
{
  sony_dvb_cxd2820_t 	cxd2820;
  T_Info 				DVBT_info;
  T2_Info 				DVBT2_info;
  C_Info 				DVBC_info;
  UINT32				conn_bandwith;
};


extern int cmd_init( void);
//extern sony_dvb_result_t DVBT_Tuning(unsigned long  freq,int BandW, int  hier,int Coex);
//extern sony_dvb_result_t DVBT2_Tuning(unsigned long  freq,int BandW, int Coex,int plpS,int plpID);


sony_dvb_result_t DVBT2_Tuning(DEMOD_CXD2820_CTX_T * pSonyDemodCtx,unsigned long freq, int BandW,int Coex, int plpS,int plpID);	
sony_dvb_result_t DVBT_Tuning(DEMOD_CXD2820_CTX_T *pSonyDemodCtx,unsigned long  freq,int BandW,int  hier,int Coex) ;
sony_dvb_result_t DVBC_Tuning(DEMOD_CXD2820_CTX_T *pSonyDemodCtx,unsigned long  freq,unsigned char region) ;

TS_FMT_T SONY_GetTsFmt(DEMOD_CXD2820_CTX_T * pSonyDemodCtx);
void SONY_SetTsPath(DEMOD_CXD2820_CTX_T * pSonyDemodCtx, BOOL fgEnable);




extern void Waitms(int ms);
extern int cmd_sleep_dvbt2(void);
//extern sony_dvb_result_t dvb_demod_CheckLock(DEM_Status system, sony_dvb_result_t *LockStatus);
sony_dvb_result_t dvb_demod_CheckLock(sony_dvb_demod_t * system, sony_dvb_demod_lock_result_t *LockStatus);
sony_dvb_result_t SONY_PISetTSInterface( DEMOD_CXD2820_CTX_T *pSonyDemodCtx,UINT8 fgParallel);
sony_dvb_result_t SONY_Initilize(DEMOD_CXD2820_CTX_T *pSonyDemodCtx);
sony_dvb_result_t SONY_Sleep( DEMOD_CXD2820_CTX_T *pSonyDemodCtx);
sony_dvb_result_t SONY_ChipPowerDown( DEMOD_CXD2820_CTX_T *pSonyDemodCtx);
sony_dvb_result_t SONY_GetStatus( DEMOD_CXD2820_CTX_T *pSonyDemodCtx);
sony_dvb_result_t SONY_GetInfo( DEMOD_CXD2820_CTX_T *pSonyDemodCtx);

INT16 SONY_GetRSSI( DEMOD_CXD2820_CTX_T *pSonyDemodCtx);
void SONY_Get_SSI_SQI( DEMOD_CXD2820_CTX_T *pSonyDemodCtx, unsigned short *pSSI, unsigned short *pSQI);
sony_dvb_result_t SONY_Demod_Write( DEMOD_CXD2820_CTX_T *pSonyDemodCtx,UINT8 bank,UINT8 addr,UINT8 data);
sony_dvb_result_t SONY_Demod_Read( DEMOD_CXD2820_CTX_T *pSonyDemodCtx,UINT8 bank,UINT8 addr,UINT8* data);
sony_dvb_result_t SONY_Tuner_Write( DEMOD_CXD2820_CTX_T *pSonyDemodCtx,UINT8 bank,UINT8 addr,UINT8 data);
sony_dvb_result_t SONY_Tuner_Read( DEMOD_CXD2820_CTX_T *pSonyDemodCtx,UINT8 bank,UINT8 addr,UINT8* data);
UINT32 SONY_GetBER( DEMOD_CXD2820_CTX_T *pSonyDemodCtx);
unsigned SONY_GetPlpInfo(DEMOD_CXD2820_CTX_T * pSonyDemodCtx);



