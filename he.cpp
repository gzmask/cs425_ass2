// he.cpp : histogram equlization
// Tested Environment: Darwin OSX X86_64 with g++ 4.2.1
// NAME:	  Shulang Lei
// SID:		  200253624
// DATE:	  October 14th, 2013

#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <math.h>
#include <stdio.h>

/* image */

typedef unsigned char uchar_t;

#define HEIGHT	256
#define WIDTH	256
#define DEPTH	256

uchar_t	inputimage[HEIGHT][WIDTH];
uchar_t outputimage[HEIGHT][WIDTH];
uchar_t coloroutputimage[3][HEIGHT][WIDTH];
int histogram[DEPTH];
int cumulative_histogram[DEPTH];

void
ProcessImage();

void
ReadImage(char*, uchar_t[HEIGHT][WIDTH]);

void
WriteImage(char*, uchar_t[HEIGHT][WIDTH]);

/* error */
void
error(char*, ...);

/* main function */

int main(int argc, char *argv[])
{
	/* initialization */

	memset( inputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );
	memset( outputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );


	/* command line arguments */
	if(argc != 2) {
		error((char *)"USAGE: %s <image.raw>\n",  argv[0]);
	}


	/* read image */
	ReadImage( argv[1], inputimage );


	/* process image */
	ProcessImage();


	/* write image */
	WriteImage((char *)"he_output.raw", outputimage );
		
	return 0;
}


/* image */

void
ProcessImage()
{
  //initial histograms to all 0s
  for (int i=0; i< DEPTH; i++) {
      histogram[i]=0;
      cumulative_histogram[i]=0;}


  //calculate histogram
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++) {
      histogram[inputimage[i][j]]++;}

  //calculate cumulative histogram
  int sum = 0;
  for (int i=0; i< DEPTH; i++) {
    sum += histogram[i];
    cumulative_histogram[i] = sum;}


  //calculate proper cumulative histogram
  double scalar = (DEPTH-1)/((double)HEIGHT*WIDTH);
  for (int i=0; i< DEPTH; i++) {
    cumulative_histogram[i] = cumulative_histogram[i] * scalar;}

  //set background to be black
  cumulative_histogram[0] = 0;

  //calculate resulting image
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++) 
      outputimage[i][j] = cumulative_histogram[inputimage[i][j]];
}

void
ReadImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "rb" ) ) == NULL )
		error((char *)"ERROR: Could not open image \"%s\".\n", pszPath);

	if( fread( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW ) <
		( HEIGHT*WIDTH ) )
	{
		error((char *)"ERROR: Could not read image \"%s\".\n", pszPath);
	}

	fclose(pRAW);
}

void
WriteImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error((char *)"ERROR: Could not create image \"%s\".\n", pszPath);

	if( fwrite( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW) <
		( HEIGHT*WIDTH) )
	{
		error((char *)"ERROR: Could not write image \"%s\".\n", pszPath);
	}

	fclose(pRAW);

}

/* error */
void error(char *psz, ...)
{
	va_list ap;
	
	va_start( ap, psz );
	vfprintf( stderr, psz, ap );
	va_end(ap);

	exit(-1);
}
