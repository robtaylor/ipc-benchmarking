PREFIX ?= /usr
CC = gcc
CFLAGS=-g -O0
CFLAGS+=$(shell pkg-config dbus-1 dbus-glib-1 --cflags)
LDFLAGS=$(shell pkg-config dbus-1 dbus-glib-1 --libs)

all: dbus-server ldata-client ldata-marshal-test role-client

dbus-server : dbus-server.o

ldata-client : ldata-client.o

role-client : role-client.o

ldata-marshal-test : ldata-marshal-test.o

clean:
	rm -rf *.o *~ *.pyc

distclean: clean
	rm -rf dbus-server ldata-client ldata-marshal-test role-client
