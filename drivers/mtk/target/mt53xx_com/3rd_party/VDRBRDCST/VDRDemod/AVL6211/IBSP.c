/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


///
/// @file
/// @brief Implements the functions declared in IBSP.h. 
/// 
//$Revision: #1 $ 
//$Date: 2012/04/27 $
// 
#include "IBSP.h"
#include "i2c_api.h"
#include "x_os.h"
#include "tunerDebug.h"
/// Please use the following table of PLL configurations if 55 Msps symbol rate is desired at the demod end. 

///	{503,  1, 6, 4, 2,  4000, 12600, 16800, 25200} < Reference clock 4 MHz,   Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{447,  1, 6, 4, 2,  4500, 12600, 16800, 25200} < Reference clock 4.5 MHz, Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  4, 6, 4, 2, 10000, 12600, 16800, 25200} < Reference clock 10 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{503,  7, 6, 4, 2, 16000, 12600, 16800, 25200} < Reference clock 16 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
///	,{111,  2, 6, 4, 2, 27000, 12600, 16800, 25200} < Reference clock 27 MHz,  Demod clock 126 MHz, FEC clock 168 MHz, MPEG clock 252 MHz

/// Feel free to contact Availink Support staff for further infromation on this.
#define mcSEMA_LOCK_MTFE(sema)                                       \
{                                                                    \
    mcDBG_MSG_L4((" [ _Lock_%s-%d ]\r\n", __FUNCTION__, __LINE__)); \
    x_sema_lock(sema, X_SEMA_OPTION_WAIT);                           \
    mcDBG_MSG_L4((" [ _Excape_%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
}
#define mcSEMA_UNLOCK_MTFE(sema)                                     \
{                                                                    \
    mcDBG_MSG_L4((" [ _Unlock_%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
    x_sema_unlock(sema);                                             \
}
/// Users can remove unused elements from the following array to reduce the SDK footprint size.
const struct AVL_DVBSx_PllConf pll_conf[] =
{
	 {503,  1, 7, 4, 2,  4000, 11200, 16800, 25200} ///< Reference clock 4 MHz,   Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{447,  1, 7, 4, 2,  4500, 11200, 16800, 25200} ///< Reference clock 4.5 MHz, Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  4, 7, 4, 2, 10000, 11200, 16800, 25200} ///< Reference clock 10 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  7, 7, 4, 2, 16000, 11200, 16800, 25200} ///< Reference clock 16 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{111,  2, 7, 4, 2, 27000, 11200, 16800, 25200} ///< Reference clock 27 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
};
const AVL_uint16 pll_array_size = sizeof(pll_conf)/sizeof(struct AVL_DVBSx_PllConf);

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Initialize(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Dispose(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Delay( AVL_uint32 uiMS )
{
	x_thread_delay(uiMS);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CRead(  const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff, AVL_puint16 puiSize )
{
	UINT8 ucI2cAddr=pAVLChip->m_SlaveAddr;
	ucI2cReadOnly(ucI2cAddr, pucBuff, *puiSize);
	return(AVL_DVBSx_EC_OK);
}
AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CWrite(  const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff,  AVL_puint16  puiSize )
{
	UINT8 ucI2cAddr=pAVLChip->m_SlaveAddr;
	ucI2cWriteOnly( ucI2cAddr, pucBuff, *puiSize);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_InitSemaphore( AVL_psemaphore pSemaphore )
{
HANDLE_T Semphore_temp=0;
	  x_sema_create(&Semphore_temp, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
	  *pSemaphore=Semphore_temp;
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_WaitSemaphore( AVL_psemaphore pSemaphore )
{
	mcSEMA_LOCK_MTFE(*pSemaphore);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_ReleaseSemaphore( AVL_psemaphore pSemaphore )
{
	   mcSEMA_UNLOCK_MTFE(*pSemaphore);
	return(AVL_DVBSx_EC_OK);
}
