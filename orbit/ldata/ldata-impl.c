/* This is a template file generated by command */
/* orbit-idl-2 --skeleton-impl ldata.idl */
/* User must edit this file, inserting servant  */
/* specific code between markers. */

#include "ldata.h"

/*** App-specific servant structures ***/

#if !defined(_typedef_impl_POA_Test_LargeSeq_)
#define _typedef_impl_POA_Test_LargeSeq_ 1
typedef struct {
POA_Test_LargeSeq servant;
PortableServer_POA poa;
   /* ------ add private attributes here ------ */
   /* ------ ---------- end ------------ ------ */
} impl_POA_Test_LargeSeq;
#endif


/*** Implementation stub prototypes ***/

#if !defined(_decl_impl_Test_LargeSeq__destroy_)
#define _decl_impl_Test_LargeSeq__destroy_ 1
static void impl_Test_LargeSeq__destroy(impl_POA_Test_LargeSeq *servant,
CORBA_Environment *ev);
#endif

#if !defined(_decl_impl_Test_LargeSeq_get_)
#define _decl_impl_Test_LargeSeq_get_ 1
static Test_LargeSeq_LargeData*
impl_Test_LargeSeq_get(impl_POA_Test_LargeSeq *servant,
const CORBA_long dataSize,
CORBA_Environment *ev);
#endif


/*** epv structures ***/

#if !defined(_impl_Test_LargeSeq_base_epv_)
#define _impl_Test_LargeSeq_base_epv_ 1
static PortableServer_ServantBase__epv impl_Test_LargeSeq_base_epv = {
NULL,             /* _private data */
(gpointer) & impl_Test_LargeSeq__destroy, /* finalize routine */
NULL,             /* default_POA routine */
};
#endif

#if !defined(_impl_Test_LargeSeq_epv_)
#define _impl_Test_LargeSeq_epv_ 1
static POA_Test_LargeSeq__epv impl_Test_LargeSeq_epv = {
NULL, /* _private */
(gpointer)&impl_Test_LargeSeq_get,
};
#endif


/*** vepv structures ***/

#if !defined(_impl_Test_LargeSeq_vepv_)
#define _impl_Test_LargeSeq_vepv_ 1
static POA_Test_LargeSeq__vepv impl_Test_LargeSeq_vepv = {
&impl_Test_LargeSeq_base_epv,
&impl_Test_LargeSeq_epv,
};
#endif


/*** Stub implementations ***/

#if !defined(_impl_Test_LargeSeq__create_)
#define _impl_Test_LargeSeq__create_ 1
static Test_LargeSeq impl_Test_LargeSeq__create(PortableServer_POA poa, CORBA_Environment *ev)
{
Test_LargeSeq retval;
impl_POA_Test_LargeSeq *newservant;
PortableServer_ObjectId *objid;

newservant = g_new0(impl_POA_Test_LargeSeq, 1);
newservant->servant.vepv = &impl_Test_LargeSeq_vepv;
newservant->poa = (PortableServer_POA) CORBA_Object_duplicate((CORBA_Object)poa, ev);
POA_Test_LargeSeq__init((PortableServer_Servant)newservant, ev);
   /* Before servant is going to be activated all
    * private attributes must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

objid = PortableServer_POA_activate_object(poa, newservant, ev);
CORBA_free(objid);
retval = PortableServer_POA_servant_to_reference(poa, newservant, ev);

return retval;
}
#endif

#if !defined(_impl_Test_LargeSeq__destroy_)
#define _impl_Test_LargeSeq__destroy_ 1
static void
impl_Test_LargeSeq__destroy(impl_POA_Test_LargeSeq *servant, CORBA_Environment *ev)
{
    CORBA_Object_release ((CORBA_Object) servant->poa, ev);

    /* No further remote method calls are delegated to 
    * servant and you may free your private attributes. */
   /* ------ free private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

POA_Test_LargeSeq__fini((PortableServer_Servant)servant, ev);

g_free (servant);
}
#endif

#if !defined(_impl_Test_LargeSeq_get_)
#define _impl_Test_LargeSeq_get_ 1
static Test_LargeSeq_LargeData*
impl_Test_LargeSeq_get(impl_POA_Test_LargeSeq *servant,
const CORBA_long dataSize,
CORBA_Environment *ev)
{
Test_LargeSeq_LargeData* retval;
 /* ------   insert method code here   ------ */
 /* ------ ---------- end ------------ ------ */
CORBA_long i=0;
retval = ORBit_sequence_alloc (TC_Test_LargeSeq_LargeData, dataSize);
for(i=0; i<dataSize; i++)
  {
    ORBit_sequence_index(retval, i) = (CORBA_long) 0xAAAAAAAA;
  }

return retval;
}
#endif

