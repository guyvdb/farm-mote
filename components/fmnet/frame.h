#ifndef FRAME_H
#define FRAME_H

#include <stddef.h>
#include <stdint.h>
/*
  A frame is set for communication.



  orgn:       MAC or "SERVER" - the mac address of the originating node or the word "SERVER"
  dest:       MAC or "SERVER" - the destination mac address or the word "SERVER"
  route:      frame route - an array of MAC addresses to final destination 
  cnt:        counter - a monotonic incrementing number of the origin node - can be reset at any time
  ts:         unix time stamp - a time stamp generated on the origin node
  ref:        ts:cnt - a time stamp and counter value of a reference frame 
  cmd:        command - the name of a command to execute 
  act:        action - the name of an action to execute
  params:     parameters - an array of parameters

*/


typedef struct {
  char *origin;
  char *destination;
  uint16_t counter;
  uint64_t timestamp;
  char *ref;
  char *command;
  char *action;
  size_t paramcount;
  char **params;
} frame_t;




frame_t *decode_frame(char *data, size_t len);  // caller must free result


frame_t *create_frame(const char *origin,   // caller must free result
                      const char *dest,
                      uint16_t counter,
                      uint64_t timestamp,
                      const char *ref,
                      const char *command,
                      const char *action,
                      const char **params,
                      size_t paramcount);


char *encode_frame(frame_t *frame) ; // caller must free result


void free_frame(frame_t *f);






/*

// Structure of the frame bytes 
//
// |--------------|---------------|------------|---------------|-------------------|---------------|------------|-----------|--------------------|--------------|
// | SFLAG 1 byte | version 1byte | id 2 bytes | tcount 1 byte | timestamp 4 bytes | refid 2 bytes | cmd 1 byte | len 1byte | payload <varbytes> | EFLAG 1 byte |
// |--------------|---------------|------------|---------------|-------------------|---------------|------------|-----------|--------------------|--------------|
//


 */

#endif 
