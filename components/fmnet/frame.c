
#include <frame.h>
#include <stdio.h>
#include <string.h>




/*
struct {
  char *origin
  char *destination
  uint16_t counter;
  uint64_t timestamp;
  char *ref;
  char *command;
  char *action;
  char *params[];
  size_t paramcount;
} frame_t;

*/


/* ------------------------------------------------------------------------
 * PRIVATE DECLARATION
 * --------------------------------------------------------------------- */
static char * malloc_copy(const char *data);
static char ** malloc_copy_array(const char **data, size_t len);


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
frame_t *decode_frame(char *data, size_t len) {
  return 0x0;
}

/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
char *encode_frame(frame_t *f) {

  //  int asprintf(char **restrict strp, const char *restrict fmt, ...);

  char *data;
  char *array;
  size_t len;
  int idx;




  // calculate the array string
  //---------------------------
  // array is going to be:
  // ["token","token","token"]
  // len = 2(brace) + (len * 2 (quote)) + (len-1 * 1 (comma)) + string lens

  len = 2 + (f->paramcount *2) + (f->paramcount -1 );
  for(size_t i=0;i<f->paramcount;i++) {
    len += strlen(f->params[i]);
  }
  len += 1; // for null terminator 


  // populate array string
  array = malloc(len);
  array[0] = '[';
  idx = 1;
  for(size_t i=0;i<f->paramcount;i++) {
    array[idx] = '"';
    idx++;
    for(int j = 0; j < strlen(f->params[i]); j++) {
      array[idx] = f->params[i][j];
      idx++;
    }
    array[idx] = '"';
    idx++;
    if(i < f->paramcount - 1) {
      array[idx] = ',';
      idx++;
    }
  }
  array[idx] = ']';
  idx++;
  array[idx] = 0x0;


  // create the json string
  asprintf(&data, "{"
           "\"orgn\":\"%s\","
           "\"dest\":\"%s\","
           "\"route\":[],"
           "\"cnt\":%d,"
           "\"ts\":%lld,"
           "\"ref\":\"%s\","
           "\"cmd\":\"%s\","
           "\"act\":\"%s\","
           "\"params\":%s"
           "}\n",
           f->origin,
           f->destination,
           f->counter,
           f->timestamp,
           f->ref,
           f->command,
           f->action,
           array
           );

  // free the array
  free(array);


  return data;
}


/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
frame_t *create_frame(const char *origin, const char *destination, uint16_t counter, uint64_t timestamp, const char *ref, const char *command, const char *action, const char **params, size_t paramcount) {
  frame_t *f = malloc(sizeof(frame_t));

  f->origin = malloc_copy(origin);
  f->destination = malloc_copy(destination);
  f->counter = counter;
  f->timestamp = timestamp;
  f->ref = malloc_copy(ref);
  f->command = malloc_copy(command);
  f->action = malloc_copy(action);
  f->params = malloc_copy_array(params, paramcount);
  f->paramcount = paramcount;
  return f;
}



/* ------------------------------------------------------------------------
 * PUBLIC
 * --------------------------------------------------------------------- */
void free_frame(frame_t *f) {
  free(f->origin);
  free(f->destination);
  free(f->ref);
  free(f->command);
  free(f->action);

  if(f->params != 0x0) {
    for(int i=0;i<f->paramcount;i++) {
      free(f->params[i]);
    }
    free(f->params);
  }
  
  free(f);
}



/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static char *malloc_copy(const char *data) {

  if(data == 0x0) {
    return  0x0;
  }

  char *v = malloc(strlen(data) + 1);
  strcpy(v, data);
  return v;
}

/* ------------------------------------------------------------------------
 * PRIVATE
 * --------------------------------------------------------------------- */
static char ** malloc_copy_array(const char **data, size_t len) {

  if(len == 0) {
    return 0x0;
  }

  char **l = malloc(sizeof(char*) * len);
  for(int i=0;i<len;i++) {
    const char *v = data[i];
    l[i] = malloc(strlen(v) + 1);
    strcpy(l[i], v);
  }
  return l;
}




/* 
char** list; 

list = malloc(sizeof(char*)*number_of_row);
for(i=0;i<number_of_row; i++) 
  list[i] = malloc(sizeof(char)*number_of_col);  

Additionally, if you are allocating memory dynamically. you are to free it as work done:

  for(i=0;i<number_of_row; i++) 
    free(list[i] );
free(list);  


*/
