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

/* https://stackoverflow.com/questions/4384359/quick-way-to-implement-dictionary-in-c 
struct nlist *lookup(char *s)
{
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
          return np; //found
    return NULL;     //not found 
}
*/

void process_cmd(char buf[])
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
            if (user_list[i].logged_in == 0){
                printf("This user is already logged in. \n");
                break;
            }
            if (strcmp(token, user_list[i].name) == 0){
                // continue with this user
                printf("user %s found. \n", user_list[i].name);
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
                    break;
                }
            }
        printf("Invaild user or password. \n");
        }
    }


    else if (){
        //logout
    }

    else if (strcmp(token, "/lookup\n") == 0){
        //lookup
    }

    else if (strcmp(token, "/lookup") == 0){

    }

    else {
        printf("You can either log in or exit the program. \n");
    }

    token = strtok(NULL, s);
    printf("Received: %s \n", buf);
}

int lookup(char token){
    int online = 0;
    for (int i = 0; i < USERNUM; i++){
        if (){
            strcmp(user_list[i].name, token);
        }

    }

}

/*
 * Based on csapp echo - read and echo text lines until client closes connection
 */
/* $begin echo */
void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
	    printf("server received %d bytes\n", (int)n);
        process_cmd(buf);
	    Rio_writen(connfd, buf, n);
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


