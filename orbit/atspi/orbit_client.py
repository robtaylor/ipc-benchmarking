import sys
import ORBit, bonobo
from time import time
from threading import Thread

TYPELIB_NAME = "Accessibility"
orb = ORBit.CORBA.ORB_init()
ORBit.load_typelib(TYPELIB_NAME)

import Accessibility
import Accessibility__POA

NUM_CALLS = 18000

class ORBThread(Thread):
	def __init__(self):
		Thread.__init__(self)
		self.setDaemon(True)

	def run(self):
		orb.run()

class AtspiEventListener(Accessibility__POA.EventListener):
	def ref(self): 
		pass
    
	def unref(self): 
		pass

	def register(self, reg, name):
		reg.registerGlobalEventListener(self._this(), name)
    
	def unregister(self, reg, name):
		reg.deregisterGlobalEventListener(self._this(), name)

	def notifyEvent(self, ev):
		pass
     
def main(args):
	f = file('/tmp/test-atspi.ior', 'r')
	ior_str = f.readline()[:-1]
	parent = orb.string_to_object(ior_str)
	f.close()

	t = time()
	for count in range(NUM_CALLS):
		role = parent.getRole()
	print "Time taken (getRole): %f\n" % (time() - t)

	t = time()
	for count in range(NUM_CALLS):
		accessible = parent.parent
	print "Time taken (parent): %f\n" % (time() - t)
	
	t = time()
	for count in range(NUM_CALLS):
		relations = parent.getRelationSet()
	print "Time taken (getRelationSet): %f\n" % (time() - t)

	f = file('/tmp/test-registry-atspi.ior', 'r')
	ior_str = f.readline()[:-1]
	reg = orb.string_to_object(ior_str)
	f.close()
	eventListener = AtspiEventListener()

	# Spawn thread to deal with event calls
	tr = ORBThread()
	tr.start()

	t = time()
	# This function is going to call NUM_CALLS events 
	# on the event listener before returning
	reg.registerGlobalEventListener(eventListener._this(), "")
	print "Time taken (Events): %f\n" % (time() - t)

if __name__ == '__main__':
	sys.exit(main(sys.argv))
