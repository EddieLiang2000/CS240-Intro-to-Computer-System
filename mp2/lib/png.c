#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "crc32.h"
#include "png.h"

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;
const char check[8] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };

/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 * 
 * (Note: The function follows the same function prototype as `fopen`.)
 * 
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 * 
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 */
PNG * PNG_open(const char *filename, const char *mode) {
  FILE *file = fopen(filename, mode);
  PNG *returned = malloc(sizeof(PNG));
  if (mode[0] == 'r' && file != NULL) {
    char buffer[8];
    size_t length = fread(buffer, sizeof(char), 8, file);
    if (length != 8 || memcmp(buffer, check, length) != 0) {
      return NULL;
    } else {
      returned->file = file;
      return returned;
    }
  }
  if (mode[0] == 'w' && file != NULL) {
    fwrite(check, sizeof(char), sizeof(check), file);
    returned->file = file;
    return returned;
  } else {
      return NULL;
  }
}
  
    
  


/**
 * Reads the next PNG chunk from `png`.
 * 
 * If a chunk exists, the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 * 
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk) {
  size_t result = 0;
  result += fread(&chunk->len, 1, sizeof(uint32_t), png->file);
  chunk->len = ntohl(chunk->len);

  result += fread(chunk->type, sizeof(char), 4, png->file);
  chunk->type[4] = 0;

  if (chunk->len > 0) {
    chunk->data = malloc(chunk->len);
    result += fread(chunk->data, sizeof(char), chunk->len, png->file);
  } else {
    chunk->data = NULL;
  }

  result += fread(&chunk->crc, 1, sizeof(uint32_t), png->file);
  chunk->crc = ntohl(chunk->crc);

  return result;
}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written.   As part of writing the `chunk`, you must calculate the CRC
 * based on the other data in the `chunk`; do not assume the CRC given has been calculated for you.
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {
  size_t bytes = 0;

  uint32_t networkOrder_len = htonl(chunk->len);
  bytes += fwrite(&networkOrder_len, sizeof(uint32_t), 1, png->file) * sizeof(uint32_t);

  bytes += fwrite(chunk->type, sizeof(char), 4, png->file);
  bytes += fwrite(chunk->data, sizeof(char), chunk->len, png->file);

  unsigned char *crcBuffer = malloc(4 + chunk->len);
  memcpy(crcBuffer, chunk->type, 4);
  memcpy(crcBuffer + 4, chunk->data, chunk->len);
  uint32_t crc = 0;
  crc32(crcBuffer, 4 + chunk->len, &crc);
  free(crcBuffer);

  uint32_t networkOrder_crc = htonl(crc);
  bytes += fwrite(&networkOrder_crc, sizeof(uint32_t), 1, png->file) * sizeof(uint32_t);

  return bytes;
}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  free(chunk->data);
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  fclose(png->file);
}
