/*------------------------------------------------------------------------------

 <dev:header>
    Copyright(c) 2011 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**

 @file    sony_stdlib.h

          This file provides the C standard lib function mapping.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_STDLIB_H
#define SONY_STDLIB_H

#include "sony_dvb.h"
#include "x_os.h"

/*
 PORTING. Please modify if ANCI C standard library is not available.
*/
//#include <string.h>

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/

/**
 @brief Alias for memcpy.
*/
//extern void* x_memcpy(void*, const void*, unsigned int);
#define sony_memcpy  x_memcpy

/**
 @brief Alias for memset.
*/
//extern void* x_memset(void*, uint8_t, unsigned int);
#define sony_memset  x_memset

/**
 @brief Alias for abs.
*/
#define sony_abs  abs

#endif /* SONY_STDLIB_H */
