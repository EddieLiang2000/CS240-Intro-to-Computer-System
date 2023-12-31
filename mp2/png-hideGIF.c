#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>

int png_hideGIF(const char *png_filename, const char *gif_filename) {
PNG *png = PNG_open(png_filename, "r");
  if (!png) { return ERROR_INVALID_FILE; }
  printf("PNG Header: OK\n");  


  // Read chunks until reaching "IEND" or an invalid chunk:
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNG_Chunk chunk;
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      return ERROR_INVALID_CHUNK_DATA;
    }
    if (strcmp (chunk.type, "gAMA") == 0) {
      PNG_Chunk addchunk = malloc(sizeof(PNG_Chunk));
      FILE *file = fopen(gif_filename, "w");
      fseek(file, 0, SEEK_END);
      long int sz;
      sz = ftell(file);
      rewind(file);
      addchunk.len = sz;
      addchunk.type[0] = 'u';
      addchunk.type[1] = 'i';
      addchunk.type[2] = 'u';
      addchunk.type[3] = 'c';
      addchunk.type[4] = '\0';
      char *buffer = malloc(sizeof(char)*sz);
      memcpy(buffer,file,sz);
      fwrite((char*)addchunk.data, sz, 1,buffer);
      PNG_write(png, &addchunk);
      fclose(file);
    }
      
    // Check for the "IEND" chunk to exit:
    if (strcmp(chunk.type, "IEND") == 0) {
      PNG_free_chunk(&chunk);
      break;
    }
    // Free the memory associated with the chunk we just read:
    PNG_free_chunk(&chunk);
  }

  PNG_close(png);
  return 0;
}

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF File>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_hideGIF(argv[1], argv[2]);
}
