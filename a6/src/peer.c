#include <stdio.h>
#include "csapp.h"
#include "peer.h"

#define ARGNUM 0 // TODO: Put the number of args you want to take

int process_reply(char input[])
{   
    if (strcmp(input, "You are now logged out.\n") == 0) {
        return -1;
    }
    return 0;
}

void login(char args[]) {
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    host = "localhost";
    port = "10000";

    clientfd = Open_clientfd(host, port);

    // 'SPECIAL' HANDSHAKE
    printf("'Special' handshaking...\n");
    Rio_readinitb(&rio, clientfd);            // Setup rio
    Rio_writen(clientfd, args, strlen(args)); // Send login args to server
    Rio_readlineb(&rio, buf, MAXLINE);        // Read reply
    if (!(strcmp(buf, "You are now logged in.\n") == 0)) {
        printf("Failed.\n"); // Close connection if reply not affirmative
	    Fputs(buf, stdout);      
        Close(clientfd); 
        return;
    }
    printf("Succes!\n");
    Fputs(buf, stdout); // Print server reply to stdout

    // Echo-like continuous, open connection
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        Rio_writen(clientfd, buf, strlen(buf)); // Send request
	    Rio_readlineb(&rio, buf, MAXLINE);      // Read reply
	    Fputs(buf, stdout);                     // Print reply
        if (process_reply(buf) == -1) {
            break;
        }
    }
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

    char buf[MAXLINE];
    const char s[2] = " ";
    char *token;
    
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        token = strtok(strdup(buf), s);

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
