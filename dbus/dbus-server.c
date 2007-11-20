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
#include <glib.h>
#include <dbus/dbus-glib.h>

static const char* introspection_string = 
"<node name=\"/test/dbus/atspi/accessible\">"
"	<interface name=\"test.dbus.atspi.Accessible\">"
"		<method name=\"getRole\">"
"			<arg name=\"role\" type=\"i\" direction=\"out\"/>"
"		</method>"
"		<method name=\"get_parent\">"
"			<arg name=\"parent\" type=\"(so)\" direction=\"out\"/>"
"		</method>"
"		<method name=\"getRelationSet\">"
"			<arg name=\"relations\" type=\"a(so)\" direction=\"out\"/>"
"		</method>"
"	</interface>"
"</node>";

#define NUM_RELATIONS 10

static const char* connection_string;
static const char* bus_name = ":AF1435D";
static const char* object_path = "/test/dbus/atspi/accessible";

static void
abort_out_of_memory(void)
{
  fprintf(stderr, "Out Of Memory!\n");
  exit(1);
}

static void
reply_to_Introspect(DBusConnection* conn,
		    DBusMessage* msg)
{
  DBusMessage* reply;
  DBusMessageIter args;

  reply = dbus_message_new_method_return(msg);
  dbus_message_iter_init_append(reply, &args);
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &introspection_string))
    abort_out_of_memory();

  if (!dbus_connection_send(conn, reply, NULL))
    abort_out_of_memory();

  dbus_connection_flush(conn);
  dbus_message_unref(reply);
}

static void
reply_to_getRole(DBusConnection* conn,
		 DBusMessage* msg)
{
  DBusMessage* reply;
  DBusMessageIter args;
  int role = 6;

  reply = dbus_message_new_method_return(msg);
  dbus_message_iter_init_append(reply, &args);
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT16, &role))
    abort_out_of_memory();

  if (!dbus_connection_send(conn, reply, NULL))
    abort_out_of_memory();

  dbus_connection_flush(conn);
  dbus_message_unref(reply);
}

static void
reply_to_get_parent(DBusConnection* conn,
		    DBusMessage* msg)
{
  DBusMessage* reply;
  DBusMessageIter args;
  DBusMessageIter sub;

  reply = dbus_message_new_method_return(msg);
  dbus_message_iter_init_append(reply, &args);
  if (!dbus_message_iter_open_container(&args, DBUS_TYPE_STRUCT, NULL, &sub))
    abort_out_of_memory();
    
  if (!dbus_message_iter_append_basic(&sub, DBUS_TYPE_STRING, &connection_string))
    abort_out_of_memory();

  if (!dbus_message_iter_append_basic(&sub, DBUS_TYPE_OBJECT_PATH, &object_path))
    abort_out_of_memory();

  if (!dbus_message_iter_close_container(&args, &sub))
    abort_out_of_memory();

  if (!dbus_connection_send(conn, reply, NULL))
    abort_out_of_memory();

  dbus_connection_flush(conn);
  dbus_message_unref(reply);
}

static void
reply_to_getRelationSet(DBusConnection* conn,
		       	DBusMessage* msg)
{
  DBusMessage* reply;
  DBusMessageIter args;
  DBusMessageIter subarr;
  DBusMessageIter substr;
  int i;

  reply = dbus_message_new_method_return(msg);
  dbus_message_iter_init_append(reply, &args);

  if (!dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "(so)", &subarr))
    abort_out_of_memory();
  for (i=0; i<NUM_RELATIONS; i++)
    {
      if (!dbus_message_iter_open_container(&subarr, DBUS_TYPE_STRUCT, NULL, &substr))
        abort_out_of_memory();
      
      if (!dbus_message_iter_append_basic(&substr, DBUS_TYPE_STRING, &connection_string))
        abort_out_of_memory();

      if (!dbus_message_iter_append_basic(&substr, DBUS_TYPE_OBJECT_PATH, &object_path))
        abort_out_of_memory();

      if (!dbus_message_iter_close_container(&subarr, &substr))
        abort_out_of_memory();
    }
  if (!dbus_message_iter_close_container(&args, &subarr))
    abort_out_of_memory();

  if (!dbus_connection_send(conn, reply, NULL))
    abort_out_of_memory();

  dbus_connection_flush(conn);
  dbus_message_unref(reply);
}

static void
unregister_function(DBusConnection* conn,
		    void* user_data)
{
  /*Nothing to do*/
}

DBusHandlerResult 
method_handler_function(DBusConnection* conn,
			 DBusMessage* msg,
			 void* usr_data)
{
  if (dbus_message_is_method_call(msg, "test.dbus.atspi.Accessible", "getRole")) 
    reply_to_getRole(conn, msg);
  else if (dbus_message_is_method_call(msg, "test.dbus.atspi.Accessible", "get_parent")) 
    reply_to_get_parent(conn, msg);
  else if (dbus_message_is_method_call(msg, "test.dbus.atspi.Accessible", "getRelationSet")) 
    reply_to_getRelationSet(conn, msg);
  else if (dbus_message_is_method_call(msg, "org.freedesktop.DBus.Introspectable", "Introspect")) 
    reply_to_Introspect(conn, msg);
}

const DBusObjectPathVTable object_vtable = 
{
  unregister_function,
  method_handler_function,
  NULL,
  NULL,
  NULL,
  NULL
};

static void
handle_connection(DBusServer* server,
		  DBusConnection* conn,
		  void* data)
{
  dbus_connection_register_object_path(conn, "/test/dbus/atspi/accessible", &object_vtable, NULL);
  connection_string = object_path;
  while (dbus_connection_read_write_dispatch(conn, -1))
    ;
}

int
main(int argc, char* argv[]) 
{
   DBusError err;
   int ret;
   char* param;

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
       DBusServer* server;
       GMainLoop* main_loop;

       main_loop = g_main_loop_new(NULL, FALSE);

       server = dbus_server_listen("unix:path=/tmp/testatspi", &err);
       dbus_server_setup_with_g_main(server, NULL);
       dbus_server_set_new_connection_function(server, handle_connection, NULL, NULL);

       g_main_run(main_loop);
     }
   else
     {
       DBusConnection* conn;
   
       conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
       if (dbus_error_is_set(&err)) { 
          fprintf(stderr, "Connection Error (%s)\n", err.message); 
          dbus_error_free(&err); 
       }
       if (NULL == conn) {
          fprintf(stderr, "Connection Null\n"); 
          exit(1); 
       }
       connection_string = bus_name;

       dbus_connection_register_object_path(conn, "/test/dbus/atspi/accessible", &object_vtable, NULL);
   
       ret = dbus_bus_request_name(conn, "test.dbus.atspi", DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
       if (dbus_error_is_set(&err)) { 
          fprintf(stderr, "Name Error (%s)\n", err.message); 
          dbus_error_free(&err);
       }
       if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
          fprintf(stderr, "Not Primary Owner (%d)\n", ret);
          exit(1); 
       }

       while (dbus_connection_read_write_dispatch(conn, -1))
         ;
     }
}
