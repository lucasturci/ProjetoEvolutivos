SRC = $(wildcard *.cpp)
OBJECTS = $(SRC:.cpp=.o)
PROGNAME = main
LINKS = `pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5 --libs --cflags`

all: $(OBJECTS)
	g++ -o $(PROGNAME) $(OBJECTS) $(LINKS) 
run:
	@./$(PROGNAME)	
%.o: %.c
	@g++ -o $@ -c $<