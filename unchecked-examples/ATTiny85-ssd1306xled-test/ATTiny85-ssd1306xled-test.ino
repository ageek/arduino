//http://cyaninfinite.com/tutorials/interfacing-a-i2c-0-96-oled-display-with-attiny85/

//#include <EEPROM.h>
#include "font6x8.h"
#include "font8X16.h"
#include "font16x16cn.h"
#include "ssd1306xled.h"

// comment this for ATTiny85 core, required for digispark kit
#define _delay_ms delay

#define F_CPU 8000000UL      //ATtiny85 CPU Clock speed (8MHz optimal[8000000], 1MHz Default[1000000])

// ----------------------------------------------------------------------------

// ---------------------  // Vcc, Pin 1 on SSD1306 Board
// ---------------------  // GND, Pin 2 on SSD1306 Board
#define SSD1306_SCL   PB0 // SCL, Pin 3 on SSD1306 Board
#define SSD1306_SDA   PB1 // SDA, Pin 4 on SSD1306 Board

#define SSD1306_SA    0x78  // Slave address

// ----------------------------------------------------------------------------


void setup(){ 
  // Small delay is necessary if ssd1306_initis the first operation in the application.
  _delay_ms(40);
  ssd1306_init();

  ssd1306_fillscreen(0x00);
  ssd1306_char_f8x16(1, 2, "Photon OS");
  ssd1306_char_f6x8(1, 5, "Loading test...");

  ssd1306_char_f6x8(1, 2, "Photon OS");
  _delay_ms(4000);


  uint8_t p = 0xff;

  for (uint8_t i = 0; i < 4; i++)
  {
    p = (p >> i);
    ssd1306_fillscreen(~p);
    _delay_ms(1);
  }
}

void loop()
{
  delay(40);
      
  ssd1306_init();
  ssd1306_fillscreen(0x00);
  ssd1306_char_f6x8(1, 0, "I'm a ATtiny85"); //Cannot start at 0 for x. Gives problem

  ssd1306_char_f6x8(1, 0, "I'm a small and mighty.");
  _delay_ms(5000);

  ssd1306_char_f6x8(1, 0, "Who says I can't run this LED screen?");
  _delay_ms(5000);


  ssd1306_char_f6x8(1, 0, "There am I running it now.");
  _delay_ms(5000);

}

// ----------------------------------------------------------------------------

#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT) PORTB &= ~(1 << PORT)

// ----------------------------------------------------------------------------

// Some code based on "IIC_wtihout_ACK" by http://www.14blog.com/archives/1358

void ssd1306_init(void)
{
  DDRB |= (1 << SSD1306_SDA); // Set port as output
  DDRB |= (1 << SSD1306_SCL); // Set port as output

  ssd1306_send_command(0xAE); // display off
  ssd1306_send_command(0x00); // Set Memory Addressing Mode
  ssd1306_send_command(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  ssd1306_send_command(0x40); // Set Page Start Address for Page Addressing Mode,0-7
  ssd1306_send_command(0x81); // Set COM Output Scan Direction
  ssd1306_send_command(0xCF); // ---set low column address
  ssd1306_send_command(0xA1); // ---set high column address
  ssd1306_send_command(0xC8); // --set start line address
  ssd1306_send_command(0xA6); // --set contrast control register
  ssd1306_send_command(0xA8);
  ssd1306_send_command(0x3F); // --set segment re-map 0 to 127
  ssd1306_send_command(0xD3); // --set normal display
  ssd1306_send_command(0x00); // --set multiplex ratio(1 to 64)
  ssd1306_send_command(0xD5); // 
  ssd1306_send_command(0x80); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  ssd1306_send_command(0xD9); // -set display offset
  ssd1306_send_command(0xF1); // -not offset
  ssd1306_send_command(0xDA); // --set display clock divide ratio/oscillator frequency
  ssd1306_send_command(0x12); // --set divide ratio
  ssd1306_send_command(0xDB); // --set pre-charge period
  ssd1306_send_command(0x40); // 
  ssd1306_send_command(0x20); // --set com pins hardware configuration
  ssd1306_send_command(0x02);
  ssd1306_send_command(0x8D); // --set vcomh
  ssd1306_send_command(0x14); // 0x20,0.77xVcc
  ssd1306_send_command(0xA4); // --set DC-DC enable
  ssd1306_send_command(0xA6); // 
  ssd1306_send_command(0xAF); // --turn on oled panel 
}

void ssd1306_xfer_start(void)
{
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
}

void ssd1306_xfer_stop(void)
{
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
}

void ssd1306_send_byte(uint8_t byte)
{
  uint8_t i;
  for(i=0; i<8; i++)
  {
    if((byte << i) & 0x80)
      DIGITAL_WRITE_HIGH(SSD1306_SDA);
    else
      DIGITAL_WRITE_LOW(SSD1306_SDA);
    
    DIGITAL_WRITE_HIGH(SSD1306_SCL);
    DIGITAL_WRITE_LOW(SSD1306_SCL);
  }
  DIGITAL_WRITE_HIGH(SSD1306_SDA);
  DIGITAL_WRITE_HIGH(SSD1306_SCL);
  DIGITAL_WRITE_LOW(SSD1306_SCL);
}

void ssd1306_send_command(uint8_t command)
{
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);  // Slave address, SA0=0
  ssd1306_send_byte(0x00);  // write command
  ssd1306_send_byte(command);
  ssd1306_xfer_stop();
}

void ssd1306_send_data_start(void)
{
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);
  ssd1306_send_byte(0x40);  //write data
}

void ssd1306_send_data_stop(void)
{
  ssd1306_xfer_stop();
}

void ssd1306_setpos(uint8_t x, uint8_t y)
{
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);  //Slave address,SA0=0
  ssd1306_send_byte(0x00);  //write command

  ssd1306_send_byte(0xb0+y);
  ssd1306_send_byte(((x&0xf0)>>4)|0x10); // |0x10
  ssd1306_send_byte((x&0x0f)|0x01); // |0x01

  ssd1306_xfer_stop();
}

void ssd1306_fillscreen(uint8_t fill_Data)
{
  uint8_t m,n;
  for(m=0;m<8;m++)
  {
    ssd1306_send_command(0xb0+m); //page0-page1
    ssd1306_send_command(0x00);   //low column start address
    ssd1306_send_command(0x10);   //high column start address
    ssd1306_send_data_start();
    for(n=0;n<128;n++)
    {
      ssd1306_send_byte(fill_Data);
    }
    ssd1306_send_data_stop();
  }
}

void ssd1306_char_f6x8(uint8_t x, uint8_t y, const char ch[])
{
  uint8_t c,i,j=0;
  while(ch[j] != '\0')
  {
    c = ch[j] - 32;
    if(x>126)
    {
      x=0;
      y++;
    }
    ssd1306_setpos(x,y);
    ssd1306_send_data_start();
    for(i=0;i<6;i++)
    {
      ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font6x8[c*6+i]));
    }
    ssd1306_send_data_stop();
    x += 6;
    j++;
  }
}

void ssd1306_char_f8x16(uint8_t x, uint8_t y,const char ch[])
{
  uint8_t c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c = ch[j] - 32;
    if (x>120)
    {
      x=0;
      y++;
    }
    ssd1306_setpos(x,y);
    ssd1306_send_data_start();
    for(i=0;i<8;i++)
    {
      ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8X16[c*16+i]));
    }
    ssd1306_send_data_stop();
    ssd1306_setpos(x,y+1);
    ssd1306_send_data_start();
    for(i=0;i<8;i++)
    {
      ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8X16[c*16+i+8]));
    }
    ssd1306_send_data_stop();
    x+=8;
    j++;
  }
}

void ssd1306_char_f16x16(uint8_t x, uint8_t y, uint8_t N)
{
  uint8_t wm=0;
  unsigned int adder = 32 * N;
  ssd1306_setpos(x , y);
  ssd1306_send_data_start();
  for(wm = 0; wm < 16; wm++)
  {
    ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8X16cn[adder]));
    adder += 1;
  }
  ssd1306_send_data_stop();
  ssd1306_setpos(x,y + 1);
  ssd1306_send_data_start();
  for(wm = 0;wm < 16;wm++)
  {
    ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8X16cn[adder]));
    adder += 1;
  }
  ssd1306_send_data_stop();
}

void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bitmap[])
{
  unsigned int j = 0;
  uint8_t x,y;

  if(y1%8==0)
  y=y1/8;
  else
  y=y1/8+1;
  for(y=y0;y<y1;y++)
  {
    ssd1306_setpos(x0,y);
    ssd1306_send_data_start();
    for(x=x0;x<x1;x++)
    {
      ssd1306_send_byte(pgm_read_byte(&bitmap[j++]));
    }
    ssd1306_send_data_stop();
  }
}


