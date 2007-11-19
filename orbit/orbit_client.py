import sys
import ORBit, bonobo
from time import time

TYPELIB_NAME = "Accessibility"
orb = ORBit.CORBA.ORB_init()
ORBit.load_typelib(TYPELIB_NAME)

import Accessibility

NUM_CALLS = 10000

def main(args):
	f = file('/tmp/test-atspi.ior', 'r')
	ior_str = f.readline()[:-1]
	parent = orb.string_to_object(ior_str)

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
	print "Time taken (parent): %f\n" % (time() - t)

if __name__ == '__main__':
	sys.exit(main(sys.argv))
