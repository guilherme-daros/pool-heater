#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_ADDR 0x27
#define LCD_CHR 1
#define LCD_CMD 0
#define LINE1 0x80
#define LINE2 0xC0
#define LCD_BACKLIGHT 0x08
#define ENABLE 0b00000100

class LCD {
private:
  int fd;
  void lcd_init(void);
  void lcd_byte(int bits, int mode);
  void lcd_toggle_enable(int bits);

public:
  LCD();
  ~LCD();
  void typeInt(int);
  void typeFloat(float);
  void typeln(const char *);
  void typeChar(char);
  void lcdLoc(int);
  void ClrLcd(void);
};

LCD::LCD() {
  wiringPiSetup();
  fd = wiringPiI2CSetup(I2C_ADDR);
  lcd_init();
}

LCD::~LCD() {}

// prints float values
void LCD::typeFloat(float myFloat) {
  char buffer[20];
  sprintf(buffer, "%4.2f", myFloat);
  typeln(buffer);
}

// prints int values
void LCD::typeInt(int i) {
  char array1[20];
  sprintf(array1, "%d", i);
  typeln(array1);
}

// clear lcd go home
void LCD::ClrLcd(void) {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void LCD::lcdLoc(int line) { lcd_byte(line, LCD_CMD); }

// out char to LCD at current position
void LCD::typeChar(char val) { lcd_byte(val, LCD_CHR); }

// this allows use of any size string
void LCD::typeln(const char *s) {

  while (*s)
    lcd_byte(*(s++), LCD_CHR);
}

// Send byte to data pins
void LCD::lcd_byte(int bits, int mode) {
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

// Toggle enable pin on LCD display
void LCD::lcd_toggle_enable(int bits) {
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}

// Initialise display
void LCD::lcd_init() {
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}