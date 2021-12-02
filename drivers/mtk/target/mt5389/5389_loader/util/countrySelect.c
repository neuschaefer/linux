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
/*-----------------------------------------------------------------------------
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#include "x_util.h"
#include "drvcust_if.h"
#include "eeprom_if.h"

UINT8 _u4TargetCountry = COUNTRY_US;

void SetTargetCountry(UINT32 u4Country) 
{
    #ifdef COUNTRY_FROM_EEPROM
    UINT32 u4CountryFromEEPROM, u4Offset;
    UINT8 u1Country, i;
    QUERY_TYPE_T arKey[] = {eCountryEEPROMOffset1, eCountryEEPROMOffset2, eCountryEEPROMOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);
    #endif /* COUNTRY_FROM_EEPROM */
    
    _u4TargetCountry = u4Country;
    
    #ifdef COUNTRY_FROM_EEPROM
    u1Country = (UINT8)_u4TargetCountry;
    
    if (DRVCUST_OptQuery(eCountryFromEEPROM, &u4CountryFromEEPROM) == 0)
    {
        if (u4CountryFromEEPROM)
        {
			for (i = 0; i < u4Size; i++)
			{
				if (DRVCUST_OptQuery(arKey[i], &u4Offset) == 0)
				{
					if (EEPROM_Write((UINT64)u4Offset, (UINT32)&u1Country, 1) != 0)
					{
						Printf("SetTargetCountry fail, eeprom addr=0x%x\n", u1Country);
					}
				}
			}
		}
    }
    #endif /* COUNTRY_FROM_EEPROM */
}

UINT32 GetTargetCountry(void)
{
    static UINT8 u1Init = 0 ;

    #ifndef COUNTRY_FROM_EEPROM
    {
        if (u1Init == 0)
        {
            #ifndef CC_MT5301A
            UINT32 u4Country;
            // Init target country
            if (DRVCUST_InitQuery(eCountryDefaultSetting, &u4Country) == 0)
            {
                // get target country settnig from driver customization
                SetTargetCountry(u4Country);
            }
            else
            #endif    
            {
                #ifdef CC_ISDB_SUPPORT
                    SetTargetCountry(COUNTRY_BZ);
                #elif defined(CC_TWN_SUPPORT)
                    SetTargetCountry(COUNTRY_TW);
                #else
                    #ifdef CC_DVBT_SUPPORT
                    // init as EU model
                    SetTargetCountry(COUNTRY_EU);
                    #else
                    SetTargetCountry(COUNTRY_US);
                    #endif
                #endif
            }
            u1Init = 1;
        }
    }
    #else  //COUNTRY_FROM_EEPROM
    {
        UINT32 u4CountryFromEEPROM, u4Offset;
        UINT8 i,j, u4ValidCounter = 0,u1FinalCountry=0xff ;
        UINT8 au1Country[] = {0xff, 0xff, 0xff};
        QUERY_TYPE_T arKey[] = {eCountryEEPROMOffset1, eCountryEEPROMOffset2, eCountryEEPROMOffset3};
        UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);
       
        if (DRVCUST_OptQuery(eCountryFromEEPROM, &u4CountryFromEEPROM) == 0)
        {
            if (u4CountryFromEEPROM)
            {

                if(u1Init == 0 )
                {
    				// read configuration from eeprom
    				for (i = 0; i < u4Size; i++)
    				{
    					if (DRVCUST_OptQuery(arKey[i], &u4Offset) == 0)
    					{
    						UNUSED(EEPROM_Read((UINT64)u4Offset, (UINT32)&au1Country[i], 1));
    						if(au1Country[i] != 0xff)
    						{
    							u4ValidCounter++;	
    						}
    					}
    				}
    				if (u4ValidCounter == 1)
    				{
    					for (i = 0; i < u4Size; i++)
    					{
    						if (au1Country[i] != 0xff)
    						{
    							u1FinalCountry= au1Country[i];
    							break;
    						}
    					}
    				}
    				else if (u4ValidCounter >= 2)
    				{
    					for (i = 0; i < u4Size; i++)
    					{
    						if (au1Country[i] == 0xff)
    						{
    							continue;
    						}
    						for (j = 0; j < u4Size; j++)
    						{
    							if ((i != j) && (au1Country[i] == au1Country[j]))
    							{
    								u1FinalCountry = au1Country[i];
    								break;
    							}
    						}
    						if (u1FinalCountry != 0xff)
    						{
    							break;
    						}
    					}	
    				}
    				//Printf(" u4ValidCounter %d u1FinalCountry %d\n", u4ValidCounter, u1FinalCountry);
    				if (u1FinalCountry == 0xFF)
    				{
    					// If EEPROM is empty, set country as 
    					// Enable CC & Audio, DTV, ATV as US standard
#ifdef NEW_COUNTRY_TYPE 
						u1FinalCountry = COUNTRY_US;
#else
    					u1FinalCountry = COUNTRY_CC_ENABLE_MASK;
#endif
    				}
    				_u4TargetCountry = (UINT32)u1FinalCountry;
    				SetTargetCountry(_u4TargetCountry);
    				u1Init = 1 ;
                }                
            }
        }
    }
    #endif /* COUNTRY_FROM_EEPROM */

    return _u4TargetCountry;
}



#ifdef NEW_COUNTRY_TYPE
UINT32 GetTargetCountryMask(UINT32 u4Mask)
{
    UINT32 u4Pos = 0;
    UINT32 u4Country = 0;

    u4Country = GetTargetCountry();
    if (u4Mask == COUNTRY_AUDIO_MASK)
    {
        u4Pos = COUNTRY_AUDIO_POS;
    }
    else if (u4Mask == COUNTRY_VBI_MASK)
    {
        u4Pos = COUNTRY_VBI_POS;    
    }
    else if (u4Mask == COUNTRY_ATV_MASK)
    {
        u4Pos = COUNTRY_ATV_POS;    
    }
    else if (u4Mask == COUNTRY_DTV_MASK)
    {
        u4Pos = COUNTRY_DTV_POS;    
    }
    else    // unknow
    {
        return 0;    
    }

    u4Country = (u4Country & u4Mask) >> u4Pos;

    return u4Country;
}
#endif


