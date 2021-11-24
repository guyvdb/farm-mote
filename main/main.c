
#include <stdio.h>
#include <esp_log.h>

#include <fmconsole.h>
#include <fmstorage.h>
#include <fmwifi.h>
#include <fmmesh.h>
#include <fmqueue.h>
#include <fmcommand.h>
#include <fmsystem.h>
#include <fmnet.h>

//#include "sdkconfig.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "esp_system.h"
//#include "esp_spi_flash.h"


//#include <esp_system.h>



void app_main(void) {

  esp_log_level_set("*", ESP_LOG_NONE);

  // initialize the components
  system_initialize();
  console_initialize();
  storage_initialize();
  wifi_initialize();
  mesh_initialize();
  queue_initialize();


  // register console commands with the command system
  command_register_commands();
  system_register_commands();
  storage_register_commands();
  wifi_register_commands();
  mesh_register_commands();
  queue_register_commands();


  net_initialize();

  // start the console event loop
  // This runs within this task and will block
  console_event_loop();


}

