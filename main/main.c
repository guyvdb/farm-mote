
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

  printf("A\n");
  system_initialize();
  console_initialize();
  storage_initialize();
  wifi_initialize();
  mesh_initialize();
  queue_initialize();


  printf("B\n");
  // register console commands with the command system
  command_register_commands();

  printf("C\n");

  // print free mem

  //  uint32_t fmem = esp_get_free_heap_size();
  //printf("Heap free %d\n", fmem);

  system_register_commands();











  printf("D\n");
  storage_register_commands();

  printf("E\n");
  wifi_register_commands();

  printf("F\n");
  mesh_register_commands();

  printf("G\n");
  queue_register_commands();


  net_initialize();

  // start the console event loop
  // This runs within this task and will block
  printf("D\n");
  console_event_loop();


}

