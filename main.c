/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "DS18B20.h"
#include <stdio.h>
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>

void USART_Init(unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter 
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}


void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)))
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void sendstring(unsigned char* data)
{
	while(*data != 0x00)
	{
		USART_Transmit(*data);
		data++;
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	board_init();
	USART_Init(MYUBRR);
	unsigned char num[20]={0};
	uint16_t temp;
	uint8_t configuration;
	float temperature = 0;
	/* Insert application code here, after the board has been initialized. */
	while(1)
	{
		//initialing the DS18B20
		initDS18B20();
		//sending in skip_rom command because only using 1 slave
		rom_cmd(SKIP_ROM);
		function_cmd(CONVERT_T);
		//checking to see if there is a successful response on the bus
		initDS18B20();
		//sending in skip_rom command because only using 1 slave
		rom_cmd(SKIP_ROM);
		temp = read_scratchpad(0);
		configuration = info[4];
		temperature = temp*0.0625;
		temperature = (temperature)*9/5 +32;
		char *tmpSign = (temperature < 0) ? "-" : "";
		float tmpVal = (temperature < 0) ? -temperature : temperature;
		int tmpInt1 = tmpVal;                  // Get the integer (678).
		float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
		int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).
		// Print as parts, note that you need 0-padding for fractional bit.
		sprintf (num, "Temperature is %s%d.%04d", tmpSign, tmpInt1, tmpInt2);
		sendstring(num);
		sendstring(" degrees Fahrenheit\r\n");
		_delay_ms(1000);
	}
}
