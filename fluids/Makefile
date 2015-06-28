CC=g++
LIB_PATH=
CFLAGS=-I./include -I../mathlib/include
LDFLAGS=-lmath
SRCPATH=src/
SRCS=src/fluid2d.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	LIB_PATH += -Lbin/osx
	LIB_PATH += -L../mathlib/bin/osx

	LDFLAGS+=-framework SDL2
	LDFLAGS+=-framework OpenGL
	LDFLAGS+=-lglew
	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH += -Lbin/linux
	LIB_PATH += -L../mathlib/bin/linux
	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(OBJS) $(BINPATH)libfluids.a test

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@
	mv $@ $(BINPATH)

$(BINPATH)libfluids.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libfluids.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libfluids.a
	$(CC) $(CFLAGS) $(LIB_PATH) $(LDFLAGS) -lfluids test.cpp -o test

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libfluids.a
	rm -f test