/*
 *
 *  Copyright (C) ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR :   <xxx.yyy@st.com> */

#ifndef _HCL_DEFS_H
#define _HCL_DEFS_H


/* Type definition */

typedef unsigned char t_uint8;
typedef signed char t_sint8;
typedef unsigned short t_uint16;
typedef signed short t_sint16;
typedef unsigned long t_uint32;
typedef signed long t_sint32;
typedef unsigned int t_bitfield;

#if !defined(FALSE) &&  !defined(TRUE)   
typedef enum {FALSE, TRUE} t_bool;
#else /* FALSE & TRUE already defined */

typedef enum {BOOL_FALSE, BOOL_TRUE} t_bool;
#endif /* !defined(FALSE) &&  !defined(TRUE) */

/* Definition of the different kind of addresses manipulated into a system with    MMU (handle physical AND logical addresses) */
typedef t_uint32 t_physical_address;
typedef t_uint32 t_logical_address;

/* define a common enum for fequencies */
typedef enum {
	HCL_FREQ_192KHZ,	
	HCL_FREQ_176_4KHZ,    
	HCL_FREQ_128KHZ,
	HCL_FREQ_96KHZ,
	HCL_FREQ_88_2KHZ,
	HCL_FREQ_64KHZ,
	HCL_FREQ_48KHZ,
	HCL_FREQ_44_1KHZ,
	HCL_FREQ_32KHZ,
	HCL_FREQ_24KHZ,
	HCL_FREQ_22_05KHZ,
	HCL_FREQ_16KHZ,
	HCL_FREQ_12KHZ,
        HCL_FREQ_11_025KHZ,
        HCL_FREQ_8KHZ
} t_frequency;

typedef enum core_id
{
	ARM1 = 0,
	ARM2,
	RAS1,
	RAS2	
}t_CoreId;

typedef struct {
	t_physical_address physical;
	t_logical_address logical;
} t_system_address;

/*
 * Define a type used to manipulate size of various buffers
 */
typedef t_uint32 t_size;

typedef struct {
 t_bitfield minor:8;
 t_bitfield major:8;
 t_bitfield version:16;
} t_version;

/* Keyword definition */
#define PUBLIC       extern 
#define PRIVATE      static

#ifndef NULL
#define NULL     (0)
#endif 

#define HCL_ERROR                           (-1)
#define HCL_OK                              (0)
#define HCL_INTERNAL_EVENT                  (1)
#define HCL_REMAINING_PENDING_EVENTS        (2)
#define HCL_REMAINING_FILTER_PENDING_EVENTS (3)
#define HCL_NO_MORE_PENDING_EVENT           (4)
#define HCL_NO_MORE_FILTER_PENDING_EVENT    (5)
#define HCL_NO_PENDING_EVENT_ERROR          (7)

/* Bit setting or clearing */
#define HCL_SET_BITS(reg,mask)			((reg) |=  (mask))
#define HCL_CLEAR_BITS(reg,mask)		((reg) &= ~(mask))
#define HCL_READ_BITS(reg,mask)			((reg) &   (mask))
#define HCL_WRITE_BITS(reg,val,mask)	((reg) =   (((reg) & ~(mask)) | ((val) & (mask))))
#define HCL_READ_REG(reg)				(reg)
#define HCL_WRITE_REG(reg,val)			((reg) = (val))

/* field offset extraction from a structure */
#define FIELD_OFFSET(typeName, fieldName) (t_uint32)(&(((typeName *)0)->fieldName))

/* Bit mask definition */
#define MASK_NULL8    0x00
#define MASK_NULL16   0x0000
#define MASK_NULL32   0x00000000
#define MASK_ALL8     0xFF 
#define MASK_ALL16    0xFFFF 
#define MASK_ALL32    0xFFFFFFFF
#define MASK_BIT0     (1UL<<0)
#define MASK_BIT1     (1UL<<1) 
#define MASK_BIT2     (1UL<<2) 
#define MASK_BIT3     (1UL<<3) 
#define MASK_BIT4     (1UL<<4) 
#define MASK_BIT5     (1UL<<5) 
#define MASK_BIT6     (1UL<<6) 
#define MASK_BIT7     (1UL<<7) 
#define MASK_BIT8     (1UL<<8) 
#define MASK_BIT9     (1UL<<9) 
#define MASK_BIT10    (1UL<<10) 
#define MASK_BIT11    (1UL<<11) 
#define MASK_BIT12    (1UL<<12) 
#define MASK_BIT13    (1UL<<13) 
#define MASK_BIT14    (1UL<<14) 
#define MASK_BIT15    (1UL<<15) 
#define MASK_BIT16    (1UL<<16) 
#define MASK_BIT17    (1UL<<17) 
#define MASK_BIT18    (1UL<<18) 
#define MASK_BIT19    (1UL<<19) 
#define MASK_BIT20    (1UL<<20) 
#define MASK_BIT21    (1UL<<21)
#define MASK_BIT22    (1UL<<22) 
#define MASK_BIT23    (1UL<<23) 
#define MASK_BIT24    (1UL<<24) 
#define MASK_BIT25    (1UL<<25) 
#define MASK_BIT26    (1UL<<26) 
#define MASK_BIT27    (1UL<<27) 
#define MASK_BIT28    (1UL<<28) 
#define MASK_BIT29    (1UL<<29) 
#define MASK_BIT30    (1UL<<30)
#define MASK_BIT31    (1UL<<31) 

/* quartet shift definition */
#define MASK_QUARTET    (0xFUL)
#define SHIFT_QUARTET0  0
#define SHIFT_QUARTET1  4
#define SHIFT_QUARTET2  8
#define SHIFT_QUARTET3  12
#define SHIFT_QUARTET4  16
#define SHIFT_QUARTET5  20
#define SHIFT_QUARTET6  24
#define SHIFT_QUARTET7  28
#define MASK_QUARTET0   (MASK_QUARTET << SHIFT_QUARTET0)
#define MASK_QUARTET1   (MASK_QUARTET << SHIFT_QUARTET1)
#define MASK_QUARTET2   (MASK_QUARTET << SHIFT_QUARTET2)
#define MASK_QUARTET3   (MASK_QUARTET << SHIFT_QUARTET3)
#define MASK_QUARTET4   (MASK_QUARTET << SHIFT_QUARTET4)
#define MASK_QUARTET5   (MASK_QUARTET << SHIFT_QUARTET5)
#define MASK_QUARTET6   (MASK_QUARTET << SHIFT_QUARTET6)
#define MASK_QUARTET7   (MASK_QUARTET << SHIFT_QUARTET7)

/* Byte shift definition */
#define MASK_BYTE      (0xFFUL)
#define SHIFT_BYTE0	0
#define SHIFT_BYTE1	8
#define SHIFT_BYTE2	16
#define SHIFT_BYTE3	24
#define MASK_BYTE0      (MASK_BYTE << SHIFT_BYTE0)
#define MASK_BYTE1      (MASK_BYTE << SHIFT_BYTE1)
#define MASK_BYTE2      (MASK_BYTE << SHIFT_BYTE2)
#define MASK_BYTE3      (MASK_BYTE << SHIFT_BYTE3)
 
/* Halfword shift definition */
#define MASK_HALFWORD       (0xFFFFUL)
#define SHIFT_HALFWORD0	    0
#define SHIFT_HALFWORD1	    16
#define MASK_HALFWORD0      (MASK_HALFWORD << SHIFT_HALFWORD0)
#define MASK_HALFWORD1      (MASK_HALFWORD << SHIFT_HALFWORD1)

/* Global constants definition */
 #define ONE_KB        (1024)
 #define ONE_MB        (ONE_KB * ONE_KB)
 
 
/* Address translation macros declaration */
#if defined(__PLATFORM_MEK0) || defined(__PLATFORM_MEK1) || defined(__PLATFORM_MEK2) || defined(__PLATFORM_MEK3) || defined(__PLATFORM_MEK4)
#define ARM_TO_AHB_ADDR(addr) (addr | MASK_BIT31)
#define AHB_TO_ARM_ADDR(addr) (addr & ~MASK_BIT31)
#endif 

#if defined(__PLATFORM_MEVKLITE) || defined(__PLATFORM_MEVKFULL)
#define ARM_TO_AHB_ADDR(addr) (addr)
#define AHB_TO_ARM_ADDR(addr) (addr)
#endif 


#endif /* _HCL_DEFS_H */

/* End of file hcl_defs.h */
