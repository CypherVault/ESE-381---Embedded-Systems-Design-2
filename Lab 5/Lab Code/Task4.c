/*
 * USART3_echo_usb.c
 *
 * Created: 2/25/2024 1:43:17 PM
 * Author : MysticOwl
 */ 

#define F_CPU 4000000 //freq
#define USART3_BAUD_RATE(BAUD_RATE) ((float)((F_CPU * 64 )/ (16 * (float)BAUD_RATE)) + .5)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

volatile char charToSend;


void waitTxReady(void)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
}

void USART3_Send(char c)
{
	
	waitTxReady();
	
	USART3.TXDATAL = c;

	
}


ISR(USART3_RXC_vect)
{
	USART3.CTRLA &= ~(USART_RXCIE_bm); // stop interrupts
	
	charToSend = USART3.RXDATAL;
	
	USART3_Send(charToSend);
	
	USART3.CTRLA |= USART_RXCIE_bm; /* Receive Complete Interrupt must be enabled */

	
}




int main(void)
{
    /* Replace with your application code */
	
	
	  sei();
	SREG |=  CPU_I_bm;
	
	PORTB.DIR = 0x01; // make the whole port an input.; // make the single pin an output.
	
	USART3.BAUD =  (uint16_t)USART3_BAUD_RATE(9600);      //baud rate
	
	USART3.CTRLC =  0x03 ;    //frame format
	
	USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	
    USART3.CTRLA |= USART_RXCIE_bm; /* Receive Complete Interrupt must be enabled */
	
    while (1) 
    {
		 
		asm volatile ("nop");
		
    }
}

