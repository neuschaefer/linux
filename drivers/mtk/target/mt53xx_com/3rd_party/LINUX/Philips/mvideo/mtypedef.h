/*==========================================================================*/
/*
;     SOURCE_FILE:    mtypedef.h
;     APPLICATION:    LC08SP
;     COMPONENT:      Type define
;     VERSION:        %version: 0.4 %
;     DATE:           %date_created: Mon May 05 11:09:05 2008 %
;     MODIFIER:       %derived_by: christina %
;
;     %full_filespec: mtypedef.h,0.4:incl:sgp75lc8#1 %
;
;     (C)2007 Philips Consumer Electronics - LoB Mainstream Displays
;
;     All rights are reserved. Reproduction in whole or in part is
;     prohibited without the prior written consent of the copyright
;     owner. The information presented in this document does not
;     form part of any quotation or contract, is believed to be
;     accurate and reliable and may be changed without notice.
;     No liability will be accepted by the publisher for any
;     consequence of its use. Publication thereof does not convey
;     nor imply any license under patent- or other industrial or
;     intellectual property rights.
*/
#ifndef _MTYPEDEF_H
#define _MTYPEDEF_H

/*==========================================================================*/
/*     Includes                                                             */
/*==========================================================================*/
#include <stdarg.h>
#include <stddef.h>
#include "u_common.h"

/*==========================================================================*/
/*     Typedefs, enums, defines                                             */
/*==========================================================================*/

#ifndef _TYPEDEF_MD
typedef unsigned long DWord;    // UINT32
typedef short SWord;            // INT16
typedef unsigned short Word;    // UINT16
typedef unsigned char Byte;     // UINT8
typedef unsigned char Bool;     // UINT8
typedef signed char SByte;      // INT8
#define _TYPEDEF_MD
#endif 

#define ON          1
#define OFF         0
#define YES         1
#define NO          0
#define DOWN        1       /* eg. move down in menu, teletext */
#define UP          0
#define INC         1       /* eg. increase a value */
#define DEC         0
#define TOGGLE      2
#define ABSENT      0
#define PRESENT     1

#define HIGH_B( a )                 ((Byte) ((a) >> 8))
#define LOW_B( a )                  ((Byte) (a))
#define C_WORD( a, b )              (unsigned short)((((Word) (a)) << 8) | (Word)(b))

#endif // _MTYPEDEF_H
