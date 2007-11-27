#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <orbit/orbit.h>

#include "Accessibility-impl.c"
#include "examples-toolkit.h"

#include "Accessibility.h"

static CORBA_ORB          global_orb = CORBA_OBJECT_NIL;
static PortableServer_POA root_poa   = CORBA_OBJECT_NIL;
	
static
void
server_shutdown (int sig)
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
server_init (int                 *argc_ptr, 
	     char                *argv[],
	     CORBA_ORB           *orb,
	     PortableServer_POA  *poa,
	     CORBA_Environment   *ev)
{
	PortableServer_POAManager  poa_manager = CORBA_OBJECT_NIL; 

	CORBA_Environment  local_ev[1];
	CORBA_exception_init(local_ev);

	signal(SIGINT,  server_shutdown);
	signal(SIGTERM, server_shutdown);
	
	
        (*orb) = CORBA_ORB_init(argc_ptr, argv, "orbit-local-mt-orb", ev);
	if (etk_raised_exception(ev)) 
		goto failed_orb;

        (*poa) = (PortableServer_POA) 
		CORBA_ORB_resolve_initial_references(*orb, "RootPOA", ev);
	if (etk_raised_exception(ev)) 
		goto failed_poa;

        poa_manager = PortableServer_POA__get_the_POAManager(*poa, ev);
	if (etk_raised_exception(ev)) 
		goto failed_poamanager;

	PortableServer_POAManager_activate(poa_manager, ev);
	if (etk_raised_exception(ev)) 
		goto failed_activation;

        CORBA_Object_release ((CORBA_Object) poa_manager, ev);
	return;

 failed_activation:
 failed_poamanager:
        CORBA_Object_release ((CORBA_Object) poa_manager, local_ev);
 failed_poa:
	CORBA_ORB_destroy(*orb, local_ev);		
 failed_orb:
	return;
}

static void 
server_run (CORBA_ORB          orb,
	    CORBA_Environment *ev)
{
        CORBA_ORB_run(orb, ev);
	if (etk_raised_exception(ev)) return;
}

static void 
server_cleanup (CORBA_ORB           orb,
		PortableServer_POA  poa,
		CORBA_Object        ref,
		CORBA_Object        reg,
		CORBA_Environment  *ev)
{
	int i = 0;
	PortableServer_ObjectId   *objid       = NULL;

	objid = PortableServer_POA_reference_to_id (poa, ref, ev);
	if (etk_raised_exception(ev)) return;
		
	PortableServer_POA_deactivate_object (poa, objid, ev);
	if (etk_raised_exception(ev)) return;

        CORBA_Object_release (ref, ev);
	if (etk_raised_exception(ev)) return;

	CORBA_free (objid);

	objid = PortableServer_POA_reference_to_id (poa, reg, ev);
	if (etk_raised_exception(ev)) return;
		
	PortableServer_POA_deactivate_object (poa, objid, ev);
	if (etk_raised_exception(ev)) return;

        CORBA_Object_release (reg, ev);
	if (etk_raised_exception(ev)) return;

	CORBA_free (objid);

	
	for (i=0; i<NUM_RELATIONS; i++)
	{
		objid = PortableServer_POA_reference_to_id (poa, global_accessible_relations[i], ev);
		if (etk_raised_exception(ev)) return;
		
		PortableServer_POA_deactivate_object (poa, objid, ev);
		if (etk_raised_exception(ev)) return;

		CORBA_Object_release (ref, ev);
		if (etk_raised_exception(ev)) return;

		CORBA_free (objid);
	}

	PortableServer_POA_destroy (poa, TRUE, FALSE, ev);
	if (etk_raised_exception(ev)) return;

        CORBA_Object_release ((CORBA_Object) poa, ev);
	if (etk_raised_exception(ev)) return;

        if (orb != CORBA_OBJECT_NIL)
        {
                CORBA_ORB_destroy(orb, ev);
		if (etk_raised_exception(ev)) return;
        }
}

static CORBA_Object
server_activate_service (CORBA_ORB           orb,
			 PortableServer_POA  poa,
			 CORBA_Environment  *ev)
{
	int i = 0;
	global_accessible_parent = impl_Accessibility_Accessible__create (poa, ev);
	if (etk_raised_exception(ev)) 
		return CORBA_OBJECT_NIL;

	for (i=0; i<NUM_RELATIONS; i++)
	{
		global_accessible_relations[i] = impl_Accessibility_Relation__create(poa, ev);
		if (etk_raised_exception(ev)) { 
			global_accessible_relations[i] = CORBA_OBJECT_NIL;
			g_print ("Writing NIL relation object");
		}
	}
	
	return global_accessible_parent;
}

static CORBA_Object
registry_activate_service (CORBA_ORB           orb,
	   		   PortableServer_POA  poa,
			   CORBA_Environment  *ev)
{
	int i = 0;
	CORBA_Object accessible_registry = impl_Accessibility_Registry__create (poa, ev);
	if (etk_raised_exception(ev)) 
		return CORBA_OBJECT_NIL;

	return accessible_registry;
}

int
main (int argc, char *argv[])
{
	CORBA_Object servant = CORBA_OBJECT_NIL;
	CORBA_Object registry_servant = CORBA_OBJECT_NIL;
	
	CORBA_char filename[] = "/tmp/test-atspi.ior";
	CORBA_char registry_filename[] = "/tmp/test-registry-atspi.ior";

	CORBA_Environment  ev[1];
	CORBA_exception_init(ev);
	
	server_init (&argc, argv, &global_orb, &root_poa, ev);
	etk_abort_if_exception(ev, "failed ORB init");

	servant = server_activate_service (global_orb, root_poa, ev);
	etk_abort_if_exception(ev, "failed activating service");

	g_print ("Writing service reference to: %s\n\n", filename);

	etk_export_object_to_file (global_orb, 
				   servant, 
				   filename, 
				   ev);
	etk_abort_if_exception(ev, "failed exporting IOR");

	registry_servant = registry_activate_service (global_orb, root_poa, ev);
	etk_abort_if_exception(ev, "failed activating registry");

	g_print ("Writing registry reference to: %s\n\n", registry_filename);

	etk_export_object_to_file (global_orb, 
				   registry_servant, 
				   registry_filename, 
				   ev);
	etk_abort_if_exception(ev, "failed exporting IOR");
	
	server_run (global_orb, ev);
	etk_abort_if_exception(ev, "failed entering main loop");

	server_cleanup (global_orb, root_poa, servant, registry_servant, ev);
	etk_abort_if_exception(ev, "failed cleanup");

	exit (0);
}
