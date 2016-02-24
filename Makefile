# $@ $^ - left and right side of the :
# $< - first item in the dependencies list
# %.o - rule that applies to all files ending with the .o suffix

INC=-Id:/soft/sfml/CSFML-2.1/include
LIB=-Ld:/soft/sfml/CSFML-2.1/lib/gcc -lcsfml-graphics -lcsfml-system -lcsfml-window
CC=gcc
CFLAGS=-std=c99 $(INC) -Wall -Wextra -mwindows
HEADERS=game.h grid.h resourecs_holder.h common.h icon.h
OBJS=entrypoint.o game.o grid.o resources_holder.o common.o

main.exe : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -rf *.o
