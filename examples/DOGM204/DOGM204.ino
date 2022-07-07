// example for DOGM204
#include <Arduino.h>
#include <SSD1803A_I2C.h>

const uint8_t i2caddr = 0x3D;
const uint8_t resetPin = 3;

SSD1803A_I2C lcd(i2caddr, resetPin);

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
  lcd.begin(DOGM204);
  lcd.create(0, smiley);
  lcd.create(1, frownie);
  lcd.locate(0, 0); 
  lcd.print("hello");
  lcd.locate(1, 0);
  lcd.write(0);
  lcd.write(1);
  delay(1000);
  for (int i = 0; i <= 64; i++) {
    lcd.display(CONTRAST, i);
    delay(100);
    }
  lcd.display(CONTRAST);
  delay(1000);
  lcd.locate(0, 0);
  delay(1000);
  lcd.display(DISPLAY_OFF);
  delay(1000);
  lcd.display(DISPLAY_ON);
  delay(1000);
  lcd.display(CURSOR_ON);
  delay(1000);
  lcd.display(BLINK_ON);
  delay(1000);
  for (int i = 0; i < 5; i++) {
    lcd.display(DISPLAY_SHIFT_RIGHT);
    delay(500);
    }
  for (int i = 0; i < 5; i++) {
    lcd.display(DISPLAY_SHIFT_LEFT);
    delay(500);
    }
  lcd.display(BLINK_OFF);
  delay(1000);
  lcd.display(CURSOR_OFF);
  delay(1000);
  }

  void loop() {}