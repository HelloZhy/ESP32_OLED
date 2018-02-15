/**************************
 *ESP32 driver:OLED SSD1603
 *Communication:I2C
 *Resolution:128*64
 **************************/
#ifndef _OLED_H_
#define _OLED_H_

#include <stdio.h>
#include <math.h>
#include <esp_types.h>
#include <esp_err.h>
#include <driver/gpio.h>

#define True  0x1
#define False 0x0

#define OLED_SCL	GPIO_NUM_15
#define OLED_SDA	GPIO_NUM_4
#define OLED_RST	GPIO_NUM_16
#define OLED_ADDR	0x78
#define OLED_CMD	0x00
#define OLED_DAT	0x40

#define DISP_WIDTH		128
#define DISP_HEIGHT		64

#define ACK_CHECK_EN	0x1
#define ACK_CHECK_DIS	0x0
#define ACK_VAL			0x0
#define NACK_VAL		0x1

#define PAGE_ADDR		0xB0
#define SEG_H_ADDR		0x10
#define SEG_L_ADDR		0x00

// Display commands
#define CHARGEPUMP			0x8D
#define COLUMNADDR			0x21
#define COMSCANDEC			0xC8
#define COMSCANINC			0xC0
#define DISPLAYALLON		0xA5
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYOFF			0xAE
#define DISPLAYON			0xAF
#define EXTERNALVCC			0x1
#define INVERTDISPLAY		0xA7
#define MEMORYMODE			0x20
#define NORMALDISPLAY		0xA6
#define PAGEADDR			0x22
#define SEGREMAP			0xA0
#define SETCOMPINS			0xDA
#define SETCONTRAST			0x81
#define SETDISPLAYCLOCKDIV  0xD5
#define SETDISPLAYOFFSET	0xD3
#define SETHIGHCOLUMN		0x10
#define SETLOWCOLUMN		0x00
#define SETMULTIPLEX		0xA8
#define SETPRECHARGE		0xD9
#define SETSEGMENTREMAP		0xA1
#define SETSTARTLINE		0x40
#define SETVCOMDETECT		0xDB
#define SWITCHCAPVCC		0x2

/*
 * Make sure your task is locked before you use APIs below
 *
 * !!!These function are not Task Safe!!!
 */
extern uint8_t buffer[DISP_HEIGHT/8][DISP_WIDTH];
#define LENGTH_OF_TAB		95
extern const uint8_t char_table[][6];

void gpio_init();
void i2c_start();
void i2c_stop();
void i2c_write_byte(uint8_t byte);
void oled_write_cmd(uint8_t cmd);
void oled_write_dat(uint8_t dat);

void oled_refresh();
void oled_cls(uint8_t need_refresh);
void oled_init();

/*high level api*/
//string operation on oled
esp_err_t oled_write_char(uint8_t x, uint8_t y, char ch, \
		uint8_t need_refresh);
esp_err_t oled_write_str(int8_t x, int8_t y, char *pstr, \
		uint8_t need_refresh);

//graphic operation on oled
esp_err_t oled_draw_point(uint8_t x, uint8_t y, uint8_t need_refresh);
esp_err_t oled_draw_circle(uint8_t x, uint8_t y, uint8_t rad, \
		uint8_t need_refresh);
esp_err_t oled_draw_rect(uint8_t x, uint8_t y, uint8_t width, \
		uint8_t height, uint8_t need_refresh);

#endif
