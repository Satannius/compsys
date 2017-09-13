#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.

int print_hello_world() {
  return fprintf(stdout, "Hello, world!\n");
}

int main() {
  int retval = EXIT_SUCCESS;

  if (print_hello_world() <= 0) {
    retval = EXIT_FAILURE;
  }

  return retval;
}
