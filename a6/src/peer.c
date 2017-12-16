#include <stdio.h>
#include "csapp.h"
#include "peer.h"

#define ARGNUM 2 // TODO: Put the number of you want to take

/*
 * Based on csapp echoclient.c - An echo client
 */
/* $begin echoclientmain */
int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	    exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        Rio_writen(clientfd, buf, strlen(buf));
	    Rio_readlineb(&rio, buf, MAXLINE);
        if (strcmp(buf, "\\exit\n") == 0){
            Fputs("Bye!\n", stdout);
            exit(0);
       }
	    //Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
