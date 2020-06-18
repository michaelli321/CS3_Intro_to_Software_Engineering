PROGS = echo squares ip
LIB_MATH = -lm 
LIBS = $(LIB_MATH) -lSDL2 -lSDL2_gfx
CFLAGS = -Iinclude -Wall -g -fno-omit-frame-pointer -fsanitize=address

all: $(PROGS)

echo: echo.c network_util.c
	clang $^ $(LIBS) $(CFLAGS) -o $@ 

squares: squares.c network_util.c list.c vector.c sdl_wrapper.c polygon.c
	clang $^ $(LIBS) $(CFLAGS) -o $@ 

clean:
	rm -f $(PROGS)
