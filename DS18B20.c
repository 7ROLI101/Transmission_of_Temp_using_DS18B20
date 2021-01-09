/*
 * DS18B20.c
 *
 * Created: 1/6/2021 10:50:22 PM
 *  Author: Aaron
 */ 
#include "DS18B20.h"
/*
*PURPOSE: To set up communication between the ATMEGA328P and the DSB1820
*PARAMETERS: N/A
*ASSUMES: The sensor output is connected to PB0
*RETURNS: 1 for failure, 0 for successful setup of communication
*/
int initDS18B20()
{
	//PREPARING THE MASTER TO TAKE CONTROL OF THE BUS
	//making PB0 an output pin
	DDRB |= (1<<PORTB0);
	//having the master send in a RESET pulse
	PORTB &= ~(1<<PORTB0);
	//waiting 480 us (pulse should be 480us minimum)
	_delay_us(480);
	//making PB0 an input pin with no pull up
	DDRB &= ~(1<<PORTB0);
	PORTB &= ~(1<<PORTB0);
	//PREPARING FOR THE DSB1820 TO TAKE CONTROL OF THE BUS
	//waiting for the DSB1820 to pull the line down
	//else if successful, check the bus for a response
	while(PINB&0x01){};
	_delay_us(480);
	return 0;
}

/*
*PURPOSE: Write 0's or 1's from the master to the DS18B20
*PARAMETERS: The bit(either 0 or 1) that you want to write
*ASSUMES: The sensor output is connected to PB0
*RETURNS: N/A
*/
void write_bit(uint8_t bit)
{
	//ensure 1us recovery time between slots
	_delay_us(1);
	
	//PREPARING THE MASTER TO TAKE CONTROL OF THE BUS
	//making PB0 an output pin
	DDRB |= (1<<PORTB0);
	//having the master send in a RESET pulse
	PORTB &= ~(1<<PORTB0);
	_delay_us(1);
	if(bit)
	{
		//release the bus
		DDRB &= ~(1<<PORTB0);
		PORTB &= ~(1<<PORTB0);
		_delay_us(20);
	}
	//wait 60us
		_delay_us(60);
		//release the bus
		DDRB &= ~(1<<PORTB0);
		PORTB &= ~(1<<PORTB0);
}
		

void write_zero(void)
{
	//ensure 1us recovery time between slots
	_delay_us(1);
	
	//PREPARING THE MASTER TO TAKE CONTROL OF THE BUS
	//making PB0 an output pin
	DDRB |= (1<<PORTB0);
	//having the master send in a RESET pulse
	PORTB &= ~(1<<PORTB0);
	//wait 60us
	_delay_us(60);
    //release the bus
	DDRB &= ~(1<<PORTB0);
	PORTB &= ~(1<<PORTB0);
}

void write_one()
{
	//ensure 1us recovery time between slots
	_delay_us(1);
	
	//PREPARING THE MASTER TO TAKE CONTROL OF THE BUS
	//making PB0 an output pin
	DDRB |= (1<<PORTB0);
	//having the master send in a RESET pulse
	PORTB &= ~(1<<PORTB0);
	_delay_us(1);
	//release the bus
	DDRB &= ~(1<<PORTB0);
	PORTB &= ~(1<<PORTB0);
}
/*
*PURPOSE: Read 0's or 1's from the DS18B20 to the master
*PARAMETERS: N/A
*ASSUMES: The sensor output is connected to PB0
*RETURNS: The bit recEIved from the DS18B20
*/
uint8_t read_bit()
{
	//ensure 1us recovery time between slots
	_delay_us(1);
	//PREPARING THE MASTER TO TAKE CONTROL OF THE BUS
	//making PB0 an output pin
	DDRB |= (1<<PORTB0);
	//having the master send in a RESET pulse
	PORTB &= ~(1<<PORTB0);
	//wait 1us
	_delay_us(3);
	//release the bus
	DDRB &= ~(1<<PORTB0);
	PORTB &= ~(1<<PORTB0);
	//wait 5us, to be sure that data from the sensor is valid
	_delay_us(5);
	
	//read the value of the bit
	uint8_t bit = (PINB&0x01);
	//have a delay to fulfill the 60us minimum
	_delay_us(40);
	return bit;
}

/*
*PURPOSE: Read bytes from the DS18B20 to the master
*PARAMETERS: N/A
*ASSUMES: The sensor output is connected to PB0
*RETURNS: The byte received from the DS18B20
*/
uint8_t read_byte()
{
	uint8_t byte=0;
	uint8_t bit=0;
	for(int i = 0;i<8;i++)
	{
		bit = read_bit();
		byte |= (bit<<i);
	}
	return byte;
}

/*
*PURPOSE: Write bytes from the master to the DS18B20
*PARAMETERS: The byte that you want to write
*ASSUMES: The sensor output is connected to PB0
*RETURNS: N/A
*/
void write_byte(uint8_t byte)
{
	uint8_t bitval = 0;
	for(int i = 0;i<8;i++)
	{
		bitval = (byte>>i) & 0x01;
		write_bit(bitval);
	}
}

//ROM COMMANDS
/*
*PURPOSE: Write the ROM_CMD 
*PARAMETERS:The ROM_CMD you want to write (either READ_ROM,MATCH_ROM,
SEARCH_ROM, ALARM_SEARCH or SKIP_ROM)
*ASSUMES: The sensor output is connected to PB0
*RETURNS: N/A
*/
void rom_cmd(uint8_t cmd)
{
	switch(cmd)
	{
		case READ_ROM:
		{
			read_rom_cmd();
			break;
		}
		
		case MATCH_ROM:
		{
			match_rom_cmd(0);
			break;
		}
		
		case SEARCH_ROM:
		{
			search_rom_cmd();
			break;
		}
		
		case ALARM_SEARCH:
		{
			alarm_search_cmd();
			break;
		}
		
		case SKIP_ROM:
		{
			skip_rom_cmd();
			break;
		}
	}
}

/*
*PURPOSE: Allows the master to read the slave's 64 bit ROM code without
using the SEARCH_ROM procedure
*PARAMETERS: N/A
*ASSUMES: The sensor output is connected to PB0, only one slave is 
connected to the bus
*RETURNS: N/A
*/
void read_rom_cmd()
{
	//sending out the read_rom command
	write_byte(READ_ROM);
	ROM_code[0] |= read_byte();
	for(int i = 0;i<6;i++)
	{
		ROM_code[0] |= ((uint64_t)read_byte()<<((i+1)*8));
	}
	ROM_code[0] |= ((uint64_t)read_byte()<<56);
}

/*
*PURPOSE: Allows the master to address a specific slave device on 
the bus(usually used for when either a single slave or multiple slaves
are on the bus)
*PARAMETERS: The index of the array signifying the device you want to access
*ASSUMES: The sensor output is connected to PB0, only one slave is
connected to the bus, and that the ROM code is valid for a device on the bus
*RETURNS: N/A
*/
void match_rom_cmd(int index)
{
	//sending out the match_rom command
	write_byte(MATCH_ROM);
	
	uint8_t byte;
	byte = ROM_code[index]&(uint64_t)0xFF;
	write_byte(byte);
	for(int i = 0;i<6;i++)
	{	
		byte = ROM_code[index]&((uint64_t)0xFF<<((i+1)*8));
		write_byte(byte);
	}
	byte = ROM_code[index]&&((uint64_t)0xFF<<56);
	write_byte(byte);
}

/*
*PURPOSE: Allows the master to address all devices on the bus
simultaneously without specifying a ROM code(be careful of data 
collision on the bus when using multiple slaves)
*PARAMETERS: N/A
*ASSUMES: The sensor output is connected to PB0, only one slave is
connected to the bus
*RETURNS: N/A
*/
void skip_rom_cmd()
{
	//sending out the skip_rom command
	write_byte(SKIP_ROM);
}

//WILL IMPLEMENT THESE IF NEEDED LATER
/*
*PURPOSE: Identifies all the ROM codes of the slave devices connected on the bus,
and stores them in an array ROM_codes[]
*PARAMETERS: N/A
*ASSUMES: The sensor output is connected to PB0, multiple slaves or single slave 
can be connected
connected to the bus
*RETURNS: N/A
*/
void search_rom_cmd()
{
	
}

/*
*PURPOSE: Allows the master device to determine if any slaves experience an alarm 
condition during the most recent temperature conversion
*PARAMETERS: N/A
*ASSUMES: The sensor output is connected to PB0, multiple slaves or single slave
can be connected
connected to the bus
*RETURNS: N/A
*/
void alarm_search_cmd()
{
	
}


//FUNCTION COMMANDS
/*
*PURPOSE: Write the FUNCTION_CMD
*PARAMETERS:The FUNCTION_CMD you want to write (either CONVERT_T, WRITE_SCRATCHPAD
READ_SCRATCHPAD, COPY_SCRATCHPAD, RECALL_E2, READ_POWER_SUPPLY)
*ASSUMES: The sensor output is connected to PB0
*RETURNS: N/A
*/
void function_cmd(uint8_t cmd)
{
	switch(cmd)
	{
		case CONVERT_T:
		{
			convert_temp();
			break;
		}
		
		case WRITE_SCRATCHPAD:
		{
			//write_scratchpad();
			break;
		}
		
		case COPY_SCRATCHPAD:
		{
			//copy_scratchpad();
			break;
		}
		
		case RECALL_E2:
		{
			//recall_e2();
			break;
		}
		case READ_POWER_SUPPLY:
		{
			read_power_supply();
			break;
		}
	}
}

/*
*PURPOSE: Initiates a single temperature conversion 
*PARAMETERS:N/A
*ASSUMES: The sensor output is connected to PB0
*RETURNS: Temperature data is stored in the 
scratchpad memory
*/
void convert_temp()
{
	//sending out the convert_temp command
	write_byte(CONVERT_T);
	
	//if the DS18B20 is powered with an external power supply  
	if(read_power_supply())
	{
		//DS18B20 begins conversion
		//while it is converting, wait
		while(!read_bit())
		{
			
		}
		return;
	}
	//if the DS18B20 has parasitic power applied
	else
	{
		//DS18B20 begins conversion
		//while it is converting, wait
		while(!read_bit())
		{
			//should have a max delay of 10us
			//after sending in the CONVERT_T 
			//command
			_delay_us(5);
			//have a pull-up PMOS connected PB1 and activate
			DDRB |= 1<<PORTB1;
			PORTB |= 0<<PORTB1;
			//wait for at least 5ms
			_delay_ms(5);
			//disable pullup resistor
			DDRB |= 0<<PORTB1;
		}
		return;
	}
}


/*
*PURPOSE: Used to check if parasite power is applied,
or if there is an external power supply used
*PARAMETERS:N/A
*ASSUMES: The sensor output is connected to PB0
*RETURNS: 1 if there is an external power supply, or
0 if there is parasite power applied
*/
bool read_power_supply()
{
	//sending out the read_power_supply command
	write_byte(READ_POWER_SUPPLY);
	return read_bit();
}

/*
*PURPOSE: Used to receive the data that is stored in the
scratchpad of the DS18B20. This is how data is stored:
BYTE 0 -----> TEMPERATURE LSB
BYTE 1 -----> TEMPERATURE MSB
BYTE 2 -----> TH REGISTER/USER BYTE 1
BYTE 3 -----> TL REGISTER/USER BYTE 2
BYTE 4 -----> CONFIGURATION REGISTER
BYTE 5 -----> RESERVED (FFh)
BYTE 6 -----> RESERVED
BYTE 7 -----> RESERVED (10h)
BYTE 8 -----> CRC
*PARAMETERS: Byte to stop at (number given will stop at that number - 1)
A zero will take in everything.
*ASSUMES: The sensor output is connected to PB0
*RETURNS: Data in bytes are received, starting from byte 0 going
to byte 8 at the very end. The receiving process can be canceled at 
any time by sending. Returns the data array with all data bytes received
*/

uint16_t read_scratchpad(int stop)
{
	write_byte(READ_SCRATCHPAD);
	if((stop == 0)||(stop>9))
	{
		stop = 9;
	}
	uint8_t data[9];
	for(int i = 0;i<stop;i++)
	{
		//reading bytes
		info[i] = read_byte();
	}
	uint8_t temp_msb = info[1];
	uint8_t temp_lsb = info[0];
	uint16_t temp = (temp_msb<<8)|(temp_lsb);
	//sending reset
	initDS18B20();
	return temp;
}