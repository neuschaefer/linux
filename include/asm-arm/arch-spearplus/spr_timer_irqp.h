/*
 * linux/include/asm/arch/spr_timer_irqp.h
 *
 *  Copyright(C)2006 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*/


#ifndef _TMR_IRQP_H_
#define _TMR_IRQP_H_


#ifdef __cplusplus
extern "C"
{
#endif

/*------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------*/
/* Generic macros to access bit fields */

// SPEAR
#define TMR_HCL_FIELDMASK(fieldId)  ((t_uint32) (((((t_uint32) (~0)) >> (32 - (fieldId##width))) << (fieldId##shift))))

#define TMR_HCL_FIELDSET(datum, fieldId, val) \
    (datum) = ((t_uint32) (datum) & (~TMR_HCL_FIELDMASK(fieldId))) | ((t_uint32) (((t_uint32) (val)) << ((t_uint32) (fieldId##shift))) & (TMR_HCL_FIELDMASK(fieldId)))
    
#define TMR_HCL_FIELDGET(datum, fieldId) \
        ((t_uint32) (((datum) & ((t_uint32) TMR_HCL_FIELDMASK(fieldId))) >> (fieldId##shift)))

// our registers are of 16 bits */
// #define TMR_HCL_FIELDMASK(fieldId)  ((t_uint16) (((((t_uint16) (~0)) >> (16 - (fieldId##width))) << (fieldId##shift))))
//#define TMR_HCL_FIELDSET(datum, fieldId, val) \
    (datum) = ((t_uint16) (datum) & (~TMR_HCL_FIELDMASK(fieldId))) | ((t_uint16) (((t_uint16) (val)) << ((t_uint16) (fieldId##shift))) & (TMR_HCL_FIELDMASK(fieldId)))
//#define TMR_HCL_FIELDGET(datum, fieldId) \
        ((t_uint16) (((datum) & ((t_uint16) TMR_HCL_FIELDMASK(fieldId))) >> (fieldId##shift)))

#define TMR_INSTANCES_OF_TMR_UNITS  4

/*------------------------------------------------------------------------
 * New types
 *----------------------------------------------------------------------*/

/* C struct view */

typedef volatile struct
{
    t_uint16 tmr_control;			/* @0 */
	t_uint16 unused1;				/* @2 */

	t_uint16 tmr_status_int_ack ;   /* @4 */ /* Status & int ack are same Regs When read its Status & write its Int_ack*/
	t_uint16 unused2;				/* @6 */

    t_uint16 tmr_compare;			/* @8 */
	t_uint16 unused3;				/* @a */

    t_uint16 tmr_count;		    	/* @c */
	t_uint16 unused4;				/* @e */

    t_uint16 tmr_redge_capt;		/* @10 */
	t_uint16 unused5;				/* @12 */

    t_uint16 tmr_fedge_capt;		/* @14 */
	t_uint16 unused6;				/* @16 */

}t_tmr_register_subset;

typedef volatile struct
{
	t_uint8  unused_0[128];

    t_uint16 tmr_control1;				/* @0 */
	t_uint8  unused_1[2];

	t_uint16 tmr_status_int_ack1;    /* @4 */ /* Status & int ack are same Regs When read its Status & write its Int_ack */
	t_uint8  unused_2[2];				/* @6 */

    t_uint16 tmr_compare1;			/* @8 */
	t_uint8   unused_3[2];				/* @a */

    t_uint16 tmr_count1;			/* @c */
	t_uint8   unused_4[2];				/* @e */

    t_uint16 tmr_redge_capt1;		/* @10 */
	t_uint8   unused_5[2];				/* @12 */

    t_uint16 tmr_fedge_capt1;		/* @14 */
	t_uint8   unused_6[2];				/* @16 */

	t_uint8  unused_7[104];

	/* Registers for timer 2 */
	t_uint16 tmr_control2;			/* @1a */
	t_uint16 unused7;				/* @1c */

	t_uint16 tmr_status_int_ack2;    /* @1e */ /* Status & int ack are same Regs When read its Status & write its Int_ack */
	t_uint16 unused8;				/* @20 */

    t_uint16 tmr_compare2;			/* @22 */
	t_uint16 unused9;				/* @24 */

    t_uint16 tmr_count2;			/* @26 */
	t_uint16 unused10;				/* @28 */

    t_uint16 tmr_redge_capt2;		/* @2a */
	t_uint16 unused11;				/* @2c  */

    t_uint16 tmr_fedge_capt2;		/* @2e */
	t_uint16 unused12;				/* @30 */

} t_tmr_register;



#define TMR_TIMER_IDS_IN_TMR_UNIT0          ((t_uint32)TMR_ID_0 | (t_uint32)TMR_ID_1)
#define TMR_TIMER_IDS_IN_TMR_UNIT1          ((t_uint32)TMR_ID_2 | (t_uint32)TMR_ID_3)
#define TMR_TIMER_IDS_IN_TMR_UNIT2          ((t_uint32)TMR_ID_4 | (t_uint32)TMR_ID_5)
#define TMR_TIMER_IDS_IN_TMR_UNIT3          ((t_uint32)TMR_ID_6 | (t_uint32)TMR_ID_7)

#define TMR_NUMBER_OF_TIMERS_IN_ONE_UNIT    4

#define TMR_MASK_INT_UNIT0   0x0000003f  /* 111111: First 6 bits, each timer has 3 bits */
#define TMR_MASK_INT_UNIT1   0x00000fc0  /* 111111000000: next 6 bits */
#define TMR_MASK_INT_UNIT2   0x0003f000  /* 111111-000000-000000: next 6 bits */
#define TMR_MASK_INT_UNIT3   0x0fc00000  /* 111111-000000-000000-000000: next 6 bits */


/****************interrupt ENABLE registers access macros*******************/
/*
 * timer interrupt ENABLE register
 */
/* SPEAR#define DTU_TIMER_INTERRUPT_ENABLE      TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, TIMER_INTERRUPT_ENABLE, 1)  */
#define DTU_TIMER_INTERRUPT_ENABLE_0    TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control1, TIMER_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_INTERRUPT_ENABLE_1    TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control2, TIMER_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_INTERRUPT_ENABLE_2    TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control1, TIMER_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_INTERRUPT_ENABLE_3    TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control2, TIMER_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_INTERRUPT_ENABLE_4    TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control1, TIMER_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_INTERRUPT_ENABLE_5    TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control2, TIMER_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_INTERRUPT_ENABLE_6    TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control1, TIMER_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_INTERRUPT_ENABLE_7    TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control2, TIMER_INTERRUPT_ENABLE, 1)


#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_0  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control1, TIMER_FEDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_1  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control2, TIMER_FEDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_2  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control1, TIMER_FEDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_3  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control2, TIMER_FEDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_4  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control1, TIMER_FEDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_5  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control2, TIMER_FEDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_6  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control1, TIMER_FEDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_FEDGE_INTERRUPT_ENABLE_7  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control2, TIMER_FEDGE_INTERRUPT_ENABLE, 1)


#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_0  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control1, TIMER_REDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_1  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control2, TIMER_REDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_2  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control1, TIMER_REDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_3  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control2, TIMER_REDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_4  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control1, TIMER_REDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_5  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control2, TIMER_REDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_6  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control1, TIMER_REDGE_INTERRUPT_ENABLE, 1)
#define DTU_TIMER_REDGE_INTERRUPT_ENABLE_7  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control2, TIMER_REDGE_INTERRUPT_ENABLE, 1)


/*
 * timer interrupt ENABLE register
 */
#define TIMER_INTERRUPT_ENABLEwidth 1
#define TIMER_INTERRUPT_ENABLEshift 8   /*CHECK IT IN NOMADIK IT WAS 5 */

#define  TIMER_FEDGE_INTERRUPT_ENABLEwidth 1
#define  TIMER_FEDGE_INTERRUPT_ENABLEshift 9

#define  TIMER_REDGE_INTERRUPT_ENABLEwidth 1
#define  TIMER_REDGE_INTERRUPT_ENABLEshift 10

/****************interrupt DISABLE registers access macros*******************/
/*
 * timer interrupt DISABLE register
 */
/*SPEAR#define DTU_TIMER_INTERRUPT_DISABLE     (TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, TIMER_INTERRUPT_DISABLE, 0)) */
#define DTU_TIMER_INTERRUPT_DISABLE_0   (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control1, TIMER_INTERRUPT_DISABLE, 0))
#define DTU_TIMER_INTERRUPT_DISABLE_1   (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control2, TIMER_INTERRUPT_DISABLE, 0))
#define DTU_TIMER_INTERRUPT_DISABLE_2   (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control1, TIMER_INTERRUPT_DISABLE, 0))
#define DTU_TIMER_INTERRUPT_DISABLE_3   (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control2, TIMER_INTERRUPT_DISABLE, 0))
#define DTU_TIMER_INTERRUPT_DISABLE_4   (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control1, TIMER_INTERRUPT_DISABLE, 0))
#define DTU_TIMER_INTERRUPT_DISABLE_5   (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control2, TIMER_INTERRUPT_DISABLE, 0))
#define DTU_TIMER_INTERRUPT_DISABLE_6   (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control1, TIMER_INTERRUPT_DISABLE, 0))
#define DTU_TIMER_INTERRUPT_DISABLE_7   (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control2, TIMER_INTERRUPT_DISABLE, 0))


#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_0  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control1, TIMER_FEDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_1  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control2, TIMER_FEDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_2  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control1, TIMER_FEDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_3  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control2, TIMER_FEDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_4  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control1, TIMER_FEDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_5  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control2, TIMER_FEDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_6  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control1, TIMER_FEDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_FEDGE_INTERRUPT_DISABLE_7  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control2, TIMER_FEDGE_INTERRUPT_DISABLE, 0)


#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_0  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control1, TIMER_REDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_1  TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_control2, TIMER_REDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_2  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control1, TIMER_REDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_3  TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_control2, TIMER_REDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_4  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control1, TIMER_REDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_5  TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_control2, TIMER_REDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_6  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control1, TIMER_REDGE_INTERRUPT_DISABLE, 0)
#define DTU_TIMER_REDGE_INTERRUPT_DISABLE_7  TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_control2, TIMER_REDGE_INTERRUPT_DISABLE, 0)


/*
 * timer interrupt DISABLE register
 */
#define TIMER_INTERRUPT_DISABLEwidth    1
#define TIMER_INTERRUPT_DISABLEshift    8

#define  TIMER_FEDGE_INTERRUPT_DISABLEwidth 1
#define  TIMER_FEDGE_INTERRUPT_DISABLEshift 9

#define  TIMER_REDGE_INTERRUPT_DISABLEwidth 1
#define  TIMER_REDGE_INTERRUPT_DISABLEshift 10

/****************interrupt clear registers access macros*******************/
/*
 * timer interrupt clear register
 */
/*#define DTU_TIMER_INTERRUPT_CLEAR   (TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_icr, TIMER_INTERRUPT_CLEAR, 1)) */
#define DTU_TIMER_INTERRUPT_CLEAR_0 (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_status_int_ack1, TIMER_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_INTERRUPT_CLEAR_1 (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_status_int_ack2, TIMER_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_INTERRUPT_CLEAR_2 (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_status_int_ack1, TIMER_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_INTERRUPT_CLEAR_3 (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_status_int_ack2, TIMER_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_INTERRUPT_CLEAR_4 (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_status_int_ack1, TIMER_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_INTERRUPT_CLEAR_5 (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_status_int_ack2, TIMER_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_INTERRUPT_CLEAR_6 (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_status_int_ack1, TIMER_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_INTERRUPT_CLEAR_7 (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_status_int_ack2, TIMER_INTERRUPT_CLEAR, 1))


#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_0 (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_status_int_ack1, FEDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_1 (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_status_int_ack2, FEDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_2 (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_status_int_ack1, FEDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_3 (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_status_int_ack2, FEDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_4 (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_status_int_ack1, FEDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_5 (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_status_int_ack2, FEDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_6 (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_status_int_ack1, FEDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_FEDGE_INTERRUPT_CLEAR_7 (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_status_int_ack2, FEDGE_INTERRUPT_CLEAR, 1))


#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_0 (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_status_int_ack1, REDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_1 (TMR_HCL_FIELDSET(gp_tmr_register[0]->tmr_status_int_ack2, REDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_2 (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_status_int_ack1, REDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_3 (TMR_HCL_FIELDSET(gp_tmr_register[1]->tmr_status_int_ack2, REDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_4 (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_status_int_ack1, REDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_5 (TMR_HCL_FIELDSET(gp_tmr_register[2]->tmr_status_int_ack2, REDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_6 (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_status_int_ack1, REDGE_INTERRUPT_CLEAR, 1))
#define DTU_TIMER_REDGE_INTERRUPT_CLEAR_7 (TMR_HCL_FIELDSET(gp_tmr_register[3]->tmr_status_int_ack2, REDGE_INTERRUPT_CLEAR, 1))


/*
 * timer interrupt clear registers
 */
#define TIMER_INTERRUPT_CLEARwidth  1
#define TIMER_INTERRUPT_CLEARshift  0

#define FEDGE_INTERRUPT_CLEARwidth  1
#define FEDGE_INTERRUPT_CLEARshift  1

#define REDGE_INTERRUPT_CLEARwidth  1
#define REDGE_INTERRUPT_CLEARshift  2

/*SPEAR To Do: add the int clear for Capture fns */
/****************masked interrupt status register access macros*******************/
#define DTU_INTERRUPT_STATUS_0   (TMR_HCL_FIELDGET(gp_tmr_register[0]->tmr_status_int_ack1, TIMER_MASKED_INTERRUPT_STATUS))
#define DTU_INTERRUPT_STATUS_1   (TMR_HCL_FIELDGET(gp_tmr_register[0]->tmr_status_int_ack2, TIMER_MASKED_INTERRUPT_STATUS))
#define DTU_INTERRUPT_STATUS_2   (TMR_HCL_FIELDGET(gp_tmr_register[1]->tmr_status_int_ack1, TIMER_MASKED_INTERRUPT_STATUS))
#define DTU_INTERRUPT_STATUS_3   (TMR_HCL_FIELDGET(gp_tmr_register[1]->tmr_status_int_ack2, TIMER_MASKED_INTERRUPT_STATUS))
#define DTU_INTERRUPT_STATUS_4   (TMR_HCL_FIELDGET(gp_tmr_register[2]->tmr_status_int_ack1, TIMER_MASKED_INTERRUPT_STATUS))
#define DTU_INTERRUPT_STATUS_5   (TMR_HCL_FIELDGET(gp_tmr_register[2]->tmr_status_int_ack2, TIMER_MASKED_INTERRUPT_STATUS))
#define DTU_INTERRUPT_STATUS_6   (TMR_HCL_FIELDGET(gp_tmr_register[3]->tmr_status_int_ack1, TIMER_MASKED_INTERRUPT_STATUS))
#define DTU_INTERRUPT_STATUS_7   (TMR_HCL_FIELDGET(gp_tmr_register[3]->tmr_status_int_ack2, TIMER_MASKED_INTERRUPT_STATUS))


#define TIMER_MASKED_INTERRUPT_STATUSwidth  3
#define TIMER_MASKED_INTERRUPT_STATUSshift  0

#ifdef __cplusplus
}   /* allow C++ to use these headers */
#endif /* __cplusplus */
#endif /* _TMR_IRQP_H_*/

/* End of file - tmr_irqp.h*/

