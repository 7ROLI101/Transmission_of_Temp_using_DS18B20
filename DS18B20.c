/*
 * DS18B20.c
 *
 * Created: 1/6/2021 10:50:22 PM
 *  Author: Aaron
 */ 

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
	//checking to see if there is a response
	_delay_us(60);
	//if no response, then restart
	if(PINB&0X01)
	{
		return 1;
	};
	//else if successful, check the bus for a response
	while(PINB&0x01){};
	return 0;
}

int read_rom_cmd()
{
	//sending out the read_rom command
	write_cmd(READ_ROM);
	//find a way to read in the values and save the data
	return 0;
}

void write_bit(uint8_t bit)
{
	//PREPARING THE MASTER TO TAKE CONTROL OF THE BUS
	//making PB0 an output pin
	DDRB = (1<<PORTB0);
	//having the master pull the bus low
	PORTB = (0<<PORTB0);
	//wait 1us
	_delay_us(1);
	if(bit)
	{
	//release the bus
	DDRB &= ~(1<<PORTB0);
	PORTB &= ~(1<<PORTB0);
	}
	//wait 60us
	_delay_us(60);
	//release the bus
	DDRB &= ~(1<<PORTB0);
	PORTB &= ~(1<<PORTB0);
}

void write_cmd(uint8_t cmd)
{	
	uint8_t bitval = 0;
	for(int i = 0;i<8;i++)	
	{
		bitval = (cmd>>i) & 0x01;
		write_bit(bitval);
	}
}
