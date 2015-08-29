CC=g++
LIB_PATH=
CFLAGS=-I./include -I../mathlib/include -I../graphics/include -I/usr/local/include
CFLAGS+=-I../game/include
CFLAGS+= -Ofast
CFLAGS+=$(shell pkg-config sdl2 --cflags)
CFLAGS+=$(shell pkg-config SDL2_image --cflags)
CFLAGS+=$(shell pkg-config freetype2 --cflags)
LDFLAGS=-lmath $(shell pkg-config sdl2 --libs)
LDFLAGS+=$(shell pkg-config SDL2_image --libs)
LDFLAGS+=$(shell pkg-config freetype2 --libs)
LDFLAGS+=-lgraphics
SRCPATH=src/
SRCS=src/fluid2d.cpp src/fluid2d_interactor.cpp src/fluid2d_inflow.cpp src/fluid2d_turbulence.cpp
SRCS+=src/fluid2d_turbulence_inflow.cpp src/fluid2d_angle_snapper.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../mathlib/bin/osx
	LIB_PATH+=-L../graphics/bin/osx
	LIB_PATH+=-L../game/bin/osx

	#LDFLAGS+=-framework SDL2
	#LDFLAGS+=-framework SDL2_image
	LDFLAGS+=-framework OpenGL
	#LDFLAGS+=-lglew
	BINPATH=bin/osx/
else

	CFLAGS+=$(shell pkg-config gl --cflags)
	CFLAGS+=$(shell pkg-config glew --cflags)
	#generic link flags
	LIB_PATH+=-Lbin/linux
	LIB_PATH+=-L../mathlib/bin/linux
	LIB_PATH+=-L../graphics/bin/linux
	LIB_PATH+=-L../game/bin/linux

	#LDFLAGS += $(shell pkg-config --libs sdl2)
	#LDFLAGS += $(shell pkg-config --libs SDL2_image)
	LDFLAGS+=$(shell pkg-config gl --libs)
	LDFLAGS+=$(shell pkg-config glew --libs)

	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libfluids.a test

$(BINPATH):
	mkdir -p $(BINPATH)

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)fluid2d.o: $(SRCPATH)fluid2d.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d.cpp -o $(BINPATH)fluid2d.o

$(BINPATH)fluid2d_interactor.o: $(SRCPATH)fluid2d_interactor.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d_interactor.cpp -o $(BINPATH)fluid2d_interactor.o

$(BINPATH)fluid2d_inflow.o: $(SRCPATH)fluid2d_inflow.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d_inflow.cpp -o $(BINPATH)fluid2d_inflow.o

$(BINPATH)fluid2d_turbulence.o: $(SRCPATH)fluid2d_turbulence.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d_turbulence.cpp -o $(BINPATH)fluid2d_turbulence.o

$(BINPATH)fluid2d_turbulence_inflow.o: $(SRCPATH)fluid2d_turbulence_inflow.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d_turbulence_inflow.cpp -o $(BINPATH)fluid2d_turbulence_inflow.o

$(BINPATH)fluid2d_angle_snapper.o: $(SRCPATH)fluid2d_angle_snapper.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d_angle_snapper.cpp -o $(BINPATH)fluid2d_angle_snapper.o

$(BINPATH)libfluids.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libfluids.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libfluids.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) -lfluids -lgraphics -lgame $(LDFLAGS)

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libfluids.a
	rm -f test
