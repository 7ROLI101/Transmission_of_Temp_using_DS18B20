/*
 * DS18B20.h
 *
 * Created: 1/6/2021 10:49:18 PM
 *  Author: Aaron
 */ 
#ifndef DS18B20_H_
#define DS18B20_H_

#define F_CPU 16000000UL
#include <stdint.h>
#include <asf.h>
#include <util/delay.h>


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


int initDS18B20(void);

//variables for different devices on 1-wire protocol
//FOR THE ROM_CMDS
static uint64_t ROM_code[10];
static uint8_t info[9];

void write_bit(uint8_t bit);
void write_zero(void);
void write_one(void);
void write_byte(uint8_t byte);
uint8_t read_bit(void);
uint8_t read_byte(void);

//Function you use to access the ROM commands
void rom_cmd(uint8_t cmd);
//ROM COMMANDS
void read_rom_cmd(void);
void match_rom_cmd(int index);
void skip_rom_cmd(void);
void search_rom_cmd(void);
void alarm_search_cmd(void);

//Function you use to access the ROM commands
void function_cmd(uint8_t cmd);
//FUNCTION COMMANDS
void convert_temp(void);
uint16_t read_scratchpad(int stop);
void write_scratchpad(void);
void copy_scratchpad(void);
void recall_e2(void);
bool read_power_supply(void);
#endif /* DS18B20_H_ */