/*
 * LCDShifter.cpp
 *
 *  Created on: Jul 13, 2014
 *      Author: Jason
 */

#include "LCDShifter.h"

LCDShifter::LCDShifter(int data, int latch, int clock) {
	this->_data = data;
	this->_latch = latch;
	this->_clock = clock;
}

void LCDShifter::clearDisplay() {
	sendCommand(LCD_CMD_CLEAR_DISPLAY);
	delayMicroseconds(2000);
}

void LCDShifter::cursorControl(int options) {
	sendCommand(LCD_CMD_CURSOR_SHIFT | options);
}

void LCDShifter::displayControl(int options){
	sendCommand(LCD_CMD_DISPLAY | options);
}

void LCDShifter::entryModeSet(int options){
	sendCommand(LCD_CMD_ENTRYMODE_SET | options);
}

void LCDShifter::functionSet(int options){
	sendCommand(LCD_CMD_FUNCTION_SET | options);
}

void LCDShifter::pulseClock(){
	digitalWrite(this->_clock, HIGH);
	delay(1);
	digitalWrite(this->_clock, LOW);
}

void LCDShifter::pulseLCD(int data, int pinState) {
	write16Bits(data, (pinState | ENABLE_LOW));
	delayMicroseconds(1);

	write16Bits(data, (pinState | ENABLE_HIGH));
	delayMicroseconds(1);

	write16Bits(data, (pinState | ENABLE_LOW));
	delayMicroseconds(100);
}

void LCDShifter::returnHome(){
	sendCommand(LCD_CMD_RETURN_HOME);
	delayMicroseconds(2000);
}

void LCDShifter::sendCommand(int data) {
	int pinState = RS_LOW | RW_LOW | ENABLE_LOW;
	write16Bits(data, pinState);
	pulseLCD(data, pinState);
}

void LCDShifter::setCGRAMAddr(int options) {
	sendCommand(LCD_CMD_SET_CGRAM_ADDR | options);
}

void LCDShifter::setCursor(uint8_t col, uint8_t row) {
	// Credit to LiquidCrystal library for this function
	int offset[] = { 0x00, 0x40, 0x14, 0x54 };
	sendCommand(LCD_CMD_SET_DDRAM_ADDR | (col + offset[row]));
}

void LCDShifter::setDDRAMAddr(int options) {
	sendCommand(LCD_CMD_SET_DDRAM_ADDR | options);
}

void LCDShifter::write16Bits(int data, int pinState){
	// Latch should start low
	digitalWrite(this->_latch, LOW);

	// Write 8 bits for the second chip
	write8Bits(data);

	// Write 8 bits for the first chip
	write8Bits(pinState);

	// Pull the latch high, shifting the data out
	digitalWrite(this->_latch, HIGH);
}

// Write 8 bits to the shift register
// Data should be called first, command pins second (RS, RW, EN)
void LCDShifter::write8Bits(int data) {
	// Start with the left most bit, check it for HIGH/LOW
	for(int i = 0; i < 8; i++) {
		int value = data << i;
		if ((value & B10000000) == B10000000) {
			digitalWrite(this->_data, HIGH);
		} else {
			digitalWrite(this->_data, LOW);
		}

		pulseClock();
	}
}

void LCDShifter::writeData(const char *str) {
	int pinState;
	size_t length;

	pinState = RS_HIGH | RW_LOW | ENABLE_LOW;

	length = strlen(str);

	for (size_t i = 0; i < length; i++) {
		write16Bits(str[i], pinState);
		pulseLCD(str[i], pinState);
	}
}

void LCDShifter::begin() {
	delayMicroseconds(50000);

	int pinState = RS_LOW | ENABLE_LOW | RW_LOW;

	write16Bits(NO_DATA, pinState);

	pulseLCD(NO_DATA, pinState);

	// Function set (1)
	sendCommand(LCD_CMD_FUNCTION_SET | LCD_CMDOP_FUNC_INTERFACE_8BIT | LCD_CMDOP_FUNC_DISPLAY_2LINES);
	delayMicroseconds(4500);// Wait for more than 4.1 ms

	// Function set (2)
	sendCommand(LCD_CMD_FUNCTION_SET | LCD_CMDOP_FUNC_INTERFACE_8BIT | LCD_CMDOP_FUNC_DISPLAY_2LINES);
	// Wait for more than 100 microseconds
	delayMicroseconds(150);

	// Function set (3)
	sendCommand(LCD_CMD_FUNCTION_SET | LCD_CMDOP_FUNC_INTERFACE_8BIT | LCD_CMDOP_FUNC_DISPLAY_2LINES);

	//**************************
	// Final initialization table (page 45)
	sendCommand(LCD_CMD_FUNCTION_SET | LCD_CMDOP_FUNC_INTERFACE_8BIT | LCD_CMDOP_FUNC_DISPLAY_2LINES);

	sendCommand(LCD_CMD_DISPLAY | LCD_CMDOP_DISP_ON);

	sendCommand(LCD_CMD_CLEAR_DISPLAY);

	sendCommand(LCD_CMD_ENTRYMODE_SET | LCD_CMDOP_ENTRY_CURSOR_MV_DIR_RIGHT);
}
