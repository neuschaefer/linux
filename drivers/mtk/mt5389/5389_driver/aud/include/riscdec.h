#ifndef _RISCDEC_H_
#define _RISCDEC_H_

#include "x_stl_lib.h"
#include "aud_if.h"


BOOL u4DSP_DecChkFrmSz(AUD_DEC_HANDLE_T * pHandle, UINT32 u4startaddr, 
                                                                UINT32 u4length);
UINT32 u4DSP_GetBits(AUD_DEC_HANDLE_T * pHandle, const UINT32 u4Len,
                                                                UINT8 *pbData);
void vDSP_DecMovFIFOPnt(const UINT32 u4STRBLK, const UINT32 u4ENDBLK, UINT32 **u4PNT, 
                                                                const INT32 u4len);

#endif //_RISCDEC_H_
