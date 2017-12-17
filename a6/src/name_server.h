#include "csapp.h" //You can remove this if you do not wish to use the helper functions
#define USERNUM 2

/*
* 
* User list struct and functions
*
*/
struct list{
    int  port;
    int  logged_in;
    char name[32];
    char password[32];
    char ip[32];
    struct list *next;
};

/*
* 
* Command parsing functions
*
*/
void login(struct list *usr_lst, char *token, char reply[], int *usr_id)
{
    const char s[2] = " ";
    token = strtok(NULL, s);
    
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
    char result[MAXLINE];
    // write a reply with all online users' info
    for (int i = 0; i < USERNUM; i++) {
        if (usr_lst[i].logged_in == 1) {  
            sprintf(result, "Nick: %s, IP: %s, Port: %i ", 
                    usr_lst[i].name, usr_lst[i].ip, usr_lst[i].port);
            strcat(reply, result);
        }    
    }
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

void process_cmd(struct list *usr_lst, char input[], char output[], int *usr_id)
{
    const char s[2] = " ";
    char reply[MAXLINE];
    memset(&reply[0], 0, sizeof(reply)); // Flush reply
    char *token;
    token = strtok(strdup(input), s);
    
    if (strcmp(token, "/login") == 0) {
        login(usr_lst, token, reply, usr_id);
    } else if (strcmp(token, "/logout\n") == 0) {
        logout(reply);
    } else if (strcmp(token, "/lookup\n") == 0) {
        lookup(usr_lst, token, reply);
    } else if (strcmp(token, "/lookup") == 0) {
        lookup_nick(usr_lst, token, reply);
    } else {
        sprintf(reply, "Available commands: /lookup, /lookup <nick>, /logout");
    }

    strcat(reply, "\n");
    strcpy(output, reply); // Write reply via pointer to output
    printf("Received input: %s", input); // Notice if input contains linebreak
    printf("Replying %s", reply);
}
