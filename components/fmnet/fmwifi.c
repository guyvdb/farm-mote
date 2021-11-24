

#include <fmwifi.h>
#include <stdio.h>
#include <string.h>
#include <fmconsole.h>
#include <fmcommand.h>
#include <fmstorage.h>


#define BUFLEN 128

/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static int wifi_command_set_handler(int argc, char **argv);
static int wifi_command_get_handler(int argc, char **argv);
static int wifi_command_up_handler(int argc, char **argv);
static int wifi_command_down_handler(int argc, char **argv);

static void print_nvs_key_error(void);

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void wifi_initialize(void) {
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void wifi_register_commands(void) {
  static command_t cmd_wifi = {.next = 0x0, .action = 0x0, .command = "wifi", .help = "Modify wifi settings."};
  static action_t act_wifi_set = {.defact = 0, .next = 0x0, .action = "set", .help = "Set a wifi parameter.", .func =  wifi_command_set_handler};
  static action_t act_wifi_get = {.defact = 0, .next = 0x0, .action = "get", .help = "Get a wifi parameter.", .func =  wifi_command_get_handler};
  static action_t act_wifi_up = {.defact = 0, .next = 0x0, .action = "up", .help = "Bring up the wifi connection.", .func =  wifi_command_up_handler};
  static action_t act_wifi_down = {.defact = 0, .next = 0x0, .action = "down", .help = "Bring down the wifi connection.", .func =  wifi_command_down_handler};

  command_add(&cmd_wifi);
  command_add_action(&cmd_wifi, &act_wifi_set);
  command_add_action(&cmd_wifi, &act_wifi_get);
  command_add_action(&cmd_wifi, &act_wifi_up);
  command_add_action(&cmd_wifi, &act_wifi_down);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void wifi_task_start(void) {

}

/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static void print_nvs_key_error(void) {
  printf("Error - invalid key. Keys are: ssid & password.\n");
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int wifi_command_set_handler(int argc, char **argv) {


  // there should be two args -- key and value

  if(argc < 2) {
    printf("Error - usage: wifi set <key> <value>\n");
    return 1;
  }

  char *key = argv[0];
  char *value = argv[1];

  if(strcmp(key, "ssid") == 0) {
    storage_set_wifi_ssid(value);
    printf("Wifi ssid set to '%s'.\n", value);
    return 0;
  }

  if(strcmp(key, "password") == 0) {
    storage_set_wifi_password(value);
    printf("Wifi password set to '%s'.\n", value);
    return 0;
  }

  print_nvs_key_error();
  return 1;
}


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int wifi_command_get_handler(int argc, char **argv) {
  char buf[BUFLEN];

  if(argc < 1) {
    printf("Error - usage: wifi get <key>\n");
    return 1;
  }

  char *key = argv[0];

  if(strcmp(key, "ssid") == 0) {
    storage_get_wifi_ssid(buf, BUFLEN);
    printf("Wifi ssid: '%s'.\n", buf);
    return 0;
  }

  if(strcmp(key, "password") == 0) {
    storage_get_wifi_password(buf, BUFLEN);
    printf("Wifi password: '%s'.\n", buf);
    return 0;
  }

  print_nvs_key_error();
  return 1;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int wifi_command_up_handler(int argc, char **argv) {
  printf("wifi_command_up_handler: argc: %d\n", argc);
  for(int i=0;i<argc;i++) {
    printf("   %d) %s\n", i, argv[i]);
  }
  return 0;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int wifi_command_down_handler(int argc, char **argv) {
  printf("wifi_command_down_handler: argc: %d\n", argc);
  for(int i=0;i<argc;i++) {
    printf("   %d) %s\n", i, argv[i]);
  }
  return 0;
}
