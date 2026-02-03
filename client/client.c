#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <poll.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_MESSAGE_LENGTH 1024
#define MAX_USERNAME_LENGTH 128
#define MAX_MESSAGE_BACKLOG 1024

int running = 1;

void signal_handler(int sig){
    running = 0;
}

int main(){
    struct sockaddr_in address;
    socklen_t address_length = sizeof(address);
    int ret;
    int bytes_sent;

    signal(SIGINT, signal_handler); 

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0){
        perror("socket"); 
        close(client_fd);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons(SERVER_PORT);

    ret = connect(client_fd, (struct sockaddr*)&address, address_length);
    if (client_fd < 0){
        perror("connect");
        close(client_fd);
        return -1;
    }    
    
    printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);

    while (running){
        char in_buffer[MAX_MESSAGE_LENGTH];
        printf(">> ");
        fgets(in_buffer, MAX_MESSAGE_LENGTH - 2, stdin);
        
        in_buffer[strlen(in_buffer) - 1] = '\0';
        
        bytes_sent = send(client_fd, in_buffer, strlen(in_buffer), 0);
        if (bytes_sent < 0){
            perror("send");
            break;
        }
        
        char out_buffer[MAX_MESSAGE_LENGTH];

        int bytes_read = recv(client_fd, out_buffer, MAX_MESSAGE_LENGTH - 1, 0);
        if (bytes_read < 0){
            perror("recv");
            break;
        }
       
        out_buffer[bytes_read] = '\0';

        if (strcmp(out_buffer, "EXIT890\t") == 0) {
            break;
        }

        printf("%s", out_buffer);
    } 
    
    running = 0;
    close(client_fd);
    printf("Disconnected\n");
    return 0;
}
