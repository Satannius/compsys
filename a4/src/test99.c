#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void string_stream(const void *arg, FILE *out) {
  printf("string_stream arg: %lx, out: %lx\n", arg, *out);
  fputs((const char*) arg, out);
}

void save_stream(void *arg, FILE *in) {
  /* We will be writing bytes to this location. */
  unsigned char *d = arg;

  while (fread(d, sizeof(unsigned char), 1, in) == 1) {
    d++; /* Move location ahead by one byte. */
  }
}

int main() {
  stream* s[1]; // Længden er 1, dvs. første element på index 0. Men hvorfor er stream et array?

  char *input = "Hello, World!";
  // char *output = malloc(strlen(input)+1);
  // output[strlen(input)] = '\0'; /* Ensure terminating NULL. */
  printf("test99.c s: %lx\n", *s);

  assert(transducers_link_source(&s[0], string_stream, input) == 0); // Laver en stream i s[0], med indholdet af string_stream(input)

  printf("test99.c s: %lx\n", *s);

  return 0;
}
