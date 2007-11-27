import sys
import dbus
from time import time

from dbus.mainloop.glib import DBusGMainLoop
import gobject

adapter = None
loop = None
gt = None

NUM_CALLS = 18000

def make_calls():
	global adapter

	t = time()
	adapter.registerEventListener()
	print "Time taken (registerEventListener): %f\n" % (time() - t)
	
	return False

count = 0
def handle_event(event):
	global count
	global gt
	if count == 0:
		gt = time()
	count += 1
	if count == NUM_CALLS:
		print "Time taken (HandleEvents): %f\n" % (time() - gt)
		loop.quit()

def main(args):
	global adapter
	global loop
	DBusGMainLoop(set_as_default=True)
	if len(args) > 1:
		if (args[1] == '--direct'):
			bus = dbus.connection.Connection('unix:path=/tmp/testatspi')
	else:
		bus = dbus.SessionBus()

	obj = bus.get_object('test.dbus.atspi', '/test/dbus/atspi/accessible')
	adapter = dbus.Interface(obj, 'test.dbus.atspi.Accessible')

	bus.add_signal_receiver(handle_event, 'notifyEvent', 'test.dbus.atspi.Accessible', None, None)

	loop = gobject.MainLoop()
	gobject.idle_add(make_calls)
	try:
		loop.run()
	except KeyboardInterrupt:
		pass

if __name__ == '__main__':
	sys.exit(main(sys.argv))
