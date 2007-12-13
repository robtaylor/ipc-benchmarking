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

#define NUM_CALLS 18000

static DBusMessage* premade_getRole_msg;
static DBusMessage* premade_getRolePremade_msg;

static char* marshaled_getRole;
static char* marshaled_getRolePremade;

static void
abort_out_of_memory(void)
{
  fprintf(stderr, "Out Of Memory!\n");
  exit(1);
}

static void
make_void_call(DBusConnection* conn, const char *method)
{  
  DBusError err;
  DBusMessage* msg;
  DBusMessage* reply;
  int result;

  dbus_error_init(&err);
  msg = dbus_message_new_method_call("test.dbus.atspi",
		  "/test/dbus/atspi/accessible",
		  "test.dbus.atspi.Accessible",
		  method);

  reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
  if (dbus_error_is_set(&err)) { 
     fprintf(stderr, "Message sending error (%s)\n", err.message); 
     dbus_error_free(&err); 
  }

  dbus_message_get_args(reply, &err, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID);

  dbus_message_unref(msg);
  dbus_message_unref(reply);
}

static void
make_premade_void_call(DBusConnection* conn, DBusMessage* cmsg)
{  
  DBusError err;
  DBusMessage* reply;
  DBusMessage* msg;
  int result;

  msg = dbus_message_copy(cmsg);

  dbus_error_init(&err);

  reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
  if (dbus_error_is_set(&err)) { 
     fprintf(stderr, "Message sending error (%s)\n", err.message); 
     dbus_error_free(&err); 
  }

  dbus_message_get_args(reply, &err, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID);

  dbus_message_unref(reply);
  dbus_message_unref(msg);
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

   int i;

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

   premade_getRolePremade_msg = dbus_message_new_method_call("test.dbus.atspi",
		  			      "/test/dbus/atspi/accessible",
		  			      "test.dbus.atspi.Accessible",
		  			      "getRolePremade");

   premade_getRole_msg = dbus_message_new_method_call("test.dbus.atspi",
		  			      "/test/dbus/atspi/accessible",
		  			      "test.dbus.atspi.Accessible",
		  			      "getRole");

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

   gettimeofday(&start, NULL);
   for (i=0; i<NUM_CALLS; i++)
     {
       make_void_call(conn, "getRole");
     }
   gettimeofday(&end, NULL);
   d_secs = end.tv_sec - start.tv_sec;
   d_usec = end.tv_usec - start.tv_usec;
   printf("\nBuilt -> Built : %f", (float) d_secs + ((float) d_usec / 1000000.0));

   gettimeofday(&start, NULL);
   for (i=0; i<NUM_CALLS; i++)
     {
       make_void_call(conn, "getRolePremade");
     }
   gettimeofday(&end, NULL);
   d_secs = end.tv_sec - start.tv_sec;
   d_usec = end.tv_usec - start.tv_usec;
   printf("\nBuilt -> Premade : %f", (float) d_secs + ((float) d_usec / 1000000.0));

   gettimeofday(&start, NULL);
   for (i=0; i<NUM_CALLS; i++)
     {
       make_premade_void_call(conn, premade_getRole_msg);
     }
   gettimeofday(&end, NULL);
   d_secs = end.tv_sec - start.tv_sec;
   d_usec = end.tv_usec - start.tv_usec;
   printf("\nPremade -> Built : %f", (float) d_secs + ((float) d_usec / 1000000.0));

   gettimeofday(&start, NULL);
   for (i=0; i<NUM_CALLS; i++)
     {
       make_premade_void_call(conn, premade_getRolePremade_msg);
     }
   gettimeofday(&end, NULL);
   d_secs = end.tv_sec - start.tv_sec;
   d_usec = end.tv_usec - start.tv_usec;
   printf("\nPremade -> Premade : %f\n\n", (float) d_secs + ((float) d_usec / 1000000.0));
   
   dbus_message_unref(premade_getRole_msg);
   dbus_message_unref(premade_getRolePremade_msg);
}
