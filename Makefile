CC = gcc
CFLAGS = -lm
RM = rm -f

default: 	
	$(CC) $(CFLAGS) -o ComplexNumbers main.c gnuplot_i.o

clean: 
	$(RM) ComplexNumbers
