CC=g++
LIB_PATH=
CFLAGS=-I./include -I../game/include -I../graphics/include -I../mathlib/include
CFLAGS+=-I/usr/local/include
CFLAGS+=$(shell pkg-config freetype2 --cflags)
CFLAGS+=$(shell pkg-config glew --cflags)
CFLAGS+=$(shell pkg-config sdl2 --cflags)
CFLAGS+=$(shell pkg-config SDL2_image --cflags)
#CFLAGS+=-Ofast
CFLAGS+= -g3 -O0

LDFLAGS=$(shell pkg-config freetype2 --libs)
LDFLAGS+=$(shell pkg-config glew --libs)
LDFLAGS+=$(shell pkg-config sdl2 --libs)
LDFLAGS+=$(shell pkg-config SDL2_image --libs)
LDFLAGS+=-lmath -lgame -lgraphics
SRCPATH=src/
SRCS=src/label.cpp src/widget.cpp src/push_button.cpp src/widget_wrangler.cpp
SRCS+=src/toolbox.cpp src/check_button.cpp src/rectangular_widget.cpp
SRCS+=src/radial_widget.cpp src/radial_push_button.cpp src/menu.cpp
SRCS+=src/curve_editor.cpp src/text_edit.cpp
OBJS=$(SRCS:.cpp=.o)

UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	#osx
	#CFLAGS+=-I/opt/X11/include/freetype2/
	CFLAGS+=-Wno-deprecated-declarations
	#CFLAGS+=-I$(shell xcrun --show-sdk-path)/System/Library/Frameworks/

	LIB_PATH+=-Lbin/osx
	LIB_PATH+=-L../mathlib/bin/osx
	LIB_PATH+=-L../graphics/bin/osx
	LIB_PATH+=-L../game/bin/osx

	#LDFLAGS+=-framework SDL2
	#LDFLAGS+=-framework SDL2_image
	LDFLAGS+=-framework OpenGL

	BINPATH=bin/osx/
else
	#generic link flags
	LIB_PATH += -Lbin/linux
	LIB_PATH += -L../mathlib/bin/linux
	LIB_PATH += -L../fluids/bin/linux

	BINPATH=bin/linux/
	#CFLAGS+=-I/usr/include/freetype2/
	CFLAGS+=$(shell pkg-config --cflags sdl2)

#	LDFLAGS+=$(shell pkg-config --libs sdl2)
	#LDFLAGS+=$(shell pkg-config --libs SDL2_image)
	LDFLAGS+=-lGL
endif

OBJS_BIN=$(addprefix $(BINPATH), $(notdir $(OBJS)))
#echo $(OBJS_BIN)


all: $(BINPATH) $(OBJS_BIN) $(BINPATH)libui.a test

$(BINPATH):
	mkdir -p $(BINPATH)

#$(OBJS): $(SRCS)
#	$(CC) $(CFLAGS) -c $< -o $@
#	mv $@ $(BINPATH)

$(BINPATH)widget.o: $(SRCPATH)widget.cpp
	$(CC) $(CFLAGS) -c src/widget.cpp -o $(BINPATH)widget.o

$(BINPATH)rectangular_widget.o: $(SRCPATH)rectangular_widget.cpp
	$(CC) $(CFLAGS) -c src/rectangular_widget.cpp -o $(BINPATH)rectangular_widget.o

$(BINPATH)radial_widget.o: $(SRCPATH)radial_widget.cpp
	$(CC) $(CFLAGS) -c src/radial_widget.cpp -o $(BINPATH)radial_widget.o

$(BINPATH)widget_wrangler.o: $(SRCPATH)widget_wrangler.cpp
		$(CC) $(CFLAGS) -c src/widget_wrangler.cpp -o $(BINPATH)widget_wrangler.o

$(BINPATH)label.o: $(SRCPATH)label.cpp
	$(CC) $(CFLAGS) -c src/label.cpp -o $(BINPATH)label.o

$(BINPATH)push_button.o: $(SRCPATH)push_button.cpp
	$(CC) $(CFLAGS) -c src/push_button.cpp -o $(BINPATH)push_button.o

$(BINPATH)radial_push_button.o: $(SRCPATH)radial_push_button.cpp
	$(CC) $(CFLAGS) -c src/radial_push_button.cpp -o $(BINPATH)radial_push_button.o

$(BINPATH)check_button.o: $(SRCPATH)check_button.cpp
		$(CC) $(CFLAGS) -c src/check_button.cpp -o $(BINPATH)check_button.o

$(BINPATH)toolbox.o: $(SRCPATH)toolbox.cpp
	$(CC) $(CFLAGS) -c src/toolbox.cpp -o $(BINPATH)toolbox.o

$(BINPATH)menu.o: $(SRCPATH)menu.cpp
	$(CC) $(CFLAGS) -c src/menu.cpp -o $(BINPATH)menu.o

$(BINPATH)curve_editor.o: $(SRCPATH)curve_editor.cpp
	$(CC) $(CFLAGS) -c src/curve_editor.cpp -o $(BINPATH)curve_editor.o

$(BINPATH)text_edit.o: $(SRCPATH)text_edit.cpp
	$(CC) $(CFLAGS) -c src/text_edit.cpp -o $(BINPATH)text_edit.o

$(BINPATH)libui.a: $(OBJS_BIN)
	ar -cvq $(BINPATH)libui.a $(OBJS_BIN)

test: test.cpp $(BINPATH)libui.a
	$(CC) $(CFLAGS) test.cpp -o test $(LIB_PATH) -lui $(LDFLAGS)

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libui.a
	rm -f test
