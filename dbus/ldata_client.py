import sys
import dbus
from time import time

NUM_EXP = 16
NUM_LINEAR = 64

STEP_EXP = 2
STEP_LINEAR = 512

NUM_CALLS = 5000

def main(args):
	if len(args) > 1:
		if (args[1] == '--direct'):
			bus = dbus.connection.Connection('unix:path=/tmp/testatspi')
	else:
		bus = dbus.SessionBus()

	obj = bus.get_object('test.dbus.atspi', '/test/dbus/atspi/accessible')
	adapter = dbus.Interface(obj, 'test.dbus.atspi.Accessible')

	asize = STEP_LINEAR
	for expon in range(NUM_LINEAR):
		t = time()
		for count in range(NUM_CALLS):
			intarray = adapter.getIntArray(asize)
		print "%d,%f" % (asize, (time() - t))
		asize += STEP_LINEAR

if __name__ == '__main__':
	sys.exit(main(sys.argv))
