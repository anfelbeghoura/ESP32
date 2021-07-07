/*
 * main.c
 *
 *  Created on: 4 nov. 2019
 *      Author: anfel.beghoura
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"


#include "esp_log.h"


#include "esp_wifi_config.h"



/* FreeRTOS event group to signal when we are connected*/




void app_main(void)
{
	wifi_station_init();
	wifi_station_config();
}
