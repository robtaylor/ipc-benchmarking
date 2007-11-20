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

static const char* introspection_string = 
"<node name=\"/test/dbus/atspi/accessible\">"
"	<interface name=\"test.dbus.atspi.Accessible\">"
"		<method name=\"getRole\">"
"			<arg name=\"role\" type=\"i\" direction=\"out\"/>"
"		</method>"
"	</interface>"
"</node>";

static void
reply_to_Introspect(DBusConnection* conn,
		    DBusMessage* msg)
{
  DBusMessage* reply;
  DBusMessageIter args;

  reply = dbus_message_new_method_return(msg);
  dbus_message_iter_init_append(reply, &args);
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &introspection_string))
    { 
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
    }

  if (!dbus_connection_send(conn, reply, NULL))
    { 
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
    }
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
    { 
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
    }

  if (!dbus_connection_send(conn, reply, NULL))
    { 
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
    }
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
  else if (dbus_message_is_method_call(msg, "org.freedesktop.DBus.Introspectable", "Introspect")) 
    reply_to_Introspect(conn, msg);
}

int
main(int argc, char* argv[]) 
{
   DBusMessage* msg;
   DBusMessage* reply;
   DBusMessageIter args;
   DBusConnection* conn;
   DBusError err;
   int ret;
   char* param;
   const DBusObjectPathVTable object_vtable = 
     {
       unregister_function,
       method_handler_function,
       NULL,
       NULL,
       NULL,
       NULL
     };

   printf("Listening for method calls\n");

   dbus_error_init(&err);

   printf("\n%s", introspection_string);
   
   conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
   if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }
   if (NULL == conn) {
      fprintf(stderr, "Connection Null\n"); 
      exit(1); 
   }

   dbus_connection_register_object_path(conn, "/test/dbus/atspi/accessible", &object_vtable, conn);
   
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
