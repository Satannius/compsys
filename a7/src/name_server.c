#include <stdio.h>
#include <assert.h>
#include "csapp.h"
#include "name_server.h"
#define ARGNUM 1 // TODO: Put the number of you want to take
#define BUFSIZE 128

static struct list user_list[2];

void init_users() {
    strcpy(user_list[0].name, "banana");
    strcpy(user_list[0].password, "123");
    strcpy(user_list[1].name, "fisk");
    strcpy(user_list[1].password, "654");
    user_list[0].logged_in = 0;
    user_list[1].logged_in = 0;
}

void login(struct list *usr_lst, char *token, char reply[], int *usr_id)
{
    const char s[2] = " ";
    token = strtok(NULL, s);

    if (*usr_id >= 0) {
        sprintf(reply, "You are already logged in.");
        return;
    }
    
    for (int i = 0; i < USERNUM; i++) {
        // Ensure user is not already logged in.
        if ((strcmp(token, usr_lst[i].name) == 0) && usr_lst[i].logged_in == 1) {
            sprintf(reply, "This user is already logged in.");
            return;
        }
        // If user is found and password correct, set values
        if (strcmp(token, usr_lst[i].name) == 0) {
            token = strtok(NULL, s);
            if (strcmp(token, usr_lst[i].password) == 0) {
                token = strtok(NULL, s);
                strcpy(usr_lst[i].ip, token);
                token = strtok(NULL, s);
                char *target = strtok(token, "\n");
                usr_lst[i].port = atoi (target);
                usr_lst[i].logged_in = 1;
                *usr_id = i;  
                sprintf(reply, "You are now logged in.");
                return;
            }
        }
    }
    // Error msg if no user matched or wrong pass
    sprintf(reply, "Invaild nick or password.");
    return;
}

void lookup(struct list *usr_lst, char *token, char reply[])
{
    const char s[2] = " ";
    token = strtok(NULL, s);
    int online = 0;
    // counts online users 
    for (int i = 0; i < USERNUM; i++) {
        if (usr_lst[i].logged_in == 1)  
            online++;
    }
    sprintf(reply, "People online: %i  ", online);
    char *result = (char*) malloc(BUFSIZE);
    // write a reply with all online users' info
    for (int i = 0; i < USERNUM; i++) {
        if (usr_lst[i].logged_in == 1) {  
            sprintf(result, "Nick: %s, IP: %s, Port: %i ", 
                    usr_lst[i].name, usr_lst[i].ip, usr_lst[i].port);
            strcat(reply, result);
        }    
    }
    free(result);
}

void lookup_nick(struct list *usr_lst, char *token, char reply[])
{
    const char s[2] = " ";
    token = strtok(NULL, s);
    char *target = strtok(token, "\n");
    int found = 0; // set to 1 if a match is found 
    for (int i = 0; i < USERNUM; i++) {
        if ((strcmp(usr_lst[i].name, target) == 0) && 
            (usr_lst[i].logged_in == 1)) {
            found = 1;
            sprintf(reply, "%s is online, IP: %s, Port: %i ",
                    usr_lst[i].name, usr_lst[i].ip, usr_lst[i].port);
            return;
        }
        else if ((strcmp(usr_lst[i].name, target) == 0) && 
                (usr_lst[i].logged_in == 0)) {
            found = 1;
            sprintf(reply, "%s is offline.", usr_lst[i].name);
            return;
        }
    }
    if (found == 0)
        // Error msg if there is no match
        sprintf(reply, "%s is not found.", target);
        return;
        
}

void logout(char reply[])
{
    sprintf(reply, "You are now logged out.");
}

void msg(struct list *usr_lst, char *token, char reply[])
{
    const char s[2] = " ";
    token = strtok(NULL, s);
    char *target = strtok(token, "\n");
    int found = 0; // set to 1 if a match is found 
    for (int i = 0; i < USERNUM; i++) {
        if ((strcmp(usr_lst[i].name, target) == 0) && 
            (usr_lst[i].logged_in == 1)) {
            found = 1;
            sprintf(reply, "%s %i", usr_lst[i].ip, usr_lst[i].port);
            return;
        }
        else if ((strcmp(usr_lst[i].name, target) == 0) && 
                (usr_lst[i].logged_in == 0)) {
            found = 1;
            sprintf(reply, "%s is offline.", usr_lst[i].name);
            return;
        }
    }
    if (found == 0)
        // Error msg if there is no match
        sprintf(reply, "%s is not found.", target);
        return;
        
}

void process_cmd(struct list *usr_lst, char input[], char output[], int *usr_id)
{
    const char s[2] = " ";
    char *reply = (char*) malloc(BUFSIZE);
    memset(reply, 0, strlen(reply)); // Flush reply
    char *token;
    char *cmd;
    cmd = strdup(input);
    token = strtok(cmd, s);
    
    if (strcmp(token, "/login") == 0) {
        login(usr_lst, token, reply, usr_id);
    } else if (strcmp(token, "/logout\n") == 0) {
        logout(reply);
    } else if (strcmp(token, "/lookup\n") == 0) {
        lookup(usr_lst, token, reply);
    } else if (strcmp(token, "/lookup") == 0) {
        lookup_nick(usr_lst, token, reply);
    } else if (strcmp(token, "/msg") == 0) {
        msg(usr_lst, token, reply);
    } else {
        sprintf(reply, "Available commands: /lookup, /lookup <nick>, /logout");
    }


    strcat(reply, "\n");
    strcpy(output, reply); // Write reply via pointer to output
    
    free(cmd);
    free(reply);
    
    printf("Received input: %s  from: %s\n", input, usr_lst[*usr_id].name); // Notice if input contains linebreak
    printf("Replying %s  to: %s\n", reply, usr_lst[*usr_id].name);
}

/*
 * Based on csapp echo - read and echo text lines until client closes connection
 */
/* $begin echo */
void connect_peer(int connfd)
{
    size_t n;
    char *input_buf = (char*) malloc(BUFSIZE);
    char *output_buf = (char*) malloc(BUFSIZE);
    rio_t rio;
    int usr_id = -1;

    Rio_readinitb(&rio, connfd);

    while((n = Rio_readlineb(&rio, input_buf, BUFSIZE)) != 0) { //line:netp:echo:eof
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
    free(input_buf);
    free(output_buf);
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
    connect_peer(connfd);
    Close(connfd);
    printf("Closing connection thread.\n");
    return NULL;
}


