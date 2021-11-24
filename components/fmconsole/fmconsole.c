#include <fmconsole.h>
#include <fmcommand.h>

#include <string.h>
#include <stdio.h>
#include <esp_log.h>
#include <esp_console.h>
#include <linenoise/linenoise.h>
#include <driver/uart.h>
#include <esp_vfs_dev.h>




//#define MAX_CMDLINE_LEN 256
#define MAX_CMDLINE_ARGS 10
static char *argv[MAX_CMDLINE_ARGS]; // the command line arguments


/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static int parse_cmdline(char *cmdline);
void console_task_handler(void *params);


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void console_initialize(void) {
  fflush(stdout);
  fsync(fileno(stdout));

  // Disable buffering on stdin
  setvbuf(stdin, NULL, _IONBF, 0);

  // set CR as line ending
  //esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
  // end CRLF as tx ending
  //esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

  const uart_config_t uart_config = {
                                     .baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE,
                                     .data_bits = UART_DATA_8_BITS,
                                     .parity = UART_PARITY_DISABLE,
                                     .stop_bits = UART_STOP_BITS_1,
                                     .source_clk = UART_SCLK_REF_TICK,
  };

  // Install UART driver for interrupt-driven reads and writes
  ESP_ERROR_CHECK( uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0) );
  ESP_ERROR_CHECK( uart_param_config(CONFIG_ESP_CONSOLE_UART_NUM, &uart_config) );

  // Tell VFS to use UART driver
  esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);

  // Initialize the console
  esp_console_config_t console_config = {
                                         .max_cmdline_args = 8,
                                         .max_cmdline_length = 256,
  };
  ESP_ERROR_CHECK( esp_console_init(&console_config) );


  // set up line noise
  linenoiseSetMultiLine(0);
  linenoiseSetCompletionCallback(&esp_console_get_completion);
  linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);
  linenoiseHistorySetMaxLen(100);
  
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void console_register_commands(void) {
  
}



/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void console_task_start(void) {
  //  xTaskCreate(console_task_handler, "console", 4 * 1024, 0x0, tskID)
  
}

/*


// Function that creates a task.
void vOtherFunction( void )
{
static uint8_t ucParameterToPass;
TaskHandle_t xHandle = NULL;

// Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
// must exist for the lifetime of the task, so in this case is declared static.  If it was just an
// an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
// the new task attempts to access it.
xTaskCreate( vTaskCode, "NAME", STACK_SIZE, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle );
configASSERT( xHandle );

// Use the handle to delete the task.
if( xHandle != NULL )
{
vTaskDelete( xHandle );
}
}




*/


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void console_event_loop(void) {
  const char* prompt = "farm-mote> ";
  linenoiseSetDumbMode(1);
  while(true) {
    char* line = linenoise(prompt);

    if (line == NULL || strcmp(line,"") == 0 ) {
      continue;
    }


    int argc = parse_cmdline(line);
    if(argc>0) {
      command_t *c = command_find(argv[0]);
      if(c == 0x0) {
        // invalid command
        printf("Invalid command. Try 'help'.\n");
      } else {

        action_t *a = 0x0;
        // there might be an action specified
        if(argc > 1) {
          a = command_find_action(c, argv[1]);
        }
        // if the was no action look for a default
        if(a == 0x0) {
          a = command_find_default_action(c);
        }
        // did we find an action?
        if(a != 0x0) {
          char **args = (char**)&argv;
          int result = -1;
          if(a->defact == 1) {
            // there should be no args
            result = a->func(0, args);
          } else {
            if(argc - 2 > 0) {
              args+= 2;
              result = a->func(argc-2,args);
            } else {
              // no args
              result = a->func(0, args);
            }
          }
        } else {
          printf("Invalid command. Try 'help'.\n");
        }
      }
    } else {
      // argc <= 0
      printf("Invalid command. Try 'help'.\n");
    }
    linenoiseFree(line);
  }
}


/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
void console_task_handler(void *params) {
  /*


  const char* prompt = "farm-mote> ";
  linenoiseSetDumbMode(1);
  while(true) {
    char* line = linenoise(prompt);

    if (line == NULL || strcmp(line,"") == 0 ) {
      continue;
    }


    int argc = parse_cmdline(line);
    if(argc>0) {
      command_t *c = command_find(argv[0]);
      if(c == 0x0) {
        // invalid command
        printf("Invalid command. Try 'help'.\n");
      } else {

        action_t *a = 0x0;
        // there might be an action specified
        if(argc > 1) {
          a = command_find_action(c, argv[1]);
        }
        // if the was no action look for a default
        if(a == 0x0) {
          a = command_find_default_action(c);
        }
        // did we find an action?
        if(a != 0x0) {
          char **args = (char**)&argv;
          int result = -1;
          if(a->defact == 1) {
            // there should be no args
            result = a->func(0, args);
          } else {
            if(argc - 2 > 0) {
              args+= 2;
              result = a->func(argc-2,args);
            } else {
              // no args
              result = a->func(0, args);
            }
          }
        } else {
          printf("Invalid command. Try 'help'.\n");
        }
      }
    } else {
      // argc <= 0
      printf("Invalid command. Try 'help'.\n");
    }
    linenoiseFree(line);
  }

  */
}




/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static int parse_cmdline(char *cmdline) {
	char *delim = " ";
  int argc = 0;
	char *ptr = strtok(cmdline, delim);

	while(ptr != NULL) {
    argv[argc]=ptr;
		ptr = strtok(NULL, delim);
    argc++;
    if (argc >= MAX_CMDLINE_ARGS) {
      return argc-1;
    }
	}
  return argc;
}
