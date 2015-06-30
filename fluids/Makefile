CC=g++
LIB_PATH=
CFLAGS=-I./include -I../mathlib/include -I../graphics/include -Ofast
LDFLAGS=-lmath
SRCPATH=src/
SRCS=src/fluid2d.cpp src/fluid2d_interactor.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	LIB_PATH += -Lbin/osx
	LIB_PATH += -L../mathlib/bin/osx
	LIB_PATH += -L../graphics/bin/osx

	LDFLAGS+=-framework SDL2
	LDFLAGS+=-framework SDL2_image
	LDFLAGS+=-framework OpenGL
	LDFLAGS+=-lglew
	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH += -Lbin/linux
	LIB_PATH += -L../mathlib/bin/linux
	LIB_PATH += -L../graphics/bin/osx

	LDFLAGS += $(shell pkg-config --libs sdl2)
	LDFLAGS += $(shell pkg-config --libs SDL2_image)
	LDFLAGS += $(shell pkg-config --libs gl)

	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(OBJS_BIN) $(BINPATH)libfluids.a test

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)fluid2d.o: $(SRCPATH)fluid2d.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d.cpp -o $(BINPATH)fluid2d.o

$(BINPATH)fluid2d_interactor.o: $(SRCPATH)fluid2d_interactor.cpp
		$(CC) $(CFLAGS) -c $(SRCPATH)fluid2d_interactor.cpp -o $(BINPATH)fluid2d_interactor.o


$(BINPATH)libfluids.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libfluids.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libfluids.a
	$(CC) $(CFLAGS) $(LIB_PATH) $(LDFLAGS) -lfluids -lgraphics test.cpp -o test

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libfluids.a
	rm -f test
