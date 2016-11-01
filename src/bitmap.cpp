#include <stdio.h> 
#include <stdlib.h>
#include <iostream>

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
