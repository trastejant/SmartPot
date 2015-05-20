#include "NokiaLCD.h"
#include "ASCII.h"

NokiaLCD::NokiaLCD(uint SCK, uint MOSI, uint DC, uint RST, uint CS) 
{
  //Configure control pins
  pinMode(CS, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);

  _SCK = SCK;
  _MOSI = MOSI;
  _DC = DC;
  _RST = RST;
  _CS = CS;
}

void NokiaLCD::init()
{
  digitalWrite(_RST, LOW);
  digitalWrite(_RST, HIGH);
  
  write(LCD_COMMAND, LCDEXTENDED);  //Tell LCD that extended commands follow
  write(LCD_COMMAND, LCDCONTRAST);  //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  write(LCD_COMMAND, LCDTEMPC); 	//Set Temp coefficent
  write(LCD_COMMAND, LCDMODE);      //LCD bias mode 1:48: Try 0x13 or 0x14

  write(LCD_COMMAND, LCDBCMODE);    //We must send 0x20 before modifying the display control mode
  write(LCD_COMMAND, LCDCONTROL);   //Set display control, normal mode. 0x0D for inverse
}

void NokiaLCD::write(byte data_or_command, byte data) 
{
  digitalWrite(_DC, data_or_command); 
  digitalWrite(_CS, LOW);
  shiftOut(_MOSI, _SCK, MSBFIRST, data);
  digitalWrite(_CS, HIGH);
}

void NokiaLCD::clear(void) 
{
  for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++)
   write(LCD_DATA, 0x00);
   setCursor(0, 0); //After we clear the display, return to the home position
}

void NokiaLCD::setCursor(int x, int y) 
{
  write(0, 0x80 | x);  // Column.
  write(0, 0x40 | y);  // Row.  ?
}

void NokiaLCD::bitmap(unsigned char bmp[])
{
  for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++)
  {
	write(LCD_DATA, bmp[index]);
  }
}

void NokiaLCD::sBitmap()
{
  if(Serial.available())
  {
     //clear();
     for(int i=0;i<(84*48/8);i++)
     {
       while(!Serial.available())
       {
         // Nada, perder tiempo.
       }
       char c = Serial.read();
       img[i] = c;
     }
     bitmap(img);
  }
}

void NokiaLCD::character(char character) 
{
  write(LCD_DATA, 0x00);
  for (int index = 0 ; index < 5 ; index++)
  write(LCD_DATA, ASCII[character - 0x20][index]);
  write(LCD_DATA, 0x00);
}

void NokiaLCD::print(char *characters) 
{
  while (*characters)
  character(*characters++);
}





