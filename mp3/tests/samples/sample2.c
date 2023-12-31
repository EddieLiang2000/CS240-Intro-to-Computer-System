#include <stdio.h>
#include <stdlib.h>

int main() {
  void *a = malloc(1280);
  printf("the address of a is %p\n", a);
  void *b = malloc(1280);
  printf("the address of b is %p\n", b);
  void *c = malloc(1280);
  printf("the address of c is %p\n", c);
  void *d = malloc(1280);
  printf("the address of d is %p\n", d);
  void *e = malloc(1280);
  printf("the address of e is %p\n", e);

  free(d);
  free(c);
  free(b);

  void *f = malloc(2600);
  printf("the address of f is %p\n", f);
  void *g = malloc(1);
  printf("the address of g is %p\n", g);
}