/*
 * clk_main_32768Hz.c
 *
 * Created: 2/11/2024 1:51:11 PM
 * Author : Chris
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	
	
	CPU_CCP = CCP_IOREG_gc; //unprotect
	CLKCTRL_MCLKCTRLA = CLKCTRL_CLKSEL_XOSC32K_gc |  CLKCTRL_CLKOUT_bm ; // grab normal HF oscilliator and put it out.
	
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = CLKCTRL_ENABLE_bm;

	
	
    while (1) 
    {
    }
}

