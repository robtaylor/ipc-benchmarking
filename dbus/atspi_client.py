import sys
import dbus
from time import time

NUM_CALLS = 18000

def main(args):
	if len(args) > 1:
		if (args[1] == '--direct'):
			bus = dbus.connection.Connection('unix:path=/tmp/testatspi')
	else:
		bus = dbus.SessionBus()

	obj = bus.get_object('test.dbus.atspi', '/test/dbus/atspi/accessible')
	adapter = dbus.Interface(obj, 'test.dbus.atspi.Accessible')

	t = time()
	for count in range(NUM_CALLS):
		role = adapter.getRole()
	print "Time taken (getRole): %f\n" % (time() - t)

	t = time()
	for count in range(NUM_CALLS):
		parent = adapter.get_parent()
	print "Time taken (get_parent): %f\n" % (time() - t)
	
	t = time()
	for count in range(NUM_CALLS):
		parent = adapter.getRelationSet()
	print "Time taken (getRelationSet): %f\n" % (time() - t)

if __name__ == '__main__':
	sys.exit(main(sys.argv))
