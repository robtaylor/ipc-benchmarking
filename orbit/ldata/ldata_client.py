import sys
import ORBit, bonobo
from time import time
from threading import Thread
from os.path import abspath

IDL_NAME = abspath('ldata.idl')
orb = ORBit.CORBA.ORB_init()
ORBit.load_file(IDL_NAME)

import Test

NUM_EXP = 16
NUM_LINEAR = 64

STEP_EXP = 2
STEP_LINEAR = 512

NUM_CALLS = 2000

def main(args):
	f = file('/tmp/test-ldata.ior', 'r')
	ior_str = f.readline()[:-1]
	ldata = orb.string_to_object(ior_str)
	f.close()

	asize = STEP_LINEAR
	for expon in range(NUM_LINEAR):
		t = time()
		for count in range(NUM_CALLS):
			intarray = ldata.get(asize)
		print "%d,%f" % (asize, (time() - t))
		asize += STEP_LINEAR

if __name__ == '__main__':
	sys.exit(main(sys.argv))
