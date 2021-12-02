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
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_descrambler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_descrambler.c
 *  Demux driver - descrambler
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "x_assert.h" 

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _DMX_SetDesIV
 *  Set initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetDesIV(UINT32 u4IvHi, UINT32 u4IvLo)
{
	DMXCMD_WRITE32(DMX_REG_DES_IV_LO_EVEN, u4IvLo);
	DMXCMD_WRITE32(DMX_REG_DES_IV_HI_EVEN, u4IvHi);
	DMXCMD_WRITE32(DMX_REG_DES_IV_LO_ODD, u4IvLo);
	DMXCMD_WRITE32(DMX_REG_DES_IV_HI_ODD, u4IvHi);	
}

void _DMX_SetDesIVEx(UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven)
{
    if(fgEven) 
    {
    	DMXCMD_WRITE32(DMX_REG_DES_IV_LO_EVEN, u4IvLo);
    	DMXCMD_WRITE32(DMX_REG_DES_IV_HI_EVEN, u4IvHi);
    }
    else 
    {
    	DMXCMD_WRITE32(DMX_REG_DES_IV_LO_ODD, u4IvLo);
    	DMXCMD_WRITE32(DMX_REG_DES_IV_HI_ODD, u4IvHi);	
    }	
}


//-----------------------------------------------------------------------------
/** _DMX_SetDesKey
 *  Set a desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fg3Des          TRUE: 3DES, FALSE: DES
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven,
    const UINT32 au4Key[6])
{
    UINT32 i;
    UINT32* pu4CaKeys;
    UINT32 au4DesMap[6] = { 0, 1, 2, 3, 4, 5};
    UINT32 au43DesMap[6] = { 4, 5, 2, 3, 0, 1};  // Swap key-1 and key-3
    
    ASSERT(au4Key != NULL);

    if (fgEven)
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_EVEN_KEY;
    }
    else
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_ODD_KEY;
    }

    for (i = 0; i < 6; i++)
    {
        UINT32 u4MappedIndex;

        u4MappedIndex = fg3Des ? au43DesMap[i] : au4DesMap[i];
        pu4CaKeys[(u1Index * 8) + i] = au4Key[u4MappedIndex];
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesIV
 */
//-----------------------------------------------------------------------------
void _DMX_SetAesIVEx(const UINT32 au4Iv[4], BOOL fgEven)
{
    UINT32 i;

    if(au4Iv != NULL)
    {
        if(fgEven) 
        {
            for (i = 0; i < 4; i++)
            {
                DMXCMD_WRITE32(DMX_REG_AES_IV_0_EVEN + (3 - i), au4Iv[i]);
            }
        }
        else
        {
            for (i = 0; i < 4; i++)
            {
                DMXCMD_WRITE32(DMX_REG_AES_IV_0_ODD + (3 - i), au4Iv[i]);
            }
        }
    }
}

void _DMX_SetAesIV(const UINT32 au4Iv[4])
{
    _DMX_SetAesIVEx(au4Iv, TRUE);
    _DMX_SetAesIVEx(au4Iv, FALSE);
}



//-----------------------------------------------------------------------------
/** _DMX_SetAesKeyLen
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetAesKeyLen(UINT32 u4KeyLen)
{
    UINT32 u4Ctrl;

    if((u4KeyLen != 128) && (u4KeyLen != 192) && (u4KeyLen != 256))
    {
        return FALSE;
    }

    if(u4KeyLen == 128)
    {
        u4Ctrl = 0x0;
    }
    else if(u4KeyLen == 192)
    {
        u4Ctrl = 0x2;
    }
    else if(u4KeyLen == 256)
    {
        u4Ctrl = 0x4;
    }

    DMXCMD_WRITE32(DMX_REG_AES_CTRL, u4Ctrl);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesKey
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8])
{
    UINT32 i;
    UINT32* pu4CaKeys;
    UINT32 au4AesMap[8] = { 0, 1, 2, 3, 4, 5, 6, 7};

    ASSERT(au4Key != NULL);

    if (fgEven)
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_EVEN_KEY;
    }
    else
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_ODD_KEY;
    }

    for (i = 0; i < 8; i++)
    {
        UINT32 u4MappedIndex;

        u4MappedIndex = au4AesMap[i];
        pu4CaKeys[(u1Index * 8) + i] = au4Key[u4MappedIndex];
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetDmemAesIV
 */
//-----------------------------------------------------------------------------
void _DMX_SetDmemAesIV(const UINT32 au4Iv[4])
{
    UINT32 i;

    if(au4Iv != NULL)
    {
        for (i = 0; i < 4; i++)
        {
            *(volatile UINT32*)(DMX_DMEM_CA_IV_BASE + (4 * i)) = au4Iv[3 - i];            
        }
    }
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2Key
 *  Set a Multi2 desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetMulti2Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[2])
{
    UINT32 i;
    UINT32* pu4CaKeys;
    //UINT32 au4KeyIndex[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    UINT32 au4KeyIndex[2] = {0, 1};

    ASSERT(au4Key != NULL);

    pu4CaKeys = (fgEven) ? (UINT32*)DMX_SECTION_CA_EVEN_KEY : (UINT32*)DMX_SECTION_CA_ODD_KEY;

    for (i = 0; i < 2; i++)
    {
        pu4CaKeys[(u1Index * 8) + i] = au4Key[au4KeyIndex[i]];
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2CbcIV
 *  Set initial vector of MULTI-2 CBC mode
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2CbcIV(UINT32 u4IvHi, UINT32 u4IvLo)
{
	DMXCMD_WRITE32(DMX_REG_M2_CBC_IV_LO, u4IvLo);
	DMXCMD_WRITE32(DMX_REG_M2_CBC_IV_HI, u4IvHi);
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2OfbIV
 *  Set initial vector of MULTI-2 OFB mode
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2OfbIV(UINT32 u4IvHi, UINT32 u4IvLo)
{
	DMXCMD_WRITE32(DMX_REG_M2_OFB_IV_LO, u4IvLo);
	DMXCMD_WRITE32(DMX_REG_M2_OFB_IV_HI, u4IvHi);
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2SysKey
 *  Set system keys of MULTI-2
 *
 *  @param  au4Key          The system keys
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2SysKey(const UINT32 au4Key[8])
{
	UINT32 i;

    ASSERT(au4Key != NULL);

	for (i = 0; i < 8; i++)
	{
		DMXCMD_WRITE32(DMX_REG_M2_SYS_KEY0 + i, au4Key[i]);
	}
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2Config
 *  Set MULTI-2 configurations
 *
 *  @param  fgOfbFreeRun    Is OFB free-running mode
 *  @param  u2Iteration     Number of iterations
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2Config(BOOL fgOfbFreeRun, UINT16 u2Iteration)
{
	UINT8 u1OfbFreeRun;

	u1OfbFreeRun = (UINT8)(fgOfbFreeRun ? 1 : 0);

	DMXCMD_WRITE32(DMX_REG_M2_CFG, (u1OfbFreeRun << 16) | u2Iteration);
}

void _DMX_SetCpuDescKeyIv(UINT32 *pu4Key, UINT32 *pu4Iv)
{
	UINT8 i;

    for (i = 0; i < 4; i++)
    {
        DMXCMD_WRITE32(DMX_REG_CPU_DSCR_IV_0 + i, *(UINT32 *)(pu4Iv + i));
    }

    for (i = 0; i < 8; i++)
    {
        DMXCMD_WRITE32(DMX_REG_CPU_DSCR_KEY_0 + i, *(UINT32 *)(pu4Key + i));
    }
}

void _DMX_SetCpuDescIntrClear()
{
    // Clear interrupt
    DMXCMD_WRITE32(DMX_REG_CPU_DSCR_INTR_STATUS, 0x0);
    DMXCMD_WRITE32(DMX_REG_CPU_DSCR_INTR_STATUS, 0x0);
}
