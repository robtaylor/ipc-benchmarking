PREFIX ?= /usr
CC = gcc
ORBIT_IDL=$(PREFIX)/bin/orbit-idl-2
CFLAGS=$(shell pkg-config ORBit-2.0 ORBit-CosNaming-2.0 --cflags)
LDFLAGS=$(shell pkg-config ORBit-2.0 ORBit-CosNaming-2.0 --libs)

TARGETS=ldata-server
IDLOUT=ldata-common.c ldata-stubs.c ldata-skels.c ldata.h

all: $(IDLOUT) ldata-server

ldata-server.o: ldata-server.c ldata-impl.c
ldata-server : ldata-server.o ldata-skels.o ldata-common.o examples-toolkit.o 

$(IDLOUT): ldata.idl
	$(ORBIT_IDL) ldata.idl

clean:
	rm -rf *.o *~ $(IDLOUT) *.ior *.ref

distclean: clean
	rm -rf ldata-server
