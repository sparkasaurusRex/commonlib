CC=g++
LIB_PATH=-Lbin/osx
CFLAGS=-I./include -I../game/include -Ofast -I/usr/local/include
LDFLAGS=$(shell pkg-config --libs sdl2)
SRCPATH=src/
SRCS=src/math_utility.cpp src/perlin.cpp src/matrix.cpp src/aabb.cpp src/quaternion.cpp src/voronoi.cpp src/triangulation.cpp src/shapes.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	#LIB_PATH += -L../../common/mathlib/bin/osx
	LIB_PATH += -L../game/bin/osx
	#LIB_PATH += -L/usr/local/Cellar/cgal/4.6.1/lib/
	#LIB_PATH += -L/usr/local/Cellar/SDL2/lib/

	#CFLAGS += -I/usr/local/Cellar/sdl/2.0.3/

	BINPATH=bin/osx/

	#LDFLAGS+=-framework SDL2
	#LDFLAGS+=-framework SDL2_image
	LDFLAGS+=-framework OpenGL
	#LDFLAGS+=-lCGAL -lCGAL_Core
else
	#generic link flags
	#LIB_PATH += -L../../common/mathlib/bin/linux
	BINPATH=bin/linux/

	#LDFLAGS+=
	LDFLAGS+=-lGL
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)

all: $(OBJS_BIN) $(BINPATH)libmath.a test

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

#$(BINPATH)perlin.o: $(SRCPATH)/perlin.cpp
#	$(CC) $(CFLAGS) -c $(SRCPATH)/perlin.cpp -o $(BINPATH)perlin.o

$(BINPATH)libmath.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libmath.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libmath.a
	$(CC) $(CFLAGS) $(LIB_PATH) $(LDFLAGS) test.cpp -lmath -lgame -o test

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libmath.a
	rm -f test
