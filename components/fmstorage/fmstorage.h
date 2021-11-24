#ifndef FM_STOREAGE_H
#define FM_STORAGE_H

#include <esp_err.h>

#define MOUNT_PATH "/data"

void storage_initialize(void);
void storage_register_commands(void);

esp_err_t storage_get_wifi_ssid(char *buf, size_t len);
esp_err_t storage_set_wifi_ssid(const char *value);
esp_err_t storage_get_wifi_password(char *buf, size_t len);
esp_err_t storage_set_wifi_password(const char *value);
esp_err_t storage_get_mesh_id(char *buf, size_t len);
esp_err_t storage_set_mesh_id(const char *value);
esp_err_t storage_get_mesh_password(char *buf, size_t len);
esp_err_t storage_set_mesh_password(const char *value);
esp_err_t storage_get_queue_address(char *buf, size_t len);
esp_err_t storage_set_queue_address(const char *value);
esp_err_t storage_get_queue_port(uint16_t *value);
esp_err_t storage_set_queue_port(uint16_t value);
esp_err_t storage_get_node_name(char *buf, size_t len);
esp_err_t storage_set_node_name(const char *value);



esp_err_t storage_read_string(const char *key, char *value, size_t len);
esp_err_t storage_write_string(const char *key, const char *value);
esp_err_t storage_read_uint16(const char *key, uint16_t *value);
esp_err_t storage_write_uint16(const char *key, uint16_t value);
esp_err_t storage_read_uint32(const char *key, uint32_t *value);
esp_err_t storage_write_uint32(const char *key, uint32_t value);

#endif 
