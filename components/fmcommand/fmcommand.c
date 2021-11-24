
#include <fmcommand.h>
#include <string.h>
#include <stdio.h>


static command_t *root_command = 0x0;



/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static command_t *get_last_command(void);
static action_t *get_last_action(command_t *c);

static int command_help_handler(int argc, char **argv);


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void command_register_commands(void) {
  command_t *c = command_add("help", "Display help text.");
  command_add_default_action(c, "Display help text.", command_help_handler);
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
command_t* command_add(const char *command, const char *help) {

  // create the command 
  command_t *c = malloc(sizeof(command_t));

  c->command = malloc(strlen(command)+1);
  strcpy(c->command, command);

  c->help = malloc(strlen(help)+ 1);
  strcpy(c->help, help);

  c->next = 0x0;
  c->action = 0x0;


  // link the command to linked list
  command_t *last = get_last_command();
  if(last == 0x0) {
    root_command = c; 
  } else {
    last->next = (void*)c;
  }

  return c;
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
action_t* command_add_action(command_t *command, const char *action, const char *help, command_action_func func) {
  // create action 
  action_t *a = malloc(sizeof(action_t));

  if(action != 0x0) {
    a->action = malloc(strlen(action));
    strcpy(a->action, action);
  } else {
    a->action = 0x0;
  }

  a->help = malloc(strlen(help));
  strcpy(a->help, help);

  a->func = func;
  a->next = 0x0;
  a->defact = 0;

  // link the action to the linked list
  action_t *last = get_last_action(command);
  if(last == 0x0) {
    command->action = a;
  } else {
    last->next = (void*)a;
  }

  return a;
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
action_t* command_add_default_action(command_t *command, const char *help, command_action_func func) {
  action_t *a = command_add_action(command,0x0,help, func);
  a->defact = 1;

  return a;
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
command_t * command_find(const char *command) {

  if(root_command == 0x0) {
    return 0x0;
  }

  command_t *c = root_command;
  while(1) {
    if(strcmp(c->command, command) == 0) {
      return c;
    }
    c = (command_t*)c->next;
    if(c == 0x0) {
      return c;
    }
  }

  // should never get here
  return 0x0;
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
action_t * command_find_action(command_t *command, const char *action) {
  if(command->action == 0x0) {
    return 0x0;
  }

  action_t *a = (action_t*)command->action;

  while(1) {
    if(a->action != 0x0 && strcmp(a->action, action) == 0) {
      return a;
    }

    a = (action_t*)a->next;

    if(a == 0x0) {
      return 0x0;
    }
  }

  // should never get here
  return 0x0;

}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
action_t * command_find_default_action(command_t *command) {
  if(command->action == 0x0) {
    return 0x0;
  }

  action_t *a = (action_t*)command->action;

  while(1) {
    if (a->defact == 1) {
      return a;
    }

    a = (action_t*)a->next;

    if(a == 0x0) {
      return 0x0;
    }
  }

  // should never get here
  return 0x0;

}



/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void command_print_command_tree(void) {
  command_t *c = root_command;

  while(c != 0x0) {
    printf("%s:\n", c->command);
    action_t *a = (action_t*)c->action;
    while(a != 0x0) {
      if(a->action == 0x0) {
        printf("   [default] - %s\n", a->help);
      } else {
        printf("   %s - %s\n", a->action, a->help);
      }
      a = (action_t*)a->next;
    }
    //printf("\n");
    c = (command_t*)c->next;
  }
}


/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static command_t *get_last_command(void) {
  if(root_command == 0x0) {
    return root_command;
  }

  command_t *prev = root_command;

  while(1) {
    if(prev->next == 0x0) {
      return prev;
    }
    prev = (command_t*)prev->next;
  }

  // should never get here
  return 0x0;

}


/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static action_t *get_last_action(command_t *c) {

  if(c->action == 0x0) {
    return 0x0;
  }

  action_t *prev = (action_t*)c->action;

  while(1) {
    if(prev->next == 0x0) {
      return prev;
    }
    prev = (action_t*)prev->next;
  }

  // should never get here
  return 0x0;
}


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int command_help_handler(int argc, char **argv) {
  command_print_command_tree();
  return 0;
}
