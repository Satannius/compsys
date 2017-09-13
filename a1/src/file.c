#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

const char * const FILE_TYPE_STRINGS[] = {
  	"data",
  	"empty",
  	"ASCII text",
};

// Assumes: errnum is a valid error number
int print_error(char *path, int errnum) {
    return fprintf(stdout, "%s: cannot determine (%s)\n",
        path, strerror(errnum));
}

int main(int argc, char *argv[]) {
  if (argc >= 2){
    
    int i;

    while( i+1 <= argc)
    {
      int c;
      FILE *fp;
      fp = fopen(argv[i],"r");
      

      if (fp == NULL){
        print_error(argv[i], errno);
        break;
        //exit(EXIT_SUCCESS);
      }

        //Finds out if the file is empty
        fseek(fp, 0, 2);
        int size = ftell(fp);
        if (size == 0) {
          printf("%s: %s\n", argv[i], FILE_TYPE_STRINGS[1]);
          //exit(EXIT_SUCCESS);
        }
        fseek(fp, 0, SEEK_SET);

        //Finds out if the file is ASCII or Binary 
        while (!feof(fp)) 
        {
          c = fgetc(fp);
          if(c != EOF){ // End of character not included 
          //If any non-Ascii char has been found, the file we be deemed as a data file.
            if (!( (c >= 0x07 && c <= 0x0D) || c == 0x1B || (c >= 0x20 && c <= 0x7E) )) { 
              printf("%s: %s\n", argv[i], FILE_TYPE_STRINGS[0]);
              break;
              //exit(EXIT_SUCCESS);
            }
            else{
              printf("%s: %s\n", argv[i], FILE_TYPE_STRINGS[2]);
              break;
              //exit(EXIT_SUCCESS);             
            }
          }
        }


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
