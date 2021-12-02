/*------------------------------------------------------------------------------

 <dev:header>
    Copyright(c) 2009 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/

#ifndef CXD2828_MATH_H_
#define CXD2828_MATH_H_

/*------------------------------------------------------------------------------
 Includes
------------------------------------------------------------------------------*/
#include "cxd2828_common.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

/**
 @brief Get an integer based log2 of x and return as (log2(x) * 100)
        No checking of 0 parameter. Passing in 0
        will return 0.

 @param x The value to get the log2() of.

 @return log2(x) * 100.
*/
UINT32 cxd2828_math_log2 (UINT32 x);

/**
 @brief Get and integer based log10 of x and return as (log10(x) * 100)
        No checking of 0 parameter. Passing in 0
        will return 0.

 @param x The value to get the log10() of.

 @return log10(x) * 100.
*/
UINT32 cxd2828_math_log10 (UINT32 x);

/**
 @brief Get an integer based ln (log) of x and return as (ln(x) * 100)
        No checking of 0 parameter. Passing in 0
        will return 0.

 @param x The value to get the ln() of.

 @return ln(x) * 100.
*/
UINT32 cxd2828_math_log (UINT32 x);

/**
 @brief minimum macro if not already defined.  Returns the minimum value between two numbers

 @param a First number to compare 
 @param b Second number to compare
*/
#ifndef min
#   define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif /* CXD2828_MATH_H_ */
