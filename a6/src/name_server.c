#include <stdio.h>
#include "csapp.h"
#include "name_server.h"
#define ARGNUM 1 // TODO: Put the number of you want to take
#define USERNUM 2

struct list{
    int  port;
    int  logged_in;
    char name[32];
    char password[32];
    char ip[32];
    struct list *next;
};

static struct list user_list[2];

const char *process_cmd(char buf[])
{
    const char s[2] = " ";
    char *token;
    token = strtok(strdup(buf), s);
    printf("first token: %s \n", token);
    
    if (strcmp(token, "/login") == 0){
        // verify the user and password 
        token = strtok(NULL, s);
        printf("token: %s \n", token);
        for (int i = 0; i < USERNUM; i++){
            if (user_list[i].logged_in == 1){
                printf("This user is already logged in. \n");
                return ("User %s is already logged in. \n", user_list[i].name);
            }
            if (strcmp(token, user_list[i].name) == 0){
                // continue with this user
                printf("user %s is found. \n", user_list[i].name);
                token = strtok(NULL, s);
                if (strcmp(token, user_list[i].password) == 0){
                    printf("Password is correct. \n");
                    token = strtok(NULL, s);
                    user_list[i].port = atoi (token);
                    printf("port: %d \n", user_list[i].port);
                    token = strtok(NULL, s);
                    strcpy(user_list[i].ip, token);
                    printf("IP: %s ", token);
                    user_list[i].logged_in = 1;
                    return ("Your are now online.");
                }
                else{
                    printf("Invaild user or password. \n");
                    return ("Invaild user or password. \n");
                }
            }
        }
    }

    //else if (){
    //    //logout
    //}

    else if (strcmp(token, "/lookup\n") == 0){
        int online = 0;
        for (int i = 0; i < USERNUM; i++){
            if (user_list[i].logged_in == 1)  
                online++;
        }
        printf("People online: %i \n", online);
        for (int i = 0; i < USERNUM; i++){
            if (user_list[i].logged_in == 1){   
                printf("looking up people \n");
                printf("Nick: %s \n", user_list[i].name);
                printf("Ip: %s", user_list[i].ip);
                printf("port: %i \n", user_list[i].port);
            }
        }
    }

    else if (strcmp(token, "/lookup") == 0){
        token = strtok(NULL, s);
        char *target = strtok(token, "\n");
        printf("target: %s\n", target);
        int found = 0;
        for (int i = 0; i < USERNUM; i++){
            if ((strcmp(user_list[i].name, target) == 0) && 
                (user_list[i].logged_in == 1)){
                found = 1;
                printf("%s is online. \n", user_list[i].name);
                printf("IP: %s\n", user_list[i].ip);
                printf("Port: %i", user_list[i].port);
                return ("%s is online. \nIP: %s\nPort: %i", user_list[i].name, 
                        user_list[i].ip, user_list[i].port);
            }
            else if ((strcmp(user_list[i].name, target) == 0) && 
                    (user_list[i].logged_in == 0)){
                found = 1;
                printf("%s is offline. \n", user_list[i].name);
                return ("%s is offline. \n", user_list[i].name);
            }
        }
        if (found == 0)
            printf("%s is not found. \n", target);
            return ("%s is not found. \n", target);
        
    }

    else {
        printf("You can either log in or exit the program. \n");
        return ("You can either log in or exit the program. \n");
    }

    //printf("Received: %s \n", buf);
}

/*
 * Based on csapp echo - read and echo text lines until client closes connection
 */
/* $begin echo */
void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    char *feedback[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
	    printf("server received %d bytes\n", (int)n);
        feedback = process_cmd(buf);
	    Rio_writen(connfd, feedback, n);
    }

    printf("%s", buf);
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

    strcpy(user_list[0].name, "banana");
    strcpy(user_list[0].password, "123456");
    strcpy(user_list[1].name, "fisk");
    strcpy(user_list[1].password, "654321");
    user_list[0].logged_in = 0;
    user_list[1].logged_in = 0;
    //printf("user_list: %s\n", user_list[0].name);
    //printf("user_list: %s\n", user_list[0].password);
    //printf("user_list: %i\n", user_list[0].port);

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
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    echo(connfd);
    Close(connfd);
    return NULL;
}


