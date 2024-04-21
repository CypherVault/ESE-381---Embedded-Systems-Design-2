/*
 * UART_sw_read.c
 *
 * Created: 2/18/2024 7:08:30 PM
 * Author : MysticOwl
 */ 


#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h> 
#define BAUD_RATE  9600 // <- just swap 9600 for what you need.
volatile uint8_t data;
uint8_t received_char = 0;






// Function to receive a byte over GPIO
char UART_sw_read() {
	// Wait for the start bit


	while ((PORTB.IN & 0b00000010) != 0b00000000) {
   
   asm volatile ("nop");
   
}
_delay_us(((1000000/BAUD_RATE))/2);  // Adjust delay based on baud rate and system clock

if((PORTB.IN & 0b00000010) != 0b00000000) { //false start check
   
  return 'P';
   
}

	

	// Receive 8 data bits (LSB first)
		for (int8_t i = 0; i < 8; ++i) {
			_delay_us((1000000/BAUD_RATE-10));
			received_char = received_char >> 1;
			received_char |= (((PORTB.IN & 0b00000010) << 6));
			// Adjust delay based on baud rate and system clock
			
		};
  _delay_us((1000000/BAUD_RATE));
	// Wait for the stop bit one bit later
	
	if ((PORTB.IN & 0b00000010) == 0b00000010) {// there is a 1 at the pin ;
    
	return (char)received_char;
	
	}
else {
	
	return 'A'; // framing error 
}

	
}







int main(void)
{
	volatile char data;
    /* Replace with your application code */
	
	SREG = 0b10000000; // disable global interupts
	PORTB.DIR = 0x00; // make the whole port an input.
	
//	USART3.BAUD = 0b10010110000000;      //baud rate
	
//	USART3.CTRLC =  0x03 ;    //frame format
	
//	USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	
//	USART3.CTRLB |=  0b00010000;     // enable the false start check 
	
//	USART3.STATUS(6) = 1 means the data has been sent.
	
	
    while (1) 
    {
		
		
		data =	UART_sw_read();
//	    _delay_ms(104.2);
		asm volatile ("nop");
	
    }
}

