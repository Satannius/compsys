#include "csapp.h" //You can remove this if you do not wish to use the helper functions
#define USERNUM 2

/*
* User list struct and functions
*/
struct list{
    int  port;
    int  logged_in;
    char name[32];
    char password[32];
    char ip[32];
};

/*
* Command parsing functions
*/
// Attempt to log in user. Set reply to result of attempt.
// Handle cases where user is already logged in and invalid login credentials.
void login(struct list *usr_lst, char *token, char reply[], int *usr_id);

// Lookup all users.
void lookup(struct list *usr_lst, char *token, char reply[]);

// Lookup specific user.
void lookup_nick(struct list *usr_lst, char *token, char reply[]);

// Logout user. Set reply to confirmation of request.
// connect_peer updates user information.
void logout(char reply[]);

// Interpret the command and call appropriate functions. Copy reply to output.
// Print status information to stdout.
void process_cmd(struct list *usr_lst, char input[], char output[], int *usr_id);

/*
 * Based on csapp echo - read and echo text lines until client closes connection
 */
/* $begin echo */
void connect_peer(int connfd);
