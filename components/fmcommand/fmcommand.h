#ifndef FM_COMMAND_H
#define FM_COMMAND_H


#include <stdint.h>

// example: wifi set ssid <value>
//        : wifi get ssid
//        : wifi up
//        : wifi down 



typedef int (*command_action_func)(int argc, char **argv);


typedef struct {
  uint8_t defact;
  void* next;
  char *action;
  char *help;
  command_action_func func;
} action_t;


typedef struct {
  void* next;
  void* action;
  char *command;
  char *help;  
} command_t;


void command_register_commands(void);



void command_add(command_t *c);
void command_add_action(command_t *c, action_t *a);



void command_help(void);

command_t * command_find(const char *command);
action_t * command_find_action(command_t *command, const char *action);
action_t * command_find_default_action(command_t *command);

void command_print_command_tree(void);

#endif
