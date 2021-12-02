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
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp6hwdec.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp6hwdec.c
 *  This file contains implementation of Vp6 hw decoder.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_printf.h"
#include "vdec_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END
#include "vdec_vp6util.h"
#include "vdec_vp6dec.h"
#include "vdec_vp6psr.h"
#include "vdec_vp6hw.h"
#include "vdec_vp6if.h"
;

static SORTNODE sn[256];
static const INT16 vp6_block_copy_filter[17][8][4] = 
{
{ {   0, 128,   0,   0  },  /* 0 */
  {  -3, 122,   9,   0  },
  {  -4, 109,  24,  -1  },
  {  -5,  91,  45,  -3  },
  {  -4,  68,  68,  -4  },
  {  -3,  45,  91,  -5  },
  {  -1,  24, 109,  -4  },
  {   0,   9, 122,  -3  } },
  
{ {   0, 128,   0,   0  },  /* 1 */
  {  -4, 124,   9,  -1  },
  {  -5, 110,  25,  -2  },
  {  -6,  91,  46,  -3  },
  {  -5,  69,  69,  -5  },
  {  -3,  46,  91,  -6  },
  {  -2,  25, 110,  -5  },
  {  -1,   9, 124,  -4  } },
  
{ {   0, 128,   0,   0  },  /* 2 */
  {  -4, 123,  10,  -1  },
  {  -6, 110,  26,  -2  },
  {  -7,  92,  47,  -4  },
  {  -6,  70,  70,  -6  },
  {  -4,  47,  92,  -7  },
  {  -2,  26, 110,  -6  },
  {  -1,  10, 123,  -4  } },
  
{ {   0, 128,   0,   0  },  /* 3 */
  {  -5, 124,  10,  -1  },
  {  -7, 110,  27,  -2  },
  {  -7,  91,  48,  -4  },
  {  -6,  70,  70,  -6  },
  {  -4,  48,  92,  -8  },
  {  -2,  27, 110,  -7  },
  {  -1,  10, 124,  -5  } },
  
{ {   0, 128,   0,   0  },  /* 4 */
  {  -6, 124,  11,  -1  },
  {  -8, 111,  28,  -3  },
  {  -8,  92,  49,  -5  },
  {  -7,  71,  71,  -7  },
  {  -5,  49,  92,  -8  },
  {  -3,  28, 111,  -8  },
  {  -1,  11, 124,  -6  } },
  
{ {  0,  128,   0,   0  },  /* 5 */
  {  -6, 123,  12,  -1  },
  {  -9, 111,  29,  -3  },
  {  -9,  93,  50,  -6  },
  {  -8,  72,  72,  -8  },
  {  -6,  50,  93,  -9  },
  {  -3,  29, 111,  -9  },
  {  -1,  12, 123,  -6  } },
  
{ {   0, 128,   0,   0  },  /* 6 */
  {  -7, 124,  12,  -1  },
  { -10, 111,  30,  -3  },
  { -10,  93,  51,  -6  },
  {  -9,  73,  73,  -9  },
  {  -6,  51,  93, -10  },
  {  -3,  30, 111, -10  },
  {  -1,  12, 124,  -7  } },
  
{ {   0, 128,   0,   0  },  /* 7 */
  {  -7, 123,  13,  -1  },
  { -11, 112,  31,  -4  },
  { -11,  94,  52,  -7  },
  { -10,  74,  74, -10  },
  {  -7,  52,  94, -11  },
  {  -4,  31, 112, -11  },
  {  -1,  13, 123,  -7  } },
  
{ {   0, 128,   0,  0  },  /* 8 */
  {  -8, 124,  13,  -1  },
  { -12, 112,  32,  -4  },
  { -12,  94,  53,  -7  },
  { -10,  74,  74, -10  },
  {  -7,  53,  94, -12  },
  {  -4,  32, 112, -12  },
  {  -1,  13, 124,  -8  } },
  
{ {   0, 128,   0,   0  },  /* 9 */
  {  -9, 124,  14,  -1  },
  { -13, 112,  33,  -4  },
  { -13,  95,  54,  -8  },
  { -11,  75,  75, -11  },
  {  -8,  54,  95, -13  },
  {  -4,  33, 112, -13  },
  {  -1,  14, 124,  -9  } },
{ {   0, 128,   0,   0  },  /* 10 */
  {  -9, 123,  15,  -1  },
  { -14, 113,  34,  -5  },
  { -14,  95,  55,  -8  },
  { -12,  76,  76, -12  },
  {  -8,  55,  95, -14  },
  {  -5,  34, 112, -13  },
  {  -1,  15, 123,  -9  } },
  
{ {   0, 128,   0,   0  },  /* 11 */
  { -10, 124,  15,  -1  },
  { -14, 113,  34,  -5  },
  { -15,  96,  56,  -9  },
  { -13,  77,  77, -13  },
  {  -9,  56,  96, -15  },
  {  -5,  34, 113, -14  },
  {  -1,  15, 124, -10  } },
  
{ {   0, 128,   0,   0  },  /* 12 */
  { -10, 123,  16,  -1  },
  { -15, 113,  35,  -5  },
  { -16,  98,  56, -10  },
  { -14,  78,  78, -14  },
  { -10,  56,  98, -16  },
  {  -5,  35, 113, -15  },
  {  -1,  16, 123, -10  } },
  
{ {   0, 128,   0,   0  },  /* 13 */
  { -11, 124,  17,  -2  },
  { -16, 113,  36,  -5  },
  { -17,  98,  57, -10  },
  { -14,  78,  78, -14  },
  { -10,  57,  98, -17  },
  {  -5,  36, 113, -16  },
  {  -2,  17, 124, -11  } },
  
{ {   0, 128,   0,   0  },  /* 14 */
  { -12, 125,  17,  -2  },
  { -17, 114,  37,  -6  },
  { -18,  99,  58, -11  },
  { -15,  79,  79, -15  },
  { -11,  58,  99, -18  },
  {  -6,  37, 114, -17  },
  {  -2,  17, 125, -12  } },
  
{ {   0, 128,   0,   0  },  /* 15 */   
 { -12, 124,  18,  -2  },
  { -18, 114,  38,  -6  },
  { -19,  99,  59, -11  },
  { -16,  80,  80, -16  },
  { -11,  59,  99, -19  },
  {  -6,  38, 114, -18  },
  {  -2,  18, 124, -12  } },
{ {   0, 128,   0,   0  },  /* 16 */
 {  -4, 118,  16,  -2  },
  { -7, 106,  34,  -5  },
  { -8,  90,  53,  -7  },
  { -8,  72,  72,  -8  },
  { -7,  53,  90,  -8  },
  { -5,  34, 106,  -7  },
  { -2,  16, 118,  -4  } },
};
UINT32 u4VDEC_HAL_VP6_ShiftGetBitStream(UINT32 u4ShiftBits)
{
    UINT32 dRegVal;
  
    dRegVal = u4VDecVP6VLDGetBits(0, u4ShiftBits);
    
    return(dRegVal);
}

UINT32 u4VDEC_HAL_VP6_GetBitStreamShift(UINT32 u4ShiftBits)
{
    UINT32 u4RegVal0;
  
    u4RegVal0 = u4VDecVP6VLDGetBits(0, 0);
    u4VDecVP6VLDGetBits(0, u4ShiftBits);
    
    return(u4RegVal0);
}

UINT32 u4VDEC_HAL_VP6_ShiftGetBitStreamEx(UINT32 u4Idx,UINT32 u4ShiftBits)
{
    UINT32 dRegVal;
  
    dRegVal = u4VDecVP6VLDGetBits(u4Idx, u4ShiftBits);
    
    return(dRegVal);
}

UINT32 u4VDEC_HAL_VP6_GetBitStreamShiftEx(UINT32 u4Idx,UINT32 u4ShiftBits)
{
    UINT32 u4RegVal0;
  
    u4RegVal0 = u4VDecVP6VLDGetBits(u4Idx, 0);
    u4VDecVP6VLDGetBits(u4Idx, u4ShiftBits);
    return(u4RegVal0);
}

BOOL i4VDEC_HAL_VP6_InitBarrelShifter(UINT32 u4Id, UINT32 u4ReadPointer, UINT32 u4WritePointer, VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr)
{
    INT32 i, u4VLDRemainByte, u4Cnt = 0;

    if (u4VDecReadVLDEx(u4Id, RO_VLD_SRAMCTRL) & PROCESS_FLAG)
    {
        while((u4VDecReadVLDEx(u4Id, RO_VLD_SRAMCTRL) & AA_FIT_TARGET_SCLK) == 0)
        {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                LOG(1, "WaitSramStable Fail\n");
                break;
            }
        }
    }

    u4VLDRemainByte =  u4ReadPointer & 0xf;

    vVDecWriteVLDEx(u4Id, RW_VLD_RPTR, PHYSICAL((UINT32) u4ReadPointer));
    _VDEC_SetVP6VldWPtr(u4Id, u4WritePointer);

    vVDecWriteVLDEx(u4Id, WO_VLD_SRST, 0x100);
    vVDecWriteVLDEx(u4Id, WO_VLD_SRST, 0x0);

    // start to fetch data
    vVDecWriteVLDEx(u4Id, RW_VLD_PROC, VLD_INIFET);
    
    if (!fgVDecWaitVldFetchOk(u4Id))
    {
        LOG(1, "Init fetch NG\n");
        return FALSE;
    }

    vVDecWriteVLDEx(u4Id, RW_VLD_PROC, VLD_INIBR);
    if (prFrmHdr)
    {
        UINT32 u4KeyFrame, u4Huffman, u4Multi;
        u4KeyFrame = (prFrmHdr->ucFrameType == VP6_I_FRM)? RW_VP6_KEY_FRM : 0;
        u4Huffman = (prFrmHdr->fgUseHuffman == TRUE) ? RW_VP6_HUFFMAN : 0;
        u4Multi = (prFrmHdr->fgMultiStream == TRUE) ? RW_VP6_MULTI : 0;
        vVDecWriteVP6VLD(RW_VP6_PCI_PAR, (u4KeyFrame | u4Huffman | u4Multi | RW_VP6_FLAG));
    }
    else
    {
        vVDecWriteVP6VLD(RW_VP6_PCI_PAR, RW_VP6_FLAG);
    }


    for (i=0;i<u4VLDRemainByte;i++)
    {
        u4VDecVP6VLDGetBits(u4Id, 8);    
    }
    return TRUE;
}

UINT32 u4VDEC_HAL_VP6_ReadRdPtr(UINT32 *pu4Bits)
{
    if (!pu4Bits)
    {
        return 0;
    }
    
    return u4VDecReadVP6VldRPtr(pu4Bits);
}

void vVDEC_HAL_VP6_GetMbxMby(UINT32 *pu4Mbx, UINT32 *pu4Mby)
{
    if (!pu4Mbx || !pu4Mby)
    {
        return;
    }
    
    *pu4Mbx = u4VDecReadMC(RO_MC_MBX);
    *pu4Mby = u4VDecReadMC(RO_MC_MBY);
}

static const UINT8 vp6_huff_coeff_map[] = {
    13, 14, 11, 0, 1, 15, 16, 18, 2, 17, 3, 4, 19, 20, 5, 6, 21, 22, 7, 8, 9, 10
};
static const UINT8 vp6_huff_run_map[] = {
    10, 13, 11, 12, 0, 1, 2, 3, 14, 8, 15, 16, 4, 5, 6, 7
};
static const HUFF_CODE eob_huff_code[] = {
    {0x0, 0x2}, {0x400, 0x2}, {0x800, 0x4}, {0x900, 0x4}, {0xa00, 0x4}, {0xb00, 0x4},
    {0xc00, 0x5}, {0xc80, 0x5}, {0xd00, 0x5}, {0xd80, 0x5}, {0xe00, 0x3}, {0x0, 0x0}};

static void vBoolTreeToHuffCodes(UINT8 *coeff_model,
                                const UINT8 *map, UINT32 size, UINT32 *huff_probs)
{
    int a, b, i;
	UINT32 *tmp;

    if (!coeff_model || !map || !huff_probs)
    {
        return;
    }

    tmp = &huff_probs[size];
    
    /* first compute probabilities from model */
    tmp[0] = 256;
    for (i = 0; i < size - 1; i++) 
    {
        a = tmp[i] *        coeff_model[i]  >> 8;
        b = tmp[i] * (255 - coeff_model[i]) >> 8;
        huff_probs[map[2*i  ]] = a + !a;
        huff_probs[map[2*i+1]] = b + !b;
    }
}

static void vInsertSorted (SORTNODE *sn, INT32 node, INT32 *startnode)
{
    INT32 which;
    INT32 prior;

    if (!sn || !startnode)
    {
        return;
    }

    which = *startnode;
    prior = *startnode;

    // find the position at which to insert the node
    while ((which != -1) && (sn[node].freq > sn[which].freq))
    {
        prior = which;
        which = sn[which].next;
    }

    if(which == *startnode)
    {
        *startnode = node;
        sn[node].next = which;
    }
    else
    {
        sn[prior].next = node;
        sn[node].next = which;
    }
}

UINT16 get_huff_code(HUFF_NODE *huffNode, UINT16 *pcode, UINT16 *len)
{
    UINT16 *hn;
    UINT16 len_cnt = 0;
    UINT16 torp = 0;
    UINT16 hcode = 0;
    UINT16 bit;

    if (!huffNode || !pcode || !len)
    {
        return 0;
    }

    hn = (UINT16 *)huffNode;
    
    do
    {
        bit = ((*pcode) & (1 << (*len - len_cnt - 1))) ? 1 : 0;
        hcode = (hcode << 1) | bit;
        torp =  torp + bit;
        torp = hn[torp];
        len_cnt ++;
    }
    while (!(torp & 1));
    
    *len    = len_cnt;
    *pcode  = hcode;
    
    return torp;
}

static INT32 _pow(INT32 i4Base, INT32 i4Exp)
{
    INT32 val, i;

    if(i4Exp == 0)
    {
        return 1;
    }

    val= i4Base;
    for(i=1; i<i4Exp; i++)
    {
        val *= i4Base;
    }

    return val;
}

static void vp6_build_huff_tree(HUFF_NODE *hn, UINT32 *counts, INT32 values, HUFF_CODE *pcode)
{
    INT32 i;
    INT32 sncount   = 0;
    INT32 startnode = 0;

    // NOTE:
    // Create huffman tree in reverse order so that the root will always be 0
    INT32 huffptr = values - 1;
    
    if (!hn || !counts || !pcode)
    {
        return;
    }
    
    // Set up sorted linked list of values/pointers into the huffman tree
    for (i = 0; i < values; i++)
    {
        sn[i].value = (i << 1) | 1;
        if (counts[i] == 0)
        {
            counts[i] = 1;
        }
        sn[i].freq = counts[i];
        sn[i].next = -1;
    }

    sncount = values;

    // Connect above list into a linked list
    for (i = 1; i < values; i++)
    {
        vInsertSorted (sn, i, &startnode);
    }

    // while there is more than one node in our linked list
    while (sn[startnode].next != -1)
    {
        int first   = startnode;
        int second  = sn[startnode].next;
        int sumfreq = sn[first].freq + sn[second].freq;

        // set-up new merged huffman node
        --huffptr;
        // setup new merged huffman node
        hn[huffptr].left    = sn[first].value;
        hn[huffptr].right   = sn[second].value;

        // set up new merged sort node pointing to our huffnode
        sn[sncount].value   = (huffptr << 1) | 0;
        sn[sncount].freq    = sumfreq;
        sn[sncount].next    = -1;


        // remove the two nodes we just merged from the linked list
        startnode = sn[second].next;

        // insert the new sort node into the proper location
        vInsertSorted(sn, sncount, &startnode);

        // account for new nodes
        sncount++;
    }
    
    { 
        unsigned short max_len = values;
        unsigned short len = values;
        int stop = 0;
        unsigned short hcode = 0;
        unsigned short torp;
        
        do
        {            
            torp = get_huff_code(hn, &hcode, &len);
            if(torp & 1)
            {
                pcode[(torp>>1)].hcode  = hcode << (12 - len);
                pcode[(torp>>1)].len    = len;
                if(hcode == (_pow(2,len) - 1))
                {
                    stop = 1;
                }
                hcode ++;
            }
            else
            {
                hcode = hcode << (max_len - len);
                len = max_len;
            }
        }
        while(stop != 1);
    }

    return;
}

static VOID u4VDec_HAL_VP6_Patch(UINT32 u4VDecID, VDEC_VP6_INFO_T *prDecInfo)
{
    UINT32 i, coef_reorder, reg_153, tmp, u4BSID, reg_35;
    UINT32 u4Loop, u4Remain;

    UINT8 mv_prob[34]= {
         237, 246,
         231, 243,
         253, 253, 254, 254, 254, 254, 254,
         245, 253, 254, 254, 254, 254, 254,
         254, 254, 254, 254, 254, 250, 250, 252,
         254, 254, 254, 254, 254, 251, 251, 254
    };

    UINT8 reorder_prob[64] =  {
        255, 132, 132, 159, 153, 151, 161, 170,
        164, 162, 136, 110, 103, 114, 129, 118,
        124, 125, 132, 136, 114, 110, 142, 135,
        134, 123, 143, 126, 153, 183, 166, 161,
        171, 180, 179, 164, 203, 218, 225, 217,
        215, 206, 203, 217, 229, 241, 248, 243,
        253, 255, 253, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
    };

    UINT8 dc_prob[22] = {
        146, 255, 181, 207, 232, 243, 238, 251, 244, 250, 249,
        179, 255, 214, 240, 250, 255, 244, 255, 255, 255, 255,
    };

    u4BSID = 0;
    if (!prDecInfo->rDecParam[u4VDecID].fgKeyFrame)
    {
        //UTIL_Printf("<vdec> // Trigger Mode Prob. Update, input window=%08x\n", u4VDecReadVP6VLD(u4VDecID, 60*4));
        vVDecWriteVP6VLDEx(u4VDecID, 75*4, 0x1); // only need MT8555 & MT8580A; MT8580B can be removed or not.
        vVDecWriteVP6VLDEx(u4VDecID, 39*4, 0x1);
        while ((u4VDecReadVP6VLDEx(u4VDecID, 66*4) & 0xFFFFF) != 0x1)
        { //x_thread_delay(1);
        }
		if(BSP_GetIcVersion()==IC_VER_5396_AA)
		{
			//---------- idx = 0 ----------			
			vVDecWriteVP6VLD(4*50, 0x348);
			vVDecWriteVP6VLD(4*51, 0x09200709);
			vVDecWriteVP6VLD(4*52, 0x010e030e);
			vVDecWriteVP6VLD(4*53, 0x03000000);
			vVDecWriteVP6VLD(4*54, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteVP6VLD(4*50, 0x349);
			vVDecWriteVP6VLD(4*51, 0x0f191315);
			vVDecWriteVP6VLD(4*52, 0x0c0c1217);
			vVDecWriteVP6VLD(4*53, 0x0a040000);
			vVDecWriteVP6VLD(4*54, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteVP6VLD(4*50, 0x34a);
			vVDecWriteVP6VLD(4*51, 0x29010100);
			vVDecWriteVP6VLD(4*52, 0x00000100);
			vVDecWriteVP6VLD(4*53, 0x62040000);
			vVDecWriteVP6VLD(4*54, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteVP6VLD(4*50, 0x34b);
			vVDecWriteVP6VLD(4*51, 0x16001f00);
			vVDecWriteVP6VLD(4*52, 0x00010701);
			vVDecWriteVP6VLD(4*53, 0x190a0000);
			vVDecWriteVP6VLD(4*54, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteVP6VLD(4*50, 0x34c);
			vVDecWriteVP6VLD(4*51, 0x02020000);
			vVDecWriteVP6VLD(4*52, 0x000b0100);
			vVDecWriteVP6VLD(4*53, 0x03000000);
			vVDecWriteVP6VLD(4*54, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteVP6VLD(4*50, 0x34d);
			vVDecWriteVP6VLD(4*51, 0x03030202);
			vVDecWriteVP6VLD(4*52, 0x00040402);
			vVDecWriteVP6VLD(4*53, 0x02040000);
			vVDecWriteVP6VLD(4*54, 0x0); 

		}
		else
		{
		    
            vVDecWriteMC(4*591, 0x10d23);
			vVDecWriteMC(4*592, 0x09200709);
			vVDecWriteMC(4*591, 0x10d22);
			vVDecWriteMC(4*592, 0x010e030e);
			vVDecWriteMC(4*591, 0x10d21);
			vVDecWriteMC(4*592, 0x03000000);
			vVDecWriteMC(4*591, 0x10d20);
			vVDecWriteMC(4*592, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteMC(4*591, 0x10d27);
			vVDecWriteMC(4*592, 0x0f191315);
			vVDecWriteMC(4*591, 0x10d26);
			vVDecWriteMC(4*592, 0x0c0c1217);
			vVDecWriteMC(4*591, 0x10d25);
			vVDecWriteMC(4*592, 0x0a040000);
			vVDecWriteMC(4*591, 0x10d24);
			vVDecWriteMC(4*592, 0x0);
				//---------- idx = 0 ----------
		    vVDecWriteMC(4*591, 0x10d2b);
			vVDecWriteMC(4*592, 0x29010100);
			vVDecWriteMC(4*591, 0x10d2a);
			vVDecWriteMC(4*592, 0x00000100);
			vVDecWriteMC(4*591, 0x10d29);
			vVDecWriteMC(4*592, 0x62040000);
			vVDecWriteMC(4*591, 0x10d28);
			vVDecWriteMC(4*592, 0x0);
				//---------- idx = 0 ----------
			vVDecWriteMC(4*591, 0x10d2f);
			vVDecWriteMC(4*592, 0x16001f00);
			vVDecWriteMC(4*591, 0x10d2e);
			vVDecWriteMC(4*592, 0x00010701);
			vVDecWriteMC(4*591, 0x10d2d);
			vVDecWriteMC(4*592, 0x190a0000);
			vVDecWriteMC(4*591, 0x10d2c);
			vVDecWriteMC(4*592, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteMC(4*591, 0x10d33);
			vVDecWriteMC(4*592, 0x02020000);
			vVDecWriteMC(4*591, 0x10d32);
			vVDecWriteMC(4*592, 0x000b0100);
			vVDecWriteMC(4*591, 0x10d31);
			vVDecWriteMC(4*592, 0x03000000);
			vVDecWriteMC(4*591, 0x10d30);
			vVDecWriteMC(4*592, 0x0);
			//---------- idx = 0 ----------
			vVDecWriteMC(4*591, 0x10d37);
			vVDecWriteMC(4*592, 0x03030202);
			vVDecWriteMC(4*591, 0x10d36);
			vVDecWriteMC(4*592, 0x00040402);
			vVDecWriteMC(4*591, 0x10d35);
			vVDecWriteMC(4*592, 0x02040000);
			vVDecWriteMC(4*591, 0x10d34);
			vVDecWriteMC(4*592, 0x0);         
		}

        //UTIL_Printf("<vdec> // MV Tree Prob. update, input window=%08x\n",u4VDecReadVP6VLD(u4VDecID, 60*4));
        for(i = 0; i<34; i++)
        {
            vVDecWriteVP6VLDEx(u4VDecID, 34*4, ((1 << 16) + ( ((UINT32)mv_prob[i]) << 8) + (1)));
            while ((u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x1) == 0)
            {//x_thread_delay(1);
            }

            if(u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x100)
            {
                vVDecWriteVP6VLDEx(u4VDecID, 34*4, 7);
                while ((u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x1) == 0)
                {//x_thread_delay(1);
                }
            }
            //UTIL_Printf("<vdec> MV TRee prob %d, input window=%08x\n",i,u4VDecReadVP6VLD(u4VDecID, 60*4));
        }
    }


    //UTIL_Printf("<vdec> // DC Prob. update\n");
    for(i = 0; i<22; i++)
    {
        vVDecWriteVP6VLDEx(u4VDecID, 34*4, ((1 << 16) + ( ((UINT32)dc_prob[i]) << 8) + (1)));
        while ((u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x1) == 0)
        {//x_thread_delay(1);
        }

        if(u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x100)
        {
            vVDecWriteVP6VLDEx(u4VDecID, 34*4, 7);
            while ((u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x1) == 0)
            {//x_thread_delay(1);
            };
        }
        //UTIL_Printf("<vdec> DC prob %d, input window=%08x\n",i,u4VDecReadVP6VLD(u4VDecID, 60*4));        
    }

    //UTIL_Printf("<vdec> // Scan Update bit\n");
    vVDecWriteVP6VLDEx(u4VDecID, 34*4, 1);
    while ((u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x1) == 0)
    {//x_thread_delay(1);
    }

    //UTIL_Printf("<vdec> Scan Update, input window=%08x\n",u4VDecReadVP6VLD(u4VDecID, 60*4));

    //UTIL_Printf("<vdec> // 63 coef. scan order update\n");
    if(u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x100)
    {
        for(i = 1; i < 64; i++)
        {
            vVDecWriteVP6VLDEx(u4VDecID, 34*4, ((1 << 16) + ( ((UINT32)reorder_prob[i]) << 8) + (1)));
            while ((u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x1) == 0)
            {//x_thread_delay(1);
            }

            if(u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x100)
            {
                vVDecWriteVP6VLDEx(u4VDecID, 34*4, 4);
                while ((u4VDecReadVP6VLDEx(u4VDecID, 37*4) & 0x1) == 0)
                {//x_thread_delay(1);
                }

                coef_reorder = (u4VDecReadVP6VLDEx(u4VDecID, 37*4) >> 8) & 0xFF;

                vVDecWriteVLDEx(u4VDecID, 152*4, 0x100|((i>>2)<<2));
                reg_153 = u4VDecReadVLDEx(u4VDecID, 153*4);

                tmp = 0xFFFFFFFF ^ (0x000000FF << (8*(3-i%4)));
                reg_153 = (reg_153 & tmp) | (coef_reorder << (8*(3-i%4)));
                vVDecWriteVLDEx(u4VDecID, 152*4, (i>>2)<<2);
                vVDecWriteVLDEx(u4VDecID, 153*4, reg_153);
                //UTIL_Printf("<vdec> reg_152 = %08x, reg_153 = %08x\n",(i>>2)<<2,reg_153);
            }

            //UTIL_Printf("<vdec> coef %d, input window=%08x\n",i,u4VDecReadVP6VLD(u4VDecID, 60*4));            
        }
    }

    // poll back dram_sram_ctrl rpt to frame header
    reg_35 = u4VDecReadVP6VLDEx(u4VDecID, 35*4);
	
    _VDEC_SetVP6BsInfo(prDecInfo->uEsId, prDecInfo->rPesInfo.u4VldReadPtr, prDecInfo->rDecParam[u4VDecID].u4WPtr, 
		prDecInfo->rPesInfo.u4FifoStart, prDecInfo->rPesInfo.u4FifoEnd, NULL);
    //i4VDEC_HAL_VP6_InitBarrelShifter(u4BSID, u4VDecID, prVp6BSInitPrm);    
    vVDecWriteVP6VLDEx(u4VDecID, RW_VP6_PCI_PAR + (u4BSID << 10), reg_35);

    // shift bit
    u4Loop = (prDecInfo->rDecParam[u4VDecID].u4DecReadBytes*8) / 32;
    u4Remain = (prDecInfo->rDecParam[u4VDecID].u4DecReadBytes*8)  % 32;
    //UTIL_Printf("<vdec> shit_bits_cnt =%d\n",prFrmHdr->u4ShiftBitsCnt);
    for (i = 0; i < u4Loop; i++)
    {
        u4VDEC_HAL_VP6_GetBitStreamShiftEx(u4VDecID, 32);
    }
    if (u4Remain)
    {
        u4VDEC_HAL_VP6_GetBitStreamShiftEx(u4VDecID, u4Remain);
    }

    // initial range
    u4VDEC_HAL_VP6_InitBoolCoder(u4VDecID);

    // shift bool
    
    u4Loop = prDecInfo->rDecParam[u4VDecID].u4DecReadBits/7;
    u4Remain = prDecInfo->rDecParam[u4VDecID].u4DecReadBits%7;
    //UTIL_Printf("<vdec> bool_shit_bits_cnt =%d\n",prFrmHdr->u4BoolShiftBitsCnt);
    for (i = 0; i < u4Loop; i++)
    {
        u4VDEC_HAL_VP6_GetBoolCoderShift(7);
    }
    if (u4Remain)
    {
        u4VDEC_HAL_VP6_GetBoolCoderShift(u4Remain);
    }
}

static void vInitCtx(VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr)
{
    UINT32 i, j, u4Cnt, u4Addr, u4Addr2, u4Tmp;
    UINT32 coeff_model[4];

    if (!prFrmHdr)
    {
        return;
    }

    if (prFrmHdr->fgUseHuffman)
    {
        UINT32 huff_prob[23];
        HUFF_NODE huff_node[11];
        HUFF_CODE huff_code[12];
        UINT32 code0, code1;

        // Trigger Prob Update
        u4Cnt = 0;
        vVDecWriteVP6VLD(RW_VP6_VLD_PROB_UPD, (u4VDecReadVP6VLD(RW_VP6_VLD_PROB_UPD) | 0x1));
        while(u4Cnt++ < WAIT_THRD)
        {
            if ((u4VDecReadVP6VLD(RW_VP6_VLD_PROB_UPD) & 0x1) == 0x1)
            {
                break;
            }
        }
        ASSERT(u4Cnt < WAIT_THRD);

        // DC Coeff & Run Coeff
        for (j = 0; j < 4; j++)
        {
            for (i = 0; i < 4; i++)
            {
                u4Addr = MC_VLD_WRAPPER_READ | ((VLD_WRAPPER_DC_Y + j) << 2) | (3 - i);
                vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, u4Addr);
                u4Tmp = u4VDecReadMC(RW_MC_VLD_WRAPPER_DATA);
                coeff_model[i] =    ((u4Tmp & 0x000000ff) << 24) | 
                                    ((u4Tmp & 0x0000ff00) << 8)  | 
                                    ((u4Tmp & 0x00ff0000) >> 8)  | 
                                    ((u4Tmp & 0xff000000) >> 24);
            }
            x_memset(huff_node, 0, sizeof(huff_node));
            x_memset(huff_code, 0, sizeof(huff_code));
            x_memset(huff_prob, 0, sizeof(huff_prob));
            if (j < 2)
            {
                vBoolTreeToHuffCodes((UINT8*)coeff_model, vp6_huff_coeff_map, 12, huff_prob);
                vp6_build_huff_tree(huff_node, huff_prob, 12, huff_code);
            }
            else
            {
                vBoolTreeToHuffCodes((UINT8*)coeff_model, vp6_huff_run_map, 9, huff_prob);
                vp6_build_huff_tree(huff_node, huff_prob, 9, huff_code);
            }
            
            u4Addr = VP6_CTX_WARR_HUFFMAN | VP6_CTX_WARR_WRITE | (CTX_HUFF_DC_Y + j);
            vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Addr);
            for (i = 0; i < 6; i++)
            {
                code0 = (huff_code[2*i].len << 12)   | huff_code[2*i].hcode;
                code1 = (huff_code[2*i+1].len << 12) | huff_code[2*i+1].hcode;
                vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1 + 4 * i, ((code0<<16)|code1));
            }
        }

        // AC Coeff
        u4Addr2 = VP6_CTX_WARR_HUFFMAN | VP6_CTX_WARR_WRITE | (CTX_HUFF_AC_BASE);
        for (j = 0; j < 36; j++)
        {
            if ((j % 6 == 4) || (j % 6 == 5))
            {
                continue;
            }

            for (i = 0; i < 4; i++)
            {
                u4Addr = MC_VLD_WRAPPER_READ | ((VLD_WRAPPER_AC_BASE + j) << 2) | (3 - i);
                vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, u4Addr);
                u4Tmp = u4VDecReadMC(RW_MC_VLD_WRAPPER_DATA);
                coeff_model[i] =    ((u4Tmp & 0x000000ff) << 24) | 
                                    ((u4Tmp & 0x0000ff00) << 8)  | 
                                    ((u4Tmp & 0x00ff0000) >> 8)  | 
                                    ((u4Tmp & 0xff000000) >> 24);
            }
            x_memset(huff_node, 0, sizeof(huff_node));
            x_memset(huff_code, 0, sizeof(huff_code));
            x_memset(huff_prob, 0, sizeof(huff_prob));
            vBoolTreeToHuffCodes((UINT8*)coeff_model, vp6_huff_coeff_map, 12, huff_prob);
            vp6_build_huff_tree(huff_node, huff_prob, 12, huff_code);
            
            vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Addr2);
            for (i = 0; i < 6; i++)
            {
                code0 = (huff_code[2*i].len << 12)   | huff_code[2*i].hcode;
                code1 = (huff_code[2*i+1].len << 12) | huff_code[2*i+1].hcode;
                vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1 + 4 * i, ((code0<<16)|code1));
            }
            u4Addr2++;
        }

        // EOB Run
        u4Addr = VP6_CTX_WARR_HUFFMAN | VP6_CTX_WARR_WRITE | (CTX_HUFF_EOB_RUN);
        vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Addr);
        for (i = 0; i < 6; i++)
        {
            code0 = (eob_huff_code[2*i].len << 12)   | eob_huff_code[2*i].hcode;
            code1 = (eob_huff_code[2*i+1].len << 12) | eob_huff_code[2*i+1].hcode;
            vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1 + 4 * i, ((code0<<16)|code1));
        }
    }
    else
    {
        for (i = 0; i < 40; i++)
        {
            for (j = 0; j < 4; j++)
            {
                u4Addr = MC_VLD_WRAPPER_READ | ((VLD_WRAPPER_DC_Y + i) << 2) | (3 - j);
                vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, u4Addr);
                coeff_model[j] = u4VDecReadMC(RW_MC_VLD_WRAPPER_DATA);
            }

            u4Addr = VP6_CTX_WARR_BOOL | VP6_CTX_WARR_WRITE | (CTX_BOOL_DC_Y + i);
            vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Addr);
            for (j = 0; j < 4; j++)
            {
                vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1 + 4 * j, coeff_model[j]);
            }
        }
    }
}

INT32 i4VDEC_HAL_VP6_DecStart(VDEC_VP6_INFO_T *prDecInfo)
{    
#ifdef VDEC_DDR3_SUPPORT
    UINT32 u4DDR3_PicWdith;
    UINT32 aMc406, u4DramType;
#endif          
    UINT32 u4Vp68C, u4VertMb, u4HoriMb;
    UINT32 u4KeyFrame, u4Huffman, u4Multi;
    UINT32 u4Mc85c = 0;
    UINT32 u4Mc860, u4Mc864, u4Mc868;
    UINT32 u4TurnOffDeblock;
    UINT32 u4Pp200;
    UINT32 u4YSa, u4CSa;

    VP6_DEC_PARAM_T *prDecPrm;
    VDEC_INFO_DEC_PRM_T *prDecPrmEx;
    VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr;
    VDEC_INFO_VP6_PP_INFO_T *prPpInfo;

    if ((!prDecInfo) || (prDecInfo->u1StrmId >= MAX_STRM_NS))
    {
        return -1;
    }

    prDecPrm    = &prDecInfo->rDecParam[prDecInfo->u1StrmId];
    prFrmHdr    = &prDecInfo->rFrmHdr[prDecInfo->u1StrmId];
    prDecPrmEx  = &prDecInfo->rDecParamEx;
    prPpInfo    = &prDecInfo->rPpInfo;

    //set picture parameter
    u4VertMb    = prFrmHdr->u2VFragments;
    u4HoriMb    = prFrmHdr->u2HFragments;
    u4KeyFrame  = (prFrmHdr->ucFrameType ==VP6_I_FRM)? RW_VP6_KEY_FRM : 0;
    u4Huffman   = (prFrmHdr->fgUseHuffman == TRUE) ? RW_VP6_HUFFMAN : 0;
    u4Multi     = (prFrmHdr->fgMultiStream == TRUE) ? RW_VP6_MULTI : 0;
    u4Vp68C     = (u4KeyFrame | u4Huffman | u4Multi | RW_VP6_FLAG | RW_VP6_RTN_ERR);
    vVDecWriteVP6VLD(RW_VP6_PCI_PAR, u4Vp68C);

    vVDecWriteVLDTop(RW_PIC_MB_SIZE_M1, (((u4HoriMb-1)&0x3FF) | (((u4VertMb-1)&0x3FF) << 16)));
    vVDecWriteVLDTop(RW_PIC_PIX_SIZE, (((u4HoriMb << 4)&0x3FFF) | (((u4VertMb << 4)&0x3FFF) << 16)));

    //Write IQ_QUANT
    u4Pp200 = ((prFrmHdr->u4DQuant_Ac << 8) | prFrmHdr->u4DQuant_Dc);
    vVDecWriteVP6PP(RW_VP6_QUANT_REG, u4Pp200);

    u4TurnOffDeblock = (prFrmHdr->u2LoopFilter == 0)? 1 : 0;
    u4Mc860 = ((u4TurnOffDeblock << 8)   | (prFrmHdr->u2Vp56_Filter_Threshold));
    vVDecWriteMC(RW_VP6_LOOPDBK, u4Mc860);

    if (prFrmHdr->ucFilter_Mode != 2)
    {
        if (prFrmHdr->ucFilter_Mode == 0)
        {
            vVDecWriteMC(RW_VP6_AUTOSEL, (0x1 << 28));
        }
        else
        {
            vVDecWriteMC(RW_VP6_AUTOSEL, (0x0 << 28));
        }
    }
    else
    {
        u4Mc868 = ((prFrmHdr->u4Var_Thr_En << 20) | (prFrmHdr->u4Sample_Variance_Threshold) );
        vVDecWriteMC(RW_VP6_AUTOSEL, u4Mc868);

        u4Mc864 = ((prFrmHdr->u4Mv_Thr_En << 16)   | (prFrmHdr->u4Max_Vector_Length));
        vVDecWriteMC(RW_VP6_MV_TH, u4Mc864);
    }
	
	if(VP6_ISSET(prDecInfo->rDecParamEx.u4Flag,VP6_DEC_FLG_SUPPORT_MULTI))
	{
		vVDecWriteVP6VLD(RW_VP6_VLD_ETCB, ETC_COEFF_ERROR|ETC_MC_TIMEOUT|ETC_MV_TIMEOUT|ETC_DCAC_TIMEOUT|ETC_HUFF_ERR);
	}
	else
	{
		vVDecWriteVP6VLD(RW_VP6_VLD_ETCA, ETC_COEFF_ERROR|ETC_MC_TIMEOUT|ETC_MV_TIMEOUT|ETC_DCAC_TIMEOUT|ETC_HUFF_ERR);
	}

#ifdef ERROR_TYPE_TEST
    vVDecWriteVP6VLD(RW_DCAC_TIMEOUT_THD,   0x1);
    vVDecWriteVP6VLD(RW_MV_TIMEOUT_THD,     0x1);
    vVDecWriteVP6VLD(RW_MC_TMIEOUT_THD,     0x1);
#endif

    //-------------------------------------------------
    // global setting 只要一開始設一次即可
    //-------------------------------------------------

    //Previoud Frame Buffer 
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucLastFbId, &u4YSa, &u4CSa);
    vVDecWriteMC(RW_MC_R1Y, PHYSICAL(u4YSa) >> 9); // div 512
    vVDecWriteMC(RW_MC_R1C, PHYSICAL(u4CSa) >> 8); // div 256
    
    //Golden Frame Buffer 
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucGoldenFbId, &u4YSa, &u4CSa);
    vVDecWriteMC(RW_MC_R2Y, PHYSICAL(u4YSa) >> 9); // div 512
    vVDecWriteMC(RW_MC_R2C, PHYSICAL(u4CSa) >> 8); // div 256

    //Current Frame Buffer
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucCurFbId, &u4YSa, &u4CSa);
    vVDecWriteMC(RW_MC_BY1, PHYSICAL(u4YSa) >> 9); // div 512
    vVDecWriteMC(RW_MC_BC1, PHYSICAL(u4CSa) >> 8); // div 256

#ifdef VP6_ENABLE_ERROR_CONCEALMENT
    u4Vp68C = (u4KeyFrame | u4Huffman | u4Multi | RW_VP6_FLAG);                    // concealment
    //u4Vp68C = (u4KeyFrame | u4Huffman | u4Multi | RW_VP6_FLAG | RW_VP6_DAEC);    // disable error concealment
    //u4Vp68C = (u4KeyFrame | u4Huffman | u4Multi | RW_VP6_FLAG | RW_VP6_EERE);    // re-concealment
    vVDecWriteVP6VLD(RW_VP6_PCI_PAR, u4Vp68C);

    #ifdef CC_VP6_EMULATION
    HalFlushInvalidateDCache();
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucCurFbId, &u4YSa, &u4CSa);
    x_memset(u4YSa, 0, (u4VertMb * 16)*(u4HoriMb * 16));
    x_memset(u4CSa, 0, ((u4VertMb * 16)*(u4HoriMb * 16))/2);
    HalFlushInvalidateDCache();
    #endif
#endif

    // addr swap mode
    vVDecWriteMC(RW_MC_ADDRSWAP, prDecPrmEx->u4AddrMode);

#ifdef VP6_64X32_BLK_MODE
    vVDecWriteMC(RW_MC_PITCH, (((prDecPrmEx->u4PicBW + 63)>> 6) << 2) & 0x1FF);
#else
    vVDecWriteMC(RW_MC_PITCH, ((prDecPrmEx->u4PicBW + 15)>> 4) & 0x1FF);
#endif 

    vVDecWriteVLD(RW_VLD_MBDRAM_SEL, u4VDecReadVLD(RW_VLD_MBDRAM_SEL) | (1 << 16));
    //------------------------------
    //   MC RISC WRITE PATTERN
    //------------------------------
    //----------------------------------------------------------
    // MC_reg_9 : MC output buffer setting
    //            (0:ref_pic1_buf  1:ref_pic2_buf  4:prg_B_pic1)
    //----------------------------------------------------------
    //I, P

    vVDecWriteMC(RW_MC_OPBUF, 4);

    vVDecWriteMC(RW_MC_UMV_PIC_WIDTH, prFrmHdr->u2WidthDec);
    vVDecWriteMC(RW_MC_UMV_PIC_HEIGHT, prFrmHdr->u2HeightDec);

    if (prFrmHdr->u2WidthDec > 1920)
    {
        vVDecWriteMC(RW_MC_BYPASS_DBLK_WRAP, 1);
    }
    else
    {
        vVDecWriteMC(RW_MC_BYPASS_DBLK_WRAP, 0);
    }

    if (prPpInfo->fgPpEnable)
    {                
        vVDecWriteMC(RW_MC_PP_ENABLE, 1);
        vVDecWriteMC(RW_MC_PP_WB_BY_POST, 0);

        vVDecWriteMC(RW_MC_PP_Y_ADDR, PHYSICAL(prPpInfo->u4PpYBufSa) >> 9);
        vVDecWriteMC(RW_MC_PP_C_ADDR, PHYSICAL(prPpInfo->u4PpCBufSa) >> 8);

#ifdef VP6_64X32_BLK_MODE
        vVDecWriteMC(RW_MC_PP_MB_WIDTH, (((prDecPrmEx->u4PicW + 63) >> 6) << 2));
#else
        vVDecWriteMC(RW_MC_PP_MB_WIDTH, (prDecPrmEx->u4PicW + 15) >> 4);
#endif

        vVDecWriteMC(RW_MC_PP_QP_TYPE, 0x00000114); // set qp value to 0x14
        vVDecWriteMC(RW_MC_PP_DBLK_MODE, DBLK_Y+DBLK_C);
        vVDecWriteMC(RW_MC_PP_X_RANGE, ((prDecPrmEx->u4PicW + 15) >> 4) - 1);
        vVDecWriteMC(RW_MC_PP_Y_RANGE, (((prDecPrmEx->u4PicH + 15) >> 4) >> (prDecPrmEx->ucPicStruct != FRM_PIC)) - 1);
    }
    else
    {
        vVDecWriteMC(RW_MC_PP_ENABLE, 0);
        //------------------------------------------
        // MC_reg_139 : picture width in MB
        //------------------------------------------
        vVDecWriteMC(RW_MC_PP_MB_WIDTH, prFrmHdr->u2HFragments - 1); 
    }

    if (prDecPrmEx->fgAdobeMode)
    {
        u4Mc85c = (VP6_ADOBE_EN | VP6_2FLIMIT_ZERO | VP6_CBCR_MAKE_RND);
        vVDecWriteMC(RW_VP6_BKDOOR, u4Mc85c);
    }

    vVDecWriteMC(0x5E4, (u4VDecReadMC(0x5E4) |(0x1 <<12)));

    //Set NBM address swap mode
    vVDecWriteMC(RW_MC_NBM_CTRL, ((u4VDecReadMC(RW_MC_NBM_CTRL) & 0xFFFFFFF8) | prDecPrmEx->u4AddrMode));

#ifdef VDEC_DDR3_SUPPORT
    #ifndef VDEC_FORCE_DDR3
    u4DramType = DRVCUST_InitGet(eDramType);
    if(u4DramType == DDR_III_x2)
    #endif
    {
        #ifdef VDEC_VP6_HWDEBLOCK
        vVDecWriteMC(RW_MC_PP_WB_BY_POST, 0);
        FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucDbkFbId, &u4YSa, &u4CSa);
        #else
        vVDecWriteMC(RW_MC_PP_WB_BY_POST, 1);
        FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucCurFbId, &u4YSa, &u4CSa);
        #endif
    
        vVDecWriteMC(RW_MC_PP_Y_ADDR, PHYSICAL(u4YSa) >> 9); // div 512
        vVDecWriteMC(RW_MC_PP_C_ADDR, PHYSICAL(u4CSa) >> 8); // div 256
    
        u4DDR3_PicWdith = (((prDecPrmEx->u4PicBW + 63) >> 6) << 2);
        vVDecWriteMC(RW_MC_PP_MB_WIDTH, u4DDR3_PicWdith);  
        vVDecWriteMC(RW_MC_PITCH, u4DDR3_PicWdith & 0x1FF);
    
        vVDecWriteMC(RW_MC_PP_X_RANGE, ((prDecPrmEx->u4PicW + 15) >> 4) - 1);
        vVDecWriteMC(RW_MC_PP_Y_RANGE, (((prDecPrmEx->u4PicH + 15) >> 4) >> (prDecPrmEx->ucPicStruct != FRM_PIC)) - 1);
    
        //Turn on DDR3 mode
        vVDecWriteMC(RW_MC_DDR_CTRL0, ((u4VDecReadMC(RW_MC_DDR_CTRL0)  & 0xFFFFFFFE) |0x1));
        vVDecWriteMC(RW_MC_DDR_CTRL1, ((u4VDecReadMC(RW_MC_DDR_CTRL1)  & 0xFFFFFFFE) |0x1));
    
        aMc406 = u4VDecReadMC(RW_MC_PP_DBK_FLR);
        aMc406 &= 0xFFFFFFEF;
        vVDecWriteMC(RW_MC_PP_DBK_FLR, aMc406);
    
        //Turn-on DDR3, Set 0x834[0] = 0
        vVDecWriteMC(RW_MC_DDR3_EN, (u4VDecReadMC(RW_MC_DDR3_EN)  & 0xFFFFFFFE));
    
        //DDR3 Output Selector:
        //a. MC Only:  RW_MC_PP_ENABLE = 0 && RW_MC_PP_WB_BY_POST = 0
        //b. MC+PP:    RW_MC_PP_ENABLE = 1 && RW_MC_PP_WB_BY_POST = 0
        //c. PP Only:   RW_MC_PP_ENABLE = 1 && RW_MC_PP_WB_BY_POST = 1
    
        //Post-process enable
        vVDecWriteMC(RW_MC_PP_ENABLE, (u4VDecReadMC(RW_MC_PP_ENABLE)  | 0x1));
    }
#endif

#ifdef VP6_GEN_CRC
    if (prPpInfo->fgPpEnable)
    {
        vVDecWriteVDECMisc(RW_VDEC_MISC_CRC_CON, CRC_CON_ENABLE|CRC_CON_AGENT_PP);
    }
    else
    {
        vVDecWriteVDECMisc(RW_VDEC_MISC_CRC_CON, CRC_CON_ENABLE|CRC_CON_AGENT_MC);
    }
#endif

    if (prFrmHdr->u2Buff2Offset || (prDecPrm->u1AlphaFlag & VP6_ALPHA_ENABLE))
    {
        vInitCtx(prFrmHdr);
    }

#ifdef VP6_MID_SUPPORT
    {
    VDEC_DISP_PIC_INFO_T rDispInfo = {0};

    #ifdef VP6_64X32_BLK_MODE
    UINT32 u4Width = ((prDecPrmEx->u4PicW + 63) >> 6) << 6;        
    #else
    UINT32 u4Width = ((prDecPrmEx->u4PicW + 15) >> 4) << 4;
    #endif
    UINT32 u4Height = ((prDecPrmEx->u4PicH + 31) >> 5) << 5;

    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucCurFbId, &u4YSa, &u4CSa);
    #ifdef VP6_MID_TEST_VIOLATION
    u4YSa += 0x800;
    u4CSa -= 0x800;
    #endif
    rDispInfo.pvYAddr = (void*)(u4YSa);
    rDispInfo.pvCAddr = (void*)(u4CSa);
    rDispInfo.u4YSize = u4Width * u4Height;

    #ifdef VDEC_VP6_HWDEBLOCK
    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->ucDbkFbId, &u4YSa, &u4CSa);
    rDispInfo.pvYDbkAddr = (void*)u4YSa;
    rDispInfo.pvCDbkAddr = (void*)u4CSa;
    #endif

    if(!_VDEC_MidSetting(ES0, &rDispInfo))
    {
        LOG(0, "VdecMid Fail~\n");
    }

    vVDecWriteMC(RW_MC_VDEC_LARB, (u4VDecReadMC(RW_MC_VDEC_LARB) & ~(0x80000000)));
    }
#else
    vVDecWriteMC(RW_MC_VDEC_LARB, (u4VDecReadMC(RW_MC_VDEC_LARB) & (0x80000000)));
#endif

#ifdef VDEC_TIME_PROFILE
    {
    extern void _VDEC_VP6TimeHwStart(VDEC_VP6_INFO_T *prDecInfo);
    _VDEC_VP6TimeHwStart(prDecInfo);
    }
#endif // VDEC_TIME_PROFILE
    if(0)
    {
       UINT32 u4InputIndow=0;
	   Printf("InputWindow(Before Workaround):\n");
	   u4InputIndow=u4VDEC_HAL_VP6_ShiftGetBitStreamEx(0,0);
	   Printf("VDL0:0X%X\n",u4InputIndow);
	   u4InputIndow=u4VDEC_HAL_VP6_ShiftGetBitStreamEx(1,0);
	   Printf("VDL1:0X%X\n",u4InputIndow);
    }

#if defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5368)
    if(prDecInfo->u1StrmId==0)
    {
       u4VDec_HAL_VP6_Patch(0,prDecInfo);
	   if(0)
	   {
		   UINT32 u4InputIndow=0;
		   Printf("InputWindow(After Workaround):\n");
		   u4InputIndow=u4VDEC_HAL_VP6_ShiftGetBitStreamEx(0,0);
		   Printf("VDL0:0X%X\n",u4InputIndow);
		   u4InputIndow=u4VDEC_HAL_VP6_ShiftGetBitStreamEx(1,0);
		   Printf("VDL1:0X%X\n",u4InputIndow);
	   }
    }
#endif
    vVDecWriteVP6VLD(RW_VLD_VP6DEC, VP6_DEC_START);

    return TRUE;
}

UINT32 u4VDEC_HAL_VP6_GetBoolCoderShift(UINT32 u4ShiftBits)
{
    UINT32 u4RegVal0;
  
    u4RegVal0 = u4VDecVP6BOOLGetBits(u4ShiftBits);
    
    return(u4RegVal0);
}


UINT32 u4VDEC_HAL_VP6_InitBoolCoder(UINT32 u4Id)
{  
    vVDecWriteVP6VLDEx(u4Id, RW_VP6_INIT_BOOL, RW_VP6_BOOL_EN);
    return 0;
}

void _VDEC_SetVP6BsInfo(UINT32 u4Id, UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr, UINT32 u4BsFifoStart, UINT32 u4BsFifoEnd, VDEC_INFO_VP6_FRM_HDR_T *prFrmHdr)
{
    vVDecSetVLDVFIFO(u4Id, PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd));

    while (!i4VDEC_HAL_VP6_InitBarrelShifter(u4Id, u4RDPtrAddr, u4WPtrAddr, prFrmHdr))
    {
        _VDEC_VP6VLDHWReset(u4Id);
    }
}

UINT32 u4VDEC_HAL_VP6_Default_Models_Init(BOOL fgSupportMult)
{
	UINT32 pred_range, mbt_range, mvd_range, cfm_range;
	UINT32 i, j, u4Val, u4Addr;
	UINT32 u4Tmp0, u4Tmp1, u4Tmp2, u4Tmp3;
	UCHAR vector_model_dct[2];
	UCHAR vector_model_sig[2];
    const UCHAR vp56_def_mb_types_stats[3][10][2] = 
    {
         { {  69, 42 }, {   1,  2 }, {  1,   7 }, {  44, 42 }, {  6, 22 },
            {   1,  3 }, {   0,  2 }, {  1,   5 }, {   0,  1 }, {  0,  0 }, },
         { { 229,  8 }, {   1,  1 }, {  0,   8 }, {   0,  0 }, {  0,  0 },
            {   1,  2 }, {   0,  1 }, {  0,   0 }, {   1,  1 }, {  0,  0 }, },
         { { 122, 35 }, {   1,  1 }, {  1,   6 }, {  46, 34 }, {  0,  0 },
            {   1,  2 }, {   0,  1 }, {  0,   1 }, {   1,  1 }, {  0,  0 }, },
    };
    
    const UCHAR vp6_def_pdv_vector_model[2][7] = 
    {    
    { 225, 146, 172, 147, 214,  39, 156 },   
    { 204, 170, 119, 235, 140, 230, 228 },
    };
    
    const UCHAR vp6_def_fdv_vector_model[2][8] = 
    {    
          { 247, 210, 135, 68, 138, 220, 239, 246 },
          { 244, 184, 201, 44, 173, 221, 239, 253 },
    };
    
    const UCHAR vp6_def_runv_coeff_model[2][14] = 
    {    { 198, 197, 196, 146, 198, 204, 169, 142, 130, 136, 149, 149, 191, 249 },
         { 135, 201, 181, 154,  98, 117, 132, 126, 146, 169, 184, 240, 246, 254 },
    };
    
    vector_model_dct[0] = 0xA2;
    vector_model_dct[1] = 0xA4;
    vector_model_sig[0] = 0x80;
    vector_model_sig[1] = 0x80;
    
    pred_range = 840;
	mbt_range = 102;
	mvd_range = 3;
	cfm_range = 40;

	if(fgSupportMult)
	{
		for (i=0; i<3; i++)
		{
			for (j=0; j<2; j++)
			{
				u4Addr = MC_VLD_WRAPPER_WRITE | ((pred_range+96+i*2+j) << 2);
				
				u4Tmp0 = vp56_def_mb_types_stats[i][0][j];
				u4Tmp1 = vp56_def_mb_types_stats[i][1][j];
				u4Tmp2 = vp56_def_mb_types_stats[i][2][j];
				u4Tmp3 = vp56_def_mb_types_stats[i][3][j];
				u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
				vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x3));
				vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
				u4Tmp0 = vp56_def_mb_types_stats[i][4][j];
				u4Tmp1 = vp56_def_mb_types_stats[i][5][j];
				u4Tmp2 = vp56_def_mb_types_stats[i][6][j];
				u4Tmp3 = vp56_def_mb_types_stats[i][7][j];
				u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
				vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x2));
				vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
				u4Tmp0 = vp56_def_mb_types_stats[i][8][j];
				u4Tmp1 = vp56_def_mb_types_stats[i][9][j];
				u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
				vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x1));
				vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
				
				u4Val = 0;
				vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x0));
				vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
			}
		}
		
		////////////////////
		u4Addr = MC_VLD_WRAPPER_WRITE | ((pred_range+mbt_range) << 2);
		
		u4Tmp0 = vector_model_dct[0];
		u4Tmp1 = vector_model_sig[0];
		u4Tmp2 = vector_model_dct[1];
		u4Tmp3 = vector_model_sig[1];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Val = 0;
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x2));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Val = 0;
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x1));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Val = 0;
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x0));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		////////////////////
		u4Addr = MC_VLD_WRAPPER_WRITE | ((pred_range+mbt_range+1) << 2);
		
		u4Tmp0 = vp6_def_pdv_vector_model[0][0];
		u4Tmp1 = vp6_def_pdv_vector_model[0][1];
		u4Tmp2 = vp6_def_pdv_vector_model[0][2];
		u4Tmp3 = vp6_def_pdv_vector_model[0][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_pdv_vector_model[0][4];
		u4Tmp1 = vp6_def_pdv_vector_model[0][5];
		u4Tmp2 = vp6_def_pdv_vector_model[0][6];
		u4Tmp3 = vp6_def_pdv_vector_model[1][0];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x2));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_pdv_vector_model[1][1];
		u4Tmp1 = vp6_def_pdv_vector_model[1][2];
		u4Tmp2 = vp6_def_pdv_vector_model[1][3];
		u4Tmp3 = vp6_def_pdv_vector_model[1][4];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x1));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_pdv_vector_model[1][5];
		u4Tmp1 = vp6_def_pdv_vector_model[1][6];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x0));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		////////////////////
		u4Addr = MC_VLD_WRAPPER_WRITE | ((pred_range+mbt_range+2) << 2);
		
		u4Tmp0 = vp6_def_fdv_vector_model[0][0];
		u4Tmp1 = vp6_def_fdv_vector_model[0][1];
		u4Tmp2 = vp6_def_fdv_vector_model[0][2];
		u4Tmp3 = vp6_def_fdv_vector_model[0][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_fdv_vector_model[0][4];
		u4Tmp1 = vp6_def_fdv_vector_model[0][5];
		u4Tmp2 = vp6_def_fdv_vector_model[0][6];
		u4Tmp3 = vp6_def_fdv_vector_model[0][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x2));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_fdv_vector_model[1][0];
		u4Tmp1 = vp6_def_fdv_vector_model[1][1];
		u4Tmp2 = vp6_def_fdv_vector_model[1][2];
		u4Tmp3 = vp6_def_fdv_vector_model[1][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x1));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_fdv_vector_model[1][4];
		u4Tmp1 = vp6_def_fdv_vector_model[1][5];
		u4Tmp2 = vp6_def_fdv_vector_model[1][6];
		u4Tmp3 = vp6_def_fdv_vector_model[1][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x0));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		////////////////////
		u4Addr = MC_VLD_WRAPPER_WRITE | ((pred_range+mbt_range+mvd_range+2) << 2);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][0];
		u4Tmp1 = vp6_def_runv_coeff_model[0][1];
		u4Tmp2 = vp6_def_runv_coeff_model[0][2];
		u4Tmp3 = vp6_def_runv_coeff_model[0][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][4];
		u4Tmp1 = vp6_def_runv_coeff_model[0][5];
		u4Tmp2 = vp6_def_runv_coeff_model[0][6];
		u4Tmp3 = vp6_def_runv_coeff_model[0][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x2));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][8];
		u4Tmp1 = vp6_def_runv_coeff_model[0][9];
		u4Tmp2 = vp6_def_runv_coeff_model[0][10];
		u4Tmp3 = vp6_def_runv_coeff_model[0][11];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x1));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][12];
		u4Tmp1 = vp6_def_runv_coeff_model[0][13];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x0));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		////////////////////
		u4Addr = MC_VLD_WRAPPER_WRITE | ((pred_range+mbt_range+mvd_range+3) << 2);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][0];
		u4Tmp1 = vp6_def_runv_coeff_model[1][1];
		u4Tmp2 = vp6_def_runv_coeff_model[1][2];
		u4Tmp3 = vp6_def_runv_coeff_model[1][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][4];
		u4Tmp1 = vp6_def_runv_coeff_model[1][5];
		u4Tmp2 = vp6_def_runv_coeff_model[1][6];
		u4Tmp3 = vp6_def_runv_coeff_model[1][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x2));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][8];
		u4Tmp1 = vp6_def_runv_coeff_model[1][9];
		u4Tmp2 = vp6_def_runv_coeff_model[1][10];
		u4Tmp3 = vp6_def_runv_coeff_model[1][11];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x1));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][12];
		u4Tmp1 = vp6_def_runv_coeff_model[1][13];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x0));
		vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);

	}
	else
	{
		for (i=0; i<3; i++)
		{
			for (j=0; j<2; j++)
			{
				u4Val = pred_range+96+i*2+j;
				vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Val);
		
				u4Tmp0 = vp56_def_mb_types_stats[i][0][j];
				u4Tmp1 = vp56_def_mb_types_stats[i][1][j];
				u4Tmp2 = vp56_def_mb_types_stats[i][2][j];
				u4Tmp3 = vp56_def_mb_types_stats[i][3][j];
				u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
				vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1, u4Val);
		
				u4Tmp0 = vp56_def_mb_types_stats[i][4][j];
				u4Tmp1 = vp56_def_mb_types_stats[i][5][j];
				u4Tmp2 = vp56_def_mb_types_stats[i][6][j];
				u4Tmp3 = vp56_def_mb_types_stats[i][7][j];
				u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));						
				vVDecWriteVP6VLD(RW_VP6_VLD_FCVR2, u4Val);
				
				u4Tmp0 = vp56_def_mb_types_stats[i][8][j];
				u4Tmp1 = vp56_def_mb_types_stats[i][9][j];
				u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));						
				vVDecWriteVP6VLD(RW_VP6_VLD_FCVR3, u4Val);
				
				u4Val = 0;
				vVDecWriteVP6VLD(RW_VP6_VLD_FCVR4, u4Val);
			}
		}
		
		u4Val = pred_range+mbt_range;
		vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Val);
		
		u4Tmp0 = vector_model_dct[0];
		u4Tmp1 = vector_model_sig[0];
		u4Tmp2 = vector_model_dct[1];
		u4Tmp3 = vector_model_sig[1];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1, u4Val);
		
		u4Val = 0;
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR2, u4Val);
		
		u4Val = 0;
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR3, u4Val);
		
		u4Val = 0;
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR4, u4Val);
		
		u4Val = pred_range+mbt_range+1;
		vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Val);
		
		u4Tmp0 = vp6_def_pdv_vector_model[0][0];
		u4Tmp1 = vp6_def_pdv_vector_model[0][1];
		u4Tmp2 = vp6_def_pdv_vector_model[0][2];
		u4Tmp3 = vp6_def_pdv_vector_model[0][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1, u4Val);
		
		u4Tmp0 = vp6_def_pdv_vector_model[0][4];
		u4Tmp1 = vp6_def_pdv_vector_model[0][5];
		u4Tmp2 = vp6_def_pdv_vector_model[0][6];
		u4Tmp3 = vp6_def_pdv_vector_model[1][0];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR2, u4Val);
		
		u4Tmp0 = vp6_def_pdv_vector_model[1][1];
		u4Tmp1 = vp6_def_pdv_vector_model[1][2];
		u4Tmp2 = vp6_def_pdv_vector_model[1][3];
		u4Tmp3 = vp6_def_pdv_vector_model[1][4];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR3, u4Val);
		
		u4Tmp0 = vp6_def_pdv_vector_model[1][5];
		u4Tmp1 = vp6_def_pdv_vector_model[1][6];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR4, u4Val);
		
		u4Val = pred_range+mbt_range+2;
		vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Val);
		
		u4Tmp0 = vp6_def_fdv_vector_model[0][0];
		u4Tmp1 = vp6_def_fdv_vector_model[0][1];
		u4Tmp2 = vp6_def_fdv_vector_model[0][2];
		u4Tmp3 = vp6_def_fdv_vector_model[0][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1, u4Val);
		
		u4Tmp0 = vp6_def_fdv_vector_model[0][4];
		u4Tmp1 = vp6_def_fdv_vector_model[0][5];
		u4Tmp2 = vp6_def_fdv_vector_model[0][6];
		u4Tmp3 = vp6_def_fdv_vector_model[0][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR2, u4Val);
		
		u4Tmp0 = vp6_def_fdv_vector_model[1][0];
		u4Tmp1 = vp6_def_fdv_vector_model[1][1];
		u4Tmp2 = vp6_def_fdv_vector_model[1][2];
		u4Tmp3 = vp6_def_fdv_vector_model[1][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR3, u4Val);
		
		u4Tmp0 = vp6_def_fdv_vector_model[1][4];
		u4Tmp1 = vp6_def_fdv_vector_model[1][5];
		u4Tmp2 = vp6_def_fdv_vector_model[1][6];
		u4Tmp3 = vp6_def_fdv_vector_model[1][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR4, u4Val);
		
		u4Val = pred_range+mbt_range+mvd_range+2;
		vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][0];
		u4Tmp1 = vp6_def_runv_coeff_model[0][1];
		u4Tmp2 = vp6_def_runv_coeff_model[0][2];
		u4Tmp3 = vp6_def_runv_coeff_model[0][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][4];
		u4Tmp1 = vp6_def_runv_coeff_model[0][5];
		u4Tmp2 = vp6_def_runv_coeff_model[0][6];
		u4Tmp3 = vp6_def_runv_coeff_model[0][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR2, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][8];
		u4Tmp1 = vp6_def_runv_coeff_model[0][9];
		u4Tmp2 = vp6_def_runv_coeff_model[0][10];
		u4Tmp3 = vp6_def_runv_coeff_model[0][11];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR3, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[0][12];
		u4Tmp1 = vp6_def_runv_coeff_model[0][13];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR4, u4Val);
		
		u4Val = pred_range+mbt_range+mvd_range+3;
		vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][0];
		u4Tmp1 = vp6_def_runv_coeff_model[1][1];
		u4Tmp2 = vp6_def_runv_coeff_model[1][2];
		u4Tmp3 = vp6_def_runv_coeff_model[1][3];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][4];
		u4Tmp1 = vp6_def_runv_coeff_model[1][5];
		u4Tmp2 = vp6_def_runv_coeff_model[1][6];
		u4Tmp3 = vp6_def_runv_coeff_model[1][7];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR2, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][8];
		u4Tmp1 = vp6_def_runv_coeff_model[1][9];
		u4Tmp2 = vp6_def_runv_coeff_model[1][10];
		u4Tmp3 = vp6_def_runv_coeff_model[1][11];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR3, u4Val);
		
		u4Tmp0 = vp6_def_runv_coeff_model[1][12];
		u4Tmp1 = vp6_def_runv_coeff_model[1][13];
		u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
		vVDecWriteVP6VLD(RW_VP6_VLD_FCVR4, u4Val);
		
		UNUSED(u4Addr);

	}
    UNUSED(cfm_range);
	return TRUE;
}

UINT32 u4VDEC_HAL_VP6_Parse_Mb_Type_Models(BOOL fgSupportMult)
{    
    const UCHAR    vp56_pre_def_mb_type_stats[16][3][10][2] = 
    {
       { 
      	   { {   9, 15 }, {  32, 25 }, {  7,  19 }, {   9, 21 }, {  1, 12 },
             {  14, 12 }, {   3, 18 }, { 14,  23 }, {   3, 10 }, {  0,  4 }, },
          { {  41, 22 }, {   1,  0 }, {  1,  31 }, {   0,  0 }, {  0,  0 },
             {   0,  1 }, {   1,  7 }, {  0,   1 }, {  98, 25 }, {  4, 10 }, },
          { {   2,  3 }, {   2,  3 }, {  0,   2 }, {   0,  2 }, {  0,  0 },
             {  11,  4 }, {   1,  4 }, {  0,   2 }, {   3,  2 }, {  0,  4 }, }, 
       },
       { 
           { {  48, 39 }, {   1,  2 }, { 11,  27 }, {  29, 44 }, {  7, 27 },
              {   1,  4 }, {   0,  3 }, {  1,   6 }, {   1,  2 }, {  0,  0 }, },
           { { 123, 37 }, {   6,  4 }, {  1,  27 }, {   0,  0 }, {  0,  0 },
              {   5,  8 }, {   1,  7 }, {  0,   1 }, {  12, 10 }, {  0,  2 }, },
           { {  49, 46 }, {   3,  4 }, {  7,  31 }, {  42, 41 }, {  0,  0 },
              {   2,  6 }, {   1,  7 }, {  1,   4 }, {   2,  4 }, {  0,  1 }, }, 
       },
       {
           { {  21, 32 }, {   1,  2 }, {  4,  10 }, {  32, 43 }, {  6, 23 },
              {   2,  3 }, {   1, 19 }, {  1,   6 }, {  12, 21 }, {  0,  7 }, },
           { {  26, 14 }, {  14, 12 }, {  0,  24 }, {   0,  0 }, {  0,  0 },
              {  55, 17 }, {   1,  9 }, {  0,  36 }, {   5,  7 }, {  1,  3 }, },
           { {  26, 25 }, {   1,  1 }, {  2,  10 }, {  67, 39 }, {  0,  0 },
              {   1,  1 }, {   0, 14 }, {  0,   2 }, {  31, 26 }, {  1,  6 }, }, 
       },
       {
           { {  69, 83 }, {   0,  0 }, {  0,   2 }, {  10, 29 }, {  3, 12 },
              {   0,  1 }, {   0,  3 }, {  0,   3 }, {   2,  2 }, {  0,  0 }, },
           { { 209,  5 }, {   0,  0 }, {  0,  27 }, {   0,  0 }, {  0,  0 },
              {   0,  1 }, {   0,  1 }, {  0,   1 }, {   0,  0 }, {  0,  0 }, },
           { { 103, 46 }, {   1,  2 }, {  2,  10 }, {  33, 42 }, {  0,  0 },
             {   1,  4 }, {   0,  3 }, {  0,   1 }, {   1,  3 }, {  0,  0 }, }, 
       },
       {
           { {  11, 20 }, {   1,  4 }, { 18,  36 }, {  43, 48 }, { 13, 35 },
              {   0,  2 }, {   0,  5 }, {  3,  12 }, {   1,  2 }, {  0,  0 }, },
           { {   2,  5 }, {   4,  5 }, {  0, 121 }, {   0,  0 }, {  0,  0 },
              {   0,  3 }, {   2,  4 }, {  1,   4 }, {   2,  2 }, {  0,  1 }, },
           { {  14, 31 }, {   9, 13 }, { 14,  54 }, {  22, 29 }, {  0,  0 },
              {   2,  6 }, {   4, 18 }, {  6,  13 }, {   1,  5 }, {  0,  1 }, }, 
       },
       { 
           { {  70, 44 }, {   0,  1 }, {  2,  10 }, {  37, 46 }, {  8, 26 },
              {   0,  2 }, {   0,  2 }, {  0,   2 }, {   0,  1 }, {  0,  0 }, },
           { { 175,  5 }, {   0,  1 }, {  0,  48 }, {   0,  0 }, {  0,  0 },
              {   0,  2 }, {   0,  1 }, {  0,   2 }, {   0,  1 }, {  0,  0 }, },
          { {  85, 39 }, {   0,  0 }, {  1,   9 }, {  69, 40 }, {  0,  0 },
             {   0,  1 }, {   0,  3 }, {  0,   1 }, {   2,  3 }, {  0,  0 }, }, 
       },
       { 
           { {   8, 15 }, {   0,  1 }, {  8,  21 }, {  74, 53 }, { 22, 42 },
              {   0,  1 }, {   0,  2 }, {  0,   3 }, {   1,  2 }, {  0,  0 }, },
           { {  83,  5 }, {   2,  3 }, {  0, 102 }, {   0,  0 }, {  0,  0 },
              {   1,  3 }, {   0,  2 }, {  0,   1 }, {   0,  0 }, {  0,  0 }, },
           { {  31, 28 }, {   0,  0 }, {  3,  14 }, { 130, 34 }, {  0,  0 },
              {   0,  1 }, {   0,  3 }, {  0,   1 }, {   3,  3 }, {  0,  1 }, }, 
       },
       { 
           { { 141, 42 }, {   0,  0 }, {  1,   4 }, {  11, 24 }, {  1, 11 },
              {   0,  1 }, {   0,  1 }, {  0,   2 }, {   0,  0 }, {  0,  0 }, },
           { { 233,  6 }, {   0,  0 }, {  0,   8 }, {   0,  0 }, {  0,  0 },
              {   0,  1 }, {   0,  1 }, {  0,   0 }, {   0,  1 }, {  0,  0 }, },
           { { 171, 25 }, {   0,  0 }, {  1,   5 }, {  25, 21 }, {  0,  0 },
              {   0,  1 }, {   0,  1 }, {  0,   0 }, {   0,  0 }, {  0,  0 }, },
        },
      {
           { {   8, 19 }, {   4, 10 }, { 24,  45 }, {  21, 37 }, {  9, 29 },
              {   0,  3 }, {   1,  7 }, { 11,  25 }, {   0,  2 }, {  0,  1 }, },
           { {  34, 16 }, { 112, 21 }, {  1,  28 }, {   0,  0 }, {  0,  0 },
              {   6,  8 }, {   1,  7 }, {  0,   3 }, {   2,  5 }, {  0,  2 }, },
           { {  17, 21 }, {  68, 29 }, {  6,  15 }, {  13, 22 }, {  0,  0 },
              {   6, 12 }, {   3, 14 }, {  4,  10 }, {   1,  7 }, {  0,  3 }, }, 
       },
       {
           { {  46, 42 }, {   0,  1 }, {  2,  10 }, {  54, 51 }, { 10, 30 },
              {   0,  2 }, {   0,  2 }, {  0,   1 }, {   0,  1 }, {  0,  0 }, },
           { { 159, 35 }, {   2,  2 }, {  0,  25 }, {   0,  0 }, {  0,  0 },
              {   3,  6 }, {   0,  5 }, {  0,   1 }, {   4,  4 }, {  0,  1 }, },
           { {  51, 39 }, {   0,  1 }, {  2,  12 }, {  91, 44 }, {  0,  0 },
              {   0,  2 }, {   0,  3 }, {  0,   1 }, {   2,  3 }, {  0,  1 }, }, 
       },
       {   
            { {  28, 32 }, {   0,  0 }, {  3,  10 }, {  75, 51 }, { 14, 33 },
               {   0,  1 }, {   0,  2 }, {  0,   1 }, {   1,  2 }, {  0,  0 }, },
            { {  75, 39 }, {   5,  7 }, {  2,  48 }, {   0,  0 }, {  0,  0 },
               {   3, 11 }, {   2, 16 }, {  1,   4 }, {   7, 10 }, {  0,  2 }, },
            { {  81, 25 }, {   0,  0 }, {  2,   9 }, { 106, 26 }, {  0,  0 },
               {   0,  1 }, {   0,  1 }, {  0,   1 }, {   1,  1 }, {  0,  0 }, }, 
       },
       {   
           { { 100, 46 }, {   0,  1 }, {  3,   9 }, {  21, 37 }, {  5, 20 },
              {   0,  1 }, {   0,  2 }, {  1,   2 }, {   0,  1 }, {  0,  0 }, },
           { { 212, 21 }, {   0,  1 }, {  0,   9 }, {   0,  0 }, {  0,  0 },
              {   1,  2 }, {   0,  2 }, {  0,   0 }, {   2,  2 }, {  0,  0 }, },
           { { 140, 37 }, {   0,  1 }, {  1,   8 }, {  24, 33 }, {  0,  0 },
              {   1,  2 }, {   0,  2 }, {  0,   1 }, {   1,  2 }, {  0,  0 }, },
       },
       { 
           { {  27, 29 }, {   0,  1 }, {  9,  25 }, {  53, 51 }, { 12, 34 },
              {   0,  1 }, {   0,  3 }, {  1,   5 }, {   0,  2 }, {  0,  0 }, },
           { {   4,  2 }, {   0,  0 }, {  0, 172 }, {   0,  0 }, {  0,  0 },
              {   0,  1 }, {   0,  2 }, {  0,   0 }, {   2,  0 }, {  0,  0 }, },
           { {  14, 23 }, {   1,  3 }, { 11,  53 }, {  90, 31 }, {  0,  0 },
              {   0,  3 }, {   1,  5 }, {  2,   6 }, {   1,  2 }, {  0,  0 }, }, 
       },
       { 
           { {  80, 38 }, {   0,  0 }, {  1,   4 }, {  69, 33 }, {  5, 16 },
              {   0,  1 }, {   0,  1 }, {  0,   0 }, {   0,  1 }, {  0,  0 }, },
           { { 187, 22 }, {   1,  1 }, {  0,  17 }, {   0,  0 }, {  0,  0 },
              {   3,  6 }, {   0,  4 }, {  0,   1 }, {   4,  4 }, {  0,  1 }, },
           { { 123, 29 }, {   0,  0 }, {  1,   7 }, {  57, 30 }, {  0,  0 },
              {   0,  1 }, {   0,  1 }, {  0,   1 }, {   0,  1 }, {  0,  0 }, }, 
       },
       { 
           { {  16, 20 }, {   0,  0 }, {  2,   8 }, { 104, 49 }, { 15, 33 },
              {   0,  1 }, {   0,  1 }, {  0,   1 }, {   1,  1 }, {  0,  0 }, },
           { { 133,  6 }, {   1,  2 }, {  1,  70 }, {   0,  0 }, {  0,  0 },
              {   0,  2 }, {   0,  4 }, {  0,   3 }, {   1,  1 }, {  0,  0 }, },
           { {  13, 14 }, {   0,  0 }, {  4,  20 }, { 175, 20 }, {  0,  0 },
              {   0,  1 }, {   0,  1 }, {  0,   1 }, {   1,  1 }, {  0,  0 }, }, 
       },
       {
           { { 194, 16 }, {   0,  0 }, {  1,   1 }, {   1,  9 }, {  1,  3 },
              {   0,  0 }, {   0,  1 }, {  0,   1 }, {   0,  0 }, {  0,  0 }, },
           { { 251,  1 }, {   0,  0 }, {  0,   2 }, {   0,  0 }, {  0,  0 },
              {   0,  0 }, {   0,  0 }, {  0,   0 }, {   0,  0 }, {  0,  0 }, },
           { { 202, 23 }, {   0,  0 }, {  1,   3 }, {   2,  9 }, {  0,  0 },
              {   0,  1 }, {   0,  1 }, {  0,   1 }, {   0,  0 }, {  0,  0 }, }, 
       },
    };
    UINT32    pred_range = 840;
    UINT32    idx, ctx, i;
    UINT32    u4Val, u4Tmp0, u4Tmp1, u4Tmp2, u4Tmp3;
    UINT32 u4Addr;

	if(fgSupportMult)
	{
		for (idx=0; idx<16; idx++) 
		{
			for (ctx=0; ctx<3; ctx++) 
			{
				for(i=0; i<2; i++)
				{
					u4Addr = MC_VLD_WRAPPER_WRITE | ((idx*3*2 +ctx*2 +i+pred_range) << 2);
		
					u4Tmp0 = vp56_pre_def_mb_type_stats[idx][ctx][0][i],
					u4Tmp1 = vp56_pre_def_mb_type_stats[idx][ctx][1][i],
					u4Tmp2 = vp56_pre_def_mb_type_stats[idx][ctx][2][i],
					u4Tmp3 = vp56_pre_def_mb_type_stats[idx][ctx][3][i],
					u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
					vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x3));
					vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
					u4Tmp0 = vp56_pre_def_mb_type_stats[idx][ctx][4][i],
					u4Tmp1 = vp56_pre_def_mb_type_stats[idx][ctx][5][i],
					u4Tmp2 = vp56_pre_def_mb_type_stats[idx][ctx][6][i],
					u4Tmp3 = vp56_pre_def_mb_type_stats[idx][ctx][7][i],
					u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
					vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x2));
					vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
					u4Tmp0 = vp56_pre_def_mb_type_stats[idx][ctx][8][i],
					u4Tmp1 = vp56_pre_def_mb_type_stats[idx][ctx][9][i],
					u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
					vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x1));
					vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
		
					u4Val = 0;
					vVDecWriteMC(RW_MC_VLD_WRAPPER_ADDR, (u4Addr | 0x0));
					vVDecWriteMC(RW_MC_VLD_WRAPPER_DATA, u4Val);
				}
			}
		}

	}
	else
	{
		for (idx=0; idx<16; idx++) 
		{
			for (ctx=0; ctx<3; ctx++) 
			{
				for(i=0; i<2; i++)
				{
					u4Val = (idx*3*2 +ctx*2 +i+pred_range);
					vVDecWriteVP6VLD(RW_VP6_VLD_WARR, u4Val);
		
					u4Tmp0 = vp56_pre_def_mb_type_stats[idx][ctx][0][i],
					u4Tmp1 = vp56_pre_def_mb_type_stats[idx][ctx][1][i],
					u4Tmp2 = vp56_pre_def_mb_type_stats[idx][ctx][2][i],
					u4Tmp3 = vp56_pre_def_mb_type_stats[idx][ctx][3][i],
					u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
					vVDecWriteVP6VLD(RW_VP6_VLD_FCVR1, u4Val);
		
					u4Tmp0 = vp56_pre_def_mb_type_stats[idx][ctx][4][i],
					u4Tmp1 = vp56_pre_def_mb_type_stats[idx][ctx][5][i],
					u4Tmp2 = vp56_pre_def_mb_type_stats[idx][ctx][6][i],
					u4Tmp3 = vp56_pre_def_mb_type_stats[idx][ctx][7][i],
					u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
					vVDecWriteVP6VLD(RW_VP6_VLD_FCVR2, u4Val);
		
					u4Tmp0 = vp56_pre_def_mb_type_stats[idx][ctx][8][i],
					u4Tmp1 = vp56_pre_def_mb_type_stats[idx][ctx][9][i],
					u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16));
					vVDecWriteVP6VLD(RW_VP6_VLD_FCVR3, u4Val);
		
					u4Val = 0;
					vVDecWriteVP6VLD(RW_VP6_VLD_FCVR4, u4Val);
				}
			}
		}

	}

   return TRUE;
}

UINT32 u4VDEC_HAL_VP6_Load_QMatrix(void)
{
    UINT32 i;
    UINT32 u4Tmp0, u4Tmp1, u4Tmp2, u4Tmp3;
    UINT32 u4Val;
    const UCHAR vp6_def_coeff_reorder[] = 
    {
        0,  0,  1,  1,  1,  2,  2,  2,
        2,  2,  2,  3,  3,  4,  4,  4,
        5,  5,  5,  5,  6,  6,  7,  7,
        7,  7,  7,  8,  8,  9,  9,  9,
        9,  9,  9, 10, 10, 11, 11, 11,
        11, 11, 11, 12, 12, 12, 12, 12,
        12, 13, 13, 13, 13, 13, 14, 14,
        14, 14, 15, 15, 15, 15, 15, 15,
    };

    for (i = 0; i < 64; i += 4) 
    {
        vVDecWriteVLD(RW_VLD_SCL_ADDR, ((0<<8)+i));

        u4Tmp0 = vp6_def_coeff_reorder[i];
        u4Tmp1 = vp6_def_coeff_reorder[i+1];
        u4Tmp2 = vp6_def_coeff_reorder[i+2];
        u4Tmp3 = vp6_def_coeff_reorder[i+3];      
        u4Val = ((u4Tmp0 << 24) |(u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
        vVDecWriteVLD(RW_VLD_SCL_DATA, u4Val);
    }

    return TRUE;
}

UINT32 u4VDEC_HAL_VP6_Load_Filter_Coef(UINT32 u4Select)
{
    INT32 u4Tmp0, u4Tmp1, u4Tmp2, u4Tmp3;
    INT32 u4Val;

    u4Tmp0 = (vp6_block_copy_filter[u4Select][1][0] * -1);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][2][0] * -1);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][3][0] * -1);
    u4Tmp3 = (vp6_block_copy_filter[u4Select][4][0] * -1);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
    vVDecWriteMC(RW_VP6_COEF0_P1, u4Val);

    u4Tmp0 = (vp6_block_copy_filter[u4Select][5][0] * -1);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][6][0] * -1);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][7][0] * -1);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8));
    vVDecWriteMC(RW_VP6_COEF0_P2, u4Val);


    u4Tmp0 = (vp6_block_copy_filter[u4Select][1][1]);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][2][1]);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][3][1]);
    u4Tmp3 = (vp6_block_copy_filter[u4Select][4][1]);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
    vVDecWriteMC(RW_VP6_COEF1_P1, u4Val);

    u4Tmp0 = (vp6_block_copy_filter[u4Select][5][1]);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][6][1]);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][7][1]);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8));
    vVDecWriteMC(RW_VP6_COEF1_P2, u4Val);

    u4Tmp0 = (vp6_block_copy_filter[u4Select][1][2]);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][2][2]);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][3][2]);
    u4Tmp3 = (vp6_block_copy_filter[u4Select][4][2]);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
    vVDecWriteMC(RW_VP6_COEF2_P1, u4Val);

    u4Tmp0 = (vp6_block_copy_filter[u4Select][5][2]);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][6][2]);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][7][2]);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8));
    vVDecWriteMC(RW_VP6_COEF2_P2, u4Val);

    u4Tmp0 = (vp6_block_copy_filter[u4Select][1][3] * -1);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][2][3] * -1);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][3][3] * -1);
    u4Tmp3 = (vp6_block_copy_filter[u4Select][4][3] * -1);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8) | (u4Tmp3));
    vVDecWriteMC(RW_VP6_COEF3_P1, u4Val);

    u4Tmp0 = (vp6_block_copy_filter[u4Select][5][3] * -1);
    u4Tmp1 = (vp6_block_copy_filter[u4Select][6][3] * -1);
    u4Tmp2 = (vp6_block_copy_filter[u4Select][7][3] * -1);
    u4Val = ((u4Tmp0 << 24) | (u4Tmp1 << 16) | (u4Tmp2 << 8));
    vVDecWriteMC(RW_VP6_COEF3_P2, u4Val);      
   
    return TRUE;
}

UINT32 u4VDEC_HAL_VP6_VDec_ReadFinishFlag(BOOL fgFinishParam)
{
    UINT32 u4Mbx, u4Mby;
    if(fgFinishParam)
    {
		vVDEC_HAL_VP6_GetMbxMby(&u4Mbx, &u4Mby);
		return ((u4VDecReadVP6VLD(0x100) & 0x1) && 
				(u4Mbx == (u4VDecReadVLDTop(RW_PIC_MB_SIZE_M1) & 0xFF)) &&
				(u4Mby == ((u4VDecReadVLDTop(RW_PIC_MB_SIZE_M1)>>16) & 0xFF)));
    }
	else
	{
		return u4VDecReadVP6VLD(RO_VP6_PIC_DEC_END);
	}

}

void vVDEC_HAL_VP6_VDec_ClearFinishFlag(void)
{
    vVDecWriteVP6VLD(RW_VP6_VLD_VICR, VP6_VPIC);
    vVDecWriteVP6VLD(RW_VP6_VLD_VICR, 0);
}

