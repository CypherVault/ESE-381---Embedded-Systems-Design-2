/*
 * UART_sw_write.c
 *
 * Created: 2/18/2024 7:07:54 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h>
#define BAUD_RATE 9600 // <- just swap 9600 for what you need.



// Function to send ASCII character over GPIO
void write(char character) {
	
	
	// Convert ASCII character to its hexadecimal value
	uint8_t hex_value = (uint8_t)character;

	// Start bit (assuming 1 start bit, 8 data bits, and 1 stop bit)
	PORTB.OUT = 0x00; // turn off the bit
	_delay_us((1000000/BAUD_RATE));  // Adjust delay based on baud rate and system clock

	// Send 8 data bits (LSB first)
	for (int8_t i = 0; i < 8; ++i) {
		if (hex_value & (1 << i)) {
			PORTB.OUT = 0x01; // turn on the bit
			} else {
			PORTB.OUT = 0x00; // turn off the bit
		}
		_delay_us((1000000/BAUD_RATE)-10);  // Adjust delay based on baud rate and system clock
	}

	// Stop bit
	PORTB.OUT = 0x01; // turn on the bit
	_delay_us((1000000/BAUD_RATE));  // Adjust delay based on baud rate and system clock
	
	
	
	
	
	
	
	
	
};




int main(void)
{
    /* Replace with your application code */
	volatile char data = 'U';
		
	PORTB.DIR = 0x01; // make the whole port an input.; // make the single pin an output.
	
//	USART3.BAUD = 0b10010110000000;      //baud rate
	
//	USART3.CTRLC =  0x03 ;    //frame format
	
//	USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	
//  	USART3.TXDATAL = 
	
//	USART3.STATUS(5) = must be a 1 to send data.
	
//	USART3.STATUS(6) = 1 means the data has been sent.
	
	
	
    while (1) 
    {
		
		_delay_ms(500);
		
		write(data);
		
		
    }
}

