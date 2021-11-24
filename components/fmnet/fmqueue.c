#include <fmqueue.h>
#include <fmcommand.h>
#include <fmstorage.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


#define BUFLEN 128

/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static int queue_command_get_handler(int argc, char **argv);
static int queue_command_set_handler(int argc, char **argv);
static void print_nvs_key_error(void);


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void queue_initialize(void) {}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void queue_task_start(void) {}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void queue_register_commands(void) {
  static command_t cmd_queue = {.next = 0x0, .action = 0x0, .command = "queue", .help = "Modify message queue settings."};
  static action_t act_queue_set = {.defact = 0, .next = 0x0, .action = "set", .help = "Set a queue parameter.", .func =  queue_command_set_handler};
  static action_t act_queue_get = {.defact = 0, .next = 0x0, .action = "get", .help = "Get a queue parameter.", .func =  queue_command_get_handler};

  command_add(&cmd_queue);
  command_add_action(&cmd_queue, &act_queue_set);
  command_add_action(&cmd_queue, &act_queue_get);
}


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int queue_command_get_handler(int argc, char **argv) {
  char buf[BUFLEN];
  uint16_t p;


  if(argc < 1) {
    printf("Error - usage: queue get <key>\n");
    return 1;
  }

  char *key = argv[0];

  if(strcmp(key, "address") == 0) {
    storage_get_queue_address(buf, BUFLEN);
    printf("Queue address: '%s'.\n", buf);
    return 0;
  }

  if(strcmp(key, "port") == 0) {
    storage_get_queue_port(&p);
    printf("Queue port: '%d'.\n", p);
    return 0;
  }

  print_nvs_key_error();
  return 1;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int queue_command_set_handler(int argc, char **argv){
  // there should be two args -- key and value

  if(argc < 2) {
    printf("Error - usage: queue set <key> <value>\n");
    return 1;
  }

  char *key = argv[0];
  char *value = argv[1];
  uint16_t p;

  if(strcmp(key, "address") == 0) {
    storage_set_queue_address(value);
    printf("Queue address set to '%s'.\n", value);
    return 0;
  }

  if(strcmp(key, "port") == 0) {
    p = (uint16_t)atoi(value);
    storage_set_queue_port(p);
    printf("Queue port set to '%d'.\n", p);
    return 0;
  }

  print_nvs_key_error();
  return 1;
}



/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static void print_nvs_key_error(void) {
  printf("Error - invalid key. Keys are: address & port.\n");
}

