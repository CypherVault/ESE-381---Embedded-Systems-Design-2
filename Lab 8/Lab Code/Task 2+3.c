/*
 * DOG_LCD_BasicTest.c
 *
 * Created: 3/25/2024 11:12:25 PM
 * Author : MysticOwl
 */ 
#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h>
#include <stdio.h>

// Display buffer for DOG LCD using sprintf()
char dsp_buff1[17];
char dsp_buff2[17];
char dsp_buff3[17];

void lcd_spi_transmit_CMD (char cmd);  //macro for multiple write spi functions for a setup command
void lcd_spi_transmit_DATA (char cmd); //macro for multiple write spi functions for any data to be sent
void init_spi_lcd (void); //init spi0 settings for avr
void init_lcd_dog (void); //finish init commands for dog
void update_lcd_dog(void); //send buffer for line data

void init_spi_lcd (){
	
	PORTA.DIR |= PIN4_bm; /* Set MOSI pin direction to output */
	PORTA.DIR &= ~PIN5_bm; /* Set MISO pin direction to input */
	PORTA.DIR |= PIN6_bm; /* Set SCK pin direction to output */
	PORTA.DIR |= PIN7_bm; /* Set SS pin direction to output */
	PORTA.OUT |= PIN7_bm; /* Set SS pin direction to output */
	
	PORTC.DIR |= PIN0_bm; //Reg select output to the display memory
	PORTC.OUT &= ~PIN0_bm;
	
	SPI0.CTRLB |= (SPI_SSD_bm | 0x03 ); // mode 3 as per the dog waveforms 
	
	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm;
	
	init_lcd_dog();
}


void lcd_spi_transmit_CMD (char data){
	
	PORTA_OUT &= ~PIN7_bm; //Slave select ON
	PORTC.OUT &= ~PIN0_bm; // register select 0, command setting
	SPI0.DATA = data;

	while (!(SPI0.INTFLAGS & SPI_IF_bm))  /* waits until data is exchanged*/
	{
		asm volatile ("nop");
	}
	volatile uint8_t dummy;
	dummy = SPI0_DATA;
	
	
	PORTF_OUT = PIN7_bm; //Slave select OFF 
	
}

void lcd_spi_transmit_DATA (char data){
	
	PORTA_OUT &= PIN7_bm; //Slave select ON
	PORTC.OUT = PIN0_bm;  // register select 1, data setting
	SPI0.DATA = data;

	while (!(SPI0.INTFLAGS & SPI_IF_bm))  /* waits until data is exchanged*/
	{
	asm volatile ("nop");
	}


	PORTF_OUT = PIN7_bm; //Slave select OFF
	
}



void init_lcd_dog(){
	
		//start_dly_40ms:
		_delay_ms(90);    //startup delay.
		
		//func_set1:
		lcd_spi_transmit_CMD(0x39);   // send function set #1 //tell for 3 lines and data interface at 8 bits
		_delay_us(30);	//delay for command to be processed


		//func_set2:
		lcd_spi_transmit_CMD(0x39);	//send function set #2 // again??
		_delay_us(30);	//delay for command to be processed


		//bias_set:
		lcd_spi_transmit_CMD(0x1E);	//set bias value.
		_delay_us(30);	//delay for command to be processed


		//power_ctrl:
		lcd_spi_transmit_CMD(0x55);	//~ 0x50 nominal for 5V
		//~ 0x55 for 3.3V (delicate adjustment).
		_delay_us(30);	//delay for command to be processed


		//follower_ctrl:
		lcd_spi_transmit_CMD(0x6C);	//follower mode on...
		_delay_ms(220);	//delay for command to be processed SPECIAL CASE 


		//contrast_set:
		lcd_spi_transmit_CMD(0x7F);	//~ 77 for 5V, ~ 7F for 3.3V
		_delay_us(30);	//delay for command to be processed


		//display_on:
		lcd_spi_transmit_CMD(0x0c);	//display on, cursor off, blink off
		_delay_us(30);	//delay for command to be processed
	

		//clr_display:
		lcd_spi_transmit_CMD(0x01);	//clear display, cursor home
		_delay_us(420);	//delay for command to be processed
		

		//entry_mode:
		lcd_spi_transmit_CMD(0x06);	//clear display, cursor home
		_delay_us(30);	//delay for command to be processed
		
	
}



// Updates the LCD display lines 1, 2, and 3, using the
// contents of dsp_buff_1, dsp_buff_2, and dsp_buff_3, respectively.
void update_lcd_dog(void) {

	init_spi_lcd();		//init SPI port for LCD.
	
	
	
	// send line 1 to the LCD module.
	lcd_spi_transmit_CMD(0x80);	//init DDRAM addr-ctr
	_delay_us(30);	//delay for command to be processed
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff1[i]);
		_delay_us(30);	//delay for command to be processed
	}
	
	// send line 2 to the LCD module.
	lcd_spi_transmit_CMD(0x90);	//init DDRAM addr-ctr
	_delay_us(30);	//delay for command to be processed
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff2[i]);
		_delay_us(30);	//delay for command to be processed
	}
	
	// send line 3 to the LCD module.
	lcd_spi_transmit_CMD(0xA0);	//init DDRAM addr-ctr
	_delay_us(30);	//delay for command to be processed
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff3[i]);
		_delay_us(30);	//delay for command to be processed
	}
}




int main(void)
{
  
	init_spi_lcd();
  
    while (1) 
    {
		
		sprintf(dsp_buff1, "     ESE 381    ");
		sprintf(dsp_buff2, "Variable Voltage");
		sprintf(dsp_buff3, "   MAX5402 Lab  ");
		update_lcd_dog();
		
    }
}



