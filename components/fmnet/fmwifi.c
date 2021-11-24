

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
  command_t *c = command_add("wifi", "Modify wifi settings.");
  command_add_action(c, "set", "Set a wifi parameter.", wifi_command_set_handler);
  command_add_action(c, "get", "Get a wifi parameter.", wifi_command_get_handler);
  command_add_action(c, "up", "Bring up the wifi connection.", wifi_command_up_handler);
  command_add_action(c,"down", "Bring down the wifi connection.", wifi_command_down_handler);
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
