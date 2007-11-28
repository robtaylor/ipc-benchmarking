/*
 * This header file was generated from the idl
 */

#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "ldata.h"

#define NUM_EXP 16
#define NUM_LINEAR 64

#define STEP_EXP 2
#define STEP_LINEAR 512

#define NUM_CALLS 2000

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

static
void
client_run (Test_LargeSeq  ref,
	    CORBA_Environment *ev)
{
   Test_LargeSeq_LargeData* data;
   struct timeval start, end;
   long d_secs;
   long d_usec;

   int i, j;
   long arsz = STEP_LINEAR;

   for (i=0; i<NUM_LINEAR; i++)
   {
       gettimeofday(&start, NULL);
       for (j=0; j<NUM_CALLS; j++)
         {
           data = Test_LargeSeq_get(ref, arsz, ev);
           CORBA_free(data);
         }
       gettimeofday(&end, NULL);
       d_secs = end.tv_sec - start.tv_sec;
       d_usec = end.tv_usec - start.tv_usec;
       printf("\n%d,%f", arsz, (float) d_secs + ((float) d_usec / 1000000.0));
       arsz += STEP_LINEAR;
   }
}

int
main(int argc, char* argv[])
{
	CORBA_char filename[] = "/tmp/test-ldata.ior";
        
	Test_LargeSeq ldata_service = CORBA_OBJECT_NIL;

        CORBA_Environment ev[1];
        CORBA_exception_init(ev);

	client_init (&argc, argv, &global_orb, ev);
	etk_abort_if_exception(ev, "init failed");

	g_print ("Reading service reference from file \"%s\"\n", filename);

	ldata_service = (Test_LargeSeq) etk_import_object_from_file (global_orb,
							   filename,
							   ev);
        etk_abort_if_exception(ev, "import service failed");

	client_run (ldata_service, ev);
        etk_abort_if_exception(ev, "service not reachable");
 
	client_cleanup (global_orb, ldata_service, ev);
        etk_abort_if_exception(ev, "cleanup failed");
 
        exit (0);
}
