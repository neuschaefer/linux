/***************************************************************************
 \file com_type.h
 \brief This is the header file for common typedef
****************************************************************************/
#ifndef _COM_TYPE_H
#define _COM_TYPE_H

/*==========================================================================*/
/*     Includes                                                             */
/*==========================================================================*/
#include <stdarg.h>
#include <stddef.h>
//#include <math.h>

/*==========================================================================*/
/*     Typedefs, enums, defines                                             */
/*==========================================================================*/
//#ifndef _TYPEDEF_MD
////typedef signed double DOUBLE;	//INT64
////typedef unsigned double FWORD;	//UINT64
typedef double FWORD;			//INT64
typedef signed long LONG;		//INT32
typedef unsigned long DWD;    // UINT32
typedef signed int SHORT;
typedef unsigned int WORD;
//typedef unsigned char BYTE;     // UINT8
//typedef unsigned char BOOL;     // UINT8
//typedef signed char CHAR;		// INT8
//#define _TYPEDEF_MD
//#endif 

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


#define FALSE		0
#define TRUE		1

#define HIGH_B( a )                 ((Byte) ((a) >> 8))
#define LOW_B( a )                  ((Byte) (a))
#define C_WORD( a, b )              (unsigned short)((((Word) (a)) << 8) | (Word)(b))

#endif // _COM_TYPE_H
