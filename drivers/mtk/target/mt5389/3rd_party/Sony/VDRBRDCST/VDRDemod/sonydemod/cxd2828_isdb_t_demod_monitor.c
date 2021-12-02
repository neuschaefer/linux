/*------------------------------------------------------------------------------

 Copyright 2010, 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/28
 Revision: 1.1.1.0

------------------------------------------------------------------------------*/
#include "cxd2828_isdb_t_demod_monitor.h"

/*------------------------------------------------------------------------------
 Prototypes of static functions
------------------------------------------------------------------------------*/
static cxd2828_result_t IsDemodLocked(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t set_tmcc_info(UINT8* pData, cxd2828_isdb_t_tmcc_info_t* pTMCCInfo);
static cxd2828_result_t ConvertTMCCSystem(UINT8 data, cxd2828_isdb_t_tmcc_system_t* pTMCCSystem);
static cxd2828_result_t ConvertTMCCModulation(UINT8 data, cxd2828_isdb_t_tmcc_modulation_t* pTMCCModulation);
static cxd2828_result_t ConvertTMCCCodingRate(UINT8 data, cxd2828_isdb_t_tmcc_coding_rate_t* pTMCCCodingRate);
static cxd2828_result_t ConvertTMCCIlLength(UINT8 data, cxd2828_isdb_t_tmcc_il_length_t* pTMCCIlLength);

/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Monitor sync status

  pDemod     : Address of ISDB-T part instance.
  pDmdLock   : Address of demodulator lock flag
               - 0: Not lock
               - 1: Lock
  pTSLock    : Address of TS lock flag
               - 0: Not lock
               - 1: Lock
  pUnlock    : Address of unlock detection flag
               - 0: No ISDB-T signal in the receiving channel.
               - 1: Detection underway or ISDB-T signal may exist.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorSyncStat(cxd2828_isdb_t_demod_t* pDemod,
                                                UINT8* pDmdLock,
                                                UINT8* pTSLock,
                                                UINT8* pUnlock)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorSyncStat");

    /* Null check */
    if(!pDemod || !pDmdLock || !pTSLock || !pUnlock){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x10, &data, 1);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    *pDmdLock = (data & 0x40) ? 1 : 0;
    *pTSLock = (data & 0x20) ? 1 : 0;
    *pUnlock = (data & 0x10) ? 1 : 0;

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Monitor IFAGC output level

  pDemod : Address of ISDB-T part instance.
  pIFAGC : Address of IFAGC output level (0 - 4095).
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorIFAGCOut(cxd2828_isdb_t_demod_t* pDemod,
                                                UINT32* pIFAGC)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data[2] = {0,0};

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorIFAGCOut");

    /* Null check */
    if(!pDemod || !pIFAGC){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x28, data, 2);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    *pIFAGC = (((UINT32)data[0] & 0x0F) << 8) | ((UINT32)data[1] & 0xFF);

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Monitor mode and guard interval

  pDemod : Address of ISDB-T part instance.
  pMode  : Address of mode
  pGuard : Address guard interval
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorModeGuard(cxd2828_isdb_t_demod_t* pDemod,
                                                 cxd2828_isdb_t_mode_t* pMode,
                                                 cxd2828_isdb_t_guard_t* pGuard)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorModeGuard");

    /* Null check */
    if(!pDemod || !pMode || !pGuard){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x40, &data, 1);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    switch((data >> 2) & 0x03)
    {
    case 1:
        *pMode = CXD2828_ISDB_T_MODE_1;
        break;
    case 2:
        *pMode = CXD2828_ISDB_T_MODE_2;
        break;
    case 3:
        *pMode = CXD2828_ISDB_T_MODE_3;
        break;
    default:
        *pMode = CXD2828_ISDB_T_MODE_UNKNOWN;
        break;
    }

    switch(data & 0x03)
    {
    case 0:
        *pGuard = CXD2828_ISDB_T_GUARD_1_32;
        break;
    case 1:
        *pGuard = CXD2828_ISDB_T_GUARD_1_16;
        break;
    case 2:
        *pGuard = CXD2828_ISDB_T_GUARD_1_8;
        break;
    case 3:
        *pGuard = CXD2828_ISDB_T_GUARD_1_4;
        break;
    default:
        *pGuard = CXD2828_ISDB_T_GUARD_UNKNOWN;
        break;
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Monitor spectrum inversion

  This API return the spectrum polarity of input to demodulator.

  pDemod    : Address of ISDB-T part instance.
  pIsInvert : Address of spectrum polarity
              - 0: Not inverted
              - 1: Inverted
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorSpectrumSense(cxd2828_isdb_t_demod_t* pDemod,
                                                     UINT8* pIsInvert)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorSpectrumSense");

    /* Null check */
    if(!pDemod || !pIsInvert){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x91, &data, 1);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    *pIsInvert = (data & 0x01) ? 1 : 0;

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Monitor carrier offset

  The relation of "Actual broadcasting frequency(X)","Current tuning frequency(Y)"
  and "Carrier offset(Z)" is following.
    X = Y + Z
  The unit is "KHz".

  pDemod  : Address of ISDB-T part instance.
  pOffset : Address of carrier offset(KHz).
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorCarrierOffset(cxd2828_isdb_t_demod_t* pDemod,
                                                     INT32* pOffset)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data[4] = {0,0,0,0};
    UINT32 IREG_CRCG_CTLVAL = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorCarrierOffset");

    /* Null check */
    if(!pDemod || !pOffset){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x5C, data, 4);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    IREG_CRCG_CTLVAL = (((UINT32)data[0] & 0x01) << 24) | (((UINT32)data[1] & 0xFF) << 16) |
                       (((UINT32)data[2] & 0xFF) <<  8) |  ((UINT32)data[3] & 0xFF);

    /* offset = - (( IREG_CRCG_CTLVAL / 2^28 ) * 8192 ) / ( 1008 * 10^-6 ) */
    /* offset = - (IREG_CRCG_CTLVAL / 33.030144) [Hz]                      */
    *pOffset = (INT32)(cxd2828_Convert2SComplement(IREG_CRCG_CTLVAL, 25) / 33030) * (-1);

    CXD2828_TRACE_RETURN(result);
}

#ifdef CXD2828_FP_ENABLE
/*--------------------------------------------------------------------
  Monitor MER(Modulation Error Ratio)

  pDemod : Address of ISDB-T part instance.
  pMER   : Address of MER(dB).
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorMER(cxd2828_isdb_t_demod_t* pDemod,
                                           double* pMER)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT32 IREG_MER_DT = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorMER");

    /* Null check */
    if(!pDemod || !pMER){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = cxd2828_isdb_t_demod_MonitorMERDT(pDemod, &IREG_MER_DT);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(IREG_MER_DT == 0)
    {
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_HW_STATE);
    }

    /* MER[dB] = 10 * log( IREG_MER_DT / 16 ) - 2.13 */
    *pMER = (double)10 * cxd2828_log10((double)IREG_MER_DT / 16) - 2.13;

    CXD2828_TRACE_RETURN(result);
}
#endif /* CXD2828_FP_ENABLE */

/*--------------------------------------------------------------------
  Monitor MERDT

  For the environment of not use floating point calculation,
  this function provides the monitor the register IREG_MER_DT.
  If you can use floating pointer calculation, you can use
  cxd2828_isdb_t_demod_MonitorMER function for monitor MER.

  pDemod : Address of ISDB-T part instance.
  pMER   : Address of IREG_MER_DT register value.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorMERDT(cxd2828_isdb_t_demod_t* pDemod,
                                             UINT32* pMERDT)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data[2] = {0,0};

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorMERDT");

    /* Null check */
    if(!pDemod || !pMERDT){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x2F, data, 2);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    *pMERDT = (((UINT32)data[0] & 0xFF) << 8) | ((UINT32)data[1] & 0xFF);

    CXD2828_TRACE_RETURN(result);
}

#ifdef CXD2828_FP_ENABLE
/*--------------------------------------------------------------------
  Monitor BER(Bit Error Ratio) after Viterbi decoding

  pDemod : Address of ISDB-T part instance.
  target : Monitor target
  pBER   : Address of BER.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostViterbiBER(cxd2828_isdb_t_demod_t* pDemod,
                                                      cxd2828_isdb_t_demod_monitor_target_t target,
                                                      double* pBER)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT32 packetCnt = 0;
    UINT32 bitErrorCnt = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorPostViterbiBER");

    /* Null check */
    if(!pDemod || !pBER){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = cxd2828_isdb_t_demod_MonitorPostViterbiBitError(pDemod, target, &bitErrorCnt, &packetCnt);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(packetCnt == 0){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_HW_STATE);
    }

    *pBER = (double)bitErrorCnt / (double)(packetCnt * 8 * 204);

    CXD2828_TRACE_RETURN(result);
}
#endif /* CXD2828_FP_ENABLE */

/*--------------------------------------------------------------------
  Monitor bit error count after Viterbi decoding

  For the environment of not use floating point calculation,
  this function provides monitor the bit error count.
  If you can use floating pointer calculation, you can use
  cxd2828_isdb_t_demod_MonitorPostViterbiBER function for monitor BER.

  pDemod     : Address of ISDB-T part instance.
  target     : Monitor target
  pBitError  : Address of bit error count.
  pPacketCnt : Address of packet count.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostViterbiBitError(cxd2828_isdb_t_demod_t* pDemod,
                                                           cxd2828_isdb_t_demod_monitor_target_t target,
                                                           UINT32* pBitError,
                                                           UINT32* pPacketCnt)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 benum[3] = {0,0,0};
    UINT8 pnum[2] = {0,0};

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorPostViterbiBitError");

    /* Null check */
    if(!pDemod || !pBitError || !pPacketCnt){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xD0, pnum, 2);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    *pPacketCnt = (UINT16)(((UINT16)pnum[0] & 0x7F) << 8) | (UINT16)pnum[1];

    switch(target)
    {
    case CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A:
        if(pDemod->state == CXD2828_ISDB_T_DEMOD_STATE_EWS){
            /* This monitor is invalid on EWS mode */
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_SW_STATE);
        }
        result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x31, benum, 3);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B:
        if(pDemod->state == CXD2828_ISDB_T_DEMOD_STATE_EWS){
            /* This monitor is invalid on EWS mode */
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_SW_STATE);
        }
        result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x34, benum, 3);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C:
        if(pDemod->state == CXD2828_ISDB_T_DEMOD_STATE_EWS){
            /* This monitor is invalid on EWS mode */
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_SW_STATE);
        }
        result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x37, benum, 3);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    default:
        /* Parameter error */
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    *pBitError = (((UINT32)benum[0] & 0x3F) << 16) | (((UINT32)benum[1] & 0xFF) <<  8) |
                  ((UINT32)benum[2] & 0xFF);

    CXD2828_TRACE_RETURN(result);
}

#ifdef CXD2828_FP_ENABLE
/*--------------------------------------------------------------------
  Monitor PER(Packet Error Ratio) after Reed-Solomon decoding

  pDemod    : Address of ISDB-T part instance.
  target    : Monitor target
  pPostRSPER : Address of PER.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostRSPER(cxd2828_isdb_t_demod_t* pDemod,
                                                 cxd2828_isdb_t_demod_monitor_target_t target,
                                                 double* pPostRSPER)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT32 packetCnt = 0;
    UINT32 packetErrorCnt = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorPostRSPER");

    /* Null check */
    if(!pDemod || !pPostRSPER){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = cxd2828_isdb_t_demod_MonitorPostRSPacketError(pDemod, target, &packetErrorCnt, &packetCnt);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(packetCnt == 0){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_HW_STATE);
    }

    *pPostRSPER = (double)packetErrorCnt / (double)packetCnt;

    CXD2828_TRACE_RETURN(result);
}
#endif /* CXD2828_FP_ENABLE */

/*--------------------------------------------------------------------
  Monitor packet error count after Reed-Solomon decoding

  For the environment of not use floating point calculation,
  this function provides monitor the bit error count.
  If you can use floating pointer calculation, you can use
  cxd2828_isdb_t_demod_MonitorPostRSPER function for monitor PER.

  pDemod       : Address of ISDB-T part instance.
  target       : Monitor target
  pPacketError : Address of packet error count.
  pPacketCnt   : Address of packet count.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPostRSPacketError(cxd2828_isdb_t_demod_t* pDemod,
                                                         cxd2828_isdb_t_demod_monitor_target_t target,
                                                         UINT32* pPacketError,
                                                         UINT32* pPacketCnt)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 pnum[2] = {0,0};
    UINT8 penum[2] = {0,0};

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorPostRSPacketError");

    /* Null check */
    if(!pDemod || !pPacketError || !pPacketCnt){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xD0, pnum, 2);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    *pPacketCnt = (UINT16)(((UINT16)pnum[0] & 0x7F) << 8) | (UINT16)pnum[1];

    switch(target)
    {
    case CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_A:
        if(pDemod->state == CXD2828_ISDB_T_DEMOD_STATE_EWS){
            /* This monitor is invalid on EWS mode */
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_SW_STATE);
        }
        result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x3A, penum, 2);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_B:
        if(pDemod->state == CXD2828_ISDB_T_DEMOD_STATE_EWS){
            /* This monitor is invalid on EWS mode */
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_SW_STATE);
        }
        result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x3C, penum, 2);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_MONITOR_TARGET_LAYER_C:
        if(pDemod->state == CXD2828_ISDB_T_DEMOD_STATE_EWS){
            /* This monitor is invalid on EWS mode */
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_SW_STATE);
        }
        result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x3E, penum, 2);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    default:
        /* Parameter error */
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    *pPacketError = (UINT16)((UINT16)(penum[0] & 0x7F) <<  8) | (UINT16) penum[1];

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Monitor TMCC information

  pDemod    : Address of ISDB-T part instance.
  pTMCCInfo : Address of TMCC information.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorTMCCInfo(cxd2828_isdb_t_demod_t* pDemod,
                                                cxd2828_isdb_t_tmcc_info_t* pTMCCInfo)

{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data[11];

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorTMCCInfo");

    /* Null check */
    if(!pDemod || !pTMCCInfo){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x42, data, 11);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result =set_tmcc_info(data, pTMCCInfo);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Monitor preset information

  The preset information of current tuning channel can get by this function.
  And next tuning to this channel, you can shorten the tuning time to set it.

  pDemod      : Address of ISDB-T part instance.
  pPresetInfo : Address of preset information.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_MonitorPresetInfo(cxd2828_isdb_t_demod_t* pDemod,
                                                  cxd2828_isdb_t_demod_preset_info_t* pPresetInfo)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_MonitorPresetInfo");

    /* Null check */
    if(!pDemod || !pPresetInfo){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Lock check */
    result = IsDemodLocked(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x42, pPresetInfo->data, CXD2828_ISDB_T_DEMOD_PRESET_INFO_BYTES);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}


/*--------------------------------------------------------------------
  Check if the demodulator locked

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t IsDemodLocked(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 dmdLock = 0;
    UINT8 tsLock = 0;
    UINT8 unlock = 0;

    CXD2828_TRACE_ENTER("IsDemodLocked");

    /* Null check */
    if(!pDemod){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = cxd2828_isdb_t_demod_MonitorSyncStat(pDemod, &dmdLock, &tsLock, &unlock);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(!dmdLock){
        /* If IREG_DMDLOCK == 0, return CXD2828_RESULT_ERROR_HW_STATE */
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_HW_STATE);
    }

    /* If IREG_DMDLOCK == 1, return CXD2828_RESULT_OK */
    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Set TMCC information to struct

  pDemod    : Address of ISDB-T part instance.
  pTMCCInfo : Address of TMCC information struct.
--------------------------------------------------------------------*/
static cxd2828_result_t set_tmcc_info(UINT8* pData, cxd2828_isdb_t_tmcc_info_t* pTMCCInfo)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("set_tmcc_info");

    if(!pData || !pTMCCInfo){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* System */
    result = ConvertTMCCSystem((UINT8)cxd2828_BitSplitFromByteArray(pData, 0, 2), &(pTMCCInfo->system));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    /* Count down index */
    pTMCCInfo->countDownIndex = (UINT8)cxd2828_BitSplitFromByteArray(pData, 2, 4);

    /* EWS flag */
    pTMCCInfo->ewsFlag = (UINT8)cxd2828_BitSplitFromByteArray(pData, 6, 1);

    /* Current */
    pTMCCInfo->xcurrent.isPartial = (UINT8)cxd2828_BitSplitFromByteArray(pData, 7, 1);

    result = ConvertTMCCModulation((UINT8)cxd2828_BitSplitFromByteArray(pData, 8, 3), &(pTMCCInfo->xcurrent.layerA.modulation));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCCodingRate((UINT8)cxd2828_BitSplitFromByteArray(pData, 11, 3), &(pTMCCInfo->xcurrent.layerA.codingRate));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCIlLength((UINT8)cxd2828_BitSplitFromByteArray(pData, 14, 3), &(pTMCCInfo->xcurrent.layerA.ilLength));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    pTMCCInfo->xcurrent.layerA.segmentsNum = (UINT8)cxd2828_BitSplitFromByteArray(pData, 17, 4);

    result = ConvertTMCCModulation((UINT8)cxd2828_BitSplitFromByteArray(pData, 21, 3), &(pTMCCInfo->xcurrent.layerB.modulation));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCCodingRate((UINT8)cxd2828_BitSplitFromByteArray(pData, 24, 3), &(pTMCCInfo->xcurrent.layerB.codingRate));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCIlLength((UINT8)cxd2828_BitSplitFromByteArray(pData, 27, 3), &(pTMCCInfo->xcurrent.layerB.ilLength));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    pTMCCInfo->xcurrent.layerB.segmentsNum = (UINT8)cxd2828_BitSplitFromByteArray(pData, 30, 4);

    result = ConvertTMCCModulation((UINT8)cxd2828_BitSplitFromByteArray(pData, 34, 3), &(pTMCCInfo->xcurrent.layerC.modulation));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCCodingRate((UINT8)cxd2828_BitSplitFromByteArray(pData, 37, 3), &(pTMCCInfo->xcurrent.layerC.codingRate));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCIlLength((UINT8)cxd2828_BitSplitFromByteArray(pData, 40, 3), &(pTMCCInfo->xcurrent.layerC.ilLength));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    pTMCCInfo->xcurrent.layerC.segmentsNum = (UINT8)cxd2828_BitSplitFromByteArray(pData, 43, 4);

    /* Next */
    pTMCCInfo->next.isPartial = (UINT8)cxd2828_BitSplitFromByteArray(pData, 47, 1);

    result = ConvertTMCCModulation((UINT8)cxd2828_BitSplitFromByteArray(pData, 48, 3), &(pTMCCInfo->next.layerA.modulation));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCCodingRate((UINT8)cxd2828_BitSplitFromByteArray(pData, 51, 3), &(pTMCCInfo->next.layerA.codingRate));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCIlLength((UINT8)cxd2828_BitSplitFromByteArray(pData, 54, 3), &(pTMCCInfo->next.layerA.ilLength));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    pTMCCInfo->next.layerA.segmentsNum = (UINT8)cxd2828_BitSplitFromByteArray(pData, 57, 4);

    result = ConvertTMCCModulation((UINT8)cxd2828_BitSplitFromByteArray(pData, 61, 3), &(pTMCCInfo->next.layerB.modulation));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCCodingRate((UINT8)cxd2828_BitSplitFromByteArray(pData, 64, 3), &(pTMCCInfo->next.layerB.codingRate));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCIlLength((UINT8)cxd2828_BitSplitFromByteArray(pData, 67, 3), &(pTMCCInfo->next.layerB.ilLength));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    pTMCCInfo->next.layerB.segmentsNum = (UINT8)cxd2828_BitSplitFromByteArray(pData, 70, 4);

    result = ConvertTMCCModulation((UINT8)cxd2828_BitSplitFromByteArray(pData, 74, 3), &(pTMCCInfo->next.layerC.modulation));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCCodingRate((UINT8)cxd2828_BitSplitFromByteArray(pData, 77, 3), &(pTMCCInfo->next.layerC.codingRate));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = ConvertTMCCIlLength((UINT8)cxd2828_BitSplitFromByteArray(pData, 80, 3), &(pTMCCInfo->next.layerC.ilLength));
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    pTMCCInfo->next.layerC.segmentsNum = (UINT8)cxd2828_BitSplitFromByteArray(pData, 83, 4);

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Convert the binary of system to cxd2828_isdb_t_tmcc_system_t

  pDemod      : Address of ISDB-T part instance.
  data        : Data of system.
  pTMCCSystem : Address of system value.
--------------------------------------------------------------------*/
static cxd2828_result_t ConvertTMCCSystem(UINT8 data, cxd2828_isdb_t_tmcc_system_t* pTMCCSystem)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("ConvertTMCCSystem");

    /* Null check */
    if(!pTMCCSystem){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(data & 0x03)
    {
    case 0:
        *pTMCCSystem = CXD2828_ISDB_T_TMCC_SYSTEM_ISDB_T;
        break;
    case 1:
        *pTMCCSystem = CXD2828_ISDB_T_TMCC_SYSTEM_ISDB_TSB;
        break;
    case 2:
        *pTMCCSystem = CXD2828_ISDB_T_TMCC_SYSTEM_RESERVE_2;
        break;
    case 3:
        *pTMCCSystem = CXD2828_ISDB_T_TMCC_SYSTEM_RESERVE_3;
        break;
    default:
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Convert the binary of modulation to cxd2828_isdb_t_tmcc_modulation_t

  pDemod          : Address of ISDB-T part instance.
  data            : Data of modulation.
  pTMCCModulation : Address of modulation value.
--------------------------------------------------------------------*/
static cxd2828_result_t ConvertTMCCModulation(UINT8 data, cxd2828_isdb_t_tmcc_modulation_t* pTMCCModulation)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("ConvertTMCCModulation");

    /* Null check */
    if(!pTMCCModulation){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(data)
    {
    case 0:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_DQPSK;
        break;
    case 1:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_QPSK;
        break;
    case 2:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_16QAM;
        break;
    case 3:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_64QAM;
        break;
    case 4:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_4;
        break;
    case 5:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_5;
        break;
    case 6:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_RESERVED_6;
        break;
    case 7:
        *pTMCCModulation = CXD2828_ISDB_T_TMCC_MODULATION_UNUSED_7;
        break;
    default:
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Convert the binary of coding rate to cxd2828_isdb_t_tmcc_coding_rate_t

  pDemod          : Address of ISDB-T part instance.
  data            : Data of coding rate.
  pTMCCCodingRate : Address of coding rate value.
--------------------------------------------------------------------*/
static cxd2828_result_t ConvertTMCCCodingRate(UINT8 data, cxd2828_isdb_t_tmcc_coding_rate_t* pTMCCCodingRate)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("ConvertTMCCCodingRate");

    /* Null check */
    if(!pTMCCCodingRate){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(data)
    {
    case 0:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_1_2;
        break;
    case 1:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_2_3;
        break;
    case 2:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_3_4;
        break;
    case 3:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_5_6;
        break;
    case 4:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_7_8;
        break;
    case 5:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_RESERVED_5;
        break;
    case 6:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_RESERVED_6;
        break;
    case 7:
        *pTMCCCodingRate = CXD2828_ISDB_T_TMCC_CODING_RATE_UNUSED_7;
        break;
    default:
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Convert the binary of interleaving length to cxd2828_isdb_t_tmcc_il_length_t

  pDemod        : Address of ISDB-T part instance.
  data          : Data of interleaving length.
  pTMCCIlLength : Address of interleaving length value.
--------------------------------------------------------------------*/
static cxd2828_result_t ConvertTMCCIlLength(UINT8 data, cxd2828_isdb_t_tmcc_il_length_t* pTMCCIlLength)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("ConvertTMCCIlLength");

    /* Null check */
    if(!pTMCCIlLength){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(data)
    {
    case 0:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_0_0_0;
        break;
    case 1:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_4_2_1;
        break;
    case 2:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_8_4_2;
        break;
    case 3:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_16_8_4;
        break;
    case 4:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_4;
        break;
    case 5:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_5;
        break;
    case 6:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_RESERVED_6;
        break;
    case 7:
        *pTMCCIlLength = CXD2828_ISDB_T_TMCC_IL_LENGTH_UNUSED_7;
        break;
    default:
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    CXD2828_TRACE_RETURN(result);
}
