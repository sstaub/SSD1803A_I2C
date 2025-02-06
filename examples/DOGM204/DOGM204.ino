// example for DOGM204
#include <SSD1803A_I2C.h>

const uint8_t i2caddr = 0x3C;
const uint8_t resetPin = 3;

SSD1803A_I2C lcd(i2caddr);

uint8_t smiley[8] = {
	0b00000,
	0b00000,
	0b01010,
	0b00000,
	0b00000,
	0b10001,
	0b01110,
	0b00000
	};

uint8_t frownie[8] = {
	0b00000,
	0b00000,
	0b01010,
	0b00000,
	0b00000,
	0b00000,
	0b01110,
	0b10001
	};

void setup() {
	lcd.begin(DOGM204, resetPin);
	lcd.create(0, smiley);
	lcd.create(1, frownie);
	}

void loop() {
	lcd.cls();
	lcd.display(VIEW_TOP);
	lcd.locate(1, 1);
	lcd.print("hello world!");
	lcd.locate(2, 1);
	lcd.write(0);
	lcd.write(1);
	delay(1000);
	lcd.display(VIEW_BOTTOM);
	delay(1000);
	for (int i = 0; i <= 64; i++) {
		lcd.display(CONTRAST, i);
		delay(50);
		}
	lcd.display(CONTRAST);
	delay(500);
	lcd.locate(1, 1);
	delay(500);
	lcd.display(DISPLAY_OFF);
	delay(500);
	lcd.display(DISPLAY_ON);
	delay(500);
	lcd.display(CURSOR_ON);
	delay(500);
	lcd.display(BLINK_ON);
	delay(500);
	for (int i = 0; i < 20; i++) {
		lcd.display(DISPLAY_SHIFT_RIGHT);
		delay(250);
		}
	for (int i = 0; i < 20; i++) {
		lcd.display(DISPLAY_SHIFT_LEFT);
		delay(250);
		}
	lcd.display(BLINK_OFF);
	delay(500);
	lcd.display(CURSOR_OFF);
	delay(500);
	lcd.cls();
	lcd.locate(1, 1);
	lcd.printf("hello line 1");
	delay(500);
	lcd.locate(2, 1);
	lcd.printf("hello line 2");
	delay(500);
	lcd.locate(3, 1);
	lcd.printf("hello line 3");
	delay(500);
	lcd.locate(4, 1);
	lcd.printf("hello line 4");
	delay(500);
	// clear parts and rows
	lcd.clp(1, 1, 3);
	delay(500);
	lcd.clp(2, 4, 3);
	delay(500);
	lcd.clp(3, 6, 3);
	delay(500);
	lcd.clp(4, 10, 3);
	delay(500);
	lcd.clr(1);
	delay(500);
	lcd.clr(2);
	delay(500);
	lcd.clr(3);
	delay(500);
	lcd.clr(4);
	delay(500);
	}
