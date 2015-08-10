CC=g++
LIB_PATH=
CFLAGS=-I./include -I../mathlib/include  #-Ofast
SRCPATH=src/
SRCS=src/kdtree.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../mathlib/bin/osx

	BINPATH=bin/osx/
else
	LIB_PATH += -Lbin/linux
	LIB_PATH += -L../mathlib/bin/linux

	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(OBJS_BIN) $(BINPATH)libstructures.a test

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)kdtree.o: $(SRCPATH)kdtree.cpp
	$(CC) $(CFLAGS) -c src/kdtree.cpp -o $(BINPATH)kdtree.o


$(BINPATH)libstructures.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libstructures.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libstructures.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) $(LDFLAGS) -lstructures -lmath -lgraphics

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libstructures.a
	rm -f test
