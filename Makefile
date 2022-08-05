CC=g++
SRC=main.cpp glad.c shader_handlers.c
CFLAFS=-Wall -Werror -std=gnu++14
LDFLAGS=-lglfw -ldl
GARBAGE=a.out julia mandelbrot

julia: $(SRC)
	$(CC) $(SRC) $(CFLAFS) $(LDFLAGS) -o julia && ./julia julia.frag

mandelbrot: $(SRC)
	$(CC) $(SRC) $(CFLAFS) $(LDFLAGS) -o mandelbrot && ./mandelbrot mandelbrot.frag

clear:
	-rm $(GARBAGE) *.o 2>/dev/null


