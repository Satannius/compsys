#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void string_stream(const void *arg, FILE *out) {
  printf("string_stream:\n");
  printf("  arg: %s, out: %lx\n", arg, out);
  fputs((const char*) arg, out);
}

void save_stream(void *arg, FILE *in) {
  /* We will be writing bytes to this location. */
  unsigned char *d = arg;
  printf("save_stream:\n");
  printf("  arg: %lx, in: %lx\n", arg, in);

  while (fread(d, sizeof(unsigned char), 1, in) == 1) {
    d++; /* Move location ahead by one byte. */
  }
}

int main() {
  stream* s[1]; // Længden er 1, dvs. første element på index 0. Men hvorfor er stream et array?

  char *input = "Hello, World!";
  char *output = malloc(strlen(input)+1);
  output[strlen(input)] = '\0'; /* Ensure terminating NULL. */

  assert(transducers_link_source(&s[0], string_stream, input) == 0); // Laver en stream i s[0], med indholdet af string_stream(input)
  assert(transducers_link_sink(save_stream, output, &s[0]) == 0); // Bruger save_stream til at skrive fra s[0] til output

  printf("test0:\n");
  printf("  input: %s\n", input);
  printf("  output: %s\n", output);
  /* We cannot use the '==' operator for comparing strings, as strings
     in C are just pointers.  Using '==' would compare the _addresses_
     of the two strings, which is not what we want. */
  // assert(strcmp(input,output) == 0);

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  // sizeof(s) er størrelsen i bytes i alt, dvs. 8, divideret med længden af første element = antal iterationer
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++)
  {
    transducers_free_stream(s[i]); // Læs hints. Det er noget med at sætte et flag i struct, så der står at filen er ledig/optaget.
  }

  return 0;
}
