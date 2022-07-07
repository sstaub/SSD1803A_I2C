# Arduino LCD I2C library for EA DOGM204 DOGS164 and DOGS104 with SDD1803A controller

This library is for the LCDs form Display Vision EA DOGM204 (4x20), DOGS164 (4x16) and DOGS104 (4x10) with SDD1803A controller using the I2C interface.
The most difference to the original LiquidCrystal library from Arduino is a new API 

## Example
Here is a simple example which shows the capabilities of the display 
```cpp
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
  lcd.locate(0,0); 
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

```

# Documentation

## Constructor
```cpp
SSD1803A_I2C(uint8_t i2cAddr, uint8_t resetPin = 0xFF)
```
- **i2caddr** I2C address of the display, can 0x3D (SA0 = VDD) or 0x3C (SA0 = VSS)
- **resetPin** reset pin if available

Create a LCD object with a given address of the I2C interface and an optional reset pin.

**Example**
```cpp
const uint8_t i2caddr = 0x3D;
const uint8_t resetPin = 3;
SSD1803A_I2C lcd(i2caddr, resetPin);
```

## Methods

### **begin()**
```cpp
virtual void begin(display_t id)
virtual void begin(TwoWire &i2cPort, display_t id)
```
- **i2cPort** I2C Port, is only needed if you another interface than the standard WIRE like WIRE1 ...
- **id** ID of the display DOGM204, DOGS164, DOGS104

Initialize the display

**Example**
```cpp
lcd.begin(DOGM204); // initialize DOGM204
lcd.begin(WIRE1, DOGS164); // initialize DOGS164 on WIRE1
lcd.begin(WIRE1, DOGS104); // initialize DOGS104 on WIRE1
```

### **cls()**
```cpp
void cls()
```

Clears the display, set cursor to home

**Example**
```cpp
lcd.cls();
```

### **locate()**
```cpp
void locate(uint8_t row, uint8_t column)
```
- **row** starts with 0
- **column** starts with 0

Set position of the cursor, you have to set first row then the column<br> 
This different to the setCursor() methods of other Arduino libraries!!!

**Example**
```cpp
lcd.locate(1, 15); // move the cursor to the second row at position 16
```

### **home()**
```cpp
void home()
```

Set cursor to home position (0/0)

```cpp
lcd.home();
```

### **display()**
```cpp
void display(dispmode_t mode);
void display(dispmode_t mode, uint8_t value);
```

- **mode** display function you want to use
- **value** set additional value, e.g. for contrast

Set the modes and functions of the display

- VIEW_TOP reverse display orientation
- VIEW_BOTTOM display standard orientation
- DISPLAY_ON turn the display on
- DISPLAY_OFF turn the display off
- CURSOR_ON turns the underline cursor on
- CURSOR_OFF turns the underline cursor off
- BLINK_ON turn the blinking cursor on
- BLINK_OFF turn the blinking cursor off
- DISPLAY_SHIFT_LEFT shift the display to the left without changing the RAM
- DISPLAY_SHIFT_RIGHT shift the display to the right without changing the RAM
- CURSOR_SHIFT_LEFT shift the cursor to the left without changing the RAM
- CURSOR_SHIFT_RIGHT shift the cursor to the right without changing the RAM
- LEFT_TO_RIGHT this is for text that flows Left to Right
- RIGHT_TO_LEFT this is for text that flows Right to Left
- AUTOSCROLL_ON this will 'right justify' text from the cursor
- AUTOSCROLL_OFF this will 'left justify' text from the cursor
- CONTRAST change the contrast from 0...63, without a value reset to default
- LINES_4 DOGS164 and DOGS104, 4 line mode
- LINES_3_1 DOGS164 and DOGS104, 3 line mode with double height in the top
- LINES_3_2 DOGS164 and DOGS104, 3 line mode with double height in the middle
- LINES_3_3 DOGS164 and DOGS104, 3 line mode with double height in the bottom
- LINES_2 DOGS164 and DOGS104, 2 line mode with double height both lines
- SET_ROM_A change charset to ROM A
- SET_ROM_B change charset to ROM B
- SET_ROM_C change charset to ROM C

**Example**
```cpp
lcd.display(CURSOR_ON); // show the cursor
lcd.display(BLINK_ON); // blink the cursor
```

### **create()**
```cpp
void create(uint8_t location, uint8_t charmap[])
```
- **location** the location in the CGRAM (0 ... 7)
- **charmap[]** set additional value, e.g. for contrast

Create a custom character (glyph) for use on the LCD. 
Up to eight characters of 5x8 pixels are supported (numbered 0 ... 7).<br>
The appearance of each custom character is specified by an array of eight bytes, one for each row.<br>
The five least significant bits of each byte determine the pixels in that row.<br>
To display a custom character on the screen, write(n) or print('\n') its number.<br>

**Example**
```cpp
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

// in setup()
lcd.createChar(0, smiley);
lcd.createChar(1, frownie);

// to display the glyphs use print() or write()
lcd.locate(0, 0);
lcd.print('\0');
lcd.locate(1, 0);
lcd.write(1);
```