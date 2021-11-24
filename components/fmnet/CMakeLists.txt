

idf_component_register(SRCS
  "fmwifi.c"
  "fmmesh.c"
  "fmqueue.c"
  "fmnet.c"
  INCLUDE_DIRS .
  REQUIRES fmconsole fmcommand fmstorage mcommon mconfig mwifi mlink
  )


#   
