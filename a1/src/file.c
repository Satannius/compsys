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

//Checks if the file is empty
int empty_checker(FILE* fp) {
  fseek(fp, 0, 2);
  int size = ftell(fp);
  if (size == 0) 
    return 1;
  else
    return 0;
}

//Checks if the file contains non-ascii characters, if none is found 
//return 2 which mean the file is ASCII.
int ascii_checker(FILE* fp) {
  int c;
  int result = 2; 
  fseek(fp, 0, SEEK_SET);
  while (!feof(fp)) {
    c = fgetc(fp);
    if(c != EOF) {
      //Limits exclude in range of (7 to 13, 27, 32 to 137 )
      if (!( (c >= 0x07 && c <= 0x0D) || c == 0x1B || 
        (c >= 0x20 && c <= 0x7E) )) {
        result = 0;
        break;
      } 
    }
  }
  return result;
}

//Checks if the file contains non ISO-8859-1-likebytes, if none is found 
//return 3 which mean the file is ISO.
int iso_checker(FILE* fp) {
  int c;
  int result = 3; 
  fseek(fp, 0, SEEK_SET);
  while (!feof(fp)) {
    c = fgetc(fp);
    if(c != EOF) {
      //Limits exclude in range of (7 to 13, 27, 32 to 126 ) and (160 to 255)
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

//Returns the corresponding e_num.
int type_detector(FILE* fp) {
  if (fp == NULL) {
    return -1;
  }
  else if (empty_checker(fp) == 1) {
    return 1;
  }
  else if (ascii_checker(fp) == 2) {
    return 2;
  }
  else if (iso_checker(fp) == 3) {
    return 3;
  }
  else if (utf16_checker(fp) ==5) {
    return 5;
  }
  else if (utf16_checker(fp) == 6) {
    return 6;
  }
  else if (utf8_checker(fp) == 4) {
    return 4;
  }    
  else
    return 0;
}

int main(int argc, char *argv[]) {
  if (argc >= 2){
    //Finding the file with the longest name, this is used for aligning the
    //output.
    unsigned long max_length = strlen(argv[1]);
    int x;
    for (x = 1; x < argc; x++) {
      if (max_length < strlen(argv[x-1])){
        max_length = strlen(argv[x-1]);
      }
    }
    int i = 1;
    while( i < argc )
    {
      FILE *fp;
      fp = fopen(argv[i],"r");

      if (type_detector(fp) == -1)
      {
        //If no file has been found throw an error
        fprintf(stdout, "%s:%*scannot determine(%s)\n", argv[i], (int)((max_length+1) - strlen(argv[i])), " ", strerror(errno));
      }
      else
      {
        //If the file has been found, the filetype will be printed out.
        fprintf(stdout,"%s:%*s%s\n", argv[i], (int)((max_length+1) - strlen(argv[i]))," ", FILE_TYPE_STRINGS[type_detector(fp)]);  
      }
      i++;
    }
    exit(EXIT_SUCCESS); 
  }
  else
  {
    fprintf(stdout, "Usage: file path ...\n");
    exit(EXIT_FAILURE);  
  }

}
