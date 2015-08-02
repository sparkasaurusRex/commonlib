CC=g++
LIB_PATH=
CFLAGS=-I./include -I../game/include -I/usr/local/include
CFLAGS+=-D__USE_BOOST__ #use boost library for voronoi triangulation
#CFLAGS+=-Ofast
LDFLAGS=$(shell pkg-config --libs sdl2)
SRCPATH=src/
SRCS=src/math_utility.cpp src/perlin.cpp src/matrix.cpp src/aabb.cpp src/quaternion.cpp src/voronoi.cpp src/triangulation.cpp src/shapes.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../game/bin/osx
	BINPATH=bin/osx/

	LDFLAGS+=-framework OpenGL
	#LDFLAGS+=-lCGAL -lCGAL_Core
else
	#generic link flags
	LIB_PATH+=-Lbin/linux
	LIB_PATH+=-L../game/bin/linux
	BINPATH=bin/linux/

	#LDFLAGS+=
	LDFLAGS+=-lGL
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))

all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libmath.a test

$(BINPATH):
	mkdir -p $(BINPATH)

$(BINPATH)math_utility.o: $(SRCPATH)math_utility.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)math_utility.cpp -o $(BINPATH)math_utility.o

$(BINPATH)perlin.o: $(SRCPATH)perlin.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)perlin.cpp -o $(BINPATH)perlin.o

$(BINPATH)matrix.o: $(SRCPATH)matrix.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)matrix.cpp -o $(BINPATH)matrix.o

$(BINPATH)aabb.o: $(SRCPATH)aabb.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)aabb.cpp -o $(BINPATH)aabb.o

$(BINPATH)quaternion.o: $(SRCPATH)quaternion.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)quaternion.cpp -o $(BINPATH)quaternion.o

$(BINPATH)voronoi.o: $(SRCPATH)voronoi.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)voronoi.cpp -o $(BINPATH)voronoi.o

$(BINPATH)triangulation.o: $(SRCPATH)triangulation.cpp
	$(CC) $(CFLAGS) -c $(SRCPATH)triangulation.cpp -o $(BINPATH)triangulation.o

$(BINPATH)shapes.o: $(SRCPATH)shapes.cpp
		$(CC) $(CFLAGS) -c $(SRCPATH)shapes.cpp -o $(BINPATH)shapes.o

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)libmath.a: $(OBJS_BIN) $(BINPATH)
	ar -cvq $(BINPATH)libmath.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libmath.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) -lmath -lgame $(LDFLAGS)

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libmath.a
	rm -f test
