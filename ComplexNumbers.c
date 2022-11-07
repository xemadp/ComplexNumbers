#include "gnuplot_i.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <math.h> 

#define PI 3.141592653
#define SLEEP_LN 5

// Global Color Index
int colindex=0;

typedef struct {
	double real; 
	double imag;
}	compx;

const char *COLORS[10] = {
											"0x770000",
		 									"0x007700",
		 									"0x000077",
		 									"0x770077",
		 									"0x007777",
		 									"0x777700",
		 									"0x707070",
		 									"0x555555",
		 									"0x5a005a",
		 									"0x000000"
};

// compute and return the conjugate of the complex number w
compx conjugate(compx w)
{
	w.imag = -1 * w.imag;

	return w;
}

// get the argument of a complex number w 
double argument(compx w) 
{
	return atan2(w.imag, w.real);
}

// Absolute value of a complex number
double abval(compx w) 
{
	return sqrt((w.real*w.real) + (w.imag*w.imag));
}	

// convert radian to degree
double radtodeg(double rad) 
{	
	return (rad*360)/(2*PI);
}

// return the real number r as the compx datatype
compx r2cpx(double r)
{
	compx w;
	w.real = r;
	w.imag = 0;
	return w;
}

/* ------ Complex Arithmetic ------ */
// Addition of two complex numbers
compx cadd(compx w, compx z)
{
	// (a+bi)+(c+di) = (a + c) + (d + b)i
	compx wz;
	wz.real = (w.real+z.real);
	wz.imag = (w.imag+z.imag);
	return wz;
}

// Subtraction of two complex numbers w-z
compx csub(compx w, compx z)
{
	// (a+bi)-(c+di) = (a - c) + (d - b)i
	z.real *= -1;
	z.imag *= -1;
	return cadd(w,z);
}

// Multiplication of two complex numbers
compx cmult(compx w, compx z)
{
	// (a+bi)(c+di) = ac + adi + bci -bd = (ac - bd) + (ad + bc)i
	compx wz;
	wz.real = (w.real*z.real) - (w.imag*z.imag);
	wz.imag = (w.real*z.imag) + (z.real*w.imag);
	return wz;
}
// Division of two complex numbers
compx cdiv(compx w, compx z)
{
	// (a+bi)/(c+di) = (a+bi)(c-di)/(c^2 + d^2) = ac - adi + bci + bd = (ac + bd)/|z|^2 + (- ad + bc)i/|z|^2
	double abs=abval(z);
	abs*=abs; // abs = |z|^2
	compx wz = cmult(w,conjugate(z)); // compute the numerator (a+bi)(c-di)
	wz.real = wz.real/abs; // divide real part by the denominator (c^2 + d^2)
	wz.imag = wz.imag/abs; // divide imaginary part by the denominator (c^2 + d^2)
	return wz;
}


/* ------ GNUPLOT HELPER FUNCTIONS ------ */


/* Create a file named data.dat with the points representing complex numbers in C[] as vectors
   Example
   C = {1+2i, 3-4i}
   data.dat :
   0 0 1 2
   0 0 3 -4 */

void makecpxdat(compx C[], int len)
{
	FILE *data;
	char re[100];
	char im[100];

	data = fopen("data.dat", "w");
	
	// Exit if can't create data.dat
	if(data == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
	
	for(int i=0; i<len; i++)
	{
		snprintf(re,100,"%f",C[i].real);
		snprintf(im,100,"%f",C[i].imag);
		strcat(re," ");
		strcat(re,im);
		fputs("0 0 ",data);
		fputs(re,data);
		fputs(" ",data);
		fputs(COLORS[i],data);
		fputs("\n",data);
	}

	fclose(data);
}

/*	Add a given compx c to the end of data.dat 
 		Example
		data.dat :
		0 0 1 1

		c = 1+2i
		addcpx2dat(c)
		data.dat :
		0 0 1 1 
		0 0 1 2
		
		also creates data.dat utilizing makecpxdat function if data.dat doesn't exist
 */
void addcpx2dat(compx c)
{
	FILE *data;
	char re[100];
	char im[100];

	data = fopen("data.dat", "a");
	
	// Exit if can't create data.dat
	if(data == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
	
	snprintf(im,100,"%lf",c.imag);
	snprintf(re,100,"%lf",c.real);
	strcat(re," ");
	strcat(re,im);
	fputs("0 0 ",data);
	fputs(re,data);
	fputs(" ",data);
	fputs(COLORS[colindex++],data);
	fputs("\n",data);
	

	fclose(data);
}

// clear data.dat

void cleardat()
{
	FILE *data;
	char re[100];
	char im[100];

	data = fopen("data.dat", "w");
	
	// Exit if can't create data.dat
	if(data == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
	
	fclose(data);
	colindex=0;
}

// Read File Contents
void readfilecont(char filename[])
{
 
	FILE *fptr;
  
    char c;
  
    // Open file
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open %s \n", filename);
        exit(0);
    }
  
    // Read contents from file
    c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }
  
    fclose(fptr);
}


// Plot a given complex number
void plotcpx1( compx w)
{
	/* setting up gnuplot */
	gnuplot_ctrl * h;
	h = gnuplot_init();
	gnuplot_cmd(h,"set term wxt persist");
	gnuplot_cmd(h,"set xzeroaxis");
	gnuplot_cmd(h,"set xtics axis");
	gnuplot_cmd(h,"set xrange [-8:8]");
	gnuplot_cmd(h,"set yzeroaxis");
	gnuplot_cmd(h,"set ytics axis");
	gnuplot_cmd(h,"set yrange [-8:8]");
	gnuplot_cmd(h,"set border 0");
	gnuplot_cmd(h,"unset key");
	gnuplot_cmd(h,"set key");
	/* gnuplot setup finished */

	addcpx2dat(w);
	gnuplot_cmd(h, "plot \"data.dat\" using 1:2:3:4:5 with vectors filled head lw 1 lc rgb variable");
	gnuplot_close(h);
}

// Plot an array of given complex numbers
void plotcpxs(compx C[], int len)
{
	/* setting up gnuplot */
	gnuplot_ctrl * h;
	h = gnuplot_init();
	gnuplot_cmd(h,"set term wxt persist");
	gnuplot_cmd(h,"set xzeroaxis");
	gnuplot_cmd(h,"set xtics axis");
	gnuplot_cmd(h,"set xrange [-20:20]");
	gnuplot_cmd(h,"set yzeroaxis");
	gnuplot_cmd(h,"set ytics axis");
	gnuplot_cmd(h,"set yrange [-20:20]");
	gnuplot_cmd(h,"set border 0");
	gnuplot_cmd(h,"unset key");
	gnuplot_cmd(h,"set key");
	/* gnuplot setup finished */

	makecpxdat(C,len);
	gnuplot_cmd(h, "plot \"data.dat\" using 1:2:3:4:5 with vectors filled head lw 1 lc rgb variable");
	gnuplot_close(h);
}

// Plot complex numbers in data.dat
void plotdat()
{
	/* setting up gnuplot */
	gnuplot_ctrl * h;
	h = gnuplot_init();
	gnuplot_cmd(h,"set term wxt persist");
	gnuplot_cmd(h,"set xzeroaxis");
	gnuplot_cmd(h,"set xtics axis");
	gnuplot_cmd(h,"set xrange [-20:20]");
	gnuplot_cmd(h,"set yzeroaxis");
	gnuplot_cmd(h,"set ytics axis");
	gnuplot_cmd(h,"set yrange [-20:20]");
	gnuplot_cmd(h,"set border 0");
	gnuplot_cmd(h,"unset key");
	gnuplot_cmd(h,"set key");
	/* gnuplot setup finished */

	gnuplot_cmd(h, "plot \"data.dat\" using 1:2:3:4:5 with vectors filled head lw 1 lc rgb variable");
	gnuplot_close(h);
}
