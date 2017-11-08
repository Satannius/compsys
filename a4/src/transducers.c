#include "transducers.h"
#include <sys/types.h> // Used for getpid(). Not necessary to include this one explicitly.
#include <unistd.h> // Used for getpid()

struct stream {
  FILE* f; // File pointer
  // int open; // = 1 if file is in use / unavailable. = 0 if file is NOT in use / available.
};

/* file_pipe */
static int file_pipe(FILE* files[2]) {
  int fds[2];
  int r = pipe(fds);
  if (r == 0) {
    printf("files: %lx\n", files);
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
  // s=s; /* unused */
  // s->open = 0; // Set flag to available.
  // Kill any children?
}

/* Create a source and a stream for its output. */
// The transducers_link_source() function will create a new stream object and 
// write its address to the given pointer variable.
// Should use fork() to create a new process for running the given function.
// Pipes should be used for communicating between processes.
// The source function must be passed the value of the arg parameter.
// The out parameter is a pointer to a pointer.
// Skal være asynkron. Funktionspointeren skal kaldes i baggrunden, så transduceren kan returnere med det samme.
// Skal returnere en stream struct med det samme. Den har stream **out
// stream * str = malloc(sizeof(stream)) giver en ny stream
// Vi kan evt. sætte filen til noget.
// Pointeren overskrives med *out = stream;
// Læs op på pipe og fork.
int transducers_link_source(stream **out,
                            transducers_source s, const void *arg) {
  printf("transducers_link_source\n");
  
  struct stream * str = malloc(sizeof(struct stream)); // Create new stream.
  *out = str; // Set *out to new stream address
  int fp = file_pipe(*out); // Create pipes from stream

  if (fp == -1)
  {
    perror("Err: file_pipe() failed.");
    exit(1);
  }
  
  int pid; // Fork
  pid = fork();
  
  if (pid == -1)
  {
    perror("Err: fork() failed.");
    exit(1);
  }

  if (pid == 0) /* Child */
  {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("This is child %ld\n", (long)pid);
    
    fclose(out[1]);

    printf("tls:\n");
    // printf("filepipe(*out): %d\n", file_pipe(*out));
    printf("  str: %lx\n", str);
    // printf("  FILE* f: %lx\n", &str->f);
    // printf("  *str: %lx\n", *str);
    // printf("  &str: %lx\n", &str);
    // printf("  out: %lx\n", out);
    printf("  *out: %lx\n", *out);
    // printf("  **out: %lx\n", **out);
    // printf("  &out: %lx\n", &out);
    // Pass value of arg to source function with new stream
    
    s(arg,out[0]);
    exit(0); // SPØRG: Skal child exit'e?
  }
  /* Parent */
  else
  {
    pid_t parid = getpid();
    printf("This is parent %ld\n", (long)parid);
    fclose(out[0]);
    printf("tls:\n");
    // printf("filepipe(*out): %d\n", file_pipe(*out));
    printf("  str: %lx\n", str);
    // printf("  FILE* f: %lx\n", &str->f);
    // printf("  *str: %lx\n", *str);
    // printf("  &str: %lx\n", &str);
    // printf("  out: %lx\n", out);
    printf("  *out: %lx\n", *out);
    // printf("  **out: %lx\n", **out);
    // printf("  &out: %lx\n", &out);
    // exit(0);
  }

  return 0;
}

int transducers_link_sink(transducers_sink s, void *arg,
                          stream *in) {
  // s=s; /* unused */
  // arg=arg; /* unused */
  // in=in; /* unused */
  printf("transducers_link_sink\n");
  printf("  arg: %lx\n", arg);
  printf("  &arg: %lx\n", &arg);
  printf("  in: %lx\n", in);
  printf("  *in: %lx\n", *in);
  printf("  &in: %lx\n", &in);
  wait(NULL);
  // Brug waitpid. Vi skal muligvis også reape børnene, hvis de er zombier.
  s(arg,in->f[0]);
  return 0;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {
  out=out; /* unused */
  t=t; /* unused */
  arg=arg; /* unused */
  in=in; /* unused */
  return 1;
}

int transducers_link_2(stream **out,
                       transducers_2 t, const void *arg,
                       stream* in1, stream* in2) {
  out=out; /* unused */
  t=t; /* unused */
  arg=arg; /* unused */
  in1=in1; /* unused */
  in2=in2; /* unused */
  return 1;
}

int transducers_dup(stream **out1, stream **out2,
                    stream *in) {
  out1=out1; /* unused */
  out2=out2; /* unused */
  in=in; /* unused */
  return 1;
}
