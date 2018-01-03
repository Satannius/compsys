#include "buffer.h"
#include "csapp.h"

void enqueue(linked_queue* queue,  char* buffer, size_t len) {
    if (queue == NULL) {
        app_error("NULL queue");
    } else if (buffer == NULL) {
        app_error("NULL buffer");
    }
    
    queue_node* new_node = (queue_node*) Malloc(sizeof(queue_node));
    new_node -> next = NULL;
    new_node -> data = (char*) Malloc(len);
    new_node -> data_len = len;
    memcpy(new_node -> data, buffer, len);
    
    if (queue -> tail == NULL) {            // If no items enqueued, set head to new node
        queue -> head = new_node;
    } else {                                // If items enququed, set new node next-after tail
        queue -> tail -> next = new_node;
    }
    queue -> tail = new_node;               // Set new node to tail
}

ssize_t dequeue(linked_queue* queue, char* buffer, size_t len) {
    if (queue == NULL) {
        app_error("NULL queue");
    } else if (buffer == NULL) {
        app_error("NULL buffer");
    }

    queue_node* cur_head = queue->head;
    if(cur_head == NULL) {
        return -1;
    }
    
    if (cur_head -> data_len <= len) {                      // If data_len is less than requested len
        memcpy(buffer, cur_head->data, cur_head->data_len); // Copy data to pointed buffer
        if(cur_head == queue -> tail) {                     // If head == tail, set both to null.
            queue -> head = queue -> tail = NULL;
        }
        queue -> head = cur_head -> next;                   // Update head-pointer to next node
        free(cur_head -> data);                             // Free memory
        free(cur_head);
        return 0;
    } else {                                                // If data_len is more than requested len
        memcpy(buffer, cur_head -> data, len);              // Copy data to pointed buffer
        memmove(cur_head -> data, cur_head -> data + len, cur_head -> data_len - len);
        cur_head -> data_len -= len;                        // Update data_len: subtract requested len
        cur_head -> data = Realloc(cur_head -> data, cur_head -> data_len); // Realloc necessary space
        return cur_head -> data_len;                        // Return remaining len to be read.
    }
}

void set_empty_queue(linked_queue* queue) {
        queue->head = NULL;
        queue->tail = NULL;
}
