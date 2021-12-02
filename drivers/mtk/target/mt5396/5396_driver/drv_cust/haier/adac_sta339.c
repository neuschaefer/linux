/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: adac_apogee.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_apogee.c
 *  Brief of file adac_apogee.c.
 *  Details of file adac_apogee.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "adac_sta339.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "aud_cfg.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define SAMPLE_RATE_NUM         3
#define SOURCE_NUM              3
    #define SOURCE_FROM_AV      0
    #define SOURCE_FROM_ATV     1
    #define SOURCE_FROM_DTV     2
#define EQ_BIQUAD_NUM           4
#define HL_BIQUAD_NUM           2

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern INT32 GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);
extern INT32 GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val);
extern INT32 GPIO_GetIn(INT32 i4GpioNum);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
#ifdef APP_STA339BW_SUBWOOFER
static void _ST_SubWoofer_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _ST_SubWoofer_RegRead(UINT8 u1Addr);
#endif
static void _ST_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _ST_RegRead(UINT8 u1Addr);
static void _ST_Init(void);
static void _ST_Mute(BOOL fgEnable);
#ifndef APP_MODEL_VT
//static void _ST_MuteHeadphone(BOOL fgEnable);
#endif
static void _ST_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
static void _ADST_DisableEffect (void);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static BOOL fgSTMute=FALSE;
#ifdef APP_STA339BW_SUBWOOFER
static UINT8 ui1_subwoofer_setting_val=2;
#endif

static AMP_DRV_CB_T _rAmpSTDrvCB = 
{
    _ADST_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_ST_Init,					//pfnInit
	_ST_Mute,					//pfnMute 
	_ST_DacFmtCfg,  			//pfnSetDacInFmt;
	NULL,						//pfnSetDacGain;
	_ST_RegWrite, 				//pfnWrite;
	_ST_RegRead,				//pfnRead;
	&_rAmpCustDrvCB              // pfnDrvCB;	
};

static AMP_T _rAmpST = 
{
	"ST",				// szName[32];
	256,					// i2MaxGain
	-256,				// i2MaxGain
	2,					// u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifdef APP_STA339BW_SUBWOOFER
static void _ST_SubWoofer_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

#if 0 
    //u4WriteCount = SIF_Write(SIF_CLK_DIV, APOGEE_STA333BW_ADDR, u1Addr, &u1Data, 1);
    //u4WriteCount = SIF_X_Read(2, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 , u1Addr, &u1Data, 1);
    //MT5388 Use mutliple bus I2C
    u4WriteCount = SIF_X_Write(2, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 ,u1Addr, &u1Data, 1);
 #ifdef CC_AUD_AMP_SUPPORT_5_1_CH
    // BUS0: 0x38 0x3A , BUS2 : 3A
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 ,u1Addr, &u1Data, 1);
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, APOGEE_STA333BW_ADDR_1, 1 ,u1Addr, &u1Data, 1);
 #endif
#else
    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, ST_STA339BW_SUBWOOFER_ADDR, 1 ,u1Addr, &u1Data, 1);
#endif

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    }	
    else
    {
        LOG(5, "ST WRITE FAIL\n");
    }	
}

static UINT8 _ST_SubWoofer_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

#if 0 
    //u4ReadCount = SIF_Read(SIF_CLK_DIV, APOGEE_STA333BW_ADDR, u1Addr, &u1Data, 1);
    u4ReadCount = SIF_X_Read(2, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 , u1Addr, &u1Data, 1);
#else
    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, ST_STA339BW_SUBWOOFER_ADDR, 1 , u1Addr, &u1Data, 1);
#endif

    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "ST READ FAIL\n");
    	return 0;
    }	
}
#endif

static void _ST_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

#if 0 
	//u4WriteCount = SIF_Write(SIF_CLK_DIV, APOGEE_STA333BW_ADDR, u1Addr, &u1Data, 1);
	//u4WriteCount = SIF_X_Read(2, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 , u1Addr, &u1Data, 1);
	//MT5388 Use mutliple bus I2C
    u4WriteCount = SIF_X_Write(2, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 ,u1Addr, &u1Data, 1);
  #ifdef CC_AUD_AMP_SUPPORT_5_1_CH
    // BUS0: 0x38 0x3A , BUS2 : 3A
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 ,u1Addr, &u1Data, 1);
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, APOGEE_STA333BW_ADDR_1, 1 ,u1Addr, &u1Data, 1);
  #endif
#else
    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, ST_STA339BW_ADDR, 1 ,u1Addr, &u1Data, 1);
#endif

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    }	
    else
    {
        LOG(5, "ST WRITE FAIL\n");
    }	
}

static UINT8 _ST_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

#if 0 
    //u4ReadCount = SIF_Read(SIF_CLK_DIV, APOGEE_STA333BW_ADDR, u1Addr, &u1Data, 1);
    u4ReadCount = SIF_X_Read(2, SIF_CLK_DIV, APOGEE_STA333BW_ADDR, 1 , u1Addr, &u1Data, 1);
#else
    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, ST_STA339BW_ADDR, 1 , u1Addr, &u1Data, 1);
#endif

    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "ST READ FAIL\n");
    	return 0;
    }	
}

static void _ST_Init(void)
{
    AOUT_CFG_T*		prAudOutFmt;
    UINT32 u4reset,u4polarity;
	UINT32 AmpEnable,AmpEnablepolarity;

    //Printf("@@@_ST_Init\n",0);
    // Make sure I2c is init
    SIF_Init(); 
	//printk("ST339 init begin\n");
	#if 1
    DRVCUST_OptQuery(eAudioAmpEnableGpioNum, &AmpEnable);
    DRVCUST_OptQuery(eAudioAmpGpioPolarity, &AmpEnablepolarity);
    if (AmpEnable != 0xffffffff)
    {        
        GPIO_Output((INT32)AmpEnable,(INT32 *)&AmpEnablepolarity); 
    }
	#else
	AmpEnable = OPCTRL(10);
	AmpEnablepolarity = 0;
	GPIO_Output((INT32)AmpEnable,(INT32 *)&AmpEnablepolarity);
	#endif
	//x_thread_delay(100);
    DRVCUST_OptQuery(eAudioDigiAMPResetGpio, &u4reset);
    DRVCUST_OptQuery(eAudioDigiAMPResetGpioPolarity, &u4polarity);
    if (u4reset != 0xffffffff)
    {        
        GPIO_Output((INT32)u4reset,(INT32 *)&u4polarity); 
        x_thread_delay(50);
        u4polarity = !u4polarity;
        GPIO_Output((INT32)u4reset,(INT32 *)&u4polarity); 
    }

    x_thread_delay(150);  //yt 081209
    
    //Initial flow for power on sequence 
    _ST_RegWrite(STA339W_MVOL, 0x00); //yt 081709
    _ST_RegWrite(STA339W_C1CFG, 0x00); 
    _ST_RegWrite(STA339W_C2CFG, 0x40); 
    _ST_RegWrite(STA339W_C3CFG, 0x80); 

    _ST_RegWrite(STA339W_CONFD, 0x0c);//yt 110309 
    //support 2 channel	
    ///#ifdef EN_STA_339_EAPD_TWARN
    #ifdef CC_AUD_AMP_STA_339_EAPD_TWARN
    _ST_RegWrite(STA339W_CONFF, 0xDE);
    #else
    _ST_RegWrite(STA339W_CONFF, 0xDC);
    #endif
    _ST_RegWrite(STA339W_MUTELOC, 0x40); //yt 081709

    // Format config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    _ST_RegWrite(STA339W_CONFA, 0x63); // 48K and 256fs
    _ST_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
    _ST_RegWrite(STA339W_CONFC, 0xc2); //change default for HDMI & DTV 48Khz& 44.1Khz sasa background noise
    //_ST_RegWrite(STA339W_MASMUTE, MUTE_ALL_CHL);
    
    // Volume setup
    //_ST_RegWrite(STA339W_MVOL, 0x00); 
    //Emerson Modify 2009-1002 
    _ST_RegWrite(STA339W_C1VOL, 0x4c);
    _ST_RegWrite(STA339W_C2VOL, 0x4c);
	
    _ST_RegWrite(STA339W_C3VOL, 0x5a); 
    _ST_RegWrite(STA339W_C1CFG, 0x10); 
    _ST_RegWrite(STA339W_C2CFG, 0x50); 
    
    //? _ST_RegWrite(STA339W_LIMITER, 0x6f); 
    //? _ST_RegWrite(STA339W_LIMITER_THRESHOLD, 0x6f); 
    
#ifdef APP_STA339BW_SUBWOOFER
    //Initial flow for power on sequence 
    _ST_SubWoofer_RegWrite(STA339W_MVOL, 0x00); //yt 081709
    _ST_SubWoofer_RegWrite(STA339W_C1CFG, 0x00); 
    _ST_SubWoofer_RegWrite(STA339W_C2CFG, 0x40); 
    _ST_SubWoofer_RegWrite(STA339W_C3CFG, 0x80); 

    _ST_SubWoofer_RegWrite(STA339W_CONFD, 0x0c);//yt 110309 
    //support 1 channel	
    _ST_SubWoofer_RegWrite(STA339W_CONFF, 0xdf);
    _ST_SubWoofer_RegWrite(STA339W_MUTELOC, 0x40); //yt 081709

    // Format config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    _ST_SubWoofer_RegWrite(STA339W_CONFA, 0x63); // 48K and 256fs
    _ST_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
    _ST_SubWoofer_RegWrite(STA339W_CONFC, 0xc2); //change default for HDMI & DTV 48Khz& 44.1Khz sasa background noise
    //_ST_RegWrite(STA339W_MASMUTE, MUTE_ALL_CHL);
    
    _ST_SubWoofer_RegWrite(STA339W_C1VOL, 0x4c);
    _ST_SubWoofer_RegWrite(STA339W_C2VOL, 0x4c);
    ST_set_subwoofer_gain(ui1_subwoofer_setting_val);
#endif
}

static void _ST_Mute(BOOL fgEnable)
{
#ifndef APP_MODEL_VT
    //UINT32 u4PlugInGpio,u4polarity;
    //INT32 i4PinData; 
#endif	
    //Printf("@@@_ST_Mute %d\n",fgEnable);    
    fgSTMute = fgEnable;

    UNUSED(fgSTMute);

	//printf("ST339 mute begin\n");//chopper
	
    if (fgEnable)  //Mute
    {
 
        _ST_RegWrite(STA339W_MVOL, 0x80);
        x_thread_delay(15);
        _ST_RegWrite(STA339W_MVOL, 0x00); //yt 081709
        //_ST_RegWrite(STA339W_MASMUTE, MUTE_ALL_CHL);

        //yt 072909
        _ST_RegWrite(STA339W_C1VOL, 0xFE);
        _ST_RegWrite(STA339W_C2VOL, 0xFE);
        //_ST_RegWrite(STA339W_C3VOL, 0xFF);	//yt 073009
        
       	//GPIO_SetOut(203,1); //AMP/HP HW mute - OPCTRL3

#ifdef APP_STA339BW_SUBWOOFER
        _ST_SubWoofer_RegWrite(STA339W_MVOL, 0x80);
        x_thread_delay(15);
        _ST_SubWoofer_RegWrite(STA339W_MVOL, 0x00); //yt 081709

        _ST_SubWoofer_RegWrite(STA339W_C1VOL, 0xFE);
        _ST_SubWoofer_RegWrite(STA339W_C2VOL, 0xFE);
        ST_set_subwoofer_gain(ui1_subwoofer_setting_val);
#endif
    }
    else  //Unmute
    {      
       	//_ST_RegWrite(STA339W_MVOL, 0x60);    
       	//x_thread_delay(10);
       	_ST_RegWrite(STA339W_MVOL, 0x00); 	   
		//_ST_RegWrite(STA339W_MASMUTE, UNMUTE_ALL_CHL);
		
		_ST_RegWrite(STA339W_C1VOL, 0x4c);
		_ST_RegWrite(STA339W_C2VOL, 0x4c);
		_ST_RegWrite(STA339W_C3VOL, 0x5a); 
		_ST_RegWrite(STA339W_C1CFG, 0x10); 
		_ST_RegWrite(STA339W_C2CFG, 0x50); 
		
	    //_ST_RegWrite(STA339W_C3VOL, 0x41);	//yt 073009
		//GPIO_SetOut(203,0); //AMP/HP HW unmute - OPCTRL3	

#ifdef APP_STA339BW_SUBWOOFER
        _ST_SubWoofer_RegWrite(STA339W_MVOL, 0x00); 	   
        _ST_SubWoofer_RegWrite(STA339W_C1VOL, 0x4c);
        _ST_SubWoofer_RegWrite(STA339W_C2VOL, 0x4c);
        ST_set_subwoofer_gain(ui1_subwoofer_setting_val);
#endif
    }
}
#ifndef APP_MODEL_VT
#if 0
static void _ST_MuteHeadphone(BOOL fgEnable) //yt 073009
{
    
    if (fgEnable)  //Mute
    {
 
       _ST_RegWrite(STA339W_MVOL, 0x80);
       x_thread_delay(15);
       _ST_RegWrite(STA339W_MVOL, 0x00); //yt 081709
	//_ST_RegWrite(STA339W_MASMUTE, MUTE_ALL_CHL);
	
	 _ST_RegWrite(STA339W_C3VOL, 0xFE);	        
    }
    else  //Unmute
    {      
       //_ST_RegWrite(STA339W_MVOL, 0x60);    
       //x_thread_delay(10);
       _ST_RegWrite(STA339W_MVOL, 0x00); 	   
	//_ST_RegWrite(STA339W_MASMUTE, UNMUTE_ALL_CHL);
	 
	 _ST_RegWrite(STA339W_C3VOL, 0x5a);	 		
    }
}
#endif
#endif
static void _ST_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    //Printf("@@@_ST_DacFmtCfg %d, %d\n", eFormat, eMclk);
    UNUSED(eMclk);
    
    switch (eFormat)
    {
        case FORMAT_LJ:
        	_ST_RegWrite(STA339W_CONFB, 0x81);
        	break;
        case FORMAT_I2S:
        	_ST_RegWrite(STA339W_CONFB, 0x80);
        	break;
        case FORMAT_RJ:
        	_ST_RegWrite(STA339W_CONFB, 0x82);
        	break;
        default:
        	_ST_RegWrite(STA339W_CONFB, 0x81);
        	break;
    }
	
#ifdef APP_STA339BW_SUBWOOFER
    switch (eFormat)
    {
        case FORMAT_LJ:
            _ST_SubWoofer_RegWrite(STA339W_CONFB, 0x81);
            break;
        case FORMAT_I2S:
            _ST_SubWoofer_RegWrite(STA339W_CONFB, 0x80);
            break;
        case FORMAT_RJ:
            _ST_SubWoofer_RegWrite(STA339W_CONFB, 0x82);
            break;
        default:
            _ST_SubWoofer_RegWrite(STA339W_CONFB, 0x81);
            break;
    }
#endif
}

static void _ADST_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

AMP_T* _AMP_GetAmpCustFunTbl(void)
{
	return &_rAmpST;
}

#ifdef APP_STA339BW_SUBWOOFER
void ST_set_subwoofer_gain(UINT8 ui1_val)
{
    UINT32 u4HPPlugIn,u4polarity;
    ui1_subwoofer_setting_val = ui1_val;

    if (fgSTMute)// speaker mute
    {
        _ST_SubWoofer_RegWrite(STA339W_C3VOL, 0xFE); 
        return;
    }

    switch (ui1_subwoofer_setting_val)
    {
        case 0: /* off */
            _ST_SubWoofer_RegWrite(STA339W_C3VOL, 0xFE); 
            break;

        case 1: /* low */
            _ST_SubWoofer_RegWrite(STA339W_C3VOL, 0x64); 
            break;

        case 2: /* medium */
            _ST_SubWoofer_RegWrite(STA339W_C3VOL, 0x5a); 
            break;

        case 3: /* high */
            _ST_SubWoofer_RegWrite(STA339W_C3VOL, 0x54); 
            break;

        default:
            _ST_SubWoofer_RegWrite(STA339W_C3VOL, 0x5a); 
            break;
    }
}
#endif

#if 0
static void _APOGEE_RegWriteEx(UINT8 u1Addr, UINT8 u1Data, AUD_AMP_ID_T u1AmpID)
{
    UINT32 u4WriteCount = 0;
    UINT8 u1BusID;
    UINT8 u1DevID;

    UNUSED(u4WriteCount);
    UNUSED(u1BusID);
    UNUSED(u1DevID);    
    // BUS2: 0x3A
    // BUS0: 0x3A Ls/Rs
    // BUS0: 0x38 Center:8, Sub:9    
#ifdef CC_AUD_AMP_STA333BW
    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 2;
        u1DevID = APOGEE_STA333BW_ADDR;             
        break;        
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR;
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR_1;
        break;        
    default:
        u1BusID = 2;
        u1DevID = APOGEE_STA333BW_ADDR;
        break;
    };
    u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);
#else
    UNUSED(_APOGEE_RegWriteEx);
    return;
#endif //CC_AUD_AMP_STA333BW
}
#endif

#if 0
static UINT8 _APOGEE_RegReadEx(UINT8 u1Addr, AUD_AMP_ID_T u1AmpID) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;
    UINT8 u1BusID;
    UINT8 u1DevID;

    UNUSED(u1BusID);
    UNUSED(u1DevID);
    UNUSED(u4ReadCount);
    UNUSED(u1Data);
    
#ifdef CC_AUD_AMP_STA333BW
    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 2;
        u1DevID = APOGEE_STA333BW_ADDR;             
        break;        
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR;
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR_1;
        break;        
    default:
        u1BusID = 2;
        u1DevID = APOGEE_STA333BW_ADDR;
        break;
    };

    u4ReadCount = SIF_X_Read(u1BusID, SIF_CLK_DIV, u1DevID, 1 , u1Addr, &u1Data, 1);
    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "APOGEE READ FAIL\n");
    	return 0;
    }
#else
    UNUSED(_APOGEE_RegReadEx);
    return 0;
#endif // CC_AUD_AMP_STA333BW
}
#endif

#if 0
void ADAC_DigiAmp_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT8 u1Value)
{
#ifdef CC_AUD_AMP_STA333BW
    switch (u1ChId)
{
    case 0: //Master volume
        _APOGEE_RegWriteEx(REG_APOGEE_MASTER_VOL_CRL, u1Value, u1AmpID);
        break;
    case 1: //Channel 1
        _APOGEE_RegWriteEx(REG_APOGEE_CHL_1_VOL_CRL, u1Value, u1AmpID);    
        break;        
    case 2: //Channel 2
        _APOGEE_RegWriteEx(REG_APOGEE_CHL_2_VOL_CRL, u1Value, u1AmpID);
        break;
    };
#else
    return;
#endif // CC_AUD_AMP_STA333BW   
}
#endif

#if 0
void ADAC_DigiAmp_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData)    
{
#ifdef CC_AUD_AMP_STA333BW
    UINT8 u1Temp;

    //CufOff Reg: 0xC,  Bit[7:4]
    u1Temp = _APOGEE_RegReadEx(REG_APOGEE_AUTOMODE_REG2,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1Cutoff << 4);
    _APOGEE_RegWriteEx(REG_APOGEE_AUTOMODE_REG2, u1Temp, u1AmpID); 

    //Low Freq Gain Reg: 0xA,  Bit[7:0]
    _APOGEE_RegWriteEx(REG_APOGEE_CHL_3_VOL_CRL, (eDrcData.u1LfeGain), u1AmpID);

    //Attack Hi Reg: 0x32,  Bit[6:0]
    u1Temp = _APOGEE_RegReadEx(STA339_ATTACK1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackHi);
    _APOGEE_RegWriteEx(STA339_ATTACK1_THRESHOLD, u1Temp, u1AmpID); 
    //Attack Lo Reg: 0x34,  Bit[6:0]
    u1Temp = _APOGEE_RegReadEx(STA339_ATTACK2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackLo);
    _APOGEE_RegWriteEx(STA339_ATTACK2_THRESHOLD, u1Temp, u1AmpID);

    //Release Hi Reg: 0x33,  Bit[6:0]
    u1Temp = _APOGEE_RegReadEx(STA339_RELEASE1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseHi);
    _APOGEE_RegWriteEx(STA339_RELEASE1_THRESHOLD, u1Temp, u1AmpID);
    //Release Lo Reg: 0x35,  Bit[6:0]
    u1Temp = _APOGEE_RegReadEx(STA339_RELEASE2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseLo);
    _APOGEE_RegWriteEx(STA339_RELEASE2_THRESHOLD, u1Temp, u1AmpID);

    //Rate Hi Reg: 0x12,  Bit[7:4]
    u1Temp = _APOGEE_RegReadEx(STA339_ATTACK_RATE1,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1AttackHi << 4);
    _APOGEE_RegWriteEx(STA339_ATTACK_RATE1, u1Temp, u1AmpID);
    //Rate Lo Reg: 0x14,  Bit[7:4]
    u1Temp = _APOGEE_RegReadEx(STA339_ATTACK_RATE2,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1AttackLo << 4);
    _APOGEE_RegWriteEx(STA339_ATTACK_RATE2, u1Temp, u1AmpID);    
        
#else
    return;
#endif // CC_AUD_AMP_STA333BW
}
#endif
#ifndef APP_MODEL_VT
BOOL ST_DigiAmp_ReadRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, UINT8* u1Data, BOOL fgAllSet)
{
#if 0 //def CC_AUD_AMP_STA333BW
    UINT8 u1Temp;
        
    _APOGEE_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _APOGEE_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16
    u1Temp = _APOGEE_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro R1 bit in reg 0x26
    u1Temp |= 0x4;
    _APOGEE_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);

    u1Temp = _APOGEE_RegReadEx(STA339_B1CF1, u1AmpID);     //0x17 : Top 8 bits of Data.
    u1Data[0] = u1Temp;
    u1Temp = _APOGEE_RegReadEx(STA339_B1CF2, u1AmpID);     //0x18 : Mid 8 bits of Data.
    u1Data[1] = u1Temp;
    u1Temp = _APOGEE_RegReadEx(STA339_B1CF3, u1AmpID);     //0x19 : Last 8 bits of Data.
    u1Data[2] = u1Temp;    

    if (fgAllSet)        
    {
        u1Temp = _APOGEE_RegReadEx(STA339_B2CF1, u1AmpID);     
        u1Data[3] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_B2CF2, u1AmpID);     
        u1Data[4] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_B2CF3, u1AmpID);     
        u1Data[5] = u1Temp;

        u1Temp = _APOGEE_RegReadEx(STA339_A1CF1, u1AmpID);     
        u1Data[6] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_A1CF2, u1AmpID);     
        u1Data[7] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_A1CF3, u1AmpID);     
        u1Data[8] = u1Temp;

        u1Temp = _APOGEE_RegReadEx(STA339_A2CF1, u1AmpID);     
        u1Data[9] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_A2CF2, u1AmpID);     
        u1Data[10] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_A2CF3, u1AmpID);     
        u1Data[11] = u1Temp;        

        u1Temp = _APOGEE_RegReadEx(STA339_B0CF1, u1AmpID);     
        u1Data[12] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_B0CF2, u1AmpID);     
        u1Data[13] = u1Temp;
        u1Temp = _APOGEE_RegReadEx(STA339_B0CF3, u1AmpID);     
        u1Data[14] = u1Temp;                
    }

    return TRUE;
#else    
    UNUSED(ST_DigiAmp_ReadRam);
    return FALSE;
#endif //CC_AUD_AMP_STA333BW
}
BOOL ST_DigiAmp_WriteRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, AUD_AQ_AMP_RAM_DATA_T rAmpRamData, BOOL fgAllSet)
{
#if 0 //def CC_AUD_AMP_STA333BW
    UINT8 u1Temp;
        
    _APOGEE_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _APOGEE_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16

    _APOGEE_RegWriteEx(STA339_B1CF1, rAmpRamData.u1Data[0], u1AmpID);     //0x17 : Top 8 bits of Data.
    _APOGEE_RegWriteEx(STA339_B1CF2, rAmpRamData.u1Data[1], u1AmpID);     //0x18 : Mid 8 bits of Data.
    _APOGEE_RegWriteEx(STA339_B1CF3, rAmpRamData.u1Data[2], u1AmpID);     //0x19 : Last 8 bits of Data.

    if (fgAllSet)        
    {
        _APOGEE_RegWriteEx(STA339_B2CF1, rAmpRamData.u1Data[3], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_B2CF2, rAmpRamData.u1Data[4], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_B2CF3, rAmpRamData.u1Data[5], u1AmpID); 

        _APOGEE_RegWriteEx(STA339_A1CF1, rAmpRamData.u1Data[6], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_A1CF2, rAmpRamData.u1Data[7], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_A1CF3, rAmpRamData.u1Data[8], u1AmpID); 

        _APOGEE_RegWriteEx(STA339_A2CF1, rAmpRamData.u1Data[9], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_A2CF2, rAmpRamData.u1Data[10], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_A2CF3, rAmpRamData.u1Data[11], u1AmpID); 

        _APOGEE_RegWriteEx(STA339_B0CF1, rAmpRamData.u1Data[12], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_B0CF2, rAmpRamData.u1Data[13], u1AmpID); 
        _APOGEE_RegWriteEx(STA339_B0CF3, rAmpRamData.u1Data[14], u1AmpID);              
    }

    u1Temp = _APOGEE_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro W1 bit in reg 0x26
    u1Temp |= 0x1;
    _APOGEE_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);

    return TRUE;
#else    
    UNUSED(ST_DigiAmp_WriteRam);
    return FALSE;
#endif //CC_AUD_AMP_STA333BW
}

#if 0
void ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4PostSclCh1, UINT32 u4PostSclCh2)
{
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
        
    UNUSED(ADAC_DigiAmp_SetPostScale);        
    UNUSED(u4PostSclCh1);
    UNUSED(u4PostSclCh2);    

    rAmpData.u1Data[0] = (u4PostSclCh1 & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh1 & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh1 & 0xFF);    
    ADAC_DigiAmp_WriteRam(u1AmpID,0,0x34,rAmpData, FALSE);

    rAmpData.u1Data[0] = (u4PostSclCh2 & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh2 & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh2 & 0xFF);     
    ADAC_DigiAmp_WriteRam(u1AmpID,0,0x35,rAmpData, FALSE);
    
    return;
}

void ADAC_DigiAmp_SetPEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T rAQExtPeqData)
{
    UNUSED(u1AmpID);
    UNUSED(rAQExtPeqData);
    UNUSED(ADAC_DigiAmp_SetPEQ);    

#ifdef CC_AUD_AMP_STA333BW
    UINT8 u1Temp;
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
    UINT8 i,j;
    UINT8 u1Idx;

    // Set 1 to bit4 in reg 0x3
    u1Temp = _APOGEE_RegReadEx(STA232W_CONFD, u1AmpID);     
    u1Temp |= 0x10;
    _APOGEE_RegWriteEx(STA232W_CONFD, u1Temp, u1AmpID);

    // Write Coeffs to Ram. (Band 0 ~ 7 , 5 coeffs each.)
    for (i=0; i<7; i++) // 7 Bands
    {
        u1Idx= 0;
        for (j=0; j<15; j+=3) //For one band, there are 5 coeffs, each are 3 byte.
        {
            rAmpData.u1Data[j] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF0000) >> 16;
            rAmpData.u1Data[j+1] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF00)>> 8;
            rAmpData.u1Data[j+2] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF);
            LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j,rAmpData.u1Data[j]);
            LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+1,rAmpData.u1Data[j+1]);
            LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+2,rAmpData.u1Data[j+2]);
            u1Idx += 1;
        }
        ADAC_DigiAmp_WriteRam(u1AmpID, 0, (i*5), rAmpData, TRUE);
        LOG(3,"EXT_PEQ - Write PEQ to band %d, Start Addr %d\n", i, i*5);
    }        
#else
    return;
#endif //CC_AUD_AMP_STA333BW
}
#endif
#endif
