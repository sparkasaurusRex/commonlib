CC=g++
LIB_PATH=
CFLAGS=-I./include -I../mathlib/include -I/usr/include/libxml2 -I../fluids/include
SRCPATH=src/
SRCS=src/font.cpp src/glutility.cpp src/material.cpp src/shader.cpp src/texture.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	CFLAGS+=-I/opt/X11/include/freetype2/
	CFLAGS+=-Wno-deprecated-declarations

	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../mathlib/bin/osx
	LIB_PATH+=-L../fluids/bin/osx

	LDFLAGS+=-framework SDL2
	LDFLAGS+=-framework SDL2_image
	LDFLAGS+=-framework OpenGL

	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH += -Lbin/linux
	LIB_PATH += -L../mathlib/bin/linux
	LIB_PATH += -L../fluids/bin/linux

	BINPATH=bin/linux/
	CFLAGS+=-I/usr/include/freetype2/
	CFLAGS+=$(shell pkg-config --libs sdl2)

	LDFLAGS+=$(shell pkg-config --libs sdl2)
	LDFLAGS+=$(shell pkg-config --libs SDL2_image)
	LDFLAGS+=-lGL
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(OBJS_BIN) $(BINPATH)libgraphics.a test

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)font.o: $(SRCPATH)font.cpp
	$(CC) $(CFLAGS) -c src/font.cpp -o $(BINPATH)font.o

$(BINPATH)glutility.o: $(SRCPATH)glutility.cpp
	$(CC) $(CFLAGS) -c src/glutility.cpp -o $(BINPATH)glutility.o

$(BINPATH)material.o: $(SRCPATH)material.cpp
	$(CC) $(CFLAGS) -c src/material.cpp -o $(BINPATH)material.o

$(BINPATH)shader.o: $(SRCPATH)shader.cpp
	$(CC) $(CFLAGS) -c src/shader.cpp -o $(BINPATH)shader.o

#$(BINPATH)static_mesh.o: $(SRCPATH)static_mesh.cpp
#	$(CC) $(CFLAGS) -c src/static_mesh.cpp -o $(BINPATH)static_mesh.o

$(BINPATH)texture.o: $(SRCPATH)texture.cpp
	$(CC) $(CFLAGS) -c src/texture.cpp -o $(BINPATH)texture.o

$(BINPATH)libgraphics.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libgraphics.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libgraphics.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) $(LDFLAGS) -lgraphics -lmath -lfluids

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libgraphics.a
	rm -f test
