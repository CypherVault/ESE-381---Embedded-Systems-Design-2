/*
 * CLK_CPU_vs_CLK_PER.c
 *
 * Created: 2/11/2024 1:49:50 PM
 * Author : Chris
 */ 

#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
    PORTB.DIR |= PIN3_bm;
	
	CPU_CCP = CCP_IOREG_gc; //unprotect
	CLKCTRL_MCLKCTRLA = CLKCTRL_CLKSEL_OSCHF_gc; // grab normal HF oscilliator and put it out.
	CPU_CCP = CCP_IOREG_gc; //unprotect
		CLKCTRL_MCLKCTRLA |= CLKCTRL_CLKOUT_bm;
	PORTB_PIN2CTRL = 0x08;  //enable pullup resistor for SW0 for pushbutton

	
	while (1) 
    {
		
		VPORTB_OUT = VPORTB_IN << 1;
		
		if	( (VPORTB_IN & 0b00000100) == 0b00000000) { // if the whole port is ZERO, then the pushbutton is pressed which means stage 2 of testing.
			
			CPU_CCP = CCP_IOREG_gc; //unprotect
			CLKCTRL_MCLKCTRLB = CLKCTRL_PDIV_4X_gc;
			CPU_CCP = CCP_IOREG_gc; //unprotect
		 CLKCTRL_MCLKCTRLB |= CLKCTRL_PEN_bm;
			PORTB.OUT |= PIN3_bm;
			_delay_ms(500);
			PORTB.OUT &= ~PIN3_bm;
			_delay_ms(500);
			
		}
		
		else {
			
			
			PORTB.OUT |= PIN3_bm;
			_delay_ms(500);
			PORTB.OUT &= ~PIN3_bm;
			_delay_ms(500);
			
			
			
		}
		
		
		
		
		
    }
}




