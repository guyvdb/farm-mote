#include <fmstorage.h>


#include <nvs.h>
#include <nvs_flash.h>
#include <esp_vfs_dev.h>
#include <esp_vfs_fat.h>
#include <fmcommand.h>


/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static void flash_initialize(void);
static void filesystem_initialize(void);
static void list_data_partitions(void);

static int storage_command_erase_handler(int argc, char **argv);


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
void storage_initialize(void) {
  flash_initialize();
  filesystem_initialize();
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
void storage_register_commands(void) {
  static command_t cmd_flash = {.next = 0x0, .action = 0x0, .command = "flash", .help = "Access flash memory."};
  static action_t act_flash_erase = {.defact = 0, .next = 0x0, .action = "erase", .help = "Erase the flash chip.", .func = storage_command_erase_handler };


  command_add(&cmd_flash);
  command_add_action(&cmd_flash, &act_flash_erase);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_get_wifi_ssid(char *buf, size_t len) {
  return  storage_read_string("wifi.ssid", buf, len);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_set_wifi_ssid(const char *value) {
  return storage_write_string("wifi.ssid", value);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_get_wifi_password(char *buf, size_t len) {
  return  storage_read_string("wifi.pwd", buf, len);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_set_wifi_password(const char *value) {
  return storage_write_string("wifi.pwd", value);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_get_mesh_id(char *buf, size_t len) {
  return storage_read_string("mesh.id", buf, len);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_set_mesh_id(const char *value) {
  return storage_write_string("mesh.id", value);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_get_mesh_password(char *buf, size_t len) {
  return storage_read_string("mesh.pwd", buf, len);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_set_mesh_password(const char *value) {
  return storage_write_string("mesh.pwd", value);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_get_queue_address(char *buf, size_t len) {
  return storage_read_string("queue.address", buf, len);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_set_queue_address(const char *value) {
  return storage_write_string("queue.address", value);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_get_queue_port(uint16_t *value) {
  return storage_read_uint16("queue.port", value);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_set_queue_port(uint16_t value) {
  return storage_write_uint16("queue.port", value);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_get_node_name(char *buf, size_t len) {
  return storage_read_string("node.name", buf, len);
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */ 
esp_err_t storage_set_node_name(const char *value) {
  return storage_write_string("node.name", value);
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
esp_err_t storage_read_string(const char *key, char *value, size_t len) {
  size_t stored_len;
  esp_err_t err;
  nvs_handle_t handle;

  err = nvs_open("storage",NVS_READONLY, &handle);
  if (err != ESP_OK) {
    return err;
  }

  if( (err = nvs_get_str(handle, key, NULL, &stored_len)) == ESP_OK) {
    if(stored_len <= len) {
      if ((err = nvs_get_str(handle,key, value,&len)) == ESP_OK) {
        nvs_close(handle);
        return ESP_OK;
      } else {
        nvs_close(handle);
        return err;
      }
    } else {
      nvs_close(handle);
      return ESP_ERR_INVALID_SIZE;
    }
  } else {
    nvs_close(handle);
    return err;
  }
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
esp_err_t storage_write_string(const char *key, const char *value) {
  esp_err_t err;
  nvs_handle_t handle;

  err = nvs_open("storage",NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    return err;
  }

  if( (err = nvs_set_str(handle, key, value)) == ESP_OK) {
    nvs_close(handle);
    return ESP_OK;
  } else {
    nvs_close(handle);
    return err;
  }
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
esp_err_t storage_read_uint16(const char *key, uint16_t *value) {
  esp_err_t err;
  nvs_handle_t handle;

  err = nvs_open("storage",NVS_READONLY, &handle);
  if (err != ESP_OK) {
    return err;
  }

  err = nvs_get_u16(handle, key, value);
  nvs_close(handle);
  return err;
  
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
esp_err_t storage_write_uint16(const char *key, uint16_t value) {
  esp_err_t err;
  nvs_handle_t handle;

  err = nvs_open("storage",NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    return err;
  }

  err = nvs_set_u16(handle, key, value);
  nvs_close(handle);
  return err;
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
esp_err_t storage_read_uint32(const char *key, uint32_t *value) {
  esp_err_t err;
  nvs_handle_t handle;

  err = nvs_open("storage",NVS_READONLY, &handle);
  if (err != ESP_OK) {
    return err;
  }

  err = nvs_get_u32(handle, key, value);
  nvs_close(handle);
  return err;
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
esp_err_t storage_write_uint32(const char *key, uint32_t value){
  esp_err_t err;
  nvs_handle_t handle;

  err = nvs_open("storage",NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    return err;
  }

  err = nvs_set_u32(handle, key, value);
  nvs_close(handle);
  return err;
}


/* ======================================================================= */



/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static void flash_initialize(void){  
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK( nvs_flash_erase() );
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
}


/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static void filesystem_initialize(void){
  list_data_partitions();
  static wl_handle_t wl_handle;
  const esp_vfs_fat_mount_config_t mount_config = {.max_files = 4, .format_if_mount_failed = true };
  esp_err_t err = esp_vfs_fat_spiflash_mount(MOUNT_PATH, "storage", &mount_config, &wl_handle);
  if (err != ESP_OK) {
    ESP_LOGE("FILESYSTEM", "Failed to mount FATFS (%s)", esp_err_to_name(err));
    return;
  }  
}


/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static void list_data_partitions(void) {
  ESP_LOGI("", "Partitions");
  esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

  for (; it != NULL; it = esp_partition_next(it)) {
    const esp_partition_t *part = esp_partition_get(it);
    ESP_LOGI("", "  label='%s', subtype=%d, offset=0x%x, size=%d kB",
             part->label, part->subtype, part->address, part->size / 1024);
  }

  esp_partition_iterator_release(it);
}


/* ------------------------------------------------------------------------
 * COMMAND
 * --------------------------------------------------------------------- */
static int storage_command_erase_handler(int argc, char **argv) {

  // de initialize flash
  // erase nv store
  // re initialize flash


  if(nvs_flash_deinit() != ESP_OK) {
    printf("Error - could not deinitialize flash.\n");
    return 1;
  }

  ESP_ERROR_CHECK(nvs_flash_erase());

  ESP_ERROR_CHECK(nvs_flash_init());

  printf("NVS Flash has be erased.\n");

  return 0;




}



