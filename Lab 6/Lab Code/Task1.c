/*
 * usart3_init_test.c
 *
 * Created: 3/1/2024 11:48:39 PM
 * Author : MysticOwl
 */ 


#define F_CPU 4000000 //freq
#define USART3_BAUD_RATE(BAUD_RATE) ((float)((F_CPU * 64 )/ (16 * (float)BAUD_RATE)) + .5)
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

volatile uint8_t cntrlcBM; // collects the mask to assign to control C


void waitTxReady(void)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		asm volatile ("nop");
	}
}

void USART3_Send(char c)
{
	
	waitTxReady();
	
	USART3.TXDATAL = c;

}

void USART3_Init(uint16_t baud, uint8_t data_bits, unsigned char parity ){
	
 PORTB.DIR = 0x01; // make the whole port an input.; // make the single pin an output.	
	 
 USART3.BAUD =  (uint16_t)USART3_BAUD_RATE(baud);      //baud rate
 
 USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
 
	
 cntrlcBM =  0x00 ;    //frame format	
//data bits format:

switch(data_bits) {
	case 5:
	 cntrlcBM |= 0x00;
	break;
	case 6:
	cntrlcBM |= 0x01;
	break;
	case 7:
	cntrlcBM |= 0x02;
	break;
	case 8:
	cntrlcBM |= 0x03;
	break;
	case 9:
	cntrlcBM |= 0x06;
	break;
	case 10:
	cntrlcBM |= 0x07;
	break;
	default:
	cntrlcBM |= 0x00; //not valid choice
	break;
}

//stop bit mode:
 
cntrlcBM |= 0x00; //1 stop bit
//cntrlcBM |= 0x04; // 2 stop bits

//parity format:	 
	 
	 
	 switch(parity) {
		 case 'D':
		 cntrlcBM |= 0x00;
		 break;
		 case 'E':
		 cntrlcBM |= 0x20;
		 break;
		 case 'O':
		 cntrlcBM |= 0x30;
		 break;
		 default:
		 cntrlcBM |= 0x00; //not valid choice
		 break;
	 }
	 
	 USART3.CTRLC = cntrlcBM;
	
}

int main(void)
{
    /* Replace with your initialization frame data */


	volatile char data = 'S';
	USART3_Init( 9600 , 8, 'E'); // use 9 for 9BITL and 10 for 9BITH, for parity use D, E, or O.
	
	
    while (1) 
    {
		
		 USART3_Send(data);
		 _delay_ms(50);
		
    }
}

