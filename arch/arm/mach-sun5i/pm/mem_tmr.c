#include "pm_types.h"
#include "pm_i.h"


/*
*********************************************************************************************************
*                                     TIMER save
*
* Description: save timer for mem.
*
* Arguments  : none
*
* Returns    : EPDK_TRUE/EPDK_FALSE;
*********************************************************************************************************
*/
__s32 mem_tmr_save(struct tmr_state *ptmr_state)
{
	__mem_tmr_reg_t  *TmrReg;
	/* set timer register base */
	ptmr_state->TmrReg = TmrReg = (__mem_tmr_reg_t *)SW_VA_TIMERC_IO_BASE;
	
	/* backup timer registers */
	ptmr_state->TmrIntCtl   = TmrReg->IntCtl;
	ptmr_state->Tmr0Ctl     = TmrReg->Tmr0Ctl;
	ptmr_state->Tmr0IntVal  = TmrReg->Tmr0IntVal;
	ptmr_state->Tmr0CntVal  = TmrReg->Tmr0CntVal;
	ptmr_state->Tmr1Ctl     = TmrReg->Tmr1Ctl;
	ptmr_state->Tmr1IntVal  = TmrReg->Tmr1IntVal;
	ptmr_state->Tmr1CntVal  = TmrReg->Tmr1CntVal;
	
	return 0;
}


/*
*********************************************************************************************************
*                                     TIMER restore
*
* Description: restore timer for mem.
*
* Arguments  : none
*
* Returns    : EPDK_TRUE/EPDK_FALSE;
*********************************************************************************************************
*/
__s32 mem_tmr_restore(struct tmr_state *ptmr_state)
{
	__mem_tmr_reg_t  *TmrReg;

	/* set timer register base */
	TmrReg = ptmr_state->TmrReg;
	/* restore timer0 parameters */
	TmrReg->Tmr0IntVal  = ptmr_state->Tmr0IntVal;
	TmrReg->Tmr0CntVal  = ptmr_state->Tmr0CntVal;
	TmrReg->Tmr0Ctl     = ptmr_state->Tmr0Ctl;
	TmrReg->Tmr1IntVal  = ptmr_state->Tmr1IntVal;
	TmrReg->Tmr1CntVal  = ptmr_state->Tmr1CntVal;
	TmrReg->Tmr1Ctl     = ptmr_state->Tmr1Ctl;
	TmrReg->IntCtl      = ptmr_state->TmrIntCtl;
	
	return 0;
}
