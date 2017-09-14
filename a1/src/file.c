#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.


const char * const FILE_TYPE_STRINGS[] = {
  	"data",                                //0
  	"empty",                               //1
  	"ASCII text",                          //2
    "ISO-8859 text",                       //3
    "UTF-8 Unicode text",                  //4
    "Big-endian UTF-16 Unicode text",      //5
    "Little-endian UTF-16 Unicode text",   //6
};

// Assumes: errnum is a valid error number
int print_error(char *path, int errnum) {
    return fprintf(stdout, "%s: cannot determine (%s)\n",
      path, strerror(errnum));
}

int print_message(char *path, int type_num) {
  return printf("%s: %s\n", path, FILE_TYPE_STRINGS[type_num]);
}

int empty_checker(FILE* fp) {
  fseek(fp, 0, 2);
  int size = ftell(fp);
  if (size == 0) 
    return 1;
  else
    return 0;
  }

int ascii_checker(FILE* fp) {
  int c;
  int result = 2; 
  fseek(fp, 0, SEEK_SET);
  while (!feof(fp)) {
    c = fgetc(fp);
    if(c != EOF) {
      if (!( (c >= 0x07 && c <= 0x0D) || c == 0x1B || 
        (c >= 0x20 && c <= 0x7E) )) {
        
        result = 0;
        break;
      } 
    }
  }
  return result;
}

void type_detector(FILE* fp, char *path) {
  if (empty_checker(fp) == 1) {
    print_message(path, 1);
  }
  else if (ascii_checker(fp) == 2)
  {
    print_message(path, 2);
  }
  else
    print_message(path, 0);
}


int main(int argc, char *argv[]) {
  if (argc >= 2){
    int i = 1;
    while( i+1 <= argc)
    {
      FILE *fp;
      fp = fopen(argv[i],"r");
      
      if (fp == NULL){
        print_error(argv[i], errno);
        break;
        //exit(EXIT_SUCCESS);
      }
      
      type_detector(fp, argv[i]);
      
      i++;
    }
    exit(EXIT_SUCCESS);
  
  }
    // If there is more than one argument.
    else { 
      fprintf(stderr, "Usage: %s path\n", argv[0]);
      exit(EXIT_FAILURE);
    }
}
