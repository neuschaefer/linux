/*------------------------------------------------------------------------------

 Copyright 2009, 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/05/27
 Revision: 1.0.1.0

------------------------------------------------------------------------------*/

#include "cxd2828_common.h"

/* Utility function */
#define MASKUPPER(n) (((n) == 0) ? 0 : (0xFFFFFFFFU << (32 - (n))))
#define MASKLOWER(n) (((n) == 0) ? 0 : (0xFFFFFFFFU >> (32 - (n))))
/* Convert N (<32) bit 2's complement value to 32 bit signed value */
INT32 cxd2828_Convert2SComplement(UINT32 value, UINT32 bitlen)
{
    if((bitlen == 0) || (bitlen >= 32)){
        return (INT32)value;
    }

    if(value & (UINT32)(1 << (bitlen - 1))){
        /* minus value */
        return (INT32)(MASKUPPER(32 - bitlen) | value);
    }else{
        /* plus value */
        return (INT32)(MASKLOWER(bitlen) & value);
    }
}

UINT32 cxd2828_BitSplitFromByteArray(UINT8 *pArray, UINT32 startBit, UINT32 bitNum)
{
    UINT32 value = 0;
    UINT8 *pArrayRead;
    UINT8 bitRead;
    UINT32 lenRemain;

    if(!pArray){return 0;}
    if((bitNum == 0) || (bitNum > 32)){return 0;}

    pArrayRead = pArray + (startBit/8);
    bitRead = (UINT8)(startBit % 8);
    lenRemain = bitNum;

    if(bitRead != 0){
        if(((INT32)lenRemain) <= 8 - bitRead){
            value = (*pArrayRead) >> ((8 - bitRead) - lenRemain);
            lenRemain = 0;
        }else{
            value = *pArrayRead++;
            lenRemain -= 8 - bitRead;
        }
    }

    while(lenRemain > 0){
        if(lenRemain < 8){
            value <<= lenRemain;
            value |= (*pArrayRead++ >> (8 - lenRemain));
            lenRemain = 0;
        }else{
            value <<= 8;
            value |= (UINT32)(*pArrayRead++);
            lenRemain -= 8;
        }
    }

    value &= MASKLOWER(bitNum);
    return value;
}
