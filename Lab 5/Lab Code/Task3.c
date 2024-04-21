/*
 * USART3_loopback.c
 *
 * Created: 2/25/2024 1:42:58 PM
 * Author : MysticOwl
 */ 

#define F_CPU 4000000 //freq
#define USART3_BAUD_RATE(BAUD_RATE) ((float)((F_CPU * 64 )/ (16 * (float)BAUD_RATE)) + .5)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

volatile char charToSend;


typedef enum {
    SENDING = 0,
    OK,
    CONFLICT,
} status_t;


volatile status_t sendingStatus;

uint8_t toggleCase(uint8_t character) {
	if (character >= 'A' && character <= 'Z') {
		
		return (character+32);
		
	}
	else if (character >= 'a' && character <= 'z') {
		
		return (character-32);
	}
	else {
		// Return unchanged if the character is not a letter
		return  (character);
	}
}

// 
// status_t waitSendConfirmation(void)
// {
//     sendingStatus = SENDING;
//     /* Will change inside RXC interrupt handler */
//     while(sendingStatus == SENDING)
//     {
//         ;
//     }
//     return sendingStatus;

void waitTxReady()
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
}



ISR(USART3_RXC_vect)
{

//		sendingStatus = OK;
		
		USART3.CTRLA &= ~(USART_RXCIE_bm); // disABLE erruptsint
		
			
		waitTxReady(); // wait for transmit to be good
			
		charToSend = toggleCase(charToSend);  // toggle case
			
		USART3.TXDATAL = charToSend;  //send togged case char
	

}



void USART3_Send(char* str)
{
	USART3.CTRLA |= USART_RXCIE_bm; /* Receive Complete Interrupt must be enabled */
	 
	 for(size_t i = 0; i < strlen(str); i++)
	 {
		USART3.CTRLA |= USART_RXCIE_bm;
		 charToSend = str[i];
		 waitTxReady();
		 USART3.TXDATAL = charToSend;
		 		
	if( (USART3.STATUS & 0b10000000) != 0b10000000)
        {
            break;//interrupt should be right here 
        }
				  
		 
		 
		 
	 }
	
	USART3.CTRLA &= ~(USART_RXCIE_bm); 
}

int main(void)
{
   
	
	  /* Replace with your application code */
	//  volatile char data = 'S';
	  
	     sei();
	  SREG |=  CPU_I_bm;
	  
	  PORTB.DIR = 0x01; // make the whole port an input.; // make the single pin an output.
	  
	 USART3.BAUD =  (uint16_t)USART3_BAUD_RATE(9600);      //baud rate
	  
	  USART3.CTRLC =  0x03 ;    //frame format
	  
	  USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	    
	    USART3.CTRLA |= USART_LBME_bm;
	
    while (1) 
    {
		
		
		 USART3_Send("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		 _delay_ms(500);
		
		
		
    }
}

