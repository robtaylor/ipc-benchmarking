PREFIX ?= /usr
CC = gcc
ORBIT_IDL=$(PREFIX)/bin/orbit-idl-2
CFLAGS=$(shell pkg-config ORBit-2.0 ORBit-CosNaming-2.0 bonobo-activation-2.0 libbonobo-2.0 libbonoboui-2.0 --cflags)
LDFLAGS=$(shell pkg-config ORBit-2.0 ORBit-CosNaming-2.0 bonobo-activation-2.0 libbonobo-2.0 libbonoboui-2.0 --libs)
IDLFLAGS=-I/usr/share/idl/bonobo-2.0/ -I/usr/share/idl/bonobo-activation-2.0/ -I./ -D__ACCESSIBILITY_COMPILATION__

TARGETS=orbit-server
IDLOUT=Accessibility-common.c Accessibility-stubs.c Accessibility-skels.c Accessibility.h

all: $(IDLOUT) orbit-server

orbit-server.o: orbit-server.c Accessibility-impl.c

orbit-server : orbit-server.o Accessibility-skels.o Accessibility-common.o examples-toolkit.o

$(IDLOUT): Accessibility.idl
	$(ORBIT_IDL) Accessibility.idl $(IDLFLAGS)

clean:
	rm -rf *.o *~ $(IDLOUT) *.ior *.ref *.pyc

distclean: clean
	rm -rf orbit-server
