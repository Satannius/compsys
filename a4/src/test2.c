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
  stream* s[3];

  char *input1 = "Hello, World!";
  char *input2 = "Hello, Fish!";
  int length = strlen(input1)+strlen(input2);
  char *output = malloc(length+1);
  output[length] = '\0'; /* Ensure terminating NULL. */
  int inc = 1;

  /* transducers_link_2(stream **out,
                       transducers_2 t, const void *arg,
                       stream* in1, stream* in2) */
  assert(transducers_link_source(&s[0], string_stream, input1) == 0);
  assert(transducers_link_source(&s[1], string_stream, input2) == 0);
  assert(transducers_link_2(&s[2], increment_stream, &inc, s[0], s[1]) == 0);
  assert(transducers_link_sink(save_stream, output, s[2]) == 0);

  char str[length];
  strcpy(str, input1);
  strcat(str, input2);

  /* We cannot use the '==' operator for comparing strings, as strings
     in C are just pointers.  Using '==' would compare the _addresses_
     of the two strings, which is not what we want. */
  assert(strcmp(str,output) == 0);

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }

  free(output);

  return 0;
}
