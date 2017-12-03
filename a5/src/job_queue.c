#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"

int job_queue_init(struct job_queue *job_queue, int capacity) { 
  job_queue->capacity = capacity; // Maximum no. of items in queue
  job_queue->first    = 0;        // Index of first item
  job_queue->num_used = 0;        // Numer of items currently in the queue
  job_queue->buffer   = (void**)malloc((capacity * sizeof(void*))); // Pointer to a pointer, pointing to an array of void pointers
  job_queue->dead     = 0;        // 0 when alive, 1 when destoyed
  
  // Initiate mutex
  if (pthread_mutex_init(&(job_queue->mutex), NULL) != 0) {
    return 1;
  }
  // Initiate condition
  if (pthread_cond_init(&(job_queue->cond), NULL) != 0) {
    return 1;
  }

  return 0;
}

// Destroy queue
// Wake any waiting threads
// Wait for queue to empty and then freee memory
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

// Push an item into the queue
// Sleep if queue is full
int job_queue_push(struct job_queue *job_queue, void *data) {
  assert(pthread_mutex_lock(&(job_queue->mutex)) == 0);
  if (job_queue->dead){
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
  	return 1;
  }

  // Sleep
  while((job_queue->capacity == job_queue->num_used) && !(job_queue->dead)){
  	pthread_cond_wait (&(job_queue->cond), &(job_queue->mutex));
  }

  // Return -1 if queue has died in the mean time
  if (job_queue->dead == 1) {
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
    return -1;
  }

  // Compute index of vacant place
  int i = (job_queue->first + job_queue->num_used) % job_queue->capacity;
  job_queue->buffer[i] = data;
  job_queue->num_used++;

  // Wake any waiting pop-instructions
  assert(pthread_cond_broadcast(&(job_queue->cond)) == 0);

  assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
  return 0;
}

// Pop an item from the queue
// Sleep if queue is empty
int job_queue_pop(struct job_queue *job_queue, void **data) {
  assert(pthread_mutex_lock(&(job_queue->mutex)) == 0);
  if (job_queue->dead && job_queue->num_used == 0) {
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
    return 1;
  }

  while((job_queue->num_used == 0) && !(job_queue->dead)){
    pthread_cond_wait (&(job_queue->cond), &(job_queue->mutex));
  }

  // Return -1 if queue is dead and empty in the mean time.
  if ((job_queue->num_used == 0) && (job_queue->dead == 1)) {
    assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
    return -1;
  }

  // Retrieve first item in queue and write to pointer location.
  *data = job_queue->buffer[job_queue->first];
  if (job_queue->first == (job_queue->capacity - 1)) {
    job_queue->first = 0;
  }
  else {
    job_queue->first++;
  }
  
  job_queue->num_used--;

  // Wake any waiting push-instructions
  assert(pthread_cond_broadcast(&(job_queue->cond)) == 0);

  assert(pthread_mutex_unlock(&(job_queue->mutex)) == 0);
  return 0;
}
