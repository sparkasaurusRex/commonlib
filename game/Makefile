CC=g++
LIB_PATH=
CFLAGS=-I./include -I./include/cinematics -I/usr/local/include/
CFLAGS+=-I../graphics/include/ -I../ui/include
CFLAGS+=-I../mathlib/include/
CFLAGS+=$(shell pkg-config --cflags sdl2)
CFLAGS+=$(shell pkg-config --cflags freetype2)
CFLAGS+=-Ofast

LDFLAGS=$(shell pkg-config --libs sdl2)
LDFLAGS+=$(shell pkg-config --libs freetype2)
LDFLAGS+=$(shell pkg-config --libs glew)
LDFLAGS+=-lmath -lui
SRCPATH=src/
SRCS=src/sdl_game.cpp src/console.cpp src/cinematics/fade_screen.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	CFLAGS+=-Wno-deprecated-declarations
	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../graphics/bin/osx
	LIB_PATH+=-L../mathlib/bin/osx
	LIB_PATH+=-L../ui/bin/osx
	LDFLAGS+=-framework OpenGL

	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH+=-Lbin/linux
	LIB_PATH+=-L../graphics/bin/linux
	LIB_PATH+=-L../mathlib/bin/linux

	LDFLAGS+=$(shell pkg-config --libs gl)

	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libgame.a test

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH):
	mkdir -p $(BINPATH)

$(BINPATH)sdl_game.o: $(SRCPATH)sdl_game.cpp
	$(CC) $(CFLAGS) -c src/sdl_game.cpp -o $(BINPATH)sdl_game.o

$(BINPATH)console.o: $(SRCPATH)console.cpp
	$(CC) $(CFLAGS) -c src/console.cpp -o $(BINPATH)console.o

$(BINPATH)fade_screen.o: $(SRCPATH)cinematics/fade_screen.cpp
	$(CC) $(CFLAGS) -c src/cinematics/fade_screen.cpp -o $(BINPATH)fade_screen.o

$(BINPATH)libgame.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libgame.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libgame.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) -lgame -lgraphics $(LDFLAGS)

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libgame.a
	rm -f test
