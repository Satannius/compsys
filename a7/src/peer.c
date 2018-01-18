#include <stdio.h>
#include "csapp.h"
#include "peer.h"

#define ARGNUM 0 // TODO: Put the number of args you want to take
#define BUFSIZE 128
#define DEBUG 1

void *chat_server(void *port_arg);
void *thread(void *vargp);

// if cmd = /login usr pass ip port
    // Send command to server and wait for affirmation
    // Set up chat server to listen - via other thread?
// if cmd = /msg <usr> <msg>
    // Query name_server for usr info
    // Connect to target chat server
    // Send msg to usr
    // Disconnect?
// if cmd = /show
    // Show all messages received
// if cmd = /show <usr>
    // Show messages received from user
// else
    // Send cmd to server

void process_cmd(char input[], int *namefd, rio_t *rio){
    const char s[2] = " ";
    char *request = (char*) malloc(BUFSIZE);
    memset(request, 0, strlen(request)); // Flush reply
    char *token;
    char *cmd;
    cmd = strdup(input);
    token = strtok(cmd, s);
    
    if (strcmp(token, "/msg") == 0) {            // /msg <usr> <msg>
        char *host = (char*) malloc(32);
        char *port = (char*) malloc(32);
        // Lookup user in name_server
        // Establish connection to chat server
        // Send message
        // Close connection
        printf("msg\n");
        Rio_writen(*namefd, input, strlen(input)); // Send request
	    Rio_readlineb(rio, input, BUFSIZE);        // Read reply
        cmd = strdup(input);
        token = strtok(cmd, s);
        host = token;
        strtok(NULL, s);
        port = token;

	    Fputs(input, stdout);                      // Print reply
    } else if (strcmp(token, "/show\n") == 0) {  // /show
        printf("show\n");
    } else if (strcmp(token, "/show") == 0) {    // /show <usr>
        printf("show usr\n");
    } else {                                     // Otherwise
        Rio_writen(*namefd, input, strlen(input)); // Send request
	    Rio_readlineb(rio, input, BUFSIZE);        // Read reply
	    Fputs(input, stdout);                      // Print reply
    }
}

int process_reply(char input[])
{   
    if (strcmp(input, "You are now logged out.\n") == 0) {
        return -1;
    }
    return 0;
}

void login(char args[]) {
    int name_serverfd;
    char *buffer = (char*) malloc(BUFSIZE);
    char *name_server_host, *name_server_port;
    rio_t rio;
    pthread_t tid;

    name_server_host = "localhost";
    name_server_port = "10000";
    name_serverfd = Open_clientfd(name_server_host, name_server_port);

    // 'SPECIAL' HANDSHAKE
    Rio_readinitb(&rio, name_serverfd);            // Setup rio
    Rio_writen(name_serverfd, args, strlen(args)); // Send login args to server
    Rio_readlineb(&rio, buffer, BUFSIZE);        // Read reply
    if (!(strcmp(buffer, "You are now logged in.\n") == 0)) {
        // Close connection if reply not affirmative
	    Fputs(buffer, stdout);      
        Close(name_serverfd); 
        return;
    }
    Fputs(buffer, stdout); // Print server reply to stdout

    // START CHAT SERVER THREAD
    // Extract port number from login command
    const char s[2] = " ";
    char *token;
    char *cmd;
    cmd = strdup(args);
    token = strtok(cmd, s);
    for (int i = 0; i < 4; i++) {
        token = strtok(NULL,s);
    }
    printf("token %s", token);
    char *port = malloc(10);
    port = "8181";
    // int listenfd, *connfdp;
    Pthread_create(&tid, NULL, chat_server, port);

    // Echo-like continuous, open connection
    while (Fgets(buffer, BUFSIZE, stdin) != NULL) {
        // Process cmds
        // Send cmds to appropriate server
        process_cmd(buffer, &name_serverfd, &rio);
        
        if (process_reply(buffer) == -1) {
            break;
        }
    }
    free(buffer);
    Pthread_cancel(tid);
    Close(name_serverfd); //line:netp:echoclient:close
    return;
}

void *chat_server(void *port_arg) {
    char *port = (char *)port_arg;
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    listenfd = Open_listenfd(port);

    while (1) {
        clientlen=sizeof(struct sockaddr_storage);
	    connfdp = Malloc(sizeof(int)); //line:conc:echoservert:beginmalloc
	    *connfdp = Accept(listenfd, (SA *) &clientaddr, &clientlen); //line:conc:echoservert:endmalloc
	    Pthread_create(&tid, NULL, thread, connfdp);
    }
    return NULL;
}

/* Thread routine */
void *thread(void *vargp) 
{  
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    // echo(connfd);
    Close(connfd);
    return NULL;
}

/*
 * Based on csapp echoclient.c - an echo client
 */
int main(int argc, char **argv) 
{
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        fprintf(stderr, "usage: %s \n", argv[0]);
	    exit(0);
    }

    char *buffer = (char*) malloc(BUFSIZE);
    const char s[2] = " ";
    char *token;
    char *cmd;
    
    while (Fgets(buffer, BUFSIZE, stdin) != NULL) {
        cmd = strdup(buffer);
        token = strtok(cmd, s);

        if (strcmp(token, "/login") == 0){
            Fputs("Establishing connection ...\n", stdout);
            login(buffer);
        }
        else if (strcmp(token, "/exit\n") == 0){
            Fputs("Exiting ...\n", stdout);
            exit(0);
        }
        else {
            Fputs("Available commands:\n/login <nick> <password> <ip> <port>\n/exit\n", stdout);
        }
        
        free(cmd);
    }
    
    exit(0);
}
