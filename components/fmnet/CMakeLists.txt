

idf_component_register(SRCS
  "fmwifi.c"
  "fmmesh.c"
  "fmqueue.c"
  "fmnet.c"
  "frame.c"
  INCLUDE_DIRS .
  REQUIRES fmconsole fmcommand fmstorage fmsystem mcommon mconfig mwifi mlink 
  )


#   
