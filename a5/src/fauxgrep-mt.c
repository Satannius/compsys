// Setting _DEFAULT_SOURCE is necessary to activate visibility of
// certain header file contents on GNU/Linux systems.
// Run using E.G. ./fauxgrep-mt return .
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>

// err.h contains various nonstandard BSD extensions, but they are
// very handy.
#include <err.h>

#include <pthread.h>

#include "job_queue.h"

// Whenever we print to the screen, we will first lock this mutex.
// This ensures that multiple threads do not try to print
// concurrently.
pthread_mutex_t stdout_mutex = PTHREAD_MUTEX_INITIALIZER;

struct arg_struct {
  struct job_queue *jq;
  char const *needle;
};

int fauxgrep_file(char const *needle, char const *path) {
  assert(pthread_mutex_lock(&stdout_mutex) == 0);
  FILE *f = fopen(path, "r");

  if (f == NULL) {
    warn("failed to open %s", path);
    return -1;
  }

  char *line = NULL;
  size_t linelen = 0;
  int lineno = 1;

  while (getline(&line, &linelen, f) != -1) {
    if (strstr(line, needle) != NULL) {
      printf("%s:%d: %s", path, lineno, line);
    }

    lineno++;
  }

  free(line);
  fclose(f);
  assert(pthread_mutex_unlock(&stdout_mutex) == 0);
  return 0;
}

// Each thread will run this function.  The thread argument is a
// pointer to a job queue.
void* worker(void *arg) {
  struct arg_struct *args = (struct arg_struct *)arg;
  struct job_queue *jq = (args->jq);

  while (1) {
    char *path;
    if (job_queue_pop(jq, (void**)&path) == 0) {
      fauxgrep_file(args->needle, path);
      free(path);
    } else {
      // If job_queue_pop() returned non-zero, that means the queue is
      // being killed (or some other error occured).  In any case,
      // that means it's time for this thread to die.
      break;
    }
  }

  return NULL;
}

int main(int argc, char * const *argv) {
  if ((argc != 3) && (argc != 5)) {
    err(1, "usage: [-n INT] STRING paths...");
    exit(1);
  }

  int num_threads = 1;
  char const *needle = argv[1];
  char * const *paths = &argv[2];


  if (argc > 3 && strcmp(argv[1], "-n") == 0) {
    // Since atoi() simply returns zero on syntax errors, we cannot
    // distinguish between the user entering a zero, or some
    // non-numeric garbage.  In fact, we cannot even tell whether the
    // given option is suffixed by garbage, i.e. '123foo' returns
    // '123'.  A more robust solution would use strtol(), but its
    // interface is more complicated, so here we are.
    num_threads = atoi(argv[2]);

    if (num_threads < 1) {
      err(1, "invalid thread count: %s", argv[2]);
    }

    needle = argv[3];
    paths = &argv[4];

  } else {
    needle = argv[1];
    paths = &argv[2];
  }

  // assert(0); // Initialise the job queue and some worker threads here.
  struct job_queue jq;
  job_queue_init(&jq, 64);

  struct arg_struct args;
  args.jq = &jq;
  args.needle = needle;

  // Start up the worker threads.
  pthread_t *threads = calloc(num_threads, sizeof(pthread_t));
  for (int i = 0; i < num_threads; i++) {
    if (pthread_create(&threads[i], NULL, &worker, (void *)&args) != 0) {
      err(1, "pthread_create() failed");
    }
  }

  // FTS_LOGICAL = follow symbolic links
  // FTS_NOCHDIR = do not change the working directory of the process
  //
  // (These are not particularly important distinctions for our simple
  // uses.)
  int fts_options = FTS_LOGICAL | FTS_NOCHDIR;

  FTS *ftsp;
  if ((ftsp = fts_open(paths, fts_options, NULL)) == NULL) {
    err(1, "fts_open() failed");
    return -1;
  }

  FTSENT *p;
  while ((p = fts_read(ftsp)) != NULL) {
    switch (p->fts_info) {
    case FTS_D:
      break;
    case FTS_F:     
      job_queue_push(&jq, (void*)strdup(p->fts_path));
      //assert(0); // Process the file p->fts_path, somehow.
      break;
    default:
      break;
    }
  }

  fts_close(ftsp);

  // Destroy the queue.
  job_queue_destroy(&jq);

  // Wait for all threads to finish.  This is important, at some may
  // still be working on their job.
  for (int i = 0; i < num_threads; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      err(1, "pthread_join() failed");
    }
  }

  return 0;
}
