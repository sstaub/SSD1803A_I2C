//	EA DOGM204 / DOGS164 / DOGS104 LCD Display with SSD1803A controller for 4x20
//	Copyright(C) 2022 Stefan Staub under MIT license

#ifndef SSD1803A_I2C_H
#define SSD1803A_I2C_H

#include <Arduino.h>
#include <Wire.h>
#include "Print.h"

#define MODE_COMMAND                              0x00
#define MODE_DATA                                 0x40

#define COMMAND_CLEAR_DISPLAY                     0x01
#define COMMAND_RETURN_HOME                       0x02
#define COMMAND_ENTRY_MODE_SET                    0x04
#define ENTRY_MODE_LEFT_TO_RIGHT                  0x02
#define ENTRY_MODE_SHIFT_INCREMENT                0x01
#define COMMAND_SHIFT                             0x10
#define COMMAND_DISPLAY_SHIFT_LEFT                0x08
#define COMMAND_DISPLAY_SHIFT_RIGHT               0x0C
#define COMMAND_CURSOR_SHIFT_LEFT                 0x00
#define COMMAND_CURSOR_SHIFT_RIGHT                0x04

#define ADDRESS_CGRAM                             0x40
#define ADDRESS_DDRAM                             0x80
#define ADDRESS_DDRAM_DOGS164_TOP_OFFSET          0x04
#define ADDRESS_DDRAM_DOGS104_TOP_OFFSET          0x0A

#define COMMAND_8BIT_4LINES_RE1_IS0               0x3A
#define COMMAND_8BIT_4LINES_RE0_IS0_DH1           0x3C
#define COMMAND_8BIT_4LINES_RE0_IS1               0x39
#define COMMAND_8BIT_4LINES_RE0_IS1_DH1           0x3D
#define COMMAND_8BIT_4LINES_RE0_IS0               0x38

// Command from extended set (RE = 1, IS = 0)
#define COMMAND_BS1_1                             0x1E
#define COMMAND_POWER_DOWN_DISABLE                0x02
#define COMMAND_TOP_VIEW                          0x05
#define COMMAND_BOTTOM_VIEW                       0x06
#define COMMAND_4LINES                            0x09
#define COMMAND_3LINES_TOP                        0x1F
#define COMMAND_3LINES_MIDDLE                     0x17
#define COMMAND_3LINES_BOTTOM                     0x13
#define COMMAND_2LINES                            0x1B

// Command from extended set (RE = 0, IS = 1)
#define COMMAND_DISPLAY                           0x08
#define COMMAND_DISPLAY_ON                        0x04
#define COMMAND_DISPLAY_OFF                       0x00
#define COMMAND_CURSOR_ON                         0x02
#define COMMAND_CURSOR_OFF                        0x00
#define COMMAND_BLINK_ON                          0x01
#define COMMAND_BLINK_OFF                         0x00

// Command from extended set (RE = 1, IS = 1)
#define COMMAND_SHIFT_SCROLL_ENABLE               0x10
#define COMMAND_SHIFT_SCROLL_ALL_LINES            0x0F
#define COMMAND_SHIFT_SCROLL_LINE_1               0x01
#define COMMAND_SHIFT_SCROLL_LINE_2               0x02
#define COMMAND_SHIFT_SCROLL_LINE_3               0x04
#define COMMAND_SHIFT_SCROLL_LINE_4               0x08


#define COMMAND_BS0_1                             0x1B
#define COMMAND_INTERNAL_DIVIDER                  0x13
#define COMMAND_CONTRAST_DEFAULT_DOGM204          0x72
#define COMMAND_CONTRAST_DEFAULT_DOGS164          0x6B
#define COMMAND_CONTRAST_DEFAULT_DOGS104          0x7A
#define COMMAND_POWER_CONTROL_DOGM204             0x57
#define COMMAND_POWER_CONTROL_DOGS164             0x56
#define COMMAND_POWER_CONTROL_DOGS104             0x56
#define COMMAND_POWER_ICON_CONTRAST               0x5C
#define COMMAND_FOLLOWER_CONTROL_DOGM204          0x6E
#define COMMAND_FOLLOWER_CONTROL_DOGS164          0x6C
#define COMMAND_FOLLOWER_CONTROL_DOGS104          0x6E
#define COMMAND_ROM_SELECT                        0x72
#define COMMAND_ROM_A                             0x00
#define COMMAND_ROM_B                             0x04
#define COMMAND_ROM_C                             0x08

typedef enum {
	DOGM204,
	DOGS164,
	DOGS104
	} display_t;

typedef enum {
	VIEW_TOP,
	VIEW_BOTTOM,
	DISPLAY_ON,
	DISPLAY_OFF,
	CURSOR_ON,
	CURSOR_OFF,
	BLINK_ON,
	BLINK_OFF,
	DISPLAY_SHIFT_LEFT,
	DISPLAY_SHIFT_RIGHT,
	CURSOR_SHIFT_LEFT,
	CURSOR_SHIFT_RIGHT,
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
	AUTOSCROLL_ON,
	AUTOSCROLL_OFF,
	CONTRAST,
	LINES_4,
	LINES_3_1,
	LINES_3_2,
	LINES_3_3,
	LINES_2,
	SET_ROM_A,
	SET_ROM_B,
	SET_ROM_C
	} dispmode_t;

class SSD1803A_I2C : public Print {
public:
	/**
	 * @brief Construct a new ssd1803a i2c object
	 * 
	 * @param i2cAddr 0x3D (SA0 = VDD) or 0x3C (SA0 = VSS)
	 * @param resetPin if available
	 */
	SSD1803A_I2C(uint8_t i2cAddr, uint8_t resetPin = 0xFF);

	/**
	 * @brief 
	 * 
	 * @param i2cPort alternatives I2C Ports, Wire and Wire1 ...
	 * @param id display type DOGM201, DOGS164 or DOGS104
	 */
	virtual void begin(display_t id);
	virtual void begin(TwoWire &i2cPort, display_t id);

	/**
	 * @brief Clears the LCD screen and positions the cursor in the upper-left corner.
	 * 
	 */
	void cls();

	/**
	 * @brief Positions the cursor in the upper-left of the LCD. That is, use that location in outputting subsequent text to the display.
	 * @brief To also clear the display, use the clear() function instead.
	 * 
	 */
	void home();

	/**
	 * @brief Set the cursor to a given position.
	 * @brief You have to set the row number first !!!
	 * 
	 * @param row 
	 * @param column 
	 */
	void locate(uint8_t row, uint8_t column);

	/**
	 * @brief Set display modes.
	 * 
	 * @param mode 
	 * - VIEW_TOP reverse display orientation
	 * - VIEW_BOTTOM display standard orientation
	 * - DISPLAY_ON turn the display on
	 * - DISPLAY_OFF turn the display off
	 * - CURSOR_ON turns the underline cursor on
	 * - CURSOR_OFF turns the underline cursor off
	 * - BLINK_ON turn the blinking cursor on
	 * - BLINK_OFF turn the blinking cursor off
	 * - DISPLAY_SHIFT_LEFT shift the display to the left without changing the RAM
	 * - DISPLAY_SHIFT_RIGHT shift the display to the right without changing the RAM
	 * - CURSOR_SHIFT_LEFT shift the cursor to the left without changing the RAM
	 * - CURSOR_SHIFT_RIGHT shift the cursor to the right without changing the RAM
	 * - LEFT_TO_RIGHT this is for text that flows Left to Right
	 * - RIGHT_TO_LEFT this is for text that flows Right to Left
	 * - AUTOSCROLL_ON this will 'right justify' text from the cursor
	 * - AUTOSCROLL_OFF this will 'left justify' text from the cursor
	 * - CONTRAST change the contrast from 0...63, without a value reset to default
	 * - LINES_4 DOGS164 and DOGS104, 4 line mode
	 * - LINES_3_1 DOGS164 and DOGS104, 3 line mode with double height in the top
	 * - LINES_3_2 DOGS164 and DOGS104, 3 line mode with double height in the middle
	 * - LINES_3_3 DOGS164 and DOGS104, 3 line mode with double height in the bottom
	 * - LINES_2 DOGS164 and DOGS104, 2 line mode with double height both lines
	 * - SET_ROM_A change charset to ROM A
	 * - SET_ROM_B change charset to ROM B
	 * - SET_ROM_C change charset to ROM C
	 * 
	 * @param value contrast value
	 */
	void display(dispmode_t mode);
	void display(dispmode_t mode, uint8_t value);

	/**
	 * @brief Create a custom character (glyph) for use on the LCD. 
	 * @brief Up to eight characters of 5x8 pixels are supported (numbered 0 to 7). 
	 * @brief The appearance of each custom character is specified by an array of eight bytes, one for each row. 
	 * @brief The five least significant bits of each byte determine the pixels in that row. 
	 * @brief To display a custom character on the screen, write() its number.
	 * 
	 * @brief NB : When referencing custom character "0", if it is not in a variable, you need to cast it as a byte, otherwise the compiler throws an error.
	 * 
	 * @param location 
	 * @param charmap 
	 */
	void create(uint8_t location, uint8_t charmap[]);

	virtual size_t write(uint8_t value);

	virtual size_t write(const uint8_t *buffer, size_t size);

private:
	void reset();
	void init();
	void finishCommand();
	void sendCommand(uint8_t cmd);
	void sendData(uint8_t val);
	void sendBuffer(const uint8_t *buffer, size_t size);
	TwoWire *i2cPort;
	display_t id;
	uint8_t columns, rows;
	uint8_t i2cAddr;
	uint8_t resetPin;
	uint8_t entrymode;
	uint8_t displaycontrol;
	uint8_t ddramStart;
	uint8_t lines;

};
#endif
