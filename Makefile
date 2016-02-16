SRCS = main.c font.c uuid.c lfs.c gpu.c

all: oc-emu

oc-emu: $(SRCS) font_8x16.h
	gcc -g -ggdb -O2 -Wall -o oc-emu $(SRCS) -lSDL2 -I/usr/include/SDL2 -llua

font_8x16.h: fonts/koi8r-8x16
	cat $< | xxd -i > $@

clean:
	rm -f oc-emu

