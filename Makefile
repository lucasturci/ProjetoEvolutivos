SRC = $(wildcard *.hpp) $(wildcard *.cpp)
OBJECTS = main.o
PROGNAME = main
LINKS = `pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 --libs --cflags`

all: $(OBJECTS)
	g++ -o $(PROGNAME) $(OBJECTS) $(LINKS) 
run:
	@./$(PROGNAME)	
main.o: $(SRC)
	g++ -o $@ -c main.cpp
clean:
	rm -rf $(PROGNAME) *.o
gdb: $(OBJECTS)
	g++ -o $(PROGNAME) $(OBJECTS) $(LINKS) -g
