# Arduino LCD I2C library for EA DOGM204 DOGS164 and DOGS104 with SDD1803A controller

This library is for the following LCDs from Display Vision
- DOGM204 (4x20)
- DOGS164 (4x16)
- DOGS104 (4x10)

with SDD1803A controller using the I2C interface.
The most difference to the original LiquidCrystal library from Arduino is a new API which also allows `printf()` functionality.

## Version 2.x Breaking Changes
- Initialisation changed
- Offset, set the coordinates to 1 as a start point for rows and columns, set `#define OFFSET 0` for old behavior
- New clear functions `clr()` and `clp()`

## Example
Here is a simple example which shows the capabilities of the display 
```cpp
// example for DOGM204
#include <Arduino.h>
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
```

# Documentation

## Constructor
```cpp
SSD1803A_I2C(uint8_t i2cAddr, TwoWire &i2cPort = WIRE)
```
- **i2caddr** I2C address of the display, can 0x3D (SA0 = VDD) or 0x3C (SA0 = VSS)
- **i2cPort** optional I2C Port, this is only needed if you another interface than the standard Wire like Wire1 ...

Create a LCD object with a given address of the I2C interface.

**Example**
```cpp
const uint8_t i2caddr = 0x3C;
SSD1803A_I2C lcd(i2caddr, Wire1); //initialize on Wire1
```

## Methods

### **begin()**
```cpp
void begin(display_t id, uint8_t resetPin = 0xFF)
```
- **id** ID of the display DOGM204, DOGS164, DOGS104
- **resetPin** optional reset pin if available

Initialize the display and an optional reset pin, this must done in `setup()`.

**Example**
```cpp
const uint8_t resetPin = 3;
lcd.begin(DOGM204, resetPin); // initialize DOGM204 with reset pin
lcd.begin(DOGS164); // initialize DOGM164
lcd.begin(DOGS104); // initialize DOGM104
```

### **cls()**
```cpp
void cls()
```

Clears the display, set cursor to home.

**Example**
```cpp
lcd.cls();
```

### **clr()**
```cpp
void cls(uint8_t row)
```
- **row** set the row to clear

Clears a single row (line), set cursor position of the row to the first position.

**Example**
```cpp
lcd.clr(2); // clear row 2
```

### **clp()**
```cpp
void cls(uint8_t row, uint8_t column, uint8_t numbers)
```
- **row** set the row to clear
- **column** set the start column to clear
- **numbers** set the numbers of chars to clear

Clears a part of a row (line), set cursor position of the row to first column.

**Example**
```cpp
lcd.clp(1, 3, 4); // clears in row 1 four chars beginning with column 3
```

### **locate()**
```cpp
void locate(uint8_t row, uint8_t column)
```
- **row** starts with 1
- **column** starts with 1

Set position of the cursor, you have to set first row then the column<br> 
This different to the setCursor() methods of other Arduino libraries!!!

**Example**
```cpp
lcd.locate(2, 15); // move the cursor to the second row at column 15
```

### **home()**
```cpp
void home()
```

Set cursor to home position (1/1)

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

Set the modes and functions of the display.

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

Create a custom character (glyph) for use on the LCD, this must done in `setup()`.<br>
Up to eight characters of 5x8 pixels are supported (numbered 0 ... 7).<br>
The appearance of each custom character is specified by an array of eight bytes, one for each row.<br>
The five least significant bits of each byte determine the pixels in that row.<br>
To display a custom character on the screen use write(n) or print('\n') its number.<br>

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
lcd.create(0, smiley);
lcd.create(1, frownie);

// to display the glyphs use print() or write()
lcd.locate(1, 1);
lcd.print('\0');
lcd.locate(1, 3);
lcd.write(1);
```

### **write()**

```cpp
virtual size_t write(uint8_t value)
virtual size_t write(const uint8_t *buffer, size_t size)
```
- **value** write a single char
- **buffer** write the buffer
- **size** size of the buffer which should write

Writes a single char or buffer to the display.

**Example**
```cpp
Lcd.locate(1, 1);
lcd.write('A');
```