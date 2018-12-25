#include "pm_types.h"
#include "pm_i.h"

/*
*********************************************************************************************************
*                                       MEM INTERRUPT INITIALISE
*
* Description: mem interrupt initialise.
*
* Arguments  : none.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__s32 mem_int_save(struct int_state *pint_state)
{
	int i=0;
	/* save interrupt controller registers */
	/*__mem_int_reg_t  *IntcReg;
	pint_state->IntcReg = IntcReg = (__mem_int_reg_t *)SW_VA_INT_IO_BASE;
	*/
	
	/*pint_state->IrqEnReg[0] = IntcReg->IrqEn[0];
	pint_state->IrqEnReg[1] = IntcReg->IrqEn[1];
	pint_state->IrqEnReg[2] = IntcReg->IrqEn[2];
	pint_state->IrqMaskReg[0] = IntcReg->IrqMask[0];
	pint_state->IrqMaskReg[1] = IntcReg->IrqMask[1];
	pint_state->IrqMaskReg[2] = IntcReg->IrqMask[2];
	pint_state->IrqSelReg[0] = IntcReg->TypeSel[0];
	pint_state->IrqSelReg[1] = IntcReg->TypeSel[1];
	pint_state->IrqSelReg[2] = IntcReg->TypeSel[2];
	*/

	/*save all the int reg*/
	for(i=0; i<(INT_REG_LENGTH); i++){
		pint_state->int_reg_back[i] = *(volatile __u32 *)(SW_VA_INT_IO_BASE + i*0x04); 
	}
	return 0;
}


/*
*********************************************************************************************************
*                                       MEM INTERRUPT INITIALISE
*
* Description: mem interrupt initialise.
*
* Arguments  : none.
*
* Returns    : 0/-1;
*********************************************************************************************************
*/
__s32 mem_int_restore(struct int_state *pint_state)
{
	int i=0;
	/* restore interrupt registers */
	//__mem_int_reg_t  *IntcReg = pint_state->IntcReg;

	/*IntcReg->IrqEn[0] = pint_state->IrqEnReg[0];
	IntcReg->IrqEn[1] = pint_state->IrqEnReg[1];
	IntcReg->IrqEn[2] = pint_state->IrqEnReg[2];
	IntcReg->IrqMask[0] = pint_state->IrqMaskReg[0];
	IntcReg->IrqMask[1] = pint_state->IrqMaskReg[1];
	IntcReg->IrqMask[2] = pint_state->IrqMaskReg[2];
	IntcReg->TypeSel[0] = pint_state->IrqSelReg[0];
	IntcReg->TypeSel[1] = pint_state->IrqSelReg[1];
	IntcReg->TypeSel[2] = pint_state->IrqSelReg[2];
	*/

	/*restore all the int reg*/
	for(i=0; i<(INT_REG_LENGTH); i++){
		 *(volatile __u32 *)(SW_VA_INT_IO_BASE + i*0x04) = pint_state->int_reg_back[i];
	}
	return 0;
}
