#ifndef FM_SYSTEM_H
#define FM_SYSTEM_H


#include <stdint.h>
#include <stddef.h>

typedef uint8_t macaddress_t[6];
typedef 

//uint8_t base_mac_addr[6] = {0};


void system_initialize(void);
void system_register_commands(void);
void system_run(void);
void system_print_heap_free(void);



void system_get_mac_address(macaddress_t result);
void system_get_mac_address_str(char *result, size_t len);
uint64_t system_get_unix_time();
void system_get_date_time(char *result, size_t len);


#endif 
 
