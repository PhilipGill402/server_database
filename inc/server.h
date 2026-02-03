#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <poll.h>
#include <ctype.h>
#include "database.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_CLIENTS 64
#define MAX_MESSAGE_LENGTH 1024

typedef struct {
    value_t value; 
    char* key; 
    char* command;
} command_t;

struct args {
    int client_fd;
    database_t* database;
};

void* handle_client(void* arg);
command_t parse_input(char* input);

#endif // !SRC_SERVER_H_
