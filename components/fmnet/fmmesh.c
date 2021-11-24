#include <fmmesh.h>
#include <stdio.h>

#include <string.h>
//#include <fmconsole.h>
#include <fmcommand.h>
#include <fmstorage.h>






#define BUFLEN 128

/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static int mesh_command_get_handler(int argc, char **argv);
static int mesh_command_set_handler(int argc, char **argv);
static int mesh_command_routes_handler(int argc, char **argv);
static void print_nvs_key_error(void);




/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void mesh_initialize(void) {}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void mesh_task_start(void) {}



/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void mesh_register_commands(void) {

  static command_t cmd_mesh = {.next = 0x0, .action = 0x0, .command = "mesh", .help = "Modify mesh settings."};
  static action_t act_mesh_set = {.defact = 0, .next = 0x0, .action = "set", .help = "Set a mesh parameter.", .func =  mesh_command_set_handler};
  static action_t act_mesh_get = {.defact = 0, .next = 0x0, .action = "get", .help = "Get a mesh parameter.", .func =  mesh_command_get_handler};
  static action_t act_mesh_routes = {.defact = 0, .next = 0x0, .action = "up", .help = "Get the mesh routing table.", .func =  mesh_command_routes_handler};

  command_add(&cmd_mesh);
  command_add_action(&cmd_mesh, &act_mesh_set);
  command_add_action(&cmd_mesh, &act_mesh_get);
  command_add_action(&cmd_mesh, &act_mesh_routes);
}


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int mesh_command_get_handler(int argc, char **argv) {
  char buf[BUFLEN];

  if(argc < 1) {
    printf("Error - usage: mesh get <key>\n");
    return 1;
  }

  char *key = argv[0];

  if(strcmp(key, "id") == 0) {
    storage_get_mesh_id(buf, BUFLEN);
    printf("Mesh id: '%s'.\n", buf);
    return 0;
  }

  if(strcmp(key, "password") == 0) {
    storage_get_mesh_password(buf, BUFLEN);
    printf("Mesh password: '%s'.\n", buf);
    return 0;
  }

  print_nvs_key_error();
  return 1;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int mesh_command_set_handler(int argc, char **argv){
  // there should be two args -- key and value

  if(argc < 2) {
    printf("Error - usage: mesh set <key> <value>\n");
    return 1;
  }

  char *key = argv[0];
  char *value = argv[1];

  if(strcmp(key, "id") == 0) {
    storage_set_mesh_id(value);
    printf("Mesh id set to '%s'.\n", value);
    return 0;
  }

  if(strcmp(key, "password") == 0) {
    storage_set_mesh_password(value);
    printf("Mesh password set to '%s'.\n", value);
    return 0;
  }

  print_nvs_key_error();
  return 1;
}

/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int mesh_command_routes_handler(int argc, char **argv) {
  printf("show mesh routing table here.\n");
  return 0;
}


/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static void print_nvs_key_error(void) {
  printf("Error - invalid key. Keys are: id & password.\n");
}
