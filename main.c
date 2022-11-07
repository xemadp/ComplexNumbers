#include <stdio.h>
#include "ComplexNumbers.c"
int main(int argc, char* argv[])
	{
  char cmd;
	printf("commands: \n");
	printf("h - help\n");
	printf("a - add complex number to data.dat\n");
	printf("l - show the content of data.dat\n");
	printf("p - plot numbers in data.dat\n");
	printf("c - clear data.dat\n");
	printf("x - exit\n");
   
	// Main Interactive Loop
	while(1)
	{
			scanf("%c", &cmd);
	  	switch(cmd)
	  	{
	  		case 'a':
	  		compx w;
	  		printf("real part: ");
	  		scanf("%lf", &w.real);
	  		printf("imaginary part: ");
	  		scanf("%lf", &w.imag);
	  		addcpx2dat(w);
	  		break;
	  		
	  		case 'p':
	  		plotdat();
	  		break;
	  		
	  		case 'c':
	  		cleardat();
				printf("data.dat has been cleared !\n");
	  		break;

	  		case 'l':
	  		readfilecont("data.dat");
	  		break;

	  		case 'h':
	  		printf("commands: \n");
	  		printf("h - print tnis help\n");
	  		printf("a - add complex number to data.dat\n");
	  		printf("l - show the content of data.dat\n");
	  		printf("p - plot numbers in data.dat\n");
	  		printf("c - clear data.dat\n");
	  		printf("x - exit\n");
	  		break;

				case 'x':
					return 0;
				break;

	  		default:
	  		break;
	  		}

	}

		return 0;
	}
