CC=g++
LIB_PATH=
CFLAGS=-I./include -Ofast
LDFLAGS=
SRCPATH=src/
SRCS=src/math_utility.cpp src/perlin.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	LIB_PATH += -L../../common/mathlib/bin/osx
	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH += -L../../common/mathlib/bin/linux
	BINPATH=bin/linux/
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(OBJS_BIN) $(BINPATH)libmath.a test

$(BINPATH)math_utility.o: $(SRCPATH)/math_utility.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)math_utility.cpp -o $(BINPATH)math_utility.o

$(BINPATH)perlin.o: $(SRCPATH)/perlin.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)perlin.cpp -o $(BINPATH)perlin.o

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

#$(BINPATH)perlin.o: $(SRCPATH)/perlin.cpp
#	$(CC) $(CFLAGS) -c $(SRCPATH)/perlin.cpp -o $(BINPATH)perlin.o

$(BINPATH)libmath.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libmath.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libmath.a
	$(CC) $(CFLAGS) $(LIB_PATH) $(LDFLAGS) test.cpp -lmath -o test

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libmath.a
	rm -f test
