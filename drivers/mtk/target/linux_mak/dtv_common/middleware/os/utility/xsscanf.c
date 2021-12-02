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
 * $RCSfile: xsscanf.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "os/inc/os.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  maximum conversion with in [] specification.
------------------------------------------------------------------------------*/
#define   MAX_NB_CONV_OPS      32    

/*------------------------------------------------------------------------------
  size specifier.
------------------------------------------------------------------------------*/
#define   PTR_SPEC_NONE         -1
#define   PTR_SPEC_H            1
#define   PTR_SPEC_LOWER_L      2
#define   PTR_SPEC_UPPER_L      3
#define   PTR_SPEC_H_H          4

/*------------------------------------------------------------------------------
  Assignment pointer type.
-------------------------------------------------------------------------------*/
typedef enum
{
    PTR_NONE             = 0,
    PTR_INT,
    PTR_S_SHORT,
    PTR_S_LONG,
    PTR_U_INT,
    PTR_U_SHORT,
    PTR_U_LONG,
    PTR_CHAR,
    PTR_FLOAT,
    PTR_DOUBLE,
    PTR_LONG_DOUBLE,
    PTR_VOID_VOID
}  XSSCANF_PTR_TYPE_T;

/*-------------------------------------------------------------------------------
  Conversion format structure.
--------------------------------------------------------------------------------*/
typedef struct _BRACKET_FMT_T_
{
    BOOL                 b_negation;
    INT32                i4_num_of_conv_char;
    CHAR                 char_set[128];
} BRACKET_FMT_T;


typedef struct _conv_fmt_t_
{
    INT32                i4_max_fld_width;
    INT32                i4_size_spec;
    INT32                i4_nb_conv;
    BOOL                 b_suppres;
    CHAR                 ac_conv_ops[MAX_NB_CONV_OPS];
    XSSCANF_PTR_TYPE_T   e_ptr_type[MAX_NB_CONV_OPS];

    /* for '[' ']' format conversion. */
    BRACKET_FMT_T        bracket_fmt;
} CONV_FMT_T;

#define CHAR_IS_DIGIT(_c) ((_c) >= '0' && (_c) <= '9')
#define CHAR_IS_SPACE(_c) (((_c)==' ') || (((_c)>=0x09) && ((_c)<=0x0D)))

/*----------------------------------------------------------------------
 * 
 * Description: compare the input char with the conversion char_set.
 *   For matching case, if input char is in the char_set, then accept this
 *   char.
 *
 *   For negation case, if input char is NOT in the char_set, then accept this
 *   char.
 *
 * Inputs:
 *   pt_bracket_fmt  Pointer to the structure for the bracket data.
 *
 *   c_char_symbol   Input char.
 *
 * Outputs: None
 *
 * Returns: TRUE:   accept this char.
 *          FALSE:  reject this char.
 *---------------------------------------------------------------------------*/
static BOOL   match_this_char(BRACKET_FMT_T*  pt_bracket_fmt,  CHAR  c_char_symbol)
{
    BOOL         b_accept;
    INT32        i4_i;

    /*
      x_dbg_stmt("checking char: %c \n", c_char_symbol);
    */
    
    /* for matching case. */
    if ( pt_bracket_fmt->b_negation == FALSE )
    {
        b_accept = FALSE;
        for ( i4_i = 0 ; i4_i < pt_bracket_fmt->i4_num_of_conv_char ; i4_i++ )
        {
            if ( c_char_symbol == pt_bracket_fmt->char_set[i4_i] )
            {
                b_accept = TRUE;
                break;
            }
        }
    }
    else
    {
        /* for negation case. */
        b_accept = TRUE;
        for ( i4_i = 0 ; i4_i < pt_bracket_fmt->i4_num_of_conv_char ; i4_i++ )
        {
            /* x_dbg_stmt("compare %c with %c \n",
               c_char_symbol ,
               pt_bracket_fmt->char_set[i4_i]);
            */
            
            if ( c_char_symbol == pt_bracket_fmt->char_set[i4_i] )
            {
                b_accept = FALSE;
                break;
            }
        }
    }
    return b_accept;
}

/*----------------------------------------------------------------------
 * 
 * Description: This API reads computes the base to N-th power.
 *   for example power(10,2) = 100, power(10,0) = 1, power(2,1) = 2
 *
 * Inputs:
 *   base  Base value to raised to N-th power.
 *
 *   nth_power   Power to raise (positive only)
 * Outputs: None
 *
 * Returns: The pointer to the next non-whitespace char on input buffer.
 *---------------------------------------------------------------------------*/
unsigned long power(unsigned int base, unsigned int nth_power)
{
    unsigned long       result = base;
    unsigned int        x;

    if ( nth_power == 0 )
    {
        result = 1 ;
        return result;
    }
    
    for (x=1 ; x < nth_power; x++)
    {
        result = result * base;
    }
    return result;
}
    
/*----------------------------------------------------------------------------
 * Function: skip_ws_char
 *
 * Description: This API reads string buffer and skips white space until
 *    a none-whitespace char is found. If ps_str points to a 'whitespace',
 *    then this function just return without advancing the str pointer.
 *
 * Inputs:
 *    ps_str      References the input buffer.
 *
 * Outputs: None
 *
 * Returns: The pointer to the next non-whitespace char on input buffer.
 *---------------------------------------------------------------------------*/
static const CHAR*  skip_ws_char
(
    const CHAR*   ps_str
)
{
    CHAR          c_fmt_char;
    
    while ( (c_fmt_char=(*ps_str)) != '\0' )
    {
        if ( CHAR_IS_SPACE(c_fmt_char) )
        {
            ps_str++;
            continue;
        }
        else
        {
            break;
        }
    }
    return ps_str;
}

/*----------------------------------------------------------------------------
 * Function: assignment_ptr_type
 *
 * Description: This API parse the conversion specifier.
 *
 * Inputs:
 *   ps_str     References the input buffer (points to the first
 *              char after '%'.
 *
 * Outputs: 
 *   pb_valid   Boolean flag indicating if the conversion specifier
 *              has valid syntax.
 *
 * Returns: The pointer to the next char on input format string.
 *---------------------------------------------------------------------------*/
static XSSCANF_PTR_TYPE_T  assignment_ptr_type
(
    CHAR          c_conv_type,
    INT32         i4_size_spec
)
{
    XSSCANF_PTR_TYPE_T     e_ptr_type = PTR_NONE;
    
    switch (c_conv_type)
    {
        case 'd':
        case 'i':
        case 'n':
        {
            if ( i4_size_spec == PTR_SPEC_NONE )
            {
                e_ptr_type = PTR_INT;
            }
            if ( i4_size_spec == PTR_SPEC_H )
            {
                e_ptr_type = PTR_S_SHORT ;
            }
            if ( i4_size_spec == PTR_SPEC_LOWER_L )
            {
                e_ptr_type = PTR_S_LONG;
            }
            if ( i4_size_spec == PTR_SPEC_H_H )
            {
                e_ptr_type = PTR_CHAR;
            }
        }
        break;

        case 'u':
        case 'o':
        case 'x':
        {
            if ( i4_size_spec == PTR_SPEC_NONE )
            {
                e_ptr_type = PTR_U_INT;
            }
            if ( i4_size_spec == PTR_SPEC_H )
            {
                e_ptr_type = PTR_U_SHORT ;
            }
            if ( i4_size_spec == PTR_SPEC_LOWER_L )
            {
                e_ptr_type = PTR_U_LONG;
            }
            if ( i4_size_spec == PTR_SPEC_H_H )
            {
                e_ptr_type = PTR_CHAR;
            }
        }
        break;

        case 'c':
        case 's':
        case '[':
        {
            e_ptr_type = PTR_CHAR;
        }
        break;

        case 'f':
        case 'e':
        case 'g':
        {
            if ( i4_size_spec == PTR_SPEC_NONE )
            {
                e_ptr_type = PTR_FLOAT;
            }
            if ( i4_size_spec == PTR_SPEC_LOWER_L )
            {
                e_ptr_type = PTR_DOUBLE ;
            }
            if ( i4_size_spec == PTR_SPEC_UPPER_L )
            {
                e_ptr_type = PTR_LONG_DOUBLE;
            }
        }
        break;

        case 'p':
        {
            e_ptr_type = PTR_VOID_VOID;
        }
        break;

        default:
        {
            e_ptr_type = PTR_NONE;
        }
        break;
    }

    return e_ptr_type;
}
            
    
/*----------------------------------------------------------------------------
 * Function: parse_conv_spec
 *
 * Description: This API parse the conversion specifier.
 *
 * Inputs:
 *   ps_str     References the input buffer (points to the first
 *              char after '%'.
 *
 * Outputs:
 *   pt_conv_fmt  Pointer to a structure containing conversion information.
 *   
 *   pb_valid   Boolean flag indicating if the conversion specifier
 *              has valid syntax.
 *
 * Returns: The pointer to the next char on input format string.
 *---------------------------------------------------------------------------*/
static const CHAR*  parse_conv_spec
(
    const CHAR*   ps_fmt,
    CONV_FMT_T*   pt_conv_fmt,
    BOOL*         pb_valid
)
{
    CHAR          c_fmt_char;
    BOOL          b_exit = FALSE;
    
    *pb_valid                     = FALSE ;
    pt_conv_fmt->i4_max_fld_width = -1 ;  /* -1 means it is not set */
    pt_conv_fmt->i4_size_spec     = PTR_SPEC_NONE   ;
    pt_conv_fmt->b_suppres        = FALSE ;
    pt_conv_fmt->i4_nb_conv       =  0 ;
    x_memset(pt_conv_fmt->ac_conv_ops, 0, MAX_NB_CONV_OPS);
    
    while ( (b_exit == FALSE) && ((c_fmt_char=(*ps_fmt)) != '\0') )
    {
        if ( CHAR_IS_SPACE(c_fmt_char) )
        {
            c_fmt_char=' ';
        }

        switch (c_fmt_char)
        {
            case '-':
            case '+':
            case '#':
            {
                /* '-','+', '#' char are not accept by scanf()
                   coversion format */
                b_exit = TRUE;
            }
            break;
            
            case ' ':
            {
                b_exit = TRUE;
            }
            break;
                    
            case '*':
            {
                pt_conv_fmt->b_suppres = TRUE ;
                ps_fmt++;
            }
            break;
                
            case '[':
            {
                BOOL    b_first_symbol = TRUE;
                CHAR    c_input_symbol;

                /* for %[..], we expect to pointer to string buffer */
                pt_conv_fmt->i4_size_spec = PTR_SPEC_NONE ;

                pt_conv_fmt->ac_conv_ops[pt_conv_fmt->i4_nb_conv] = c_fmt_char ;

                /* Determine the expect the pointer type to receive this value. */
                pt_conv_fmt->e_ptr_type[pt_conv_fmt->i4_nb_conv]  =
                    assignment_ptr_type(c_fmt_char,pt_conv_fmt->i4_size_spec);
                
                pt_conv_fmt->i4_nb_conv++;

                pt_conv_fmt->bracket_fmt.b_negation = FALSE;
                pt_conv_fmt->bracket_fmt.i4_num_of_conv_char = 0 ;
                x_memset(pt_conv_fmt->bracket_fmt.char_set,
                         0,
                         sizeof(pt_conv_fmt->bracket_fmt.char_set));

                /* get next char. */
                ps_fmt++;
                c_input_symbol = *ps_fmt;
                    
                while (c_input_symbol != ']' )
                {
                    /* check the first input char symbole, if it is '^', then
                       this set is negation set. */
                    if ( b_first_symbol == TRUE )
                    {
                        b_first_symbol = FALSE ;

                        if ( c_input_symbol == '^' )
                        {
                            /* Set the negation flag and scan for more input
                               conversion chars. */
                            pt_conv_fmt->bracket_fmt.b_negation = TRUE ;

                            /* scan for next input conversion char. */
                            ps_fmt++;
                            c_input_symbol = *ps_fmt;
                            continue ;
                        }
                    }
                    
                    /* add the chars to the conversion set. */
                    pt_conv_fmt->bracket_fmt.char_set[pt_conv_fmt->bracket_fmt.i4_num_of_conv_char]
                        = c_input_symbol ;
                    pt_conv_fmt->bracket_fmt.i4_num_of_conv_char++ ;
                    
                    /* scan for next input conversion char. */
                    ps_fmt++;
                    c_input_symbol = *ps_fmt;
                }

                /* done. */
                b_exit = TRUE;

                if ( pt_conv_fmt->bracket_fmt.i4_num_of_conv_char > 0 )
                {
                    /* this is a valid conversion specifier. */
                    *pb_valid = TRUE;
                }
                else
                {
                    *pb_valid = FALSE;
                }
                
                ps_fmt++;
            }
            break;

            /* optional size specification.*/
            case 'h':    /* short for single 'h' and char for 'hh' */
            {
                if ( pt_conv_fmt->i4_size_spec == PTR_SPEC_NONE )
                {
                    pt_conv_fmt->i4_size_spec = PTR_SPEC_H ;
                }
                else if ( pt_conv_fmt->i4_size_spec == PTR_SPEC_H )
                {
                    /* already encounter a 'h' */
                    pt_conv_fmt->i4_size_spec = PTR_SPEC_H_H ;
                }
                ps_fmt++;
            }
            break;
            
            case 'l':    /* long   */
            {
                pt_conv_fmt->i4_size_spec = PTR_SPEC_LOWER_L ;
                ps_fmt++;
            }
            break;
            
            case 'L':    /* Long double */
            {
                
                pt_conv_fmt->i4_size_spec = PTR_SPEC_UPPER_L;
                ps_fmt++;
            }
            break;
            
            case '%':  /* match '%' in the input stream. */
            case 'd':
            case 'i':
            case 'u':
            case 'o':
            case 'x':
            case 'c':
            case 's':
            case 'p':
            case 'n':
            case 'f':
            case 'e':
            case 'g':
            {
                BOOL    b_accept = TRUE;
                /* check for invalid size specifier and conversion
                   letter combination. */
                if ( (c_fmt_char == 'c') || (c_fmt_char == 's') || (c_fmt_char == 'p') )
                {
                    if ( pt_conv_fmt->i4_size_spec != PTR_SPEC_NONE )
                    {
                        b_accept = FALSE;
                        b_exit = TRUE;
                    }   
                }

                if ( b_accept == TRUE )
                {
                    pt_conv_fmt->ac_conv_ops[pt_conv_fmt->i4_nb_conv] = c_fmt_char ;

                    /* Determine the expect the pointer type to receive this value. */
                    pt_conv_fmt->e_ptr_type[pt_conv_fmt->i4_nb_conv]  =
                        assignment_ptr_type(c_fmt_char,pt_conv_fmt->i4_size_spec);
                
                    pt_conv_fmt->i4_nb_conv++;

                    /* done. */
                    b_exit = TRUE;

                    /* this is a valid conversion specifier. */
                    *pb_valid = TRUE;
                }
                ps_fmt++;
            }
            break;
            
                
            default :
            {
                INT32    i4_fld_width = 0;
                
                if ( CHAR_IS_DIGIT(c_fmt_char) )
                {
                    /* Get the optional maximum field width */
                    while ( CHAR_IS_DIGIT(c_fmt_char) && (c_fmt_char != '\0') )
                    {
                        i4_fld_width *= 10 ;
                        i4_fld_width += (c_fmt_char - '0');
                        pt_conv_fmt->i4_max_fld_width = i4_fld_width;
                        ps_fmt++ ;
                        c_fmt_char = (*ps_fmt);
                    }
                }
                else
                {
                    /* field width must be decimal value only. */
                    b_exit = TRUE;
                }
            }
            break;
        }
    }
    
#if 0
    if ( (*pb_valid) == TRUE )
    {
        INT32     ix;
        INT32     n;
        
        /* we have processed a valid conversion format.
           %[width(digits:0-9)]
           [size_specifier(char:h,l,L)]
           [conversion letter(char:c,d,e,f,g,i,n,o,p,s,u,x,%,[]]
        */

        x_dbg_stmt("<found a valid conversion> ");
        
        x_dbg_stmt("%%");
        if ( pt_conv_fmt->i4_max_fld_width >= 0 )
        {
            x_dbg_stmt("%d",pt_conv_fmt->i4_max_fld_width);
        }

        if ( pt_conv_fmt->i4_size_spec >= 0 )
        {
            CHAR cx;
            cx=pt_conv_fmt->i4_size_spec;
            x_dbg_stmt("%c",cx);
        }

        for (ix=0; ix < pt_conv_fmt->i4_nb_conv ; ix++ )
        {
            x_dbg_stmt("%c", pt_conv_fmt->ac_conv_ops[ix]);

            if ( pt_conv_fmt->ac_conv_ops[ix] == '[' )
            {
                
                if ( pt_conv_fmt->bracket_fmt.b_negation == TRUE )
                {
                    x_dbg_stmt(" negate: ");
                }

                for ( n=0; n <  pt_conv_fmt->bracket_fmt.i4_num_of_conv_char ; n++ )
                {
                    x_dbg_stmt(" %c ", pt_conv_fmt->bracket_fmt.char_set[n]);
                }
                x_dbg_stmt(" ] ");
            }
        }
        x_dbg_stmt("\n");
    }
#endif
    
    return ps_fmt;
}



/*********************************************************************
*
*  Parse state control for floating point value.
*
**********************************************************************/
#define   ACCEPT_SIGN                       ((UINT32)1)
#define   ACCEPT_DIGIT                      ((UINT32)2)
#define   ACCEPT_DECIMAL_POINT              ((UINT32)4)
#define   ACCEPT_FRACTION                   ((UINT32)8)
#define   ACCEPT_EXPONENT_E                 ((UINT32)16)
#define   ACCEPT_EXPONENT_VALUE             ((UINT32)32)
#define   ACCEPT_EXPONENT_SIGN              ((UINT32)64)
#define   ACCEPT_THIS_FLOAT                 ((UINT32)128)
/************************************************************************
*  Structure to store parsed float value string.  The float value string
*  contains following format:
*  
* sign  float decimal fraction [exponent] [sign of exponent] [exponent value]
*
* [+|-] [0-9] [.]     [0-9]    [E|e]      [+|-]              [0-9]
*
*************************************************************************/
typedef struct _FLOAT_DATA_T_
{
    INT64      i8_value;
    INT32      i4_sign;
    INT32      i4_nb_decimal;
    INT32      i4_exponent_value;
    INT32      i4_exponent_sign;
}  FLOAT_DATA_T;

/*----------------------------------------------------------------------------
 * Function: get_double
 *
 * Description: This function computes the double value from the
 *   exponent and fraction values.
 *
 * Inputs:
 *   pt_float   References the structure containe parsed float value
 *              structure.
 *
 * Outputs:
 *   none.   
 *
 * Returns: converted long double value.
 *---------------------------------------------------------------------------*/
static long double  get_double
(
    FLOAT_DATA_T*   pt_float
)
{
    long double     dvalue;
    long            nth_power;

    nth_power = (pt_float->i4_exponent_sign * pt_float->i4_exponent_value)
        - pt_float->i4_nb_decimal;
    
    if ( nth_power < 0 )
    {
        nth_power = nth_power * (-1);
        dvalue = ((long double)(pt_float->i4_sign * pt_float->i8_value)) /
            ((long double)(power(10,(unsigned int)nth_power))) ;
    }
    else
    {
        dvalue = ((long double)(pt_float->i4_sign * pt_float->i8_value)) *
            ((long double)(power(10,(unsigned int)nth_power))) ;
    }
    return dvalue;
}

/*----------------------------------------------------------------------------
 * Function: parse_float
 *
 * Description: This API parse the input string for float/double.
 *
 * Inputs:
 *   ps_input_str  References the input buffer contain float/double
 *                 representation.
 *
 *   i4_field_max_width  Maximum field width to read from ps_input_str
 *                       buffer.
 *  
 * Outputs:
 *   pf_value   Contain the convert float/double value.
 *   
 *   pb_valid   Boolean flag indicating if the conversion is successful
 *              or not.
 *
 * Returns: The pointer to the next char on input format string.
 *---------------------------------------------------------------------------*/
static const CHAR*  parse_float
(
    const CHAR*        ps_input_str,
    INT32              i4_field_max_width,
    long double*       pf_value,
    BOOL*              pb_valid
)
{
    UINT32             e_state = 0 ;
    INT32              i4_nb_char_read = 0 ;
    INT32              i4_radix        = 10;
    const CHAR*        ps_str = ps_input_str;
    CHAR               c_inp_char;
    FLOAT_DATA_T       t_float ;

    t_float.i8_value  = 0;
    t_float.i4_sign   = 1;   /* positive value. */
    t_float.i4_nb_decimal = 0;
    t_float.i4_exponent_value = 0;
    t_float.i4_exponent_sign  = 1;

    *pb_valid = FALSE;
    *pf_value = 0;

    /* initial state [state 0] */
    e_state = ( ACCEPT_SIGN | ACCEPT_DIGIT | ACCEPT_DECIMAL_POINT ) ;

    /* skip all leading white space. */
    ps_str=skip_ws_char(ps_str);
    
    while ( ((c_inp_char=*ps_str) != '\0')           &&
            (i4_nb_char_read < i4_field_max_width)
        )
    {
        if ( (c_inp_char == '+') || (c_inp_char == '-') )
        {
            if ( (e_state & ACCEPT_SIGN ) )
            {
                e_state = ( e_state ^ ACCEPT_SIGN );

                /* sign must be followed by a digit, therefore
                   set the ACCEPT to negative. */
                if ( (e_state & ACCEPT_THIS_FLOAT) )
                {
                    e_state = ( e_state ^ ACCEPT_THIS_FLOAT );
                }
                
                if ( c_inp_char == '+' )
                {
                    t_float.i4_sign = 1;
                }
                else
                {
                    t_float.i4_sign = -1 ;
                }

                ps_str++;
                i4_nb_char_read++;
                continue;
            }
            else if ( (e_state & ACCEPT_EXPONENT_SIGN) )
            {
                e_state = ( e_state ^ ACCEPT_EXPONENT_SIGN );
                
                /* Exponent sign must be followed by a digit, therefore
                   we set the ACCEPT to negative until we
                   receive a digit subsequently.
                */
                if ( (e_state & ACCEPT_THIS_FLOAT) )
                {
                    e_state = ( e_state ^ ACCEPT_THIS_FLOAT );
                }
                
                if ( c_inp_char == '+' )
                {
                    t_float.i4_exponent_sign = 1;
                }
                else
                {
                    t_float.i4_exponent_sign = -1 ;
                }
                ps_str++;
                i4_nb_char_read++;
                continue;
            }
        }

        if ( (c_inp_char == '.') )
        {
            if ( (e_state & ACCEPT_DECIMAL_POINT) )
            {
                e_state = ( e_state ^ ACCEPT_DECIMAL_POINT );
                e_state = ( e_state | ACCEPT_FRACTION );
                ps_str++;
                i4_nb_char_read++;
                continue;
            }
        }
        
        if ( (CHAR_IS_DIGIT(c_inp_char)) )
        {
            if ( (e_state & ACCEPT_DIGIT) )
            {
                t_float.i8_value *= i4_radix;
                t_float.i8_value += (c_inp_char - '0');
                /*
                  If a digit is processed, then we don't accept 'sign'
                  for this input anymore.
                */
                if ( (e_state & ACCEPT_SIGN) )
                {
                    e_state = ( e_state ^ ACCEPT_SIGN );
                }
                e_state = ( e_state | ACCEPT_THIS_FLOAT | ACCEPT_EXPONENT_E );
            }

            if ( (e_state & ACCEPT_FRACTION) )
            {
                t_float.i4_nb_decimal++;
            }

            if ( (e_state & ACCEPT_EXPONENT_VALUE) )
            {
                t_float.i4_exponent_value *= i4_radix;
                t_float.i4_exponent_value += (c_inp_char - '0');
                if ( (e_state & ACCEPT_EXPONENT_SIGN) )
                {
                    e_state = ( e_state ^ ACCEPT_EXPONENT_SIGN );
                }
                e_state = ( e_state | ACCEPT_THIS_FLOAT );
            }
            
            ps_str++;
            i4_nb_char_read++;
            continue;    
        }

        if ( (c_inp_char=='E') || (c_inp_char=='e') )
        {
            if ( (e_state & ACCEPT_EXPONENT_E) )
            {
                /* only accept exponent sign or exponent value. */
                e_state = ( ACCEPT_EXPONENT_VALUE |
                            ACCEPT_EXPONENT_SIGN );
                ps_str++;
                i4_nb_char_read++;
                continue;
            }
        }

        /* anything else is not recognized, so we will break. */
        break;
    }

    if ( e_state & ACCEPT_THIS_FLOAT )
    {
        *pb_valid = TRUE;
        
        /* convert FLOAT data structure to actual long double value. */
        *pf_value = get_double(&t_float);
    }
    else
    {
        /* error state */
        *pb_valid = FALSE;
    }
    
    return ps_str;
}

/*----------------------------------------------------------------------------
 * Function: parse_input_str
 *
 * Description: This API parse the conversion specifier.
 *
 * Inputs:
 *   ps_str       References the input string to be interpreted.
 *
 *   pt_conv_fmt  References the conversion specification structure.
 *
 *   i4_i         Index to the conversion type to be process.
 *
 *   pv_output    Pointer to the output buffer to write out the converted value.
 * Outputs: 
 *   pb_valid   Boolean flag indicating if the conversion specifier
 *              has valid syntax.
 *
 * Returns: The pointer to the next char on input format string.
 *---------------------------------------------------------------------------*/
static const CHAR*  parse_input_str
(
    const CHAR*          ps_input_str,
    CONV_FMT_T*          pt_conv_fmt,
    INT32                i4_i,
    VOID*                pv_output, 
    BOOL*                pb_valid
    )
    {
        
    INT32                i4_field_max_width ;
    const CHAR*          ps_str ;
    CHAR                 c_inp_char;
    CHAR                 c_conv_type;
    XSSCANF_PTR_TYPE_T   e_ptr_type ;

    ps_str               = ps_input_str;
    *pb_valid            = FALSE ;

    /* check the array bound of the index for the conversion operation
       to be performed. */
    if ( i4_i >= MAX_NB_CONV_OPS )
    {
        return ps_str;
    }
    
    c_conv_type = pt_conv_fmt->ac_conv_ops[i4_i];

    e_ptr_type  = pt_conv_fmt->e_ptr_type[i4_i];
    
    /* If a maximum field width is specified, then only read
       up the maximum field width */
    if ( pt_conv_fmt->i4_max_fld_width != -1 )
    {
        i4_field_max_width = pt_conv_fmt->i4_max_fld_width ;
    }
    else
    {
        /* no maximum width specified, try a very large value. */
        i4_field_max_width = 1024*1024 ;
    }

    switch (c_conv_type)
    {
        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        {
            INT64    i8_val  = 0 ;
            INT32    i4_sign = 0 ;  /* 0 means sign +/- has not been
                                       consumed from the input.
                                       Once we process a +/-, and we
                                       will go to accept digit only
                                       state. */
            INT32    i4_power = 10;
            INT32    i4_nb_char_read = 0 ;

            if ( c_conv_type == 'o' )
            {
                i4_power = 8;
            }
            else if ( c_conv_type == 'x' )
            {
                i4_power = 16;
            }
            else if ( c_conv_type == 'i' )
            {
                /* need to read the input string to determine the base of
                   the number. */
                const CHAR*   ps_inp_symbol;
                INT32         i4_nb_symbol_read = 0;
                CHAR          ifmt_input_symbols[2];

                ifmt_input_symbols[0] = ' ' ;
                ifmt_input_symbols[1] = ' ' ;

                ps_inp_symbol=skip_ws_char(ps_str);
                while ( ((c_inp_char=*ps_inp_symbol) != '\0')  &&
                        (i4_nb_symbol_read < i4_field_max_width)
                    )
                {
                    ifmt_input_symbols[i4_nb_symbol_read] = c_inp_char ;
                    i4_nb_symbol_read++;
                    ps_inp_symbol++;
                    if ( i4_nb_symbol_read == 2 )
                    {
                        break;
                    }
                }
                if ( (ifmt_input_symbols[0] == '0')     &&
                     ((ifmt_input_symbols[1] == 'x') || (ifmt_input_symbols[1] == 'X'))
                    )
                {
                    c_conv_type = 'x';
                    i4_power=16;
                }
                else
                {
                    if ( ifmt_input_symbols[0] == '0')
                    {
                        c_conv_type = 'o';
                        i4_power = 8;
                    }
                }     
            }

            ps_str=skip_ws_char(ps_str);

            while ( ((c_inp_char=*ps_str) != '\0')       &&
                    (i4_nb_char_read < i4_field_max_width) 
                )
            {
                if ( (c_inp_char == '+') && ( i4_sign == 0 ) )
                {
                    i4_sign = 1;
                    ps_str++;
                    i4_nb_char_read++;
                    continue;
                }

                if ( (c_inp_char == '-') && ( i4_sign == 0 ) )
                {
                    i4_sign = -1 ;
                    ps_str++;
                    i4_nb_char_read++;
                    continue;
                }
                        

                if ( CHAR_IS_DIGIT(c_inp_char) )
                {
                    if ( c_conv_type == 'o' )
                    {
                        /* check if the 'octal number is valid. */
                        if ( (c_inp_char - '0') > 7 )
                        {
                            x_dbg_stmt("Numeric constant contains digits beyond the radix.\n");
                            /* invalid input. */
                            *pb_valid = FALSE;
                            break;
                        }
                    }        
                            
                    i8_val *= i4_power ;
                    i8_val += (c_inp_char - '0');
                    ps_str++;
                    i4_nb_char_read++;

                    /* after first digit is scanned, the sign is
                       set to positive.
                    */
                    if ( i4_sign == 0 )
                    {
                        i4_sign = 1;
                    }
                    /* Once a digit is processed,, we go to
                       accept digit only state.  Except for the
                       0x.... case.  For 0x...case, we should handle
                       it down below.
                    */
                    *pb_valid = TRUE;
                }
                else
                {
                    /* encounter a none-digit, need to handle the 0x case
                       if the conversion type expected is Hex
                    */
                    if ( (c_conv_type == 'x') )
                    {
                        INT32      i4_val = 0;

                        if ( c_inp_char == 'a' || c_inp_char == 'A' )
                        {
                            i4_val = 10 ;
                        }
                        else if ( c_inp_char == 'b' || c_inp_char == 'B' )
                        {
                            i4_val = 11 ;
                        }
                        else if ( c_inp_char == 'c' || c_inp_char == 'C' )
                        {
                            i4_val = 12 ;
                        }
                        else if ( c_inp_char == 'd' || c_inp_char == 'D' )
                        {
                            i4_val = 13 ;
                        }
                        else if ( c_inp_char == 'e' || c_inp_char == 'E' )
                        {
                            i4_val = 14 ;
                        }
                        else if ( c_inp_char == 'f' || c_inp_char == 'F' )
                        {
                            i4_val = 15 ;
                        }
                        else if ( c_inp_char == 'x' || c_inp_char == 'X' )
                        {
                            /* This is 0x input, at this point of parsing,
                               the accumulated input value should be 0.
                               Consume the 'x'|'X' input symbol and
                               continue scanning
                            */
                            if ( i8_val == 0 )
                            {
                                ps_str++;
                                i4_nb_char_read++;
                                continue;
                            }
                            else
                            {
                                /* invalid input symbol. */
                                break;
                            }
                        }
                        else
                        {
                            /* invalid input symbol */
                            break;
                        }

                        i8_val *= i4_power ;
                        i8_val += i4_val ;
                        ps_str++;
                        i4_nb_char_read++;

                        /* after first digit or letter a,b,c,d,e,f (for HEX) 
                           is scanned, the sign is set to positive.
                        */
                        if ( i4_sign == 0 )
                        {
                           i4_sign = 1;
                        }
                        /* Once a digit/a,b,c,d,e,f is processed,, we go to
                           accept digit/a,b,c,d,e,f only state.
                        */
                        *pb_valid = TRUE;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
                
#define MY_SSCANF_DBG 0
                
            if ( *pb_valid == TRUE )
            {
                if ( pt_conv_fmt->b_suppres == FALSE )
                {
                    switch (e_ptr_type)
                    {
                        case PTR_INT:
                        case PTR_S_LONG:
                        {
#if MY_SSCANF_DBG                                   
                            INT32   i4_val;
                            i4_val = i8_val * i4_sign ;
                            x_dbg_stmt("write INT32 [%d] to va_arg \n",i4_val);
#endif
                            *((INT32*) pv_output) = (INT32) (i8_val * i4_sign) ;
                        }
                        break;

                        case PTR_S_SHORT:
                        {
#if MY_SSCANF_DBG                                
                            INT16   i2_val;
                            i2_val =  (INT16) (i8_val * i4_sign) ;
                            x_dbg_stmt("write INT16 [%d] to va_arg \n",i2_val);
#endif
                            *((INT16*) pv_output) =  (INT16) (i8_val * i4_sign) ;
                        }
                        break;

                        case PTR_U_INT:
                        case PTR_U_LONG:
                        {
#if MY_SSCANF_DBG                               
                            UINT32  ui4_val;
                            ui4_val = (UINT32) (i8_val * i4_sign);
                            x_dbg_stmt("write INT32 [%d] to va_arg \n",ui4_val);
#endif
                            *((UINT32*) pv_output) = (UINT32) (i8_val * i4_sign) ;
                        }
                        break;

                        case PTR_U_SHORT:
                        {
#if MY_SSCANF_DBG                                
                            UINT16   ui2_val;
                            ui2_val =  (UINT16) (i8_val * i4_sign) ;
                            x_dbg_stmt("write INT16 [%d] to va_arg \n",ui2_val);
#endif
                            *((UINT16*) pv_output) =  (UINT16) (i8_val * i4_sign) ;                                
                        }
                        break;

                        case PTR_CHAR:
                        {
#if MY_SSCANF_DBG                                
                            CHAR   c_val;
                            c_val =  (CHAR) (i8_val * i4_sign) ;
                            x_dbg_stmt("write char [%c] to va_arg \n",c_val);
#endif
                            *((CHAR*) pv_output) =  (CHAR) (i8_val * i4_sign) ;                                
                        }
                        break;
                        
                        default:
                        {
                            /* No matching pointer type. something is wrong. */
                            *pb_valid = FALSE;
                        }
                        break;   
                    }
                }
            }
        }
        break;   
                            
        case 'c':
        {
            INT32    i4_nb_char_read = 0 ;
            CHAR*    pc_output;

            if ( pt_conv_fmt->i4_max_fld_width != -1 )
            {
                i4_field_max_width = pt_conv_fmt->i4_max_fld_width ;
            }
            else
            {
                /* no maximum width specified, should set to 1*/
                i4_field_max_width = 1 ;
            }

            pc_output = ((char*)(pv_output));
                
            /* Read input chars until end-of-line, or max number of width
               is reached. */
#if MY_SSCANF_DBG                
            x_dbg_stmt("<");
#endif                
            while ( ((c_inp_char=*ps_str)       != '\0')    &&
                    (i4_nb_char_read < i4_field_max_width)
                )
            {
                ps_str++;
                i4_nb_char_read++;
                *pb_valid = TRUE;
                
                if ( pt_conv_fmt->b_suppres == FALSE )
                {
                    *pc_output = c_inp_char;
                    pc_output++;
                }
#if MY_SSCANF_DBG                    
                x_dbg_stmt("%c",c_inp_char);
#endif                    
            }
#if MY_SSCANF_DBG                  
            x_dbg_stmt(">\n");
#endif                
        }
        break;

        case 's':
        {
            INT32    i4_nb_char_read = 0 ;
            CHAR*    ps_output;

            ps_output = ((char*)(pv_output));
                
            /* skip leading space. */
            ps_str=skip_ws_char(ps_str);

#if MY_SSCANF_DBG                
            x_dbg_stmt("<");
#endif
                
            /* Read input chars until a white space is encountered,
               or end-of-line, or max number of width is reached. */
            while ( ((c_inp_char=*ps_str)       != '\0')    &&
                    (CHAR_IS_SPACE(c_inp_char)  != TRUE)    &&
                    (i4_nb_char_read < i4_field_max_width)
                )
            {
                ps_str++;
                i4_nb_char_read++;
                *pb_valid = TRUE;
 
                if ( pt_conv_fmt->b_suppres == FALSE )
                {
                    *ps_output = c_inp_char;
                    ps_output++;
                }
#if MY_SSCANF_DBG                                   
                x_dbg_stmt("%c",c_inp_char);
#endif                    
            }
                
            *ps_output = '\0';
                
#if MY_SSCANF_DBG                  
            x_dbg_stmt("\0}\n");
#endif                
        }
        break;

        case 'f':
        case 'e':
        case 'g':
        {
            long double    f_value;
            /* Parse floating point number. */
            ps_str=parse_float(ps_str, i4_field_max_width, &f_value, pb_valid);
            if ( pt_conv_fmt->b_suppres == FALSE )
            {
                switch (e_ptr_type)
                {
                    case PTR_FLOAT:
                    {
#if MY_SSCANF_DBG                                   
                        FLOAT     f4_val;
                        f4_val = (FLOAT) f_value;
                        x_dbg_stmt("write float [%f] to va_arg \n",f4_val);
#endif
                        *((float*) pv_output) = (float) f_value;
                    }
                    break;

                    case PTR_DOUBLE:
                    {
#if MY_SSCANF_DBG                                   
                        DOUBLE    f4_val;
                        f4_val = f_value;
                        x_dbg_stmt("write double [%lf] to va_arg \n",f4_val);
#endif
                        *((double*) pv_output) = (double) f_value;
                    }
                    break;

                    case PTR_LONG_DOUBLE:
                    {
#if MY_SSCANF_DBG                                   
                        long double    f4_val;
                        f4_val = f_value;
                        x_dbg_stmt("write long double [%Lf] to va_arg \n",f4_val);
#endif
                        *((long double*) pv_output) = (long double) f_value;
                    }
                    break;

                    default:
                    {
                        /* No matching ptr type, something is wrong. */
                        *pb_valid = FALSE;
                    }
                    break;
                }
            }        
        }
        break;

        case '[' :
        {
            /* parse input data by matching (or negating) the input with
               chars from the conversion set specified inside [ ... ]
            */
            INT32    i4_nb_char_read = 0 ;
            CHAR*    ps_output;

            ps_output = ((char*)(pv_output));
                
            /* Read input chars until a char is encounter that is
               not in char_set (for matching), or for negation, a
               char in the char_set is encountered or max number of
               field width is reached.
            */
            
            while ( ((c_inp_char=(*ps_str)) != '\0')  &&
                    (i4_nb_char_read < i4_field_max_width)  )
            {
                
                if ( match_this_char( &(pt_conv_fmt->bracket_fmt),  c_inp_char)  == TRUE )
                {
                    if ( pt_conv_fmt->b_suppres == FALSE )
                    {
                        /*
                        x_dbg_stmt("save char: %c ", c_inp_char);
                        */
                        
                        *ps_output = c_inp_char;
                        ps_output++;
                    }

                    ps_str++;
                    i4_nb_char_read++;
                    *pb_valid = TRUE;
                }
                else
                {
                    /* terminating char encountered. */
                    break;
                }
            }
                
            *ps_output = '\0';

            /*
            x_dbg_stmt("\n[ conversion: %s\n", (char*)pv_output);
            */
            
        }
        break;
        
        default:
        {
            /* conversion type not supported. */
            *pb_valid = FALSE;
            x_dbg_stmt("Conversion type [%c] is not currently supported.\n",c_conv_type);
        }
        break;
    }
    return ps_str;
}

/*----------------------------------------------------------------------------
 * Function: vsscanf_generic
 *
 * Description: This API is a generic implementation of sscanf and is used by
 *              all the sscanf flavors (x_sscanf and x_vsscanf).
 *
 * Inputs:  ps_buf      References the output buffer.
 *          ps_fmt      References the format string.
 *          t_ap        Contains the list of parameters.
 *          z_max       Contains the maximum number of characters to copy.
 *          b_snprintf  Specifies whether z_max is relevant
 *
 * Outputs: -
 *
 * Returns: The number of characters written or that would have been written,
 *          not including the NULL character.
 *---------------------------------------------------------------------------*/
static INT32 vsscanf_generic
(
    const CHAR*        ps_input_string,
    const CHAR*        ps_format_string,
    VA_LIST            t_ap
)
{
    INT32          i4_nb_conv = 0;
    const CHAR*    ps_fmt = ps_format_string;
    const CHAR*    ps_buf = ps_input_string;
    BOOL           b_valid;
    BOOL           b_continue = TRUE;
    
    if ( (ps_buf == NULL) || (ps_fmt == NULL) )
    {
        return i4_nb_conv ;
    }
        
    while ( b_continue )
    {
        CHAR          c_fmt_char;
        CHAR          c_inp_char;
        CONV_FMT_T    t_conv_fmt;
            
        c_fmt_char = *ps_fmt ;
        c_inp_char = *ps_buf ;
        
        if ( (c_fmt_char != '\0') && (c_inp_char != '\0') )
        {
            if ( CHAR_IS_SPACE(c_fmt_char) )
            {
                /* skip white space on input string. */
                ps_buf=skip_ws_char(ps_buf);

                /* skip white space on format string. */
                ps_fmt=skip_ws_char(ps_fmt);
                
                continue;
            }
                
            if (c_fmt_char != '%')
            {
                /* Not whitespace or %, treat it as ordinary character,
                   check if it match the input pattern */
                if ( c_inp_char != c_fmt_char )
                {
                    break;
                }
                else
                {
                    ps_fmt++;
                    ps_buf++;
                    continue;
                }
            }
            else
            {
                ps_fmt++;
                ps_fmt=parse_conv_spec(ps_fmt, &t_conv_fmt, &b_valid);
                
                /* Format string contain a conversion specification. */
                if ( b_valid == TRUE )
                {
                    INT32     i4_i;
                    BOOL      b_suppress = t_conv_fmt.b_suppres ;
                    
                    for ( i4_i=0 ; i4_i < t_conv_fmt.i4_nb_conv ; i4_i++  )
                    {
                        /* get the assignment pointer.  */
                        switch (t_conv_fmt.e_ptr_type[i4_i])
                        {
                            case PTR_INT:
                            {
                                INT32*     pi4_ptr = NULL;

                                if ( b_suppress == FALSE )
                                {
                                    pi4_ptr = (INT32*) VA_ARG(t_ap, int*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pi4_ptr, &b_valid);
                            }
                            break;

                            case PTR_S_SHORT:
                            {
                                INT16*     pi2_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pi2_ptr = (INT16*) VA_ARG(t_ap, short*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pi2_ptr, &b_valid);
                            }
                            break;

                            case PTR_S_LONG:
                            {
                                INT32*     pi4_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pi4_ptr = (INT32*) VA_ARG(t_ap, long*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pi4_ptr, &b_valid);
                            }
                            break;

                            case PTR_U_INT:
                            {
                                UINT32*     pui4_ptr = NULL;
                                if (  b_suppress == FALSE )
                                {
                                    pui4_ptr = (UINT32*) VA_ARG(t_ap, unsigned int*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pui4_ptr, &b_valid);
                            }
                            break;

                            case PTR_U_SHORT:
                            {
                                UINT16*     pui2_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pui2_ptr = (UINT16*) VA_ARG(t_ap, unsigned short*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pui2_ptr, &b_valid);
                            }
                            break;

                            case PTR_U_LONG:
                            {
                                UINT32*     pui4_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pui4_ptr = (UINT32*) VA_ARG(t_ap, unsigned long*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pui4_ptr, &b_valid);
                            }
                            break;

                            case PTR_CHAR :
                            {
                                CHAR*     ps_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    ps_ptr = (CHAR*) VA_ARG(t_ap, char*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, ps_ptr, &b_valid);
                            }
                            break;

                            case PTR_FLOAT :
                            {
                                FLOAT*   pf_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pf_ptr = (FLOAT*) VA_ARG(t_ap, float*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pf_ptr, &b_valid);
                            }
                            break;

                            case PTR_DOUBLE:
                            {
                                DOUBLE*   pf_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pf_ptr = (DOUBLE*) VA_ARG(t_ap, double*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pf_ptr, &b_valid);
                            }
                            break;

                            case PTR_LONG_DOUBLE:
                            {
                                DOUBLE*   pf_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pf_ptr = (DOUBLE*) VA_ARG(t_ap, long double*);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pf_ptr, &b_valid);
                            }
                            break;

                            case PTR_VOID_VOID:
                            {
                                VOID**   pf_ptr = NULL;
                                if ( b_suppress == FALSE )
                                {
                                    pf_ptr = (VOID**) VA_ARG(t_ap, void**);
                                }
                                ps_buf=parse_input_str(ps_buf, &t_conv_fmt, i4_i, pf_ptr, &b_valid);
                            }
                            break;
                            
                            default:
                            {
                                
                            }
                            break;
                        }

                        if ( b_valid == TRUE)
                        {
                            i4_nb_conv++;
                        }
                        else
                        {
                            /* Stop processing the input string if we 
                               encounter an error parsing the input
                               string.
                            */
                            b_continue = FALSE ;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            break;
        }
    }
    
    return i4_nb_conv;
}

/*----------------------------------------------------------------------------
 * Function: x_vsscanf
 *
 * Description: This API calls vsscanf_generic, specifying that there is no
 *              limit on the output buffer (i.e. b_snprintf is FALSE).
 *
 * Inputs:  ps_buf  References the output buffer.
 *          ps_fmt  References the format string.
 *          t_ap    Contains the list of parameters.
 *
 * Outputs: -
 *
 * Returns: The number of characters written, not including the NULL character.
 *---------------------------------------------------------------------------*/
INT32 x_vsscanf
(
    const CHAR*        ps_buf,
    const CHAR*        ps_fmt,
    VA_LIST            t_ap
)
{
    return vsscanf_generic(ps_buf, ps_fmt, t_ap);
}

/*----------------------------------------------------------------------------
 * Function: c_vsscanf
 *
 * Description: This API calls vsscanf_generic, specifying that there is no
 *              limit on the output buffer (i.e. b_snprintf is FALSE).
 *
 * Inputs:  ps_buf  References the output buffer.
 *          ps_fmt  References the format string.
 *          t_ap    Contains the list of parameters.
 *
 * Outputs: -
 *
 * Returns: The number of characters written, not including the NULL character.
 *---------------------------------------------------------------------------*/
INT32 c_vsscanf
(
    const CHAR*        ps_buf,
    const CHAR*        ps_fmt,
    VA_LIST            t_ap
)
{
    return vsscanf_generic(ps_buf, ps_fmt, t_ap);
}

/*----------------------------------------------------------------------------
 * Function: x_sscanf
 *
 * Description: This API calls vsscanf_generic.
 *
 * Inputs:  ps_buf  References the output buffer.
 *          ps_fmt  References the format string.
 *          ...     Additional parameters (i.e. elements to output).
 *
 * Outputs: -
 *
 * Returns: The number of successful assignment performed.
 *---------------------------------------------------------------------------*/
INT32 x_sscanf(const CHAR *ps_buf, const CHAR *ps_fmt, ...)
{
    VA_LIST t_ap;
    INT32   i4_len;
    
    VA_START(t_ap, ps_fmt);
    
    i4_len = vsscanf_generic(ps_buf, ps_fmt, t_ap);
    
    VA_END(t_ap);
    
    return i4_len;
}

/*----------------------------------------------------------------------------
 * Function: c_sscanf
 *
 * Description: This API calls vsscanf_generic.
 *
 * Inputs:  ps_buf  References the output buffer.
 *          ps_fmt  References the format string.
 *          ...     Additional parameters (i.e. elements to output).
 *
 * Outputs: -
 *
 * Returns: The number of successful assignment performed.
 *---------------------------------------------------------------------------*/
INT32 c_sscanf(const CHAR *ps_buf, const CHAR *ps_fmt, ...)
{
    VA_LIST t_ap;
    INT32   i4_len;
    
    VA_START(t_ap, ps_fmt);
    
    i4_len = vsscanf_generic(ps_buf, ps_fmt, t_ap);
    
    VA_END(t_ap);
    
    return i4_len;
}

/*******************************************************************************/

