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
/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/

#define CUSTOMER_IMPLEMENTATION

#ifdef CUSTOMER_IMPLEMENTATION
/* Add your headers here */

#else
#include <Windows.h>
#endif

//#include <stdio.h>
#include <stdarg.h>

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmsysFrontEndTypes.h"
#include "tmUnitParams.h"


#include "tmsysOM3869S.h"
#include "tmsysOM3869Slocal.h"
#include "tmsysOM3869SInstance.h"

#ifndef CUSTOMER_IMPLEMENTATION
#include "tmdlI2C.h"
#include "tmptkDefaultDbgMsgUnManaged.h"
#include "tmsysOM3869SDbg.h"
#endif
/************************************************************************/
/* Makefile Informations                                                */
/************************************************************************/
/* Makefile preprocessor definitions to add to compile TDA18211 driver  */
/************************************************************************/
/* -DBSLTDA18211_DLFRONTEND                                             */
/* If you have a Big Endian Host processor, add also the following      */
/* -D_BIG_ENDIAN                                                        */
/************************************************************************/
/************************************************************************/
/* Include files path needed to compile FrontEnd                        */
/************************************************************************/
/* inc; tmbslTDA10048\inc; tmbslTDA10048\cfg; tmbslTDA18211A\inc        */
/************************************************************************/
/************************************************************************/
/* Source files needed to compile FrontEnd                              */
/************************************************************************/
/* .\tmbslTDA10048\src\tmbslTDA10048.c                                  */
/* .\tmbslTDA10048\src\tmbslTDA10048Fw.c                                */
/* .\tmbslTDA10048\src\tmbslTDA10048Instance.c                          */
/* .\tmbslTDA10048\cfg\tmbslTDA10048_cfg.c                              */
/* .\tmbslTDA18211A\src\tmbslTDA18211.c                                 */
/* .\tmbslTDA18211A\src\tmbslTDA18211Instance.c                         */
/* .\src\tmsysOM3869S.c                                                  */
/* .\src\tmsysOM3869SInstance.c                                          */
/* .\src\OM3869SSample.c                                                 */
/************************************************************************/


/*============================================================================*/
/*                   TYPE DEFINITION                                          */
/*============================================================================*/

#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#define P_DBGPRINTEx tmsysOM3869SPrint
#define P_DBGPRINTVALID (True)
#define OM3869S_INVALID_DEVICE_ADDRESS (0xFF)

/*============================================================================*/
/*                   CUSTOMER SPECIFIC IMPLEMENTATION                         */
/*============================================================================*/
#ifdef CUSTOMER_IMPLEMENTATION

#define I2C_BUFFER_SIZE (500)
#define PRINT_BUFFER_SIZE (1040)

/* Link your I²C Read function to this function */
/*============================================================================*/
/* tmdlI2CRead                                                                */
/*============================================================================*/
tmErrorCode_t tmdlI2CRead(tmUnitSelect_t tUnit, UInt32 uHwAddress, UInt32 AddrSize, UInt8* pAddr, UInt32 ReadLen, UInt8* pData)
{
    tmErrorCode_t   err = TM_OK;

    /* Place your code here */

    return err;
}

/* Link your I²C Write function to this function */
/*============================================================================*/
/* tmdlI2CWrite                                                               */
/*============================================================================*/
tmErrorCode_t tmdlI2CWrite(tmUnitSelect_t tUnit, UInt32 uHwAddress, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData)
{
    tmErrorCode_t   err = TM_OK;

    /* Place your code here */

    return err;
}

/* Link your Wait function to this function */
/*============================================================================*/
/* tmdlWait                                                                   */
/*============================================================================*/
tmErrorCode_t tmdlWait(UInt32 tms)
{
    tmErrorCode_t   err = TM_OK;

    /* Place your code here */

    return err;
}

/* Link your Print function to this function */
/*============================================================================*/
/* tmdlPrint                                                                  */
/*============================================================================*/
tmErrorCode_t tmdlPrint(char *buf)
{
    tmErrorCode_t   err = TM_OK;

    /* Place your code here */

    return err;
}

#else

#define I2C_BUFFER_SIZE (500)
#define PRINT_BUFFER_SIZE (1040)

#endif





/*============================================================================*/
/*                   PUBLIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/

tmErrorCode_t tmsysOM3869SI2CRead(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 ReadLen, UInt8* pData)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;
    tmErrorCode_t           errI2C = TM_OK;
    UInt8                   uBytesBuffer[I2C_BUFFER_SIZE];
    UInt8                   uSubAddress = 0;
    UInt32                  i = 0;
    tmUnitSelect_t          tUnitOM3869S = GET_SYSTEM_TUNIT(tUnit);

    if(GET_XFER_DISABLED_FLAG_TUNIT(tUnit) == False)
    {
        err = OM3869SGetInstance(tUnitOM3869S, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

        if(err == TM_OK)
        {
            if(pObj->uHwAddress[UNIT_PATH_TYPE_GET(tUnit)] != OM3869S_INVALID_DEVICE_ADDRESS)
            {          
                switch(AddrSize)
                {
                default:
                case 0:
                    uSubAddress = 0;
                    break;

                case 1:
                    uSubAddress = (*pAddr)&0xFF;
                    break;
                }

                if (ReadLen > I2C_BUFFER_SIZE)
                {
                    err = OM3869S_ERR_BAD_PARAMETER;
                }
                else
                {
                    switch(UNIT_PATH_TYPE_GET(tUnit))
                    {
                    case tmOM3869SUnitDeviceTypeTDA182I2:
                        errI2C = tmdlI2CRead(tUnit, pObj->uHwAddress[UNIT_PATH_TYPE_GET(tUnit)], AddrSize, &uSubAddress, ReadLen, uBytesBuffer);
                        break;

                    default:
                        tmDBGPRINTEx(DEBUGLVL_ERROR, "tmsysOM3869SI2CRead(%08X) failed. Unknown Device Type.", tUnit);
                        err = OM3869S_ERR_BAD_PARAMETER;
                        break;
                    }
                }

                if(errI2C == TM_OK && err == TM_OK)
                {
                    for (i=0 ; i<ReadLen ; i++) 
                    {
                        *(pData + i) = (UInt8)uBytesBuffer[i];
                    }
                }
                else
                {
                    tmDBGPRINTEx(DEBUGLVL_ERROR, "tmsysOM3869SI2CRead(0x%08X, [sub@0x%02x] %d) failed.", tUnit, uSubAddress, ReadLen);
                    err = OM3869S_ERR_HW;
                }
            }
        }
    }

    return err;
}

tmErrorCode_t tmsysOM3869SI2CWrite(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;
    tmErrorCode_t           errI2C = TM_OK;
    Bool                    bIncSubAddress = True;
    UInt8                   uSubAddress = 0;
    UInt32                  uCounter = 0;
    UInt8                   WriteBuffer[I2C_BUFFER_SIZE+1] = {0};
    UInt32                  count = 0;
    UInt32                  remain = 0;
    tmUnitSelect_t          tUnitOM3869S = GET_SYSTEM_TUNIT(tUnit);

    if(GET_XFER_DISABLED_FLAG_TUNIT(tUnit) == False)
    {
        err = OM3869SGetInstance(tUnitOM3869S, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

        if(err == TM_OK)
        {
            if(pObj->uHwAddress[UNIT_PATH_TYPE_GET(tUnit)] != OM3869S_INVALID_DEVICE_ADDRESS)
            {
                switch(AddrSize)
                {
                default:
                case 0:
                    uSubAddress = 0;
                    break;

                case 1:
                    uSubAddress = (*pAddr)&0xFF;
                    break;
                }

                /* Copy uSubAddress to first data byte */
                WriteBuffer[0] = uSubAddress;

                for(count=0; count<WriteLen; count += remain)
                {
                    remain = min(WriteLen-count, I2C_BUFFER_SIZE);

                    /* copy I2CMap data in WriteBuffer */
                    for (uCounter = 0; uCounter < remain; uCounter++)
                    {
                        WriteBuffer[uCounter+1] = pData[count+uCounter];
                    }

                    switch(UNIT_PATH_TYPE_GET(tUnit))
                    {
                    case tmOM3869SUnitDeviceTypeTDA182I2:
                        errI2C = tmdlI2CWrite(tUnit, pObj->uHwAddress[UNIT_PATH_TYPE_GET(tUnit)], AddrSize, &uSubAddress, remain, WriteBuffer+1);
                        break;

                    default:
                        tmDBGPRINTEx(DEBUGLVL_ERROR, "tmsysOM3869SI2CWrite(0x%08X) failed. Unknown Device Type.", tUnit);
                        err = OM3869S_ERR_BAD_PARAMETER;
                        break;
                    }

                    if(errI2C != TM_OK)
                    {
                        tmDBGPRINTEx(DEBUGLVL_ERROR, "tmsysOM3869SI2CWrite(0x%08X, [sub@0x%02x] %d/%d) failed.", tUnit, uSubAddress, count, WriteLen);
                        err = OM3869S_ERR_HW;
                    }

                    if(err != TM_OK)
                        break;

                    if(bIncSubAddress)
                    {
                        uSubAddress = (UInt8)(uSubAddress+remain);
                        WriteBuffer[0] = uSubAddress;
                    }
                }
            }
        }
    }

    return err;
}

/**  Wait tms ms without blocking scheduler, warning this function 
don't schedule others frontend instance */
tmErrorCode_t tmsysOM3869SWait(tmUnitSelect_t tUnit, UInt32 tms)
{
#ifndef CUSTOMER_IMPLEMENTATION
    LARGE_INTEGER freq;
    LARGE_INTEGER count1;
    LARGE_INTEGER count2;
    UInt32 time_ms = 0;

    freq.QuadPart = 0;
    count1.QuadPart = 0;
    count2.QuadPart = 0;

    if (QueryPerformanceFrequency(&freq) != 0)
    {

        if (QueryPerformanceCounter(&count1) != 0)
        {
            while(time_ms<tms)
            {
                Sleep(0);
                QueryPerformanceCounter(&count2);
                time_ms = (UInt32)((count2.QuadPart - count1.QuadPart) * 1000 / freq.QuadPart);
            }
        }
        else
        {
            Sleep(1);
            Sleep(tms);
        }
    }
#else
    /* Customer implementation */
    tmdlWait(tms);
#endif

    return TM_OK;
}

#ifdef NXPFEV2_DBG_MSG

w_tmdlDbgMsgCallBack    pDbgMsgCallBack = Null;

w_tmdlDbgMsgCallBack *tmsysOM3869SAllocDbgMsgUnManaged()
{
    return &pDbgMsgCallBack;
}

tmErrorCode_t tmsysOM3869SDeAllocDbgMsgUnManaged()
{
    pDbgMsgCallBack = Null;

    return TM_OK;
}

#endif

tmErrorCode_t tmsysOM3869SPrint(UInt32 level, const char* format, ...)
{
    tmErrorCode_t   err = TM_OK;

#ifdef NXPFEV2_DBG_MSG
    va_list         args;
    int             len = 0;
    char            *buffer = Null;

    va_start( args, format );
    len = _vscprintf(format, args ) + 1;
    buffer = (char *)malloc(len * sizeof(char));

    (void)vsprintf_s(buffer, len, format, args);

    if(pDbgMsgCallBack != Null)
    {
        err = pDbgMsgCallBack(level, buffer);
    }

    free(buffer);
#else
    static char     buf[PRINT_BUFFER_SIZE] = "\0";
//    va_list         ap;

    if(level<DEBUGLVL_VERBOSE)
    {  /* mark 3 line*/
       // va_start(ap, format);
        //(void)vsnprintf(buf, PRINT_BUFFER_SIZE, format, ap);
        //va_end(ap);

#ifdef CUSTOMER_IMPLEMENTATION
        /* Customer implementation */
        tmdlPrint(buf);
#endif
    }
#endif

    return err;
}


