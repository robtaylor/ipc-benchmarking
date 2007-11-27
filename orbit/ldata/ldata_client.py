import sys
import ORBit, bonobo
from time import time
from threading import Thread
from os.path import abspath

IDL_NAME = abspath('ldata.idl')
orb = ORBit.CORBA.ORB_init()
ORBit.load_file(IDL_NAME)

import Test

NUM_ITER = 16
NUM_CALLS = 10000

def main(args):
	f = file('/tmp/test-ldata.ior', 'r')
	ior_str = f.readline()[:-1]
	ldata = orb.string_to_object(ior_str)
	f.close()

	asize = 1
	for expon in range(NUM_ITER):
		t = time()
		for count in range(NUM_CALLS):
			intarray = ldata.get(asize)
		print "%d : %f\n" % (asize, (time() - t))
		asize *= 2

if __name__ == '__main__':
	sys.exit(main(sys.argv))
