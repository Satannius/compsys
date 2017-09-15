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
int print_error(const char *path, unsigned long max_length, int errnum) {
  return fprintf(stdout, "%s:%*scannot determine (%s)\n",
    path, (int)(max_length - strlen(path)), " ", strerror(errnum));
}

int print_message(char *path, unsigned long max_length, int type_num) {
  //return printf("%s: %s\n", path, FILE_TYPE_STRINGS[type_num]);
  return fprintf(stdout, "%s:%*s%s\n",
  path, (int)(max_length - strlen(path)), " ", FILE_TYPE_STRINGS[type_num]);
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


int iso_checker(FILE* fp) {
  int c;
  int result = 3; 
  fseek(fp, 0, SEEK_SET);
  while (!feof(fp)) {
    c = fgetc(fp);
    if(c != EOF) {
      if (!( (c >= 0x07 && c <= 0x0D) || c == 0x1B || 
        (c >= 0x20 && c <= 0x7E) || (c >= 0xA0 && c <= 0xFF) )) {  
        result = 0;
        break;
      }
    }
  } 
  return result;
}


int utf8_checker(FILE* fp) {
  int c;
  int result = 0; 
  fseek(fp, 0, SEEK_SET);
  while (!feof(fp)) {
    c = fgetc(fp);
    if(c != EOF) {
      if (((c & 0xF0) == 0xF0) && (((c+1) & 0x80) == 0x80) && 
        (((c+2) & 0x80) == 0x80) && (((c+3) & 0x80) == 0x80)){
        result = 4;
      }
      else if (( (c & 0xE0) == 0xE0) && (((c+1) & 0x80) == 0x80)
              && (((c+2) & 0x80) == 0x80)) {
        result = 4;
      }
      else if (((c & 0xc0) == 0xc0) && (((c+1) & 0x80) == 0x80)) {
        result = 4;
      } 
      else if ((c & 0xc0) == 0xc0) {
        result = 4;
      }
    }
  }
  return result;
}


int utf16_checker(FILE* fp) {
  int result = 0; 
  fseek(fp, 0, SEEK_SET);
  int first  = fgetc(fp);
  int second = fgetc(fp);

  if ((first == 0xFE) && (second == 0xFF)) {
    result = 5;
  }
  else if ((first == 0xFF) && (second == 0xFE)) {
    result = 6;
  }

  return result;
}


void type_detector(FILE* fp, char *path, unsigned long max_length) {
  if (fp == NULL) {
    print_error(path, max_length, errno);
  }
  else if (empty_checker(fp) == 1) {
    print_message(path, max_length, 1);
  }
  else if (ascii_checker(fp) == 2) {
    print_message(path, max_length, 2);
  }
  else if (iso_checker(fp) == 3) {
    print_message(path, max_length, 3);
  }
  else if (utf16_checker(fp) ==5) {
    print_message(path, max_length, 5);
  }
  else if (utf16_checker(fp) == 6) {
    print_message(path, max_length, 6);
  }
  else if (utf8_checker(fp) == 4) {
    print_message(path, max_length, 4);
  }
  else
    print_message(path, max_length, 0);
}


int main(int argc, char *argv[]) {
  if (argc >= 2){
    unsigned long max_length = strlen(argv[1]);
    int x;
    for (x = 2; x <= argc; x++) {
      if (max_length < strlen(argv[x])){
        max_length = strlen(argv[x]);
      }
    }
    int i = 1;
    while( i < argc )
    {
      FILE *fp;
      fp = fopen(argv[i],"r");
      
      type_detector(fp, argv[i], max_length);
      
      i++;
    }
    exit(EXIT_SUCCESS); 
  }
}
