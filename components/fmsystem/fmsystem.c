#include <fmsystem.h>
#include <fmconsole.h>
#include <fmcommand.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <esp_system.h>

/*  
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
*/
 
/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static int system_command_time_handler(int argc, char **argv);
static int system_command_node_id_handler(int argc, char **argv);
static int system_command_node_info_handler(int argc, char **argv);
static int system_command_node_set_handler(int argc, char **argv);
static int system_command_node_get_handler(int argc, char **argv);
static int system_command_reboot_handler(int argc, char **argv);
static int system_command_mem_handler(int argc, char **argv);
//static int system_command_tasks_handler(int argc, char **argv);
//static uint32_t get_unix_timestamp(void);

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void system_initialize(void) {
  
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void system_register_commands(void) {
  command_t *c;

  printf("1\n");

  // time
  c = command_add("time", "System time settings.");
  command_add_default_action(c,"Get the time.", system_command_time_handler);
  command_add_action(c, "set", "Set the clock.", system_command_time_handler);

  printf("2\n");

  c = command_add("node", "Node information");
  command_add_action(c, "info","Print node information.", system_command_node_info_handler);
  command_add_action(c, "id", "Print node id.", system_command_node_id_handler);
  command_add_action(c, "set", "Set a node parameter.", system_command_node_set_handler);
  command_add_action(c, "get", "Get a node parameter", system_command_node_get_handler);

  printf("3\n");

  // reboot
  //c = command_add("reboot", "Reboot");
  //command_add_default_action(c, "Reboot the system", system_command_reboot_handler);

  printf("4\n");

  // mem
  //  c = command_add("mem", "System memory.");
  // command_add_default_action(c, "Get system memory.", system_command_mem_handler);


  printf("5\n");


  // tasks
  //c = command_add("tasks", "System tasks.");
  //command_add_default_action(c, "View system task information.", system_command_tasks_handler);

}



void system_print_heap_free(void) {
  uint32_t fmem = esp_get_free_heap_size();
  printf("Heap free %d\n", fmem);
}



/* ------------------------------------------------------------------------
 *
 * --------------------------------------------------------------------- */
/* static uint32_t get_unix_timestamp(void) { */
/*   struct timeval tv; */
/*   gettimeofday(&tv, NULL); */
/*   return (uint32_t)tv.tv_sec; */
/* } */


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
/*  
static int system_command_tasks_handler(int argc, char **argv) {
  TaskStatus_t * task_status_array;
  UBaseType_t task_count;
  unsigned int percent;
  unsigned int total_runtime;

  task_count =uxTaskGetNumberOfTasks();
  task_status_array = malloc(task_count * sizeof(TaskStatus_t));

  if(task_status_array != 0x0) {
    task_count = uxTaskGetSystemState(task_status_array, task_count, &total_runtime);
    total_runtime /= 100UL;

    if(total_runtime > 0) {

      printf("-----------------\n");
      printf("%-25s%-10s%-10s\n", "Task", "Time", "%");
      for(int i=0; i < task_count; i++) {
        percent = task_status_array[i].ulRunTimeCounter / total_runtime;
        printf("%-25s%-10d%-10d\n", task_status_array[i].pcTaskName, task_status_array[i].ulRunTimeCounter, percent);
      }
    }


    free(task_status_array);
  }

  return 0;

}
*/


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_time_handler(int argc, char **argv) {
  if(argc < 1) {
    // Get time
    struct timeval now;
    gettimeofday(&now,0x0);
    time_t seconds = now.tv_sec;

    //struct tm *local = localtime(&seconds);

    char buffer[64];
    struct tm timeinfo;

    localtime_r(&seconds, &timeinfo);
    strftime(buffer, sizeof(buffer),"%c",&timeinfo);

    printf("%s\n", buffer);
  } else {
    // Set Time
    struct tm result;
    time_t time;
    struct timeval val;
    int rc;

    // 2021-09-18T09:22:00
    // "%Y-%m-%dT%H:%M:%S.%f"
    if (strptime(argv[0],"%Y-%m-%dT%H:%M:%S" ,&result) == NULL) {
      printf("\nstrptime failed\n");
    } else {
      time = mktime(&result);
      val.tv_sec = time;
      val.tv_usec = 0;

      rc = settimeofday(&val, NULL);
      if (rc == 0) {
        printf("time set\n");
      } else {
        printf("time set failed with errno = %d\n",rc);
      }
    }
  }
  return 0;
}



/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_node_id_handler(int argc, char **argv) {

  uint8_t base_mac_addr[6] = {0};
  esp_err_t ret = ESP_OK;

  ret = esp_efuse_mac_get_default(base_mac_addr);
  if (ret != ESP_OK) {
    printf("Error - failed to get base MAC address from EFUSE BLK0. (%s)", esp_err_to_name(ret));
  } else {
    printf("%X:%X:%X:%X:%X:%X\n",
           base_mac_addr[0],
           base_mac_addr[1],
           base_mac_addr[2],
           base_mac_addr[3],
           base_mac_addr[4],
           base_mac_addr[5]
           );
  }

  return 0;
}


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_node_info_handler(int argc, char **argv) {
  return 0;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_node_set_handler(int argc, char **argv) {
  return 0;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_node_get_handler(int argc, char **argv){
  return 0;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_reboot_handler(int argc, char **argv) {
  esp_restart();
  return 0;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_mem_handler(int argc, char **argv) {
  uint32_t fmem = esp_get_free_heap_size();
  printf("Heap free %d\n", fmem);
  return 0;
}

