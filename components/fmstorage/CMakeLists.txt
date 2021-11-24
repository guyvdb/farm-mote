

idf_component_register(SRCS
  "fmstorage.c"
  INCLUDE_DIRS .
  REQUIRES nvs_flash fatfs fmcommand
  )

