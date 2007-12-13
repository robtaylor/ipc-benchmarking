#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "Accessibility.h"

#define NUM_CALLS 18000

static CORBA_ORB  global_orb = CORBA_OBJECT_NIL;
 
static
void
client_shutdown (int sig)
{
        CORBA_Environment  local_ev[1];
        CORBA_exception_init(local_ev);
 
        if (global_orb != CORBA_OBJECT_NIL)
        {
                CORBA_ORB_shutdown (global_orb, FALSE, local_ev);
                etk_abort_if_exception (local_ev, "caught exception");
        }
}
        
static
void
client_init (int               *argc_ptr,
	     char              *argv[],
             CORBA_ORB         *orb,
             CORBA_Environment *ev)
{
        /* init signal handling */
 
        signal(SIGINT,  client_shutdown);
        signal(SIGTERM, client_shutdown);
         
        /* create Object Request Broker (ORB) */
         
        (*orb) = CORBA_ORB_init(argc_ptr, argv, "orbit-local-orb", ev);
        if (etk_raised_exception(ev)) return;
}

static
void
client_cleanup (CORBA_ORB                 orb,
                CORBA_Object              service,
                CORBA_Environment        *ev)
{
        CORBA_Object_release(service, ev);
        if (etk_raised_exception(ev)) return;
 
        if (orb != CORBA_OBJECT_NIL)
        {
                CORBA_ORB_destroy(orb, ev);
                if (etk_raised_exception(ev)) return;
        }
}

int
main(int argc, char* argv[])
{
	CORBA_char filename[] = "/tmp/test-atspi.ior";
        Accessibility_Role data;
	int i;
        struct timeval start, end;
        long d_secs;
        long d_usec;
        
	Accessibility_Accessible service = CORBA_OBJECT_NIL;

        CORBA_Environment ev[1];
        CORBA_exception_init(ev);

	client_init (&argc, argv, &global_orb, ev);
	etk_abort_if_exception(ev, "init failed");

	g_print ("Reading service reference from file \"%s\"\n", filename);

	service = (Accessibility_Accessible) etk_import_object_from_file (global_orb,
							   filename,
							   ev);
        etk_abort_if_exception(ev, "import service failed");


        gettimeofday(&start, NULL);
	for (i=0; i<NUM_CALLS; i++)
	  {
            data = Accessibility_Accessible_getRole(service, ev);
	  }
        gettimeofday(&end, NULL);
        d_secs = end.tv_sec - start.tv_sec;
        d_usec = end.tv_usec - start.tv_usec;
        printf("\nGetRole : %f\n", (float) d_secs + ((float) d_usec / 1000000.0));
 
	client_cleanup (global_orb, service, ev);
        etk_abort_if_exception(ev, "cleanup failed");
 
        exit (0);
}
