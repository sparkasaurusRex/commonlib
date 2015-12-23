CC=g++
LIB_PATH=
CFLAGS=-I./include -I../game/include  -I../game/include/cinematics -I/usr/local/include
CFLAGS+=-I../graphics/include -I../ui/include -I../mathlib/include
CFLAGS+=$(shell pkg-config --cflags freetype2)
CFLAGS+=-Ofast

LDFLAGS=$(shell pkg-config --libs sdl2)
LDFLAGS+=$(shell pkg-config --libs SDL2_image)
LDFLAGS+=$(shell pkg-config --libs freetype2)
LDFLAGS+=$(shell pkg-config --libs glew)

SRCPATH=src/
SRCS=src/audio.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../game/bin/osx
	LIB_PATH+=-L../graphics/bin/osx
	LIB_PATH+=-L../ui/bin/osx
	LIB_PATH+=-L../mathlib/bin/osx
	BINPATH=bin/osx/

	LDFLAGS+=-framework OpenGL
else
	#generic link flags
	LIB_PATH+=-Lbin/linux
	LIB_PATH+=-L../game/bin/linux
	LIB_PATH+=-L../graphics/bin/linux
	LIB_PATH+=-L../mathlib/bin/linux
	BINPATH=bin/linux/

	#LDFLAGS+=
	LDFLAGS+=-lGL
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))

all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libaudio.a test

$(BINPATH):
	mkdir -p $(BINPATH)

$(BINPATH)audio.o: $(SRCPATH)audio.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)audio.cpp -o $(BINPATH)audio.o

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)libaudio.a: $(OBJS_BIN) $(BINPATH)
	ar -cvq $(BINPATH)libaudio.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libaudio.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) -laudio -lgame -lgraphics -lui -lmath $(LDFLAGS)

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libaudio.a
	rm -f test
