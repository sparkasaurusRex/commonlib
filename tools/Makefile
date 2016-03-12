CC=g++

CFLAGS=-I./include -I../mathlib/include -I../graphics/include
CFLAGS+=-I/usr/local/include -I../game/include
CFLAGS+=$(shell pkg-config mxml --cflags)
#CFLAGS+=-Ofast
CFLAGS+= -g3 -O0

LIB_PATH=
LDFLAGS=$(shell pkg-config mxml --libs)
LDFLAGS+=-lgraphics -ltool -lmath
SRCPATH=src/
SRCS=src/bake_static_mesh.cpp src/bakery.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	CFLAGS+=-Wno-deprecated-declarations
	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../graphics/bin/osx
	LIB_PATH+=-L../mathlib/bin/osx
	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH += -Lbin/linux
	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)


all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libtool.a bake

$(BINPATH):
	mkdir -p $(BINPATH)

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)bakery.o: $(SRCPATH)bakery.cpp
	$(CC) $(CFLAGS) -c src/bakery.cpp -o $(BINPATH)bakery.o

$(BINPATH)bake_static_mesh.o: $(SRCPATH)bake_static_mesh.cpp
	$(CC) $(CFLAGS) -c src/bake_static_mesh.cpp -o $(BINPATH)bake_static_mesh.o

$(BINPATH)libtool.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libtool.a $(OBJS_BIN)

bake: bake.cpp $(BINPATH)libtool.a $(SRCS)
	$(CC) $(CFLAGS) bake.cpp -o bake $(LIB_PATH) $(LDFLAGS)

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libtool.a
	rm -f test
