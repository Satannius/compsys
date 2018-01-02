#include <stdio.h>
#include "csapp.h"
#include "peer.h"

#define ARGNUM 0 // TODO: Put the number of args you want to take
#define BUFSIZE 128

int process_reply(char input[])
{   
    if (strcmp(input, "You are now logged out.\n") == 0) {
        return -1;
    }
    return 0;
}

void login(char args[]) {
    int clientfd;
    char *buffer = (char*) malloc(BUFSIZE);
    char *host, *port;
    rio_t rio;

    host = "localhost";
    port = "10000";
    printf("args: %s", args);
    clientfd = Open_clientfd(host, port);

    // 'SPECIAL' HANDSHAKE
    printf("'Special' handshaking...\n");
    Rio_readinitb(&rio, clientfd);            // Setup rio
    Rio_writen(clientfd, args, strlen(args)); // Send login args to server
    Rio_readlineb(&rio, buffer, BUFSIZE);        // Read reply
    if (!(strcmp(buffer, "You are now logged in.\n") == 0)) {
        printf("Failed.\n"); // Close connection if reply not affirmative
	    Fputs(buffer, stdout);      
        Close(clientfd); 
        return;
    }
    printf("Succes!\n");
    Fputs(buffer, stdout); // Print server reply to stdout

    // Echo-like continuous, open connection
    while (Fgets(buffer, BUFSIZE, stdin) != NULL) {
        Rio_writen(clientfd, buffer, strlen(buffer)); // Send request
	    Rio_readlineb(&rio, buffer, BUFSIZE);      // Read reply
	    Fputs(buffer, stdout);                     // Print reply
        if (process_reply(buffer) == -1) {
            break;
        }
    }
    free(buffer);
    Close(clientfd); //line:netp:echoclient:close
    return;
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

    int bufsize = 128;
    char *buffer = (char*) malloc(bufsize);
    const char s[2] = " ";
    char *token;
    char *cmd;
    
    while (Fgets(buffer, bufsize, stdin) != NULL) {
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
