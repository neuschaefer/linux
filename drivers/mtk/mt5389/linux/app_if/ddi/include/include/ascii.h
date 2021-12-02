/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 2008 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file ascii.h
 *
 * This header file defines the macro definitions defining ASCII code.
 *
 *	@author 	David (DONG-IL) Lee
 *	@version	1.0
 *	@date		2008.05.08
 *	@note
 *	@see
 */

#ifndef	_ASCII_H_
#define	_ASCII_H_

#ifdef __cplusplus
extern "C" {
#endif

//	hex	/* dec	ascii	name							*/
// 0x00	/*	0	Ctrl-@	"NUL" : null					*/
// 0x01	/*	1	Ctrl-A	"SOH" : Start Of Handling		*/
// 0x02	/*	2	Ctrl-B	"STX" : Start Of Text			*/
// 0x03	/*	3	Ctrl-C	"ETX" : End Of Text				*/
// 0x04	/*	4	Ctrl-D	"EOT" : End Of Transmisstion	*/
// 0x05	/*	5	Ctrl-E	"ENQ" : ENQuiry					*/
// 0x06	/*	6	Ctrl-F	"ACK" : ACKnowledge				*/
// 0x07	/*	7	Ctrl-G	"BEL" : bell					*/
// 0x08	/*	8	Ctrl-H	"BS"  : BackSpace				*/
// 0x09	/*	9	Ctrl-I	"HT"  : Horizontal Tab			*/
// 0x0a	/*	10	Ctrl-J	"LF"  : Line Feed, newline		*/
// 0x0b	/*	11	Ctrl-K	"VT"  : Vertical Tab			*/
// 0x0c	/*	12	Ctrl-L	"FF"  : NP Form Feed			*/
// 0x0d	/*	13	Ctrl-M	"CR"  : Carriage Return			*/
// 0x0e	/*	14	Ctrl-N	"SO"  : Shift Out				*/
// 0x0f	/*	15	Ctrl-O	"SI"  : Shift In				*/
// 0x10	/*	16	Ctrl-P	"DLE" : Data Link Escape		*/
// 0x11	/*	17	Ctrl-Q	"DC1" : Device Control 1		*/
// 0x12	/*	18	Ctrl-R	"DC2" : Device Control 2		*/
// 0x13	/*	19	Ctrl-S	"DC3" : Device Control 3		*/
// 0x14	/*	20	Ctrl-T	"DC4" : Device Control 4		*/
// 0x15	/*	21	Ctrl-U	"NAK" : Negative AcKnowledge	*/
// 0x16	/*	22	Ctrl-V	"SYN" : Synchronous Idle		*/
// 0x17	/*	23	Ctrl-W	"ETB" : End of Tran Block		*/
// 0x18	/*	24	Ctrl-X	"CAN" : CANcel					*/
// 0x19	/*	25	Ctrl-Y	"EM"  : End of Medium			*/
// 0x1a	/*	26	Ctrl-Z	"SUB" : SUBstitute				*/
// 0x1b	/*	27	Ctrl-[	"ESC" : ESCape					*/
// 0x1c	/*	28	Ctrl-\	"FS"  : File Separator			*/
// 0x1d	/*	29	Ctrl-]	"GS"  : Group Separator			*/
// 0x1e	/*	30	Ctrl-^	"RS"  : Record Separator		*/
// 0x1f	/*	31	Ctrl_	"US"  : Unit Separator			*/
#define	C_SPACE	0x20 /*  32  Space		*/
//				0x21 /*  33  !			*/
//				0x22 /*  34  "			*/
//				0x23 /*  35  #			*/
//				0x24 /*  36  $			*/
//				0x25 /*  37  %			*/
//				0x26 /*  38  &			*/
//				0x27 /*  39  '			*/
//				0x28 /*  40  (			*/
//				0x29 /*  41  )			*/
//				0x2a /*  42  *			*/
//				0x2b /*  43  +			*/
//				0x2c /*  44  ,			*/
//				0x2d /*  45  -			*/
//				0x2e /*  46  .			*/
//				0x2f /*  47  /			*/
#define	C_0		0x30 /*  48  0			*/
#define	C_1		0x31 /*  49  1			*/
#define	C_2		0x32 /*  50  2			*/
#define	C_3		0x33 /*  51  3			*/
#define	C_4		0x34 /*  52  4			*/
#define	C_5		0x35 /*  53  5			*/
#define	C_6		0x36 /*  54  6			*/
#define	C_7		0x37 /*  55  7			*/
#define	C_8		0x38 /*  56  8			*/
#define	C_9		0x39 /*  57  9			*/
//		   		0x3a /*  58  :			*/
//		   		0x3b /*  59  ;			*/
//		   		0x3c /*  60  <			*/
//		   		0x3d /*  61  =			*/
//		   		0x3e /*  62  >			*/
//		   		0x3f /*  63  ?			*/
//		   		0x40 /*  64  @			*/
#define	C_A		0x41 /*  65  A			*/
#define	C_B		0x42 /*  66  B			*/
#define	C_C		0x43 /*  67  C			*/
#define	C_D		0x44 /*  68  D			*/
#define	C_E		0x45 /*  69  E			*/
#define	C_F		0x46 /*  70  F			*/
#define	C_G		0x47 /*  71  G			*/
#define	C_H		0x48 /*  72  H			*/
#define	C_I		0x49 /*  73  I			*/
#define	C_J		0x4a /*  74  J			*/
#define	C_K		0x4b /*  75  K			*/
#define	C_L		0x4c /*  76  L			*/
#define	C_M		0x4d /*  77  M			*/
#define	C_N		0x4e /*  78  N			*/
#define	C_O		0x4f /*  79  O			*/
#define	C_P		0x50 /*  80  P			*/
#define	C_Q		0x51 /*  81  Q			*/
#define	C_R		0x52 /*  82  R			*/
#define	C_S		0x53 /*  83  S			*/
#define	C_T		0x54 /*  84  T			*/
#define	C_U		0x55 /*  85  U			*/
#define	C_V		0x56 /*  86  V			*/
#define	C_W		0x57 /*  87  W			*/
#define	C_X		0x58 /*  88  X			*/
#define	C_Y		0x59 /*  89  Y			*/
#define	C_Z		0x5a /*  90  Z			*/
//		   		0x5b /*  91  [			*/
//		   		0x5c /*  92  \			*/
//		   		0x5d /*  93  ]			*/
//		   		0x5e /*  94  ^			*/
#define C__		0x5f /*  95  _			*/
//		   		0x60 /*  96  `			*/
#define	C_a		0x61 /*  97  a			*/
#define	C_b		0x62 /*  98  b			*/
#define	C_c		0x63 /*  99  c			*/
#define	C_d		0x64 /* 100  d			*/
#define	C_e		0x65 /* 101  e			*/
#define	C_f		0x66 /* 102  f			*/
#define	C_g		0x67 /* 103  g			*/
#define	C_h		0x68 /* 104  h			*/
#define	C_i		0x69 /* 105  i			*/
#define	C_j		0x6a /* 106  j			*/
#define	C_k		0x6b /* 107  k			*/
#define	C_l		0x6c /* 108  l			*/
#define	C_m		0x6d /* 109  m			*/
#define	C_n		0x6e /* 110  n			*/
#define	C_o		0x6f /* 111  o			*/
#define	C_p		0x70 /* 112  p			*/
#define	C_q		0x71 /* 113  q			*/
#define	C_r		0x72 /* 114  r			*/
#define	C_s		0x73 /* 115  s			*/
#define	C_t		0x74 /* 116  t			*/
#define	C_u		0x75 /* 117  u			*/
#define	C_v		0x76 /* 118  v			*/
#define	C_w		0x77 /* 119  w			*/
#define	C_x		0x78 /* 120  x			*/
#define	C_y		0x79 /* 121  y			*/
#define	C_z		0x7a /* 122  z			*/
//				0x7b /* 123  {			*/
//				0x7c /* 124  |			*/
//				0x7d /* 125  }			*/
//				0x7e /* 126  ~			*/
//				0x7f /* 127  DEL		*/

#ifdef __cplusplus
}
#endif

#endif	/* _ASCII_H_ */
