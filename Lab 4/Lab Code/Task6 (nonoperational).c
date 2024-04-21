/*
 * distance.c
 *
 * Created: 2/18/2024 7:11:19 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 4000000 //freq
#include <util/delay.h> 
#define BAUD_RATE 9600
volatile uint8_t data;
volatile char sent_data;
volatile uint8_t received_char = 0;

volatile char receivedData[4];  // Array to store received characters
volatile uint8_t dataIndex = 0; // Index to keep track of the position in the array
volatile uint8_t receiveCount = 0; // Counter to track the number of received characters

volatile uint8_t DATAH = 0; // Global variable to store high byte after FF pattern
volatile uint8_t DATAL = 0; // Global variable to store low byte after FF pattern
volatile float result;
char buffer[20]; // Adjust the buffer size based on your needs




// Function to send ASCII character over GPIO
void write(char character) {
	
	
	
	// Convert ASCII character to its hexadecimal value
	uint8_t hex_value = (uint8_t)character;

	// Start bit (assuming 1 start bit, 8 data bits, and 1 stop bit)
	PORTB.OUT = 0x00; // turn off the bit
	_delay_ms(1/BAUD_RATE);  // Adjust delay based on baud rate and system clock

	// Send 8 data bits (LSB first)
	for (int8_t i = 0; i < 8; ++i) {
		if (hex_value & (1 << i)) {
			PORTB.OUT = 0x10; // turn on the bit 
			} else {
			PORTB.OUT = 0x00; // turn off the bit
		}
		_delay_ms(1/BAUD_RATE); // Adjust delay based on baud rate and system clock
	}

	// Stop bit
	PORTB.OUT = 0x01; // turn on the bit
	_delay_ms(1/BAUD_RATE);  // Adjust delay based on baud rate and system clock
	
	
};

void UART_sw_write_string(const char* str) {
	// Loop through each character in the string and send it
	for (int i = 0; str[i] != '\0'; i++) {
		 write(str[i]);
	}
}



ISR(PORTB_PORT_vect) {


// False start check

_delay_ms((1/BAUD_RATE)/2);  // Adjust delay based on baud rate and system clock

if (PORTB.IN == 0b00000010) {
	// False start detected, handle accordingly
	received_char = 'P' ; // false start
};



// Receive 8 data bits (LSB first)
for (int8_t i = 0; i < 8; ++i) {
	_delay_ms(1/BAUD_RATE); 
	received_char |= (((PORTB.IN & 0b00000010) >> 1) << i);
	// Adjust delay based on baud rate and system clock
};
_delay_ms(1/BAUD_RATE); 
// Wait for the stop bit one bit later

if ((PORTB.IN & 0b00000010) == 0b00000010) {// i fthere is a 1 at the pin for stop bit;
	
}
else {
	
	received_char = 'P' ; // framing error
}


// Read the received data
receivedData[dataIndex] = received_char;

	// Increment the index for the next character
	dataIndex++;

	// Increment the receive counter
	receiveCount++;

	// Check if the index is within the array bounds
	if (dataIndex >= 4) {
		// Reset the index for the next round of data
		dataIndex = 0;

		// Check if 4 characters have been received
		if (receiveCount == 4) {
			// Reset the receive counter
			receiveCount = 0;

			// Search for the FF pattern
			for (uint8_t i = 0; i <= 2; i++) {
				if (receivedData[i] == 0xFF) {
					// Save the next 2 indexes to global variables DATAH and DATAL
					DATAH = receivedData[i + 1];
					DATAL = receivedData[i + 2];
							
					result = (uint16_t)DATAH << 8 | DATAL;
					result = (result*256)*.04;		
					UART_sw_write_string("Distance :");
						// Convert the floating-point number to a string
						sprintf(buffer, "%.1f", result);
						UART_sw_write_string(buffer);
						UART_sw_write_string("\n"); // go to next line in tera term

					// Clean the array and reset the counts
					for (uint8_t j = 0; j < 4; j++) {
						receivedData[j] = 0;
					}
					receiveCount = 0;
					
					break;  // Exit the loop after finding the FF pattern
				}
			}
		}

		// Clear the interrupt flag for the next go-around
		PORTB.INTFLAGS = 0x02;
	}
	
	return 1; 
}


int main(void)
{
	
    /* Replace with your application code */
	
	
//	SREG = 0b10000000; // disable global interrupts
	
	
	
	SREG |=  CPU_I_bm;
	
	PORTB.PIN1CTRL = 0x03;
	
	PORTB.DIR |= 0b00010000; // make TX and RX proper directions. //PB$ is the sending output now due to conflict 
	
//	USART3.BAUD = BAUD_RATE;      //baud rate
	
//	USART3.CTRLC =  0x03 ;    //frame format
	
 // USART3.CTRLA =  0b00010000 ;    //enable receive interrupt 
	
//	USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	
//	USART3.CTRLB |=  0b00010000;     // enable the false start check 
	
//	USART3.STATUS(6) = 1 means the data has been sent.
	
	
	
    while (1) 
    {
		asm volatile ("nop");
		
		
    }
}

