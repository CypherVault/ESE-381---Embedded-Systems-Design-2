/*
 * interrupt_echo.c
 *
 * Created: 2/18/2024 7:09:49 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h> 
#define BAUD_RATE 9600
volatile uint8_t data;
volatile char sent_data;
#include <avr/interrupt.h>
volatile uint8_t received_char = 0;


uint8_t toggleCase(uint8_t character) {
	if (character >= 'A' && character <= 'Z') {
	
	received_char = received_char+32;
	
	}
	else if (character >= 'a' && character <= 'z') {
		
		received_char = received_char-32;
		} 
		else {
		// Return unchanged if the character is not a letter
		received_char = received_char;
	}
}



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



ISR(PORTB_PORT_vect) {

	
	_delay_us(((1000000/BAUD_RATE))/2);  // Adjust delay based on baud rate and system clock

	if((PORTB.IN & 0b00000010) != 0b00000000) { //false start check
		PORTB.INTFLAGS = 0x02;
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
	PORTB.INTFLAGS = 0x02;
	
	if ((PORTB.IN & 0b00000010) == 0b00000010) {// there is a 1 at the pin ;
		
		
		toggleCase(received_char);
		write(received_char);

		
	}
	else {
		PORTB.INTFLAGS = 0x02;
		return 'A'; // framing error
	}

// Clear the interrupt flag for the next go-around


};



int main(void)
{
	
    /* Replace with your application code */
	
	SREG |=  CPU_I_bm;
	PORTB.PIN1CTRL = 0x03;
	
	//SREG = 0b10000000; // disable global interrupts
	
	PORTB.DIR |= 0b00000001; // make TX and RX proper directions.
	
//	USART3.BAUD = BAUD_RATE;      //baud rate
	
//	USART3.CTRLC =  0x03 ;    //frame format
	
//	USART3.CTRLA =  0b00010000 ;    //enable receive interrupt 
	
//	USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	
//	USART3.CTRLB |=  0b00010000;     // enable the false start check 
	
//	USART3.STATUS(6) = 1 means the data has been sent.
	
	
	
    while (1) 
    {
		asm volatile ("nop");
		
		
    }
}

