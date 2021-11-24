#include <fmsystem.h>
#include <fmconsole.h>
#include <fmcommand.h>
#include <fmstorage.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <esp_system.h>


#define BUFLEN 128

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
static void print_node_nvs_key_error(void);

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void system_initialize(void) {
  
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void system_register_commands(void) {
  // time
  static command_t cmd_time = {.next = 0x0, .action = 0x0, .command = "time", .help = "System time."};
  static action_t act_time_get = {.defact = 1, .next = 0x0, .action = 0x0, .help = "Get the time.", .func =  system_command_time_handler };
  static action_t act_time_set = {.defact = 0, .next = 0x0, .action = "set", .help = "Set the clock.", .func =  system_command_time_handler };

  command_add(&cmd_time);
  command_add_action(&cmd_time, &act_time_get);
  command_add_action(&cmd_time, &act_time_set);

  // Node 
  static command_t cmd_node = {.next = 0x0, .action = 0x0, .command = "node", .help = "Node information."};
  static action_t act_node_info = {.defact = 0, .next = 0x0, .action = "info", .help = "Get node information.", .func =  system_command_node_info_handler };
  static action_t act_node_id = {.defact = 0, .next = 0x0, .action = "id", .help = "Get node id.", .func = system_command_node_id_handler };
  static action_t act_node_set = {.defact = 0, .next = 0x0, .action = "set", .help = "Set a node parameter.", .func =  system_command_node_set_handler };
  static action_t act_node_get = {.defact = 0, .next = 0x0, .action = "get", .help = "Get a node parameter.", .func = system_command_node_get_handler };

  command_add(&cmd_node);
  command_add_action(&cmd_node, &act_node_info);
  command_add_action(&cmd_node, &act_node_id);
  command_add_action(&cmd_node, &act_node_set);
  command_add_action(&cmd_node, &act_node_get);


  // reboot
  static command_t cmd_reboot = {.next = 0x0, .action = 0x0, .command = "reboot", .help = "Reboot the system."};
  static action_t act_reboot = {.defact = 1, .next = 0x0, .action = 0x0, .help = "Reboot the system.", .func =  system_command_reboot_handler };

  command_add(&cmd_reboot);
  command_add_action(&cmd_reboot, &act_reboot);

  // mem
  static command_t cmd_mem = {.next = 0x0, .action = 0x0, .command = "mem", .help = "Get system memory."};
  static action_t act_mem = {.defact = 1, .next = 0x0, .action = 0x0, .help = "Get system memory.", .func =  system_command_mem_handler };

  command_add(&cmd_mem);
  command_add_action(&cmd_mem, &act_mem);
}



void system_print_heap_free(void) {
  uint32_t fmem = esp_get_free_heap_size();
  printf("Heap free %d\n", fmem);
}


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

  // there should be two args -- key and value

  if(argc < 2) {
    printf("Error - usage: node set <key> <value>\n");
    return 1;
  }

  char *key = argv[0];
  char *value = argv[1];

  if(strcmp(key, "name") == 0) {
    storage_set_node_name(value);
    printf("Node name set to '%s'.\n", value);
    return 0;
  }

  print_node_nvs_key_error();
  return 1;

  return 0;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int system_command_node_get_handler(int argc, char **argv){
  char buf[BUFLEN];

  if(argc < 1) {
    printf("Error - usage: node get <key>\n");
    return 1;
  }

  char *key = argv[0];

  if(strcmp(key, "name") == 0) {
    storage_get_node_name(buf, BUFLEN);
    printf("Node name: '%s'.\n", buf);
    return 0;
  }

  print_node_nvs_key_error();
  return 1;
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


/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static void print_node_nvs_key_error(void) {
  printf("Error - invalid key. Keys are: name.\n");
}
