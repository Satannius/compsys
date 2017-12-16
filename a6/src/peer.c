#include <stdio.h>
#include "csapp.h"
#include "peer.h"

#define ARGNUM 0 // TODO: Put the number of you want to take

void login(char args[]) {
    printf("Establishing connection ...");
    printf("Received args: %s", args);

    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    host = "localhost";
    port = "10000";

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    // PERFORM SPECIAL HANDSHAKE
    // If fails: break

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        Rio_writen(clientfd, buf, strlen(buf)); // Send request
	    Rio_readlineb(&rio, buf, MAXLINE);      // Read reply
	    Fputs(buf, stdout);                     // Print reply
    }
    Close(clientfd); //line:netp:echoclient:close
}

/*
 * Based on csapp echoclient.c - An echo client
 */
/* $begin echoclientmain */
int main(int argc, char **argv) 
{
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        fprintf(stderr, "usage: %s \n", argv[0]);
	    exit(0);
    }

    char buf[MAXLINE];
    const char s[2] = " ";
    char *token;
    
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        token = strtok(strdup(buf), s);
        printf("%s\n", token);
        
	    // Fputs(buf, stdout); // Echo user input.
        if (strcmp(token, "/login") == 0){
            Fputs("Establishing connection ...\n", stdout);
            login(buf);
        }
        else if (strcmp(token, "/exit\n") == 0){
            Fputs("Exiting ...\n", stdout);
            exit(0);
        }
        else {
            Fputs("Available commands:\n/login <nick> <password> <ip> <port>\n/exit\n", stdout);
        }
    }
    
    
    exit(0);
}
/* $end echoclientmain */
