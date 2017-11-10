#include "transducers.h"
#include <sys/types.h> // Used for getpid(). Not necessary to include this one explicitly.
#include <unistd.h> // Used for getpid()

struct stream {
  FILE* f; // File pointer
  int open; // = 1 if file is in use / unavailable. = 0 if file is NOT in use / available.
};

/* file_pipe */
static int file_pipe(FILE* files[2]) {
  int fds[2];
  int r = pipe(fds);
  if (r == 0) {
    files[0] = fdopen(fds[0], "r");
    files[1] = fdopen(fds[1], "w");
    if (files[0] && files[1]) {
      return 0;
    } 
    else {
      return 1;
    }
  } 
  else {
    return r;
  }
}

// Should contain a FILE* pointer and a flag indicating whether the stream already has a reader.
void transducers_free_stream(stream *s) {
  s->open = 0; // Set flag to available.
}

int transducers_link_source(stream **out,
                            transducers_source s, const void *arg) {
  struct stream * str = malloc(sizeof(stream)); // Create new stream.
  *out = str;
  FILE* files[2];
  int fp = file_pipe(files); // Create pipes from stream

  if (fp == -1)
  {
    return 1;
  }
  
  pid_t pid; // Fork
  pid = fork();
  
  if (pid == -1)
  {
    return 1;
  }

  if (pid == 0) /* Child */
  {
    fclose(files[0]);   // Close read-port.
    s(arg,files[1]);    // Write to write-port via source_function
    fclose(files[1]);   // Close write-port.
    exit(0);
  }
  else /* Parent */
  {
    fclose(files[1]);   // Close write-port.
    str->f = files[0];  // Read from read-port.
    str->open = 0;      // Set stream to available.
  }

  return 0;
}

int transducers_link_sink(transducers_sink s, void *arg,
                          stream *in) {
  if (in->open == 1)
  {
    return 1;
  }
  s(arg,in->f);
  // Skal link_sink gøre andet?
  return 0;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {
  if (in->open == 1)
  {
    return 1;
  }
  else
  {
    in->open = 1;
  }
  
  struct stream * str = malloc(sizeof(stream)); // Create new stream.
  *out = str;
  FILE* files[2];
  int fp = file_pipe(files); // Create pipes from stream

  if (fp == -1)
  {
    return 1;
  }
  
  pid_t pid; // Fork
  pid = fork();
  
  if (pid == -1)
  {
    return 1;
  }

  if (pid == 0) /* Child */
  {
    fclose(files[0]);   // Close read-port.
    // typedef void (*transducers_1)(const void *arg, FILE *out, FILE *in);
    t(arg,files[1],in->f);    // Write to write-port via source_function
    fclose(files[1]);   // Close write-port.
    exit(0);
  }
  else /* Parent */
  {
    fclose(files[1]);   // Close write-port.
    str->f = files[0];  // Read from read-port.
    str->open = 0;      // Set stream to available.
  }

  return 0;
}

int transducers_link_2(stream **out,
                       transducers_2 t, const void *arg,
                       stream* in1, stream* in2) {
  struct stream * str = malloc(sizeof(stream)); // Create new stream.
  *out = str;
  FILE* files[2];
  int fp = file_pipe(files); // Create pipes from stream

  if (fp == -1)
  {
    perror("Err: file_pipe() failed.");
    return 1;
  }
  
  pid_t pid; // Fork
  pid = fork();
  
  if (pid == -1)
  {
    perror("Err: fork() failed.");
    return 1;
  }

  if (pid == 0) /* Child */
  {
    fclose(files[0]);   // Close read-port.
    t(arg,files[1],in1->f,in2->f);    // Write to write-port via source_function
    fclose(files[1]);   // Close write-port.
    exit(0);
  }
  else /* Parent */
  {
    fclose(files[1]);   // Close write-port.
    str->f = files[0];  // Read from read-port.
    str->open = 1;      // Set flag.
  }

  return 0;
}

int transducers_dup(stream **out1, stream **out2,
                    stream *in) {
  out1=out1; /* unused */
  out2=out2; /* unused */
  in=in; /* unused */
  return 1;
}
