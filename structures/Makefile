CC=g++
LIB_PATH=
CFLAGS=-I./include -I../mathlib/include -g -Ofast -std=c++11
SRCPATH=src/
SRCS=src/kdtree.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../mathlib/bin/osx

	BINPATH=bin/osx/
	LINK_CMD=g++ -dynamiclib -o $(BINPATH)libstructures.dylib
else
	LIB_PATH += -Lbin/linux
	LIB_PATH += -L../mathlib/bin/linux

	BINPATH=bin/linux/
	LINK_CMD=ar -cvq $(BINPATH)libstructures.a
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libstructures.a test

$(BINPATH):
	mkdir -p $(BINPATH)

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)kdtree.o: $(SRCPATH)kdtree.cpp
	$(CC) $(CFLAGS) -c src/kdtree.cpp -o $(BINPATH)kdtree.o


$(BINPATH)libstructures.a: $(OBJS_BIN)
	$(LINK_CMD) $(OBJS_BIN)
#	ar -cvq $(BINPATH)libstructures.a $(OBJS_BIN)
#	g++ -dynamiclib -o $(BINPATH)libstructures.dylib $(OBJS_BIN)

test: test.cpp $(BINPATH)libstructures.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) $(LDFLAGS) -lstructures -lmath

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libstructures.a
	rm -f test
