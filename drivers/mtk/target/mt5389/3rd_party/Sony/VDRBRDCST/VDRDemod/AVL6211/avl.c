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


//$Revision: #1 $ 
//$Date: 2012/04/27 $
// 
#include "avl.h"

#ifdef AVL_CPLUSPLUS
extern "C" {
#endif

	void ChunkAddr(AVL_uint32 uiaddr, AVL_puchar pBuff)
	{
		pBuff[0] =(AVL_uchar)(uiaddr>>16);
		pBuff[1] =(AVL_uchar)(uiaddr>>8);
		pBuff[2] =(AVL_uchar)(uiaddr);
		return ;
	}

	void Chunk16(AVL_uint16 uidata, AVL_puchar pBuff)
	{
		pBuff[0] = (AVL_uchar)(uidata>>8);
		pBuff[1] = (AVL_uchar)(uidata & 0xff);
		return ;
	}

	AVL_uint16 DeChunk16(const AVL_puchar pBuff)
	{
		AVL_uint16 uiData;
		uiData = pBuff[0];
		uiData = (AVL_uint16)(uiData << 8) + pBuff[1];
		return uiData;
	}

	void Chunk32(AVL_uint32 uidata, AVL_puchar pBuff)
	{
		pBuff[0] = (AVL_uchar)(uidata>>24);
		pBuff[1] = (AVL_uchar)(uidata>>16);
		pBuff[2] = (AVL_uchar)(uidata>>8);
		pBuff[3] = (AVL_uchar)(uidata);
		return ;
	}

	AVL_uint32 DeChunk32(const AVL_puchar pBuff)
	{
		AVL_uint32 uiData;
		uiData = pBuff[0];
		uiData = (uiData << 8) + pBuff[1];
		uiData = (uiData << 8) + pBuff[2];
		uiData = (uiData << 8) + pBuff[3];
		return uiData;
	}

#ifdef AVL_CPLUSPLUS
}
#endif

