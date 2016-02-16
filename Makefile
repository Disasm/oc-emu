SRCS = main.c font.c uuid.c lfs.c dev_gpu.c api_component.c api_filesystem.c api_computer.c

all: oc-emu

oc-emu: $(SRCS) machine.h init.h
	gcc -g -ggdb -O2 -Wall -o oc-emu $(SRCS) -lSDL2 -I/usr/include/SDL2 -llua

%.h: %.bin
	xxd -i $< > $@

%.bin: %.lua
	luac -o $@ $<

clean:
	rm -f oc-emu machine.bin machine.h init.bin init.h

