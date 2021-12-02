#ifndef _WMALSLIF_H_
#define _WMALSLIF_H_

#include "riscdec.h"



//=====================================
INT32 u4DSP_WMALSLinit(AUD_DEC_HANDLE_T * pHandle);
INT32 u4DSP_WMALSLdecode(AUD_DEC_HANDLE_T * pHandle, UINT32* u4outlen);
INT32 u4DSP_WMAFileDecodeInitEx(AUD_DEC_HANDLE_T* pHandle);




#if 0  
TODO

06142011 //ian FIXME 06142011
*(v)pprvMCLMSPred/ pprvMCLMSUpdate/ pprvCDLMSUpdate -->改半天 加struct在define就好了
*(v)prvLMSPredictorUpdate_I16_C/ prvLMSPredictorUpdate_I32_C
*(v)prvInitOutputSampleFormatSetFunctionPtrs/ prvInitCommonSetFunctionPtrs/
(v) auMalloc /auFree 
ROUND_SATURATE_STORE_LLM

06152011 //ian FIXME 06152011
(v) CAudioObjectDecoder->m_pprvNewInputBuffer
(v) CAudioObjectDecoder CWMAInputBitStream->m_dwOwner
//ian FIXME 06152011 initial crash


typedef void (*MUSB_pfCdcTransferComplete)(UINT32 dwActualLength);
extern INT32 MUSB_CdcAsyncRx(UINT8 *pBuf, UINT32 dwCount, 
    MUSB_pfCdcTransferComplete pfXferComplete);

06162011 //ian FIXME 06162011
cWMA_BadPacketHeader
WMAF_UpdateNewPayload->WMA_ParsePacketHeader return WMAERR_FAIL

06182011
initial input buffer  
FIFO STARTADD
u4AfifoSA = VIRTUAL(u4GetAFIFOStart(0));
FIFO BUFPNT
u4FifoRp = u4AudHalGetBufRPtr(FRTH_DEC, DSP_AFIFO);
Write PRSPNT
#define vAudHalSetBufWPtr(a,b,c) vSetAWritePnt(PHYSICAL(c))
u4AudOmxFillFIFO

06192011
i4AudOmxPrepareDecoding ->  i4AudWMALSLTaskCreate

06212011 //ian FIXME 06212011
Decoder thread / WMA decode Getbits interface
1. header not aligned: remove cHeader offset while initial
2. remove _u4inBUFr_ptr,_u4inBUFd_len
3. pHandle information pass to WMAFileCBGetData: riscdec.h
4. check frame size
        remove cbFirstPacketOffset (用來parse asf header)
        remove cbLastPacketOffset/nextPacketOffset(用來計算是否為最後一個packet)
        currPacketOffset 改為 BUFPNT
5. WMAFileCBGetData
6. WMAFileCBGetData(pInt, pInt->hdr_parse.currPacketOffset, cbWanted, &pData);
    &pData WMAFileCBGetData(pInt, pInt->hdr_parse.currPacketOffset, cbWanted, 
    &pData direct point to Afifo
    //ian FIXME 06212011 GetBits crash

06222011
1. vDSP_DecMovFIFOPnt
[AUD] b4 vDSP_DecMovFIFOPnt : 0xDDC98900. 0xDDCA2500. 0xDDC98900. 0x345E.
[AUD] after vDSP_DecMovFIFOPnt : 0xDDC98900. 0xDDCA2500. 0xDDC9BE78. 0x345E.
2. u4DSP_DecChkFrmSz: refine


#endif


#endif
