
#include <png.h>
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <complex>
#include <omp.h>

#include "bitmap.cpp"

using namespace std;

int writeBitmap(const char* name, unsigned char* buff, unsigned width, unsigned height, bool flip); 

unsigned char r( unsigned long n ){return n>>16&255;}
unsigned char g( unsigned long n ){return n>>8&255;}
unsigned char b( unsigned long n ){return n>>0&255;}

void assignPixelVal(int cx, int cy, int iwidth, int rgb, unsigned char * arr);

// canvas size: iwdith * iheight
// set xL = -2, xR = 2, yB = -2, yT = 2
void mBrot(int iwidth, int iheight, float xL, float xR, float yB, float yT)
{
  int asize = iwidth*iheight*3;
  unsigned char *arr = new unsigned char[asize];
  
  float epsilon = 0.0005; // The step size across the X and Y axis
  float x;
  float y;

  int maxIterations = 25; // increasing this will give you a more detailed fractal
  int maxColours    = 256;   // Change as appropriate for your display.

  complex<float> Z(0,0);
  complex<float> C(0,0);

  int iterations;

  float xLength = fabs(xL) + fabs(xR);
  float yLength = fabs(yB) + fabs(yT);

  int xrange = (int)((fabs(xL) + fabs(xR))/epsilon); 
  int yrange = (int)((fabs(yB) + fabs(yT))/epsilon);

  cout << xrange << " " << yrange << endl;

  int modpercent = xrange/10; //= (10%)
  int inc = 0;

  for(int i=0; i<=xrange; i++) //should loop over pixels here
  {
    x = epsilon*i-fabs(xL);
  /* #pragma omp parallel for schedule(dynamic) */
    for(int j=0; j<=yrange; j++)
    {
      y = epsilon*j-fabs(yB);
      iterations = 0;
      complex<float> C(x,y);
      complex<float> Z(0,0);

      while(abs(Z) < 2. && iterations < maxIterations)
      {
        Z = Z*Z + C;
        iterations++;
      }
      /* Screen.Plot(x,y, maxColors % iterations); // depending on the number of iterations, color a pixel. */
      int xCell = (iwidth+1)*(x+fabs(xL))/xLength;
      int yCell = (iheight+1)*(y+fabs(yB))/yLength;
      /* if(yCell < 1600) */
      /*   cout << yCell << endl; */
      /* if(iterations != 1) */
      assignPixelVal(xCell, yCell, iwidth, iterations, arr);
    }

    if(i % modpercent == 1)
    {
      cout << inc*10 << "%" << endl;
      inc++;
    }

  }
  writeBitmap("./out/man.png", arr, iwidth, iheight, 1);
  delete[] arr;
}

int colours[25][3] =
{
  {255,  0,  0},
  {255, 64,  0},
  {255,128,  0},
  {255,191,  0},
  {255,255,  0},
  {191,255,  0},
  {128,255,  0},
  { 64,255,  0},
  {  0,255,  0},
  {  0,255, 64},
  {  0,255,128},
  {  0,255,191},
  {  0,255,255},
  {  0,191,255},
  {  0,128,255},
  {  0, 64,255},
  {  0,  0,255},
  { 64,  0,255},
  {128,  0,255},
  {191,  0,255},
  {255,  0,255},
  {255,  0,191},
  {255,  0,128},
  {255,  0, 64},
  {255,  0,  0}
};

void assignPixelVal(int cx, int cy, int iwidth, int colour, unsigned char * arr)
{
  int index = iwidth*cx + cy; 

  int *rgb = colours[colour];
  /* cout << cx << " " << cy << endl; */
  /* cout << "rgb val " << rgb << endl; */
  /* if(rgb != 0 && rgb != 1 ) */
  /*   cout << rgb << endl; */
  *(arr + index*3 + 0) = rgb[0];
  *(arr + index*3 + 1) = rgb[1];
  *(arr + index*3 + 2) = rgb[2];
}

void createPixelArray(int iwidth, int iheight)
{
  unsigned int *A = new unsigned int(iwidth*iheight);
  /* unsigned int rgb = (128 << 16 | 128 << 8 | 128); */
  int asize = iwidth*iheight*3;
  unsigned char parr[asize];
  unsigned long n = 0x557e89f3;

  for(int i = 0; i < iwidth*iheight/2.; i++)
  {
    int offset = i*3;
    *(parr + offset + 0) = 255-r(i);
    *(parr + offset + 1) = 255-g(i);
    *(parr + offset + 2) = 255-b(i);
  }

  for(int i = iwidth*iheight/2.; i < iwidth*iheight; i++)
  {
    int offset = i*3;
    *(parr + offset + 0) = r(i);
    *(parr + offset + 1) = g(i);
    *(parr + offset + 2) = b(i);
  }

  writeBitmap("test.png", parr, iwidth, iheight, 0);
}

void testWrite()
{
  // test the writeBitmap function
  const char* name = "test";
  unsigned int test = 10;
  printf("sizeof unsigned int = %lu\n", sizeof(test)); // 4 bytes
}


main()
{
  // pick window of interest
  // parallelisation (openMP)
  mBrot(1000, 1000, -2, 0, -2, 2);
  /* createPixelArray(100, 200); */
  // read in iterations from file
  // arbitrary shapes & colour
  //
  /* int* arr = colours[0]; */
  /* cout << arr[0] << " " << arr[1] << " " << arr[2] << endl; */

  /* cout << colours[0][] << endl; */
}

