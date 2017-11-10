#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void string_stream(const void *arg, FILE *out) {
  fputs((const char*) arg, out);
}

void increment_stream(const void *arg, FILE *out, FILE *in1, FILE *in2) {
  unsigned char c = *(char*) arg;
  while (fread(&c, sizeof(unsigned char), 1, in1) == 1) {
    fwrite(&c, sizeof(unsigned char), 1, out);
  }
  while (fread(&c, sizeof(unsigned char), 1, in2) == 1) {
    fwrite(&c, sizeof(unsigned char), 1, out);
  }  
}

void save_stream(void *arg, FILE *in) {
  /* We will be writing bytes to this location. */
  unsigned char *d = arg;

  while (fread(d, sizeof(unsigned char), 1, in) == 1) {
    d++; /* Move location ahead by one byte. */
  }
}

int main() {
  stream* s[4];

  char *input = "Hello, World!";
  int length = strlen(input)+strlen(input);
  char *output = malloc(length+1);
  output[length] = '\0'; /* Ensure terminating NULL. */
  int inc = 1;

  assert(transducers_link_source(&s[0], string_stream, input) == 0);
  assert(transducers_dup(&s[1], &s[2], s[0]) == 0);
  assert(transducers_link_2(&s[3], increment_stream, &inc, s[1], s[2]) == 0);
  assert(transducers_link_sink(save_stream, output, s[3]) == 0);
  
  char str[length];
  strcpy(str, input);
  strcat(str, input);

  assert(strcmp(str,output) == 0);

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }

  

  return 0;
}
