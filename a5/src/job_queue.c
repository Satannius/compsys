#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"

int job_queue_init(struct job_queue *job_queue, int capacity) { 
  job_queue->capacity = capacity; // MAX
  job_queue->first    = 0;        // use_ptr
  job_queue->num_used = 0;        // count
  job_queue->buffer   = (void**)malloc((capacity * sizeof(void*)));      // pointer to a pointer, pointing to an array of void pointers
  job_queue->dead     = 0;        // 0 when alive, 1 when destoyed
  
  if (pthread_mutex_init(&(job_queue->mutex), NULL) != 0) {
    return 1;
  }
  if (pthread_cond_init(&(job_queue->cond), NULL) != 0) {
    return 1;
  }

  return 0;
}

// If any threads are blocked on a call to job_queue_pop() when 
// job_queue_destroy() is called, the threads must be woken with
// job_queue_pop() returning -1
int job_queue_destroy(struct job_queue *job_queue) {
  assert(pthread_mutex_lock(&(job_queue->mutex)) == 0);
  
  if (job_queue->dead){
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
  	return 1;
  }

  job_queue->dead = 1;
  assert(pthread_cond_broadcast(&(job_queue->cond)) == 0); // Signal for pop-threads to die
  while (job_queue->num_used > 0) {
    pthread_cond_wait (&(job_queue->cond), &(job_queue->mutex)); 
  }
  
  free(job_queue->buffer);


  assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);

  return 0;
}

  // Bruge mutex og cv, hvis der ikke er plads
  // Skal sleepe, når condition siger, at der ikke kan pushes
  // (kø er fuld)
int job_queue_push(struct job_queue *job_queue, void *data) {
  assert(pthread_mutex_lock(&(job_queue->mutex)) == 0);
  if (job_queue->dead){
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
  	return 1;
  }

  while((job_queue->capacity == job_queue->num_used) && !(job_queue->dead)){
  	pthread_cond_wait (&(job_queue->cond), &(job_queue->mutex)); 
  }

  if (job_queue->dead == 1 && job_queue->num_used == 0) {
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
    return -1;
  }

  int i = (job_queue->first + job_queue->num_used) % job_queue->capacity;
  job_queue->buffer[i] = data;
  job_queue->num_used++;

  assert(pthread_cond_broadcast(&(job_queue->cond)) == 0);
  assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);

  return 0;
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  assert(pthread_mutex_lock(&(job_queue->mutex)) == 0);
  if (job_queue->dead && job_queue->num_used == 0) {
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
    return 1;
  }

  while((job_queue->num_used == 0) && !(job_queue->dead)){
    pthread_cond_wait (&(job_queue->cond), &(job_queue->mutex));
  }

  if (job_queue->dead == 1 && job_queue->num_used == 0) {
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
    return -1;
  }

  *data = job_queue->buffer[job_queue->first];
  if (job_queue->first == (job_queue->capacity - 1)) {
    job_queue->first = 0;
  }
  else {
    job_queue->first++;
  }
  
  job_queue->num_used--;
  assert(pthread_cond_broadcast(&(job_queue->cond)) == 0);
  assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
  return 0;
}
