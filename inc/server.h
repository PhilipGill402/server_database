#ifndef SRC_SERVER_H_
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
#include "database.h"

#define SRC_SERVER_H_

typedef struct {
    value_t value; 
    char* key; 
    char* command;
} command_t;

void* handle_client(void* arg);
command_t parse_command(char* input);

#endif // !SRC_SERVER_H_
