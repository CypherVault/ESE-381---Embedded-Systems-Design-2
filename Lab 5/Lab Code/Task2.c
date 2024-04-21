/*
 * A_to_Z_async_Tx.c
 *
 * Created: 2/25/2024 1:42:33 PM
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

void USART3_Send(char* str)
{
	USART3.CTRLA |= USART_RXCIE_bm; /* Receive Complete Interrupt must be enabled */
	 
	 for(size_t i = 0; i < strlen(str); i++)
	 {
		 charToSend = str[i];
		 waitTxReady();
		 
		 USART3.TXDATAL = charToSend;
		 
		 
	 }
	
}

int main(void)
{
   
	
	  /* Replace with your application code */
	 volatile char data = 'S';
	  
	  PORTB.DIR = 0x01; // make the whole port an input.; // make the single pin an output.
	  
	 USART3.BAUD =  (uint16_t)USART3_BAUD_RATE(9600);      //baud rate
	  
	  USART3.CTRLC =  0x03 ;    //frame format
	  
	  USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	
	
	
    while (1) 
    {
		
		
		 USART3_Send("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		 _delay_ms(500);
		
		
		
    }
}

