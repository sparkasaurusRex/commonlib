CC=g++
LIB_PATH=
CFLAGS=-I./include -I/usr/local/include/ -Ofast
CFLAGS+=$(shell pkg-config --cflags sdl2)
LDFLAGS=$(shell pkg-config --libs sdl2)
SRCPATH=src/
SRCS=src/sdl_game.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	CFLAGS+=-Wno-deprecated-declarations
	LIB_PATH+=-Lbin/osx
	LDFLAGS+=-framework OpenGL

	BINPATH=bin/osx/
else
	#generic link flags
	LDFLAGS+=$(shell pkg-config --libs gl)
	LIB_PATH += -Lbin/linux
	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libgame.a test

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)sdl_game.o: $(SRCPATH)sdl_game.cpp
	$(CC) $(CFLAGS) -c src/sdl_game.cpp -o $(BINPATH)sdl_game.o

$(BINPATH)libgame.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libgame.a $(OBJS_BIN)

$(BINPATH):
	mkdir -p $(BINPATH)

test: test.cpp $(BINPATH)libgame.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) -lgame $(LDFLAGS)

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libgame.a
	rm -f test
