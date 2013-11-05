// mb.cpp : motion blur with 11 pixels to lower-left
// Tested Environment: Darwin OSX X86_64 with g++ 4.2.1
//
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

#define HEIGHT  256
#define WIDTH   256	

uchar_t	inputimage[HEIGHT][WIDTH];
uchar_t outputimage[HEIGHT][WIDTH];

void
ProcessImage();

void
ReadImage(char*, uchar_t[HEIGHT][WIDTH]);

void
WriteImage(char*, uchar_t[HEIGHT][WIDTH]);

/* given pixel postion x,y and an image, returns its new gray level  */
uchar_t
mblur(int, int, uchar_t[HEIGHT][WIDTH]);

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
	WriteImage((char*)"mb_out.raw", outputimage );
		
	return 0;
}


/* image */

void
ProcessImage()
{
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++)
      outputimage[i][j] = mblur(j, i, inputimage);
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
		error((char*)"ERROR: Could not create image \"%s\".\n", pszPath);

	if( fwrite( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW) <
		( HEIGHT*WIDTH) )
	{
		error((char*)"ERROR: Could not write image \"%s\".\n", pszPath);
	}

	fclose(pRAW);
}

/* error */
void 
error(char *psz, ...)
{
	va_list ap;
	va_start( ap, psz );
	vfprintf( stderr, psz, ap );
	va_end(ap);
	exit(-1);
}

/* given pixel postion x,y and an image, returns its new gray level  */
uchar_t
mblur(int x, int y, uchar_t img[HEIGHT][WIDTH])
{
  //make a buffer to hold 11 pixels to lower-left and the original pixel gray levels
  int buffer[12];

  // init buffer to be -1s
  for (int i=0; i<11; i++)
    buffer[i] = -1;

  // read buffer from img
  for (int i=0; i<12; i++) {
    if (y+i < HEIGHT || x-i > 0)
      buffer[i-1] = img[y+i][x-i];
  }

  // calculate average
  int sum = 0;
  int count = 0;
  for (int i=0; i<12; i++){
    if (buffer[i] != -1) { /* apply sub-filtering */
      sum += buffer[i];
      count++;}}

  return sum/count;
}
