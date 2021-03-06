#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_netif.h>
#include <nvs_flash.h>
#include <lwip/err.h>
#include <lwip/sys.h>

#include <framecon.h>
#include <kv.h>
#include <console.h>



#define RETRY_MAX  10

static char wifi_ssid[32];
static char wifi_password[64];

static int retry_count = 0;
static int retry_delay = 1000;

static EventGroupHandle_t wifi_event_group;


static void wifi_reconnect(void) {
  // move wait and reconnect code from wifi_event_handler
  // to this function
}

/* ------------------------------------------------------------------------
 * 
 * --------------------------------------------------------------------- */
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
    case SYSTEM_EVENT_STA_START:
      xEventGroupSetBits(wifi_event_group, WIFI_INITIALIZED);
      //console_log_info("Wifi connecting. event = STA_START");      
 	    esp_wifi_connect();
      break;
    case SYSTEM_EVENT_STA_STOP:
      xEventGroupClearBits(wifi_event_group, WIFI_INITIALIZED);
 	    ESP_ERROR_CHECK( esp_wifi_deinit());
      //console_log_info("Wifi disconnecting. event = STA_STOP");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      xEventGroupSetBits(wifi_event_group,WIFI_CONNECTED);
      //console_log_info("Wifi connected. event = STA_CONNECTED");
      retry_count = 0;
      retry_delay = 1000; // retry in 1 second
 	    break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED);
      xEventGroupClearBits(wifi_event_group,  WIFI_GOT_IP);
      // console_log_info("Wifi disconnected. event = STA_DISCONNECTED");

      retry_count++;
      int seconds = retry_delay / 1000;
      //console_log_info("Wifi reconnect attempt in %d seconds.", seconds);
      vTaskDelay(retry_delay / portTICK_PERIOD_MS);
      retry_delay = retry_delay * 2;
      if (retry_delay > 16000) {
        retry_delay = 16000;
      }
      esp_wifi_connect();           
      break;
    default:
      console_log_info("Wifi got unkown event. base = %d, event = %d",(int)event_base, event_id);
      break;
    }
}

/* ------------------------------------------------------------------------
 * 
 * --------------------------------------------------------------------- */
static void ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
    case IP_EVENT_STA_GOT_IP:
      xEventGroupSetBits(wifi_event_group,  WIFI_GOT_IP);
      //tcpip_adapter_ip_info_t ip_info;
	    //ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
      //console_log_info("Wifi IP address assigned. IP = %s", ip4addr_ntoa(&ip_info.ip));

      // let frame connection client know interface has changed
      framecon_wifi_interface_state_change(1);
       
      
      break;
    case IP_EVENT_STA_LOST_IP:
      xEventGroupClearBits(wifi_event_group,  WIFI_GOT_IP);
      //console_log_info("Wifi IP address lost");

      // let frame connection client know interface has changed
      framecon_wifi_interface_state_change(0);
       
      

      break;
    default:
      console_log_info("Wifi got unkown event. base = %d, event = %d",(int)event_base, event_id);
      break;
    }
}

/* ------------------------------------------------------------------------
 * 
 * --------------------------------------------------------------------- */
void wifi_wait_for_interface(void) {
  const TickType_t xTicksToWait = 10000 / portTICK_PERIOD_MS; 
  xEventGroupWaitBits(wifi_event_group,WIFI_GOT_IP,0,0,xTicksToWait);
}


/* ------------------------------------------------------------------------
 * 
 * --------------------------------------------------------------------- */
void initialize_wifi(void) {

  retry_count = 0;
  retry_delay = 1000; // retry in 1 second
  
  esp_log_level_set("wifi", ESP_LOG_NONE);

  
  // init wifi 
  ESP_ERROR_CHECK(esp_netif_init());

  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));


  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

  // setup ssid + password
  get_wifi_ssid(wifi_ssid, sizeof(wifi_ssid));
  get_wifi_password(wifi_password, sizeof(wifi_password));

  wifi_config_t wifi_config = {.sta = {} };

  for(int i=0;i<sizeof(wifi_ssid);i++) {
    wifi_config.sta.ssid[i] = wifi_ssid[i];
  }

  for(int i=0;i<sizeof(wifi_password);i++) {
    wifi_config.sta.password[i] = wifi_password[i];
  }


  // start wifi 
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
  ESP_ERROR_CHECK(esp_wifi_start() );

  // set up the wifi event group
  wifi_event_group =  xEventGroupCreate();
  
}

/* ------------------------------------------------------------------------
 * 
 * --------------------------------------------------------------------- */
void deinitialize_wifi(void) {

  vEventGroupDelete(wifi_event_group);
  
  // unregister event handlers
  //ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
  //ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
}



/* ------------------------------------------------------------------------
 * the wifi is valid if it is connected and has an IP
 * --------------------------------------------------------------------- */
int wifi_valid(void) {
   EventBits_t flag = xEventGroupGetBits(wifi_event_group);
   if( (flag & WIFI_CONNECTED) && (flag & WIFI_GOT_IP) ){
     return 1;
   } else {
     return 0;
   }
}

