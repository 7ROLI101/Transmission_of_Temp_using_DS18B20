/*
 * DS18B20.h
 *
 * Created: 1/6/2021 10:49:18 PM
 *  Author: Aaron
 */ 
#ifndef DS18B20_H_
#define DS18B20_H_

//definitions for ROM_COMMANDS
#define SEARCH_ROM 0xF0
#define READ_ROM 0x33
#define MATCH_ROM 0x55
#define SKIP_ROM 0xCC
#define ALARM_SEARCH 0xEC

//definitions for FUNCTION_COMMANDS
#define CONVERT_T 0x44
#define WRITE_SCRATCHPAD 0x4E
#define READ_SCRATCHPAD 0xBE
#define COPY_SCRATCHPAD 0x48
#define RECALL_E2 0xB8
#define READ_POWER_SUPPLY 0xB4


int initDS18B20();

//variables for different devices on 1-wire protocol
//FOR THE ROM_CMDS
static uint8_t family_code[10];
static uint64_t serial_number[10];
static uint8_t CRC_byte[10];

//ROM_COMMANDS
int read_rom_cmd();
void write_cmd(uint8_t cmd)
void write_bit(uint8_t bit);


#endif /* DS18B20_H_ */