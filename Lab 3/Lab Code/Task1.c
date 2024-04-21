  /*
 * one_MHz.c
 *
 * Created: 2/11/2024 1:46:18 PM
 * Author : Chris
 */ 

#include <avr/io.h>


int main(void)
{
	/* Replace with your application code */
	
	CPU_CCP = CCP_IOREG_gc; //unprotect
	CLKCTRL_MCLKCTRLA = CLKCTRL_CLKSEL_OSCHF_gc;
	
	CPU_CCP = CCP_IOREG_gc; //unprotect
	CLKCTRL_MCLKCTRLA |= CLKCTRL_CLKOUT_bm;
	
	CPU_CCP = CCP_IOREG_gc; //unprotect
	CLKCTRL_MCLKCTRLB = CLKCTRL_PDIV_4X_gc;
	
	
	CPU_CCP = CCP_IOREG_gc; //unprotect
	CLKCTRL_MCLKCTRLB |= CLKCTRL_PEN_bm;


	
	
	while (1)
	{
	}
}

