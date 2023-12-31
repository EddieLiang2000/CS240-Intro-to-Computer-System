#include <stdlib.h>

int main() {

  void *a = malloc(256);  // 256 == 0x 100 bytes
  printf("the address of a is %p\n", a);
  void *b = malloc(256);
  printf("the address of b is %p\n", b);
  void *c = malloc(128);
  printf("the address of c is %p\n", c);
  void *d = malloc(256);
  printf("the address of d is %p\n", d);
  void *e = malloc(128);
  printf("the address of e is %p\n", e);
  /* Line 6 */
  free(a);
  free(b);
  free(d);
  /* Line 10 */
  void *r1 = malloc(10);
  printf("the address of r1 is %p\n", r1);
  void *r2 = malloc(10);
  printf("the address of r2 is %p\n", r2);
  void *r3 = malloc(300);
  printf("the address of r3 is %p\n", r3);
  void *r4 = malloc(250);  
  printf("the address of r4 is %p\n", r4);
}
