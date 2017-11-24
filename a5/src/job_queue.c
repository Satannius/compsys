#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"

int job_queue_init(struct job_queue *job_queue, int capacity) {
  // Allokere til buffer mht. kapacitet og størrelse af void.
  int size = sizeof(void*) * capacity;
  void * buf = malloc(size);

  job_queue->capacity = capacity;
  job_queue->first    = 0;    // head
  job_queue->num_used = 0;    // tail
  job_queue->buffer   = &buf; // an array of void pointers
  job_queue->dead     = 0;    // 0 when alive, 1 when destoyed
  
  if (pthread_mutex_init(&(job_queue->mutex), NULL) != 0) {
    return 1;
  }
  if (pthread_cond_init(&(job_queue->cond), NULL) != 0) {
    return 1;
  }

  return 0;
}

int job_queue_destroy(struct job_queue *job_queue) {
  // Skal sleepe, når num_used er > 0?
  // If any threads are blocked on a call to job_queue_pop() when 
  // job_queue_destroy() is called, the threads must be woken with
  // job_queue_pop() returning -1
  // Betyder at hvis der er nogle pop's der venter og køen er fuld,
  // så skal vi vække dem alle og afslutte dem, så de returnerer -1.
  assert(0);

}

int job_queue_push(struct job_queue *job_queue, void *data) {
  // Bruge mutex og cv, hvis der ikke er plads
  // Skal sleepe, når condition siger, at der ikke kan pushes
  // (kø er fuld)
  if (job_queue->dead == 1){
  	return 1;
  }

  while(job_queue->capacity == job_queue->num_used){
  	pthread_cond_wait (job_queue->&cond, job_queue->&mutex); 
  }

  pthread_mutex_lock(job_queue->&mutex);


}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  // Bruge mutex og cv
  // Skal sleepe, når condition siger, at der ikke kan poppes 
  // (intet i kø)
  while(job_queue->num_used == 0){
    pthread_cond_wait (job_queue->&cond, job_queue->&mutex); 
  }

  pthread_mutex_lock(job_queue->&mutex);
}
