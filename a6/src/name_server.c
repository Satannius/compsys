#include <stdio.h>
#include <assert.h>
#include "csapp.h"
#include "name_server.h"
#define ARGNUM 1 // TODO: Put the number of you want to take

static struct list user_list[2];

void init_users() {
    strcpy(user_list[0].name, "banana");
    strcpy(user_list[0].password, "123");
    strcpy(user_list[1].name, "fisk");
    strcpy(user_list[1].password, "654");
    user_list[0].logged_in = 0;
    user_list[1].logged_in = 0;
}

/*
 * Based on csapp echo - read and echo text lines until client closes connection
 */
/* $begin echo */
void echo(int connfd)
{
    size_t n;
    char input_buf[MAXLINE];
    char output_buf[100];
    rio_t rio;
    int usr_id = -1;

    Rio_readinitb(&rio, connfd);

    while((n = Rio_readlineb(&rio, input_buf, MAXLINE)) != 0) { //line:netp:echo:eof
        process_cmd(user_list, input_buf,output_buf,&usr_id); // Processes input and writes to output
        Rio_writen(connfd, output_buf, strlen(output_buf));

        // Flush buffers
        memset(&input_buf[0], 0, strlen(input_buf));
        memset(&output_buf[0], 0, strlen(output_buf));
    }
    if ((usr_id) >= 0) { // Will be skipped, if login failed
        printf("User %s disconnected.\n", user_list[usr_id].name);
        user_list[usr_id].logged_in = 0;
    }
}

void *thread(void *vargp);

int main(int argc, char**argv) {
	int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid; 

    if (argc != ARGNUM + 1) {
        printf("%s expects %d argument(s).\n", (argv[0]+2), ARGNUM);
        printf("usage: ./%s <port number>\n", (argv[0]+2));
        return(0);
    }

    init_users(); // Initialize premade users.

    listenfd = Open_listenfd(argv[1]);

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
		connfdp   = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
		*connfdp  = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
		Pthread_create(&tid, NULL, thread, connfdp);
    }

    return 0;
}


/* Thread routine */
void *thread(void *vargp) 
{
    printf("Creating new connection thread.\n");
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    echo(connfd);
    Close(connfd);
    printf("Closing connection thread.\n");
    return NULL;
}


