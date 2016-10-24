
#include <png.h>
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <complex>
#include <omp.h>

using namespace std;

int writeBitmap(const char* name, unsigned char* buff, unsigned width, unsigned height, bool flip); 

/* int shiftTest() */
/* { */
/*   cout << "shift test " << endl; */
/*   unsigned char x = 4; */
/*   unsigned int y = 8; */
/*   cout << sizeof(x) << endl; */
/*   cout << sizeof(y) << endl; */
/*   y = y << 1; */
/*   cout << y <<endl; */
/*   printf("x = %hhx\t sizeof = %lu\n",x,sizeof(x)); */
/*   // do a shift of 00001000 */
/*   x = x << 1lu; */
/*   x = x << 1lu; */
/*   printf("x = %hhx\t sizeof = %lu\n",x,sizeof(x)); */
/*   const char* name = "test"; */
/*   cout << "size of name " << sizeof(name) << endl; */
/*   cout << "size of name " << sizeof(*name) << endl; */
/*   printf("%c", *(name+2)); */
/*   printf("\n%s\n", name); */
/*   //garbage collector */
/* } */

unsigned char r( unsigned long n ){return n>>16&255;}
unsigned char g( unsigned long n ){return n>>8&255;}
unsigned char b( unsigned long n ){return n>>0&255;}

void assignPixelVal(int cx, int cy, int iwidth, int rgb, unsigned char * arr);

void complexTest()
{
  complex<float> com(1.0,2.0);
  cout << com << endl;
  cout << com*com << endl;
  cout << abs(com) << endl;
}


void manBrot(int iwidth, int iheight)
{
  int isize = iwidth*iheight*3;
  unsigned char *arr = new unsigned char[isize];
  
  float epsilon = 0.0005; // The step size across the X and Y axis
  float x;
  float y;

  int maxIterations = 10; // increasing this will give you a more detailed fractal
  int maxColors = 256; // Change as appropriate for your display.
  complex<float> Z(0,0);
  complex<float> C(0,0);
  int iterations;

  int xrange = (int)(4./epsilon); 
  int yrange = xrange;

  for(int i=0; i<=xrange; i++)
  {
    x = epsilon*i-2;
  /* #pragma omp parallel for schedule(dynamic) */
    for(int j=0; j<=yrange; j++)
    {
      y = epsilon*j-2;
      iterations = 0;
      complex<float> C(x,y);
      complex<float> Z(0,0);

      while(abs(Z) < 2. && iterations < maxIterations)
      {
        Z = Z*Z + C;
        iterations++;
      }
      /* Screen.Plot(x,y, maxColors % iterations); // depending on the number of iterations, color a pixel. */
      int xCell = iwidth*(x + 2)/4.;
      int yCell = iheight*(y + 2)/4.;
      /* if(iterations != 1) */
      /*   cout <<  iterations << endl; */
      assignPixelVal(xCell, yCell, iwidth, maxColors/iterations, arr);
      /* std::cout << x << std::endl; */
    }
    cout << x << endl;
  }
  writeBitmap("./out/man.png", arr, iwidth, iheight, 0);
  delete[] arr;
}

void assignPixelVal(int cx, int cy, int iwidth, int rgb, unsigned char * arr)
{
  int index = iwidth*cx + cy; 

  /* cout << cx << " " << cy << endl; */
  /* cout << "rgb val " << rgb << endl; */
  /* if(rgb != 0 && rgb != 1 ) */
  /*   cout << rgb << endl; */
  *(arr + index*3 + 0) = rgb;
  *(arr + index*3 + 1) = rgb;
  *(arr + index*3 + 2) = rgb;
}

void createPixelArray(int iwidth, int iheight)
{
  unsigned int *A = new unsigned int(iwidth*iheight);
  /* unsigned int rgb = (128 << 16 | 128 << 8 | 128); */
  int isize = iwidth*iheight*3;
  unsigned char parr[isize];
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

  /* int rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff); */
  /* int r,g,b; */
  /* int rgb = new Color(r, g, b).getRGB(); */
    /* printf("%d\n",r(testval)); */
    /* printf("%d\n",g(testval)); */
    /* printf("%d\n",b(testval)); */
    /* *(parr + offset + 0) = (i >> 16) & 0xFF; */
    /* *(parr + offset + 1) = (i >> 8) & 0xFF; */
    /* *(parr + offset + 2) = i & 0xFF; */
void testWrite()
{
  // test the writeBitmap function
  const char* name = "test";
  unsigned int test = 10;
  printf("sizeof unsigned int = %lu\n", sizeof(test)); // 4 bytes
}

int writeBitmap(const char* filename, unsigned char* buff, unsigned iwidth, unsigned iheight, bool flip = false) {

  FILE* fp = fopen(filename, "wb");
  if (!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png) abort();

  png_infop info = png_create_info_struct(png);

  if (!info) {
    png_destroy_write_struct(&png,
        (png_infopp)NULL);
    abort();
  }

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_set_IHDR(
      png,
      info,
      iwidth, iheight,
      8,
      PNG_COLOR_TYPE_RGB,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT
      );

  png_write_info(png, info);
  /* cout << "Here 1" << endl; */ 
  /* cout << "Here " << endl; */ 

  png_byte* image = (png_byte*)buff;

  unsigned k;
  png_bytep* row_pointers = new png_bytep[iheight];
  for (k = 0; k < iheight; k++) {
    row_pointers[k] = image + (flip ? (iheight - k - 1) : k) * iwidth * 3;
  }

  png_write_image(png, row_pointers);
  png_write_end(png, info);
  png_destroy_write_struct(&png, &info);
  delete[] row_pointers;
  fclose(fp);
  return 0;
}


/* public void MBrot() */
/* { */
/*   float epsilon = 0.0001; // The step size across the X and Y axis */
/*   float x; */
/*   float y; */
/*   int maxIterations = 10; // increasing this will give you a more detailed fractal */
/*   int maxColors = 256; // Change as appropriate for your display. */

/*   Complex Z; */
/*   Complex C; */
/*   int iterations; */
/*   for(float x=-2; x<=2; x+= epsilon) */
/*   { */
/*     for(float y=-2; y<=2; y+= epsilon) */
/*     { */
/*       iterations = 0; */
/*       C = new Complex(x, y); */
/*       Z = new Complex(0,0); */

/*       while(Complex.Abs(Z) < 2 && iterations < maxIterations) */
/*       { */
/*         Z = Z*Z + C; */
/*         iterations++; */
/*       } */

/*       Screen.Plot(x,y, maxColors % iterations); // depending on the number of iterations, color a pixel. */
/*     } */
/*   } */
/* } */

main()
{
  /* complexTest(); */
  /* createPixelArray(1280,1280); */
  manBrot(6400, 6400);
}

/* "name" is the filename to use, */
/* "buff" is an array of width * height unsigned integers, where each one represents a pixel.  If the red, green and blue channels are chars in the range 0-0xFF, each unsigned integer is (r | (g << 8) | (b << 16)). */
/* You also need to compile with -lpng (to link to libpng). */

//char is smallest integer type (single byte)
//0-255
//unsigned int 4 bytes
//11111111 = 255  
//char = 1 byte, 8 bits
//buffer is 
