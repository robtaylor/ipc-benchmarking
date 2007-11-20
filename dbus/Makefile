PREFIX ?= /usr
CC = gcc
CFLAGS=$(shell pkg-config dbus-1 --cflags)
LDFLAGS=$(shell pkg-config dbus-1 --libs)

all: dbus-server

dbus-server : dbus-server.o

clean:
	rm -rf *.o *~ *.pyc

distclean: clean
	rm -rf dbus-server
