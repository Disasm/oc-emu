SRCS = main.c font.c uuid.c lfs.c dev_gpu.c api_component.c api_filesystem.c api_computer.c

all: oc-emu

oc-emu: $(SRCS)
	gcc -g -ggdb -O2 -Wall -o oc-emu $(SRCS) -lSDL2 -I/usr/include/SDL2 -llua
clean:
	rm -f oc-emu

