CC=g++
LIB_PATH=
CFLAGS=-I./include -Ofast
SRCPATH=src/
SRCS=src/sdl_game.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	CFLAGS+=-Wno-deprecated-declarations

	LIB_PATH+=-Lbin/osx

	LDFLAGS+=-framework SDL2
	#LDFLAGS+=-framework SDL2_image
	LDFLAGS+=-framework OpenGL

	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH += -Lbin/linux

	BINPATH=bin/linux/
	CFLAGS+=$(shell pkg-config --cflags sdl2)

	LDFLAGS+=$(shell pkg-config --libs sdl2)
#	LDFLAGS+=$(shell pkg-config --libs SDL2_image)
	LDFLAGS+=-lGL
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(OBJS_BIN) $(BINPATH)libgame.a test

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)


#$(BINPATH)static_mesh.o: $(SRCPATH)static_mesh.cpp
#	$(CC) $(CFLAGS) -c src/static_mesh.cpp -o $(BINPATH)static_mesh.o

$(BINPATH)sdl_game.o: $(SRCPATH)sdl_game.cpp
	$(CC) $(CFLAGS) -c src/sdl_game.cpp -o $(BINPATH)sdl_game.o

$(BINPATH)libgame.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libgame.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libgame.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) $(LDFLAGS) -lgame

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libgame.a
	rm -f test
