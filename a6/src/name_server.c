#include <stdio.h>
#include <assert.h>
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

void process_cmd(char input[], char output[], int *usr_id)
{
    const char s[2] = " ";
    char reply[MAXLINE]; // PLACEHOLDER
    char *token;
    token = strtok(strdup(input), s);
    memset(&reply[0], 0, sizeof(reply));
    printf("First token: %s \n", token);
    
    if (strcmp(token, "/login") == 0){
        // verify the user and password 
        token = strtok(NULL, s);
        for (int i = 0; i < USERNUM; i++){
            if (user_list[i].logged_in == 1){
                sprintf(reply, "This user is already logged in.");
                break;
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
                    char *target = strtok(token, "\n");
                    printf("target: %s\n", target);
                    strcpy(user_list[i].ip, target);
                    printf("IP: %s \n", target);
                    user_list[i].logged_in = 1;
                    *usr_id = i;
                    sprintf(reply, "You are now online.");
                    break;
                }
                else{
                    sprintf(reply, "Invaild user or password.");
                    break;
                }
            }
        }
    } else if (strcmp(token, "/lookup\n") == 0){
        int online = 0;
        for (int i = 0; i < USERNUM; i++){
            if (user_list[i].logged_in == 1)  
                online++;
        }
        printf("People online: %i \n", online);
        char result[MAXLINE];
        for (int i = 0; i < USERNUM; i++){
            if (user_list[i].logged_in == 1){  
                sprintf(result, "Nick: %s, IP: %s, Port: %i ", 
                        user_list[i].name, user_list[i].ip, user_list[i].port);
                strcat(reply, result);
                printf("reply[%i]: %s\n", i, reply);
            }    
        }
    } else if (strcmp(token, "/lookup") == 0) {
        token = strtok(NULL, s);
        char *target = strtok(token, "\n");
        printf("target: %s\n", target);
        int found = 0;
        for (int i = 0; i < USERNUM; i++){
            if ((strcmp(user_list[i].name, target) == 0) && 
                (user_list[i].logged_in == 1)){
                found = 1;
                sprintf(reply, "%s is online, IP: %s, Port: %i ",
                        user_list[i].name, user_list[i].ip, user_list[i].port);
                break;
            }
            else if ((strcmp(user_list[i].name, target) == 0) && 
                    (user_list[i].logged_in == 0)){
                found = 1;
                sprintf(reply, "%s is offline.", user_list[i].name);
                break;
            }
        }
        if (found == 0)
            sprintf(reply, "%s is not found.", target);
        
        } else {
            sprintf(reply, "You can either log in or exit the program.");
    }

    strcat(reply, "\n");
    strcpy(output, reply); // Write reply via pointer to output
    printf("Received input: %s", input); // Notice if input contains linebreak
    printf("Writing %s to output\n", reply);
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
    int usr_id;

    Rio_readinitb(&rio, connfd);

    // Perform special handshake.

    while((n = Rio_readlineb(&rio, input_buf, MAXLINE)) != 0) { //line:netp:echo:eof
	    // printf("server received %d bytes\n", (int)n);
        process_cmd(input_buf,output_buf,&usr_id); // Processes input and writes to output
        Rio_writen(connfd, output_buf, strlen(output_buf));

        // Flush buffers
        memset(&input_buf[0], 0, strlen(input_buf));
        memset(&output_buf[0], 0, strlen(output_buf));
    }
    printf("User %s disconnected.\n", user_list[usr_id].name);
    user_list[usr_id].logged_in = 0;
}

void *thread(void *vargp);

void init_users() {
    strcpy(user_list[0].name, "banana");
    strcpy(user_list[0].password, "123");
    strcpy(user_list[1].name, "fisk");
    strcpy(user_list[1].password, "654");
    user_list[0].logged_in = 0;
    user_list[1].logged_in = 0;
}

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
    printf("Creating new thread.\n");
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self()); //line:conc:echoservert:detach
    Free(vargp);                    //line:conc:echoservert:free
    echo(connfd);
    Close(connfd);
    printf("Closing thread.\n");
    return NULL;
}


