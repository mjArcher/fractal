#include <ctime>
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
//
void mBrot(int ires, int jres, float xL, float xR, float yB, float yT, int threads)
{
  int asize = ires*jres*3;
  unsigned char *arr = new unsigned char[asize];

  int maxIterations = 25, iterations;
  /* int maxColours    = 256; */   

  float xLength = fabs(xL - xR);
  float yLength = fabs(yB - yT);

  int modpercent = ires/10; 
  int inc = 0;

  float xepsilon = xLength/ires;
  float yepsilon = yLength/jres;

  float x, y;
  int i;
  complex<float> Z(0,0);
  complex<float> temp(0,0);

  cout << "Number of threads " << omp_get_num_threads() << endl;

#pragma omp parallel for private(i,x) num_threads(4)
  for(i=0; i<ires; i++) //should loop over pixels here
  {
    x = (float)xepsilon*i-fabs(xL);
    int j;
#pragma omp parallel for private(j,y,Z,iterations) num_threads(4)
    for(j=0; j<jres; j++)
    {
      y = (float)yepsilon*j-fabs(yB);
      iterations = 0;
      complex<float> C(x,y);
      Z = temp;

      while(abs(Z) < 2. && iterations < maxIterations)
      {
        Z = Z*Z + C;
        iterations++;
        /* cout << iterations << " " << omp_get_thread_num() << endl; */
      }
      /* Screen.Plot(x,y, maxColors % iterations); // depending on the number of iterations, color a pixel. */
      //convert into cell
      float xratio = (x+fabs(xL))/xLength;
      float yratio = (y+fabs(yB))/yLength;

      int xCell = round(xratio * ires);
      int yCell = round(yratio * jres);

      assignPixelVal(xCell, yCell, ires, iterations, arr);
    }

    if(i % modpercent == 1)
    {
      cout << inc*10 << "%" << endl;
      inc++;
    }
  }
  cout << "output to file" << endl;
  writeBitmap("./out/man.png", arr, ires, jres, 1);
  delete[] arr;
}

void generateColours(int number)
{
  int rowCount = number;
  int colCount = 3;
  int** ary = new int*[rowCount];
  for(int i = 0; i < rowCount; ++i)
    ary[i] = new int[colCount];

   
  for(int i = 0; i < rowCount; ++i)
  {
    ary[i][0] = 255; 
  }   

  for(int i = 0; i < rowCount; ++i)
  {
    cout << ary[i][1] << endl;
  }   

  delete[] ary;
}

int colours[25][3] =
{
  {255,  0,  0}, //
  {255, 64,  0},
  {255,128,  0},
  {255,191,  0},
  {255,255,  0}, //
  {191,255,  0},
  {128,255,  0},
  { 64,255,  0},
  {  0,255,  0}, //
  {  0,255, 64},
  {  0,255,128},
  {  0,255,191},
  {  0,255,255}, //
  {  0,191,255},
  {  0,128,255},
  {  0, 64,255},
  {  0,  0,255}, //
  { 64,  0,255},
  {128,  0,255},
  {191,  0,255},
  {255,  0,255}, //
  {255,  0,191},
  {255,  0,128},
  {255,  0, 64},
  {255,  0,  0}  //
};

  // switches 6 times
  // +1
  // -0
  // +2
  // -1
  // +0
  // -2

void assignPixelVal(int cx, int cy, int ires, int colour, unsigned char * arr)
{
  int index = ires*cy + cx; 
  int *rgb = colours[colour];
  *(arr + index*3 + 0) = rgb[0];
  *(arr + index*3 + 1) = rgb[1];
  *(arr + index*3 + 2) = rgb[2];
}

int main(int argc, char* argv[])
{
  clock_t begin = clock();

  cout << "Threads " << argv[1] << endl;

  omp_set_dynamic(0);  
  omp_set_num_threads(atoi(argv[1]));
  mBrot(20000, 20000, -0.25, 0, -1, -0.75, atoi(argv[1]));
  
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Time " << elapsed_secs << endl;

  /* generateColours(25); */
}

