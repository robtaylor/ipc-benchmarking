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

#define NUM_EXP 16
#define NUM_LINEAR 64

#define STEP_EXP 2
#define STEP_LINEAR 512

#define NUM_CALLS 2000

static void
abort_out_of_memory(void)
{
  fprintf(stderr, "Out Of Memory!\n");
  exit(1);
}

static void
make_getIntArray_call(DBusConnection* conn, long size)
{  
  DBusError err;
  DBusMessage* msg;
  DBusMessage* reply;
  DBusMessageIter args;
  DBusMessageIter var;
  DBusMessageIter subvar;

  dbus_int32_t* int_array;
  int array_size;

  dbus_error_init(&err);
  msg = dbus_message_new_method_call("test.dbus.atspi",
		  "/test/dbus/atspi/accessible",
		  "test.dbus.atspi.Accessible",
		  "getIntArray");

  dbus_message_iter_init_append(msg, &args);
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &size))
    abort_out_of_memory();

  reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
  if (dbus_error_is_set(&err)) { 
     fprintf(stderr, "Message sending error (%s)\n", err.message); 
     dbus_error_free(&err); 
  }

  if (!dbus_message_iter_init(reply, &var))
     fprintf(stderr, "Message has no arguments!\n");

  dbus_message_iter_recurse(&var, &subvar);
  dbus_message_iter_get_fixed_array(&subvar, &int_array, &array_size);

  dbus_message_unref(msg);
  dbus_message_unref(reply);
}

int
main(int argc, char* argv[]) 
{
   DBusError err;
   int ret;
   char* param;
   DBusConnection* conn;
   
   struct timeval start, end;
   long d_secs;
   long d_usec;

   int i, j;
   long arsz = STEP_LINEAR;

   int c;
   static int direct_flag = 0;
   int option_index;
   static struct option long_options[] =
     {
	 {"direct", no_argument, &direct_flag, 1},
	 {0,0,0,0}
     };

   while((c=getopt_long(argc, argv, "", long_options, &option_index)) != -1)
     ;

   printf("Listening for method calls - %s\n", direct_flag ? "direct" : "bus");
   
   dbus_error_init(&err);

   if (direct_flag)
     {
       conn = dbus_connection_open("unix:path=/tmp/testatspi", &err);
     }
   else
     {
       conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
     }
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }
   if (NULL == conn) {
      fprintf(stderr, "Connection Null\n"); 
      exit(1); 
   }

   for (i=0; i<NUM_LINEAR; i++)
   {
       gettimeofday(&start, NULL);
       for (j=0; j<NUM_CALLS; j++)
         {
           make_getIntArray_call(conn, arsz);
         }
       gettimeofday(&end, NULL);
       d_secs = end.tv_sec - start.tv_sec;
       d_usec = end.tv_usec - start.tv_usec;
       printf("\n %d,%f", arsz, (float) d_secs + ((float) d_usec / 1000000.0));
       arsz += STEP_LINEAR;
   }
}
