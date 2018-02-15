/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "oled.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/

void oled_task(void *pvParameter)
{
	gpio_init();
	oled_init();
	int i;
    while(1) 
	{
		oled_cls(False);
		for(i=0;i<8;i++)
		{
			oled_cls(False);
			oled_write_str(10*i,0,"hello\nworld!",True);
			vTaskDelay(100);
		}
    }
}

void app_main()
{
    xTaskCreate(&oled_task, "oled_task", 4096, NULL, 2, NULL);
}
