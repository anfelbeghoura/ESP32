/*
 * esp_wifi_config.c
 *
 *  Created on: 5 nov. 2019
 *      Author: anfel.beghoura
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_wifi_config.h"


static int _NVS_RETRY_COUNT = 0;

struct WIFI_CONFIG_MANAGER WIFI_MANAG;
static EventGroupHandle_t wifi_evn_grp;

const int _WIFI_CONNECTED_BIT = BIT0;


void event_handler(void* arg, esp_event_base_t ev_base, int32_t ev_id, void* ev_data)
{
    if (ev_base == WIFI_EVENT && ev_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (ev_base == WIFI_EVENT && ev_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (_NVS_RETRY_COUNT < _NVS_MAXIMUM_RETRY) {
            esp_wifi_connect();
            xEventGroupClearBits(wifi_evn_grp, _WIFI_CONNECTED_BIT);
            _NVS_RETRY_COUNT++;
			#if WIFI_STA_DEBUG_MODE_ENABLE
            ESP_LOGI("event_handler", "NVS_RETry to connect to the AP");
			#endif
        }
		#if WIFI_STA_DEBUG_MODE_ENABLE
        ESP_LOGI("event_handler","connect to the AP fail");
		#endif
    }
    else if (ev_base == IP_EVENT && ev_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) ev_data;
        ESP_LOGI("event_handler", "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
        _NVS_RETRY_COUNT = 0;
        xEventGroupSetBits(wifi_evn_grp, _WIFI_CONNECTED_BIT);
    }
}

void wifi_station_config(){

	#if WIFI_STA_DEBUG_MODE_ENABLE
	ESP_LOGI("wifi_station_config", "wifi_station_config started ...");
	#endif
	wifi_evn_grp = xEventGroupCreate();

    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t conf = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&conf));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t WIFI_CONF = {
        .sta = {
            .ssid = "Hello_world",
            .password = "helloworld"
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &WIFI_CONF) );
    ESP_ERROR_CHECK(esp_wifi_start() );

	#if WIFI_STA_DEBUG_MODE_ENABLE
    ESP_LOGI("wifi_station_config", "connect to ap SSID: %s - password: %s", WIFI_CONF.sta.ssid, WIFI_CONF.sta.password);
	#endif
}

void wifi_station_init(){
	// NVS Init
	#if WIFI_STA_DEBUG_MODE_ENABLE
	ESP_LOGI("wifi_station_init", "station mode init");
	#endif

	#if WIFI_STA_DEBUG_MODE_ENABLE
	ESP_LOGI("wifi_station_init", "init nvs config");
	#endif
	esp_err_t NVS_RET = nvs_flash_init();

	if (NVS_RET == ESP_ERR_NVS_NO_FREE_PAGES || NVS_RET == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
	    NVS_RET = nvs_flash_init();
	}
	ESP_ERROR_CHECK(NVS_RET);
}
