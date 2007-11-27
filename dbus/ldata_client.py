import sys
import dbus
from time import time

NUM_ITER = 16
NUM_CALLS = 10000

def main(args):
	if len(args) > 1:
		if (args[1] == '--direct'):
			bus = dbus.connection.Connection('unix:path=/tmp/testatspi')
	else:
		bus = dbus.SessionBus()

	obj = bus.get_object('test.dbus.atspi', '/test/dbus/atspi/accessible')
	adapter = dbus.Interface(obj, 'test.dbus.atspi.Accessible')

	asize = 1
	for expon in range(NUM_ITER):
		t = time()
		for count in range(NUM_CALLS):
			intarray = adapter.getIntArray(asize)
		print "%d : %f\n" % (asize, (time() - t))
		asize *= 2

if __name__ == '__main__':
	sys.exit(main(sys.argv))
