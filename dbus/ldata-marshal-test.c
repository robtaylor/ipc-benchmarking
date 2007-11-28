/*
 * D-Bus server for IPC benchmarking.
 * Mark Doffman <mark.doffman (at) codethink.co.uk>
 * Based on example written by:
 * Written by Matthew Johnson <dbus@matthew.ath.cx>
 */
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>

#define NUM_ITER 16
#define NUM_CALLS 5000

#define NUM_LINEAR 64

static void
abort_out_of_memory(void)
{
  fprintf(stderr, "Out Of Memory!\n");
  exit(1);
}

static void
make_getIntArray_call(long size)
{  
  DBusMessage* reply;
  DBusMessageIter args;
  DBusMessageIter subarr;
  DBusError err;

  dbus_int32_t val = 0xAAAAAAAA;
  dbus_int32_t* int_array;
  long i;

  dbus_error_init(&err);

  reply = dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL);

  int_array =  malloc(sizeof(dbus_int32_t) * size);

  /*for (i=0; i<size; i++)
    int_array[i] = val;*/

  dbus_message_iter_init_append(reply, &args);

  if (!dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "i", &subarr))
    abort_out_of_memory();
  if (!dbus_message_iter_append_fixed_array(&subarr, DBUS_TYPE_INT32, &int_array, size))
    abort_out_of_memory();
  if (!dbus_message_iter_close_container(&args, &subarr))
    abort_out_of_memory();

  free(int_array);

  dbus_message_unref(reply);
}

int
main(int argc, char* argv[]) 
{
   struct timeval start, end;
   long d_secs;
   long d_usec;

   int i, j;
   long arsz = 512;

   for (i=0; i<NUM_LINEAR; i++)
   {
       gettimeofday(&start, NULL);
       for (j=0; j<NUM_CALLS; j++)
         {
           make_getIntArray_call(arsz);
         }
       gettimeofday(&end, NULL);
       d_secs = end.tv_sec - start.tv_sec;
       d_usec = end.tv_usec - start.tv_usec;
       printf("\n %d,%f", arsz, (float) d_secs + ((float) d_usec / 1000000.0));
       arsz += 512;
   }
   printf("\n");
}
