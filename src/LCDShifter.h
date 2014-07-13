/*
 * LCDShifter.h
 *
 *  Created on: July 13, 2014
 *      Author: jbest84
 */

#ifndef LCDSHIFTER_H_
#define LCDSHIFTER_H_

#include <arduino.h>

#define NO_DATA 0x00

// Commands
#define RS_HIGH 0x01
#define RS_LOW 0x00
#define RW_HIGH 0x02
#define RW_LOW 0x00
#define ENABLE_HIGH 0x04
#define ENABLE_LOW 0x00

#define LCD_CMD_CLEAR_DISPLAY 0x01
#define LCD_CMD_RETURN_HOME 0x02
#define LCD_CMD_ENTRYMODE_SET 0x04
#define LCD_CMD_DISPLAY 0x08
#define LCD_CMD_CURSOR_SHIFT 0x10
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM_ADDR 0x40
#define LCD_CMD_SET_DDRAM_ADDR 0x80

// Command options
#define LCD_CMDOP_ENTRY_CURSOR_MV_DIR_LEFT 0x00
#define LCD_CMDOP_ENTRY_CURSOR_MV_DIR_RIGHT 0x02
#define LCD_CMDOP_ENTRY_DISP_SHIFT 0x01
#define LCD_CMDOP_ENTRY_DISP_NOSHIFT 0x00

#define LCD_CMDOP_DISP_ON 0x04
#define LCD_CMDOP_DISP_OFF 0x00
#define LCD_CMDOP_DISP_ONCURSOR 0x02
#define LCD_CMDOP_DISP_OFFCURSOR 0x00
#define LCD_CMDOP_DISP_CURSOR_BLINK 0x01

#define LCD_CMDOP_FUNC_INTERFACE_8BIT 0x10
#define LCD_CMDOP_FUNC_INTERFACE_4BIT 0x00
#define LCD_CMDOP_FUNC_DISPLAY_2LINES 0x08
#define LCD_CMDOP_FUNC_DISPLAY_1LINE 0x00
#define LCD_CMDOP_FUNC_FONT_5x10 0x04
#define LCD_CMDOP_FUNC_FONT_5x8 0x00

#define LCD_CMDOP_CURSOR_SHIFTRIGHT 0x00
#define LCD_CMDOP_CURSOR_SHIFTLEFT 0x04
#define LCD_CMDOP_CURSOR_MOVE_DISP 0x08
#define LCD_CMDOP_CURSOR_MOVE_CURSOR 0x00

#define LCD_COL1_ROW1 0x00
#define LCD_COL1_ROW2 0x40

class LCDShifter {
private:
	int _data, _latch, _clock;
	void sendCommand(int);
	void write8Bits(int data);
	void write16Bits(int data, int pinState);
	void pulseClock();
	void pulseLCD(int data, int pinState);

public:
	LCDShifter(int data, int latch, int clock);

	void begin();

	// Table 6 from page 24
	// Command table
	void clearDisplay();
	void returnHome();
	void writeData(const char *str);
	void entryModeSet(int data);
	void displayControl(int data);
	void cursorControl(int data);
	void functionSet(int data);
	void setCGRAMAddr(int data);
	void setDDRAMAddr(int data);
	void setCursor(uint8_t col, uint8_t row);
};

#endif /* LCDSHIFTER_H_ */
