#ifdef LINUX_TURNKEY_SOLUTION

#include "riscdec.h"
#include "aud_debug.h" //LOG
#include "x_assert.h"

BOOL u4DSP_DecChkFrmSz(AUD_DEC_HANDLE_T * pHandle, UINT32 u4startaddr, UINT32 u4length){

#if 1

        UINT32 u4BufAvil;
        u4BufAvil = ((UINT32)(pHandle->pInBufPRSPNT) >= u4startaddr)?
                      ( (UINT32)(pHandle->pInBufPRSPNT) - u4startaddr):
                      ( (UINT32)pHandle->u4InBufSize - (UINT32)(u4startaddr-(UINT32)(pHandle->pInBufPRSPNT))) ;

        if(u4BufAvil >= u4length){
                return TRUE;
        }else{
                LOG(7,"wp = %X, rp = %X\n",pHandle->pInBufPRSPNT,u4startaddr);
                return FALSE;
        }              
#else
        return TRUE;
#endif
}

void vDSP_DecMovFIFOPnt(const UINT32 u4STRBLK, const UINT32 u4ENDBLK, UINT32 **u4PNT, const INT32 i4len){

        INT32 i4temp;

        i4temp = (INT32) *u4PNT;
        i4temp += i4len;
/*
         LOG(1," vDSP_DecMovFIFOPnt : 0x%X. 0x%X. 0x%X. \n", 
                        (UINT32) (*u4PNT), 
                        (INT32) i4len, 
                        (UINT32) i4temp);
*/
        //move forward
        if((UINT32)i4temp >= u4ENDBLK) {
            *u4PNT =  (UINT32 *)(u4STRBLK + ((UINT32)i4temp-u4ENDBLK));
        }
        //move backward
        else if((UINT32)i4temp < u4STRBLK) {
            *u4PNT =  (UINT32 *)(u4ENDBLK - (u4STRBLK - (UINT32)i4temp));
        }
        else 
        {
            *u4PNT = (UINT32 *)i4temp;
        }

//        LOG(1," vDSP_DecMovFIFOPnt : 0x%X.\n", (UINT32) (*u4PNT));

}

UINT32 u4DSP_GetBits(AUD_DEC_HANDLE_T * pHandle, const UINT32 u4Len ,UINT8 *pbData){

        UINT32 u4BufAvil;
        UINT32 u4FifoSA, u4FifoEA, u4FifoSize;
        UINT32 u4FifoRp, u4FifoWp;
        UINT32 u4Rp2EA;
        

        u4FifoSA = (UINT32) pHandle->pInBufSTRBLK;
        u4FifoEA = (UINT32) pHandle->pInBufENDBLK;
        u4FifoWp = (UINT32) pHandle->pInBufPRSPNT;
        u4FifoRp = (UINT32)  pHandle->pInBufCURPNT;
        u4FifoSize = (UINT32)pHandle->u4InBufSize;

        u4BufAvil = (u4FifoWp >= u4FifoRp)?
                      ( u4FifoWp - u4FifoRp):
                      (  u4FifoSize - (u4FifoRp -u4FifoWp)) ;
        VERIFY(u4BufAvil >= u4Len);


         u4Rp2EA = u4FifoEA - u4FifoRp;
        if(u4Rp2EA == u4Len){  
                x_memcpy((void*)pbData, (const void*)u4FifoRp, u4Len);
                pHandle->pInBufCURPNT = pHandle->pInBufSTRBLK;
        }else if(u4Rp2EA > u4Len){
                x_memcpy((void*)pbData, (const void*)u4FifoRp, u4Len);
                pHandle->pInBufCURPNT = (void *)(u4FifoRp+ u4Len);
        }else{
                x_memcpy((void*)pbData, (const void*)u4FifoRp, u4Rp2EA);
                u4BufAvil = u4Len - u4Rp2EA;
                u4FifoRp = u4FifoSA;
                x_memcpy((void*)(pbData+u4Rp2EA), (const void*)u4FifoRp, u4BufAvil);
                pHandle->pInBufCURPNT = (void *)(u4FifoRp + u4BufAvil);
        }


#if 0
        u4Rp2EA = u4FifoEA - u4FifoRp;
        if(u4Rp2EA == u4Len){  
                x_memcpy((void*)pbData, (const void*)u4FifoRp, u4Len);
                pHandle->pInBufCURPNT = pHandle->pInBufSTRBLK;
        }else if(u4Rp2EA > u4Len){
                x_memcpy((void*)pbData, (const void*)u4FifoRp, u4Len);
                pHandle->pInBufCURPNT = (void *)((UINT32)pHandle->pInBufSTRBLK + u4Len);
        }else{
                x_memcpy((void*)pbData, (const void*)u4FifoRp, u4Rp2EA);
                u4BufAvil = u4Len - u4Rp2EA;
                x_memcpy((void*)pbData, (const void*)u4FifoRp, u4BufAvil);
                pHandle->pInBufCURPNT = (void *)((UINT32)pHandle->pInBufSTRBLK + u4BufAvil);
        }

        if(u4Rp2EA == u4Len){  
                x_memcpy((void*)pbData, (const void*)pbData, u4Len);
                pHandle->pInBufCURPNT = pHandle->pInBufSTRBLK;
        }else if(u4Rp2EA > u4Len){
                x_memcpy((void*)pbData, (const void*)pbData, u4Len);
                pHandle->pInBufCURPNT = (void *)((UINT32)pHandle->pInBufSTRBLK + u4Len);
        }else{
                x_memcpy((void*)pbData, (const void*)pbData, u4Rp2EA);
                u4BufAvil = u4Len - u4Rp2EA;
                x_memcpy((void*)pbData, (const void*)pbData, u4BufAvil);
                pHandle->pInBufCURPNT = (void *)((UINT32)pHandle->pInBufSTRBLK + u4BufAvil);
        }
#endif   
        return u4Len;
}

#endif
