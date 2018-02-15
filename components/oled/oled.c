#include "oled.h"

uint8_t buffer[DISP_HEIGHT/8][DISP_WIDTH] = {0x00};

void gpio_init(void)
{
	gpio_pad_select_gpio(OLED_SDA);
	gpio_pad_select_gpio(OLED_SCL);
	gpio_pad_select_gpio(OLED_RST);
	gpio_set_direction(OLED_SDA, GPIO_MODE_OUTPUT);
	gpio_set_direction(OLED_SCL, GPIO_MODE_OUTPUT);
	gpio_set_direction(OLED_RST, GPIO_MODE_OUTPUT);
	gpio_set_level(OLED_RST, 1);
	gpio_set_level(OLED_RST, 0);
	gpio_set_level(OLED_RST, 1);
}

void i2c_start()
{
	gpio_set_level(OLED_SCL, 1);
	gpio_set_level(OLED_SDA, 1);
	gpio_set_level(OLED_SDA, 0);
	gpio_set_level(OLED_SCL, 0);
}

void i2c_stop()
{
	gpio_set_level(OLED_SCL, 0);
	gpio_set_level(OLED_SDA, 0);
	gpio_set_level(OLED_SCL, 1);
	gpio_set_level(OLED_SDA, 1);
}

void i2c_write_byte(uint8_t byte)
{
	uint8_t i;
    for(i=0;i<8;i++)
    {
		if(byte&0x80)
			gpio_set_level(OLED_SDA, 1);
		else
			gpio_set_level(OLED_SDA, 0);
		gpio_set_level(OLED_SCL, 1);
		gpio_set_level(OLED_SCL, 0);
		byte<<=1;
    }
	gpio_set_level(OLED_SDA, 1);
	gpio_set_level(OLED_SCL, 1);
	gpio_set_level(OLED_SCL, 0);
}

void oled_write_cmd(uint8_t cmd)
{
	i2c_start();
	i2c_write_byte(OLED_ADDR);            //Slave address,SA0=0
    i2c_write_byte(OLED_CMD);                        //write command
    i2c_write_byte(cmd);	
	i2c_stop();
}

void oled_write_dat(uint8_t dat)
{
	i2c_start();
	i2c_write_byte(OLED_ADDR);            //Slave address,SA0=0
    i2c_write_byte(OLED_DAT);                        //write command
    i2c_write_byte(dat);	
	i2c_stop();
}

void oled_refresh(void)
{
	int x,y;
	for(y=0; y<DISP_HEIGHT/8; y++)
	{
		oled_write_cmd(PAGE_ADDR|y);
		for(x=0; x<DISP_WIDTH; x++)
		{
			oled_write_cmd(SEG_H_ADDR|(x>>4));
			oled_write_cmd(SEG_L_ADDR|(x&0x0f));
			oled_write_dat(buffer[y][x]);
		}

	}
}

void oled_cls(uint8_t need_refresh)
{
	int x, y;
	for(y=0; y<8; y++)
		for(x=0; x<128; x++)
			buffer[y][x] = 0x00;
	if(need_refresh)
		oled_refresh();
}

void oled_init(void)
{
	oled_write_cmd(0xae);//--turn off oled panel
	oled_write_cmd(0x00);//---set low column address
	oled_write_cmd(0x10);//---set high column address
	oled_write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_write_cmd(0x81);//--set contrast control register
	oled_write_cmd(0xcf); // Set SEG Output Current Brightness
	oled_write_cmd(0xa0);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_cmd(0xc0);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_cmd(0xa6);//--set normal display
	oled_write_cmd(0xa8);//--set multiplex ratio(1 to 64)
	oled_write_cmd(0x3f);//--1/64 duty
	oled_write_cmd(0xd3);//-set display offset        Shift Mapping RAM Counter (0x00~0x3F)
	oled_write_cmd(0x00);//-not offset
	oled_write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	oled_write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_cmd(0xd9);//--set pre-charge period
	oled_write_cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_cmd(0xda);//--set com pins hardware configuration
	oled_write_cmd(0x12);
	oled_write_cmd(0xdb);//--set vcomh
	oled_write_cmd(0x40);//Set VCOM Deselect Level
	oled_write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_write_cmd(0x02);//
	oled_write_cmd(0x8d);//--set Charge Pump enable/disable
	oled_write_cmd(0x14);//--set(0x10) disable
	oled_write_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	oled_write_cmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	oled_write_cmd(0xaf);//--turn on oled panel
}

esp_err_t oled_write_char(uint8_t x, uint8_t y, char ch, \
		uint8_t need_refresh)
{
	uint8_t _page = y/8;
	uint8_t i;
	int pos = -1;

	for(i=0; i<LENGTH_OF_TAB; i++)
		if(char_table[i][5] == ch)
			pos = i;
	if(pos == -1)
		return ESP_FAIL;

	for(i=0; i<5; i++)
		buffer[_page][x+i] = (char_table[(uint8_t)pos][i]);
	if(need_refresh)
		oled_refresh();
		
	return ESP_OK;
}

//not finish
esp_err_t oled_write_str(int8_t x, int8_t y, char *pstr, \
		uint8_t need_refresh)
{
	int i=0;
	for(i=0; i<DISP_WIDTH; i++)
	{
		if(pstr[i]!='\0'&&pstr[i]!='\n')
		{
			if(x+5*i<DISP_WIDTH-6&&y+8<DISP_HEIGHT)
				oled_write_char(x+5*i, y, pstr[i], False);
		}
		else if(pstr[i]=='\n')
		{
			y+=8;
			x+=-5*(i+1);
		}
		else
			break;
	}
	if(need_refresh)
		oled_refresh();

	return ESP_OK;
}

esp_err_t oled_draw_point(uint8_t x, uint8_t y, uint8_t need_refresh)
{
	uint8_t _page = y/8;
	uint8_t _offset = y%8;
	buffer[_page][x] |= (0x01<<_offset);
	if(need_refresh)
		oled_refresh();
	return ESP_OK;
}

esp_err_t oled_draw_circle(uint8_t x, uint8_t y, uint8_t rad, \
		uint8_t need_refresh)
{
	int _x, _y;
	for(_x=-rad; _x<=rad; _x++)
	{
		_y=(int)sqrt(rad*rad - _x*_x);

		if(_x+x>=0)
		{
			if(-_y+y>=0)
				oled_draw_point(_x+x, -_y+y, False);
			if(_y+y>=0)
				oled_draw_point(_x+x, _y+y, False);
		}
	}
	if(need_refresh)
		oled_refresh();
	return ESP_OK;
}

esp_err_t oled_draw_rect(uint8_t x, uint8_t y, uint8_t width, \
		uint8_t height, uint8_t need_refresh)
{
	int _x, _y;
	for(_y=y; _y<y+height; _y++)
		for(_x=x; _x<x+width; _x++)
			if (_y==y || _y==y+height-1 || _x==x || _x==x+width-1)
				oled_draw_point(_x, _y, False);
	if(need_refresh)
		oled_refresh();
	return ESP_OK;
}
