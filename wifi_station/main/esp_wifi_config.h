/*
 * esp_wifi_config.h
 *
 *  Created on: 5 nov. 2019
 *      Author: anfel.beghoura
 */

#ifndef MAIN_ESP_WIFI_CONFIG_H_
#define MAIN_ESP_WIFI_CONFIG_H_


#define _NVS_MAXIMUM_RETRY  5
#define WIFI_STA_DEBUG_MODE_ENABLE 1

struct WIFI_CONFIG_MANAGER {

	int32_t STATION_CONNECT_STATUS;
};




void event_handler();
void wifi_station_config();
void wifi_station_init();

#endif /* MAIN_ESP_WIFI_CONFIG_H_ */
