/*
   // $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/HeaderMacros.h#1 $
   // Copyright (c) 2006 DivX, Inc. http://www.divx.com/corporate
   // All rights reserved.
   //
   // This software is the confidential and proprietary information of DivX
   // Inc. ("Confidential Information").  You shall not disclose such Confidential
   // Information and shall use it only in accordance with the terms of the license
   // agreement you entered into with DivX, Inc.
 */
#if ! defined DRMHEADERMACROS_H
#define DRMHEADERMACROS_H

#define int322mem(x, buf) 	buf[0] = (unsigned char)(x & 0xFF); buf[1] = (unsigned char)((x >> 8) & 0xFF); buf[2] = (unsigned char)((x >> 16) & 0xFF);	buf[3] = (unsigned char)((x >> 24) & 0xFF); buf += 4;
#define mem2int32(buf, x) x=buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24); buf+=4;

#define int162mem(x, buf) buf[0] = (unsigned char)(x & 0xFF); buf[1] = (unsigned char)((x >> 8) & 0xFF); buf += 2;
#define mem2int16(buf,x) x=buf[0] + (buf[1] << 8); buf += 2;

#define int82mem(x, buf) buf[0] = x & 0xFF;buf +=1;
#define mem2int8(buf, x) x = buf[0]; buf +=1;

#define bytes2mem(byte, mem,count) memcpy(mem,byte,count);mem += count;
#define mem2bytes(mem, byte,count) memcpy(byte,mem,count);mem += count;


#endif
/* DRMHEADERMACROS_H */
