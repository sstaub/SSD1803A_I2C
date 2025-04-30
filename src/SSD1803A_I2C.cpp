//	EA DOGM204 / DOGS164 / DOGS104 LCD Display with SSD1803A controller for 4x20
//	Copyright(C) 2022 - 2025 Stefan Staub under MIT license

#include "SSD1803A_I2C.h"

SSD1803A_I2C::SSD1803A_I2C(uint8_t i2cAddr, TwoWire &i2cPort) {
	this->i2cAddr = i2cAddr;
	this->i2cPort = &i2cPort;
	}

void SSD1803A_I2C::begin(display_t id, uint8_t resetPin) {
	this->id = id;
	this->resetPin = resetPin;
	switch (id) {
		case DOGM204:
			columns = 20;
			rows = 4;
			break;
		case DOGS164:
			columns = 16;
			rows = 4;
			break;
		case DOGS104:
			columns = 10;
			rows = 4;
			break;
		}
	ddramStart = ADDRESS_DDRAM;
	lines = 4;
	i2cPort->begin();
	reset();
	init();
	cls();
	entrymode = ENTRY_MODE_LEFT_TO_RIGHT;
	sendCommand(COMMAND_ENTRY_MODE_SET | entrymode);
	}

void SSD1803A_I2C::begin(display_t id) {
	this->id = id;
	switch (id) {
		case DOGM204:
			columns = 20;
			rows = 4;
			break;
		case DOGS164:
			columns = 16;
			rows = 4;
			break;
		case DOGS104:
			columns = 10;
			rows = 4;
			break;
		}
	ddramStart = ADDRESS_DDRAM;
	lines = 4;
	i2cPort->begin();
	reset();
	init();
	cls();
	entrymode = ENTRY_MODE_LEFT_TO_RIGHT;
	sendCommand(COMMAND_ENTRY_MODE_SET | entrymode);
	}

void SSD1803A_I2C::reset() {
	if (resetPin == 0xFF)
		return;
	delay(50);
	pinMode(resetPin, OUTPUT);
	digitalWrite(resetPin, LOW);
	delay(4);
	digitalWrite(resetPin, HIGH);
	delay(20);
	}

void SSD1803A_I2C::init() {
	sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
	sendCommand(COMMAND_4LINES);
	sendCommand(COMMAND_BOTTOM_VIEW);
	sendCommand(COMMAND_BS1_1);
	sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
	sendCommand(COMMAND_BS0_1);
	switch(id) {
		case DOGM204:
			sendCommand(COMMAND_FOLLOWER_CONTROL_DOGM204);
			sendCommand(COMMAND_POWER_CONTROL_DOGM204);
			sendCommand(COMMAND_CONTRAST_DEFAULT_DOGM204);
			break;
		case DOGS164:
			sendCommand(COMMAND_FOLLOWER_CONTROL_DOGS164);
			sendCommand(COMMAND_POWER_CONTROL_DOGS164);
			sendCommand(COMMAND_CONTRAST_DEFAULT_DOGS164);
			break;
		case DOGS104:
			sendCommand(COMMAND_FOLLOWER_CONTROL_DOGS104);
			sendCommand(COMMAND_POWER_CONTROL_DOGS104);
			sendCommand(COMMAND_CONTRAST_DEFAULT_DOGS104);
			break;
		}
	sendCommand(COMMAND_8BIT_4LINES_RE0_IS0);
	displaycontrol = COMMAND_DISPLAY_ON | COMMAND_CURSOR_OFF | COMMAND_BLINK_OFF;
	sendCommand(COMMAND_DISPLAY | displaycontrol);
	}

void SSD1803A_I2C::cls() {
	sendCommand(COMMAND_CLEAR_DISPLAY);
	}

void SSD1803A_I2C::clr(uint8_t row) {
	for (uint8_t pos = 0; pos <= columns; pos++) {
		locate(row, pos);
		write(' ');
		}
	locate(row, 0);
	}

void SSD1803A_I2C::clp(uint8_t row, uint8_t column, uint8_t numbers) {
	for (uint8_t pos = column; pos < (column + numbers); pos++) {
		locate(row, pos);
		write(' ');
		}
	locate(row, column);
	}

void SSD1803A_I2C::home() {
	sendCommand(COMMAND_RETURN_HOME);
	}

void SSD1803A_I2C::locate(uint8_t row, uint8_t column) {
	if(OFFSET == 1) {
		row = row - 1;
		column = column - 1;
		}
	sendCommand(ddramStart + (row * 0x20 + column));
	}

void SSD1803A_I2C::display(dispmode_t mode) {
	switch(mode) {
		case VIEW_TOP:
			switch(id) {
				case DOGM204:
					ddramStart = ADDRESS_DDRAM;
					break;
				case DOGS164:
					ddramStart = ADDRESS_DDRAM + ADDRESS_DDRAM_DOGS164_TOP_OFFSET;
					break;
				case DOGS104:
					ddramStart = ADDRESS_DDRAM + ADDRESS_DDRAM_DOGS104_TOP_OFFSET;
					break;
				}
			sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
			sendCommand(COMMAND_TOP_VIEW);
			finishCommand();
			break;
		case VIEW_BOTTOM:
			ddramStart = ADDRESS_DDRAM;
			sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
			sendCommand(COMMAND_BOTTOM_VIEW);
			finishCommand();
			break;
		case DISPLAY_ON:
			displaycontrol |= COMMAND_DISPLAY_ON;
			sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
			sendCommand(COMMAND_DISPLAY | displaycontrol);
			finishCommand();
			break;
		case DISPLAY_OFF:
			displaycontrol &= ~COMMAND_DISPLAY_ON;
			sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
			sendCommand(COMMAND_DISPLAY | displaycontrol);
			finishCommand();
			break;
		case CURSOR_ON:
			displaycontrol |= COMMAND_CURSOR_ON;
			sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
			sendCommand(COMMAND_DISPLAY | displaycontrol);
			finishCommand();
			break;
		case CURSOR_OFF:
			displaycontrol &= ~COMMAND_CURSOR_ON;
			sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
			sendCommand(COMMAND_DISPLAY | displaycontrol);
			finishCommand();
			break;
		case BLINK_ON:
			displaycontrol |= COMMAND_BLINK_ON;
			sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
			sendCommand(COMMAND_DISPLAY | displaycontrol);
			finishCommand();
			break;
		case BLINK_OFF:
			displaycontrol &= ~COMMAND_BLINK_ON;
			sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
			sendCommand(COMMAND_DISPLAY | displaycontrol);
			finishCommand();
			break;
		case DISPLAY_SHIFT_LEFT:
			sendCommand(COMMAND_SHIFT | COMMAND_DISPLAY_SHIFT_LEFT);
			break;
		case DISPLAY_SHIFT_RIGHT:
			sendCommand(COMMAND_SHIFT | COMMAND_DISPLAY_SHIFT_RIGHT);
			break;
		case CURSOR_SHIFT_LEFT:
			sendCommand(COMMAND_SHIFT | COMMAND_CURSOR_SHIFT_LEFT);
			break;
		case CURSOR_SHIFT_RIGHT:
			sendCommand(COMMAND_SHIFT | COMMAND_CURSOR_SHIFT_RIGHT);
			break;
		case LEFT_TO_RIGHT:
			entrymode |= ENTRY_MODE_LEFT_TO_RIGHT;
			sendCommand(COMMAND_ENTRY_MODE_SET | entrymode);
			break;
		case RIGHT_TO_LEFT:
			entrymode &= ~ENTRY_MODE_LEFT_TO_RIGHT;
			sendCommand(COMMAND_ENTRY_MODE_SET | entrymode);
			break;
		case AUTOSCROLL_ON:
			entrymode |= ENTRY_MODE_SHIFT_INCREMENT;
			sendCommand(COMMAND_ENTRY_MODE_SET | entrymode);
			break;
		case AUTOSCROLL_OFF:
			entrymode &= ~ENTRY_MODE_SHIFT_INCREMENT;
			sendCommand(COMMAND_ENTRY_MODE_SET | entrymode);
			break;
		case CONTRAST:
			switch(id) {
			case DOGM204:
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
				sendCommand(COMMAND_POWER_CONTROL_DOGM204);
				sendCommand(COMMAND_CONTRAST_DEFAULT_DOGM204);
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS0);
				break;
			case DOGS164:
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
				sendCommand(COMMAND_POWER_CONTROL_DOGS164);
				sendCommand(COMMAND_CONTRAST_DEFAULT_DOGS164);
				finishCommand();
				break;
			case DOGS104:
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
				sendCommand(COMMAND_POWER_CONTROL_DOGS104);
				sendCommand(COMMAND_CONTRAST_DEFAULT_DOGS104);
				finishCommand();
				break;
			}
			break;
		case LINES_4:
			if (id == DOGS164 || id == DOGS104) {
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS0);
				lines = 4;
				}
			break;
		case LINES_3_1:
			if (id == DOGS164 || id == DOGS104) {
				sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
				sendCommand(COMMAND_3LINES_TOP);
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS0_DH1);
				lines = 3;
				}
			break;
		case LINES_3_2:
			if (id == DOGS164 || id == DOGS104) {
				sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
				sendCommand(COMMAND_3LINES_MIDDLE);
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS0_DH1);
				lines = 3;
				}
			break;
		case LINES_3_3:
			if (id == DOGS164 || id == DOGS104) {
				sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
				sendCommand(COMMAND_3LINES_BOTTOM);
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS0_DH1);
				lines = 3;
				}
			break;
		case LINES_2:
			if (id == DOGS164 || id == DOGS104) {
				sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
				sendCommand(COMMAND_2LINES);
				sendCommand(COMMAND_8BIT_4LINES_RE0_IS0_DH1);
				lines = 2;
				}
			break;
		case SET_ROM_A:
			sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
			sendCommand(COMMAND_ROM_SELECT);
			sendData(COMMAND_ROM_A);
			finishCommand();
			break;
		case SET_ROM_B:
			sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
			sendCommand(COMMAND_ROM_SELECT);
			sendData(COMMAND_ROM_B);
			finishCommand();
			break;
		case SET_ROM_C:
			sendCommand(COMMAND_8BIT_4LINES_RE1_IS0);
			sendCommand(COMMAND_ROM_SELECT);
			sendData(COMMAND_ROM_C);
			finishCommand();
			break;
		}
	}

void SSD1803A_I2C::display(dispmode_t mode, uint8_t value) {
	if (mode == CONTRAST) {
		sendCommand(COMMAND_8BIT_4LINES_RE0_IS1);
		sendCommand(0x70 | (value & 0x0F));
		sendCommand(COMMAND_POWER_ICON_CONTRAST | ((value >> 4) & 0x03));
		finishCommand();
		}
	}

void SSD1803A_I2C::create(uint8_t location, uint8_t charmap[]) {
	location &= 0x7;
	sendCommand(ADDRESS_CGRAM | (location << 3));
	for (uint8_t i = 0; i < 8; i++)
		sendData(charmap[i]);
	}

size_t SSD1803A_I2C::write(uint8_t value) {
	sendData(value);
	return 1;
	}

size_t SSD1803A_I2C::write(const uint8_t *buffer, size_t size) {
	sendBuffer(buffer, size);
	return size;
	}

void SSD1803A_I2C::finishCommand() {
	if(lines == 4) sendCommand(COMMAND_8BIT_4LINES_RE0_IS0);
	else sendCommand(COMMAND_8BIT_4LINES_RE0_IS0_DH1);
	}

void SSD1803A_I2C::sendCommand(uint8_t cmd) {
	i2cPort->beginTransmission(i2cAddr);
	i2cPort->write(MODE_COMMAND);
	i2cPort->write(cmd);
	i2cPort->endTransmission();
	}

void SSD1803A_I2C::sendData(uint8_t val) {
	i2cPort->beginTransmission(i2cAddr);
	i2cPort->write(MODE_DATA);
	i2cPort->write(val);
	i2cPort->endTransmission();
	}

void SSD1803A_I2C::sendBuffer(const uint8_t *buffer, size_t size) {
	i2cPort->beginTransmission(i2cAddr);
	i2cPort->write(MODE_DATA);
	while (size--) i2cPort->write(*buffer++);
	i2cPort->endTransmission();
	}
