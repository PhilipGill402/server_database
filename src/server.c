#include"server.h"


int server_fd;
int running = 1;
int active_users[MAX_CLIENTS];
int num_clients = 0;

void signal_handler(int sig){ 
    running = 0;
}

void* handle_client(void* arg){
    struct args* args_ptr = (struct args*)arg;
    int client_fd = args_ptr->client_fd;
    database_t* database = args_ptr->database;

    int bytes_read; 
    int bytes_sent;

    while (1){
        //receives message 
        char buffer[MAX_MESSAGE_LENGTH];
        bytes_read = recv(client_fd, buffer, MAX_MESSAGE_LENGTH - 1, 0);
        if (bytes_read < 0){
            perror("recv");
            continue;
        } else if (bytes_read == 0){
            close(client_fd);
            return NULL; 
        }
        
        buffer[bytes_read] = '\0';
        printf("RECEIVED: %s\n", buffer);
        
        command_t command = parse_input(buffer);

        char response[MAX_MESSAGE_LENGTH];
        response[0] = '\0';
        //handle get command
        if (strcmp(command.command, "get") == 0) {
            if (command.key == NULL) {
                strcat(response, "Error: 'get' command requires key\n");
            } else {
                value_t val = get(database, command.key);
                strcat(response, val_to_str(val));
            }
        } else if(strcmp(command.command, "set") == 0) {
            if (command.key == NULL) {
                strcat(response, "Error: 'set' command requires key\n");
            }

            if (command.value.type == NONE) {
                strcat(response, "Error: 'set' command requires a value\n");
            }
            
            if (command.key != NULL && command.value.type != NONE) {
                set(database, command.key, command.value);
                sprintf(response, "'%s' successfully set\n", command.key);
            }
        } else if (strcmp(command.command, "del") == 0) {
            if (command.key == NULL) {
                strcat(response, "Error: 'del' command requires key\n");
            } else {
                del(database, command.key);
                sprintf(response, "'%s' successfully deleted\n", command.key);
            }
        } else if (strcmp(command.command, "exists") ==  0) {
            if (command.key == NULL) {
                strcat(response, "Error: 'exists' command requires a key\n");
            } else {
                int res = exists(database, command.key);
                
                if (res == 0) {
                    sprintf(response, "'%s' is not in the database\n", command.key);
                } else {
                    sprintf(response, "'%s' is in the database\n", command.key);
                }

            }
        } else if (strcmp(command.command, "exit") == 0) {
            //added random characters to make sure something like this wouldn't be stored in the actual database
            strcat(response, "EXIT890\t"); 
        } else {
            strcat(response, "Error: didn't recognize command\n");
        }
        
        printf("SENDING: %s\n", response);

        size_t to_send = strlen(response);
        size_t sent_total = 0;
        while (sent_total < to_send) {
            ssize_t n = send(client_fd, response + sent_total, to_send - sent_total, 0);
            if (n < 0) {
                perror("send");
                break; // or close client_fd + return NULL
            }
            sent_total += (size_t)n;
        }
    }
    
    close(client_fd);
    return NULL;
}

command_t parse_input(char* input) {
    command_t command; 

    char* func = strtok(input, " ");
    char* key = strtok(NULL, " ");
    char* val = NULL;

    if (key != NULL) {
        val = strtok(NULL, " ");
    }

    if (func && func[strlen(func) - 1] == '\n') {
        func[strlen(func) - 1] = '\0';
    } else if (key && key[strlen(key) - 1] == '\n') {
        key[strlen(key) - 1] = '\0';
    } else if (val && val[strlen(val) - 1] == '\n') {
        val[strlen(val) - 1] = '\0';
    }
    
    //convert the function to all lowercase
    for (int i = 0; func[i]; i++) {
        func[i] = tolower(func[i]);
    } 
    
    value_t value; 
    if (val != NULL) {
        int is_int = 1;
        int is_double = 0;
        for (int i = 0; i < strlen(val); i++) {
            if (val[i] == '.') {
                is_int = 0;
                is_double = 1;
                continue;
            } else if (!isdigit(val[i])) {
                is_int = 0;
                is_double = 0;
                break;
            }
        }

        if (is_int) {
            value = make_int(atoi(val));
        } else if (is_double) {
            value = make_double(strtod(val, NULL));
        } else {
            value = make_string(val);
        }

    } else {
        value.type = NONE;
    }
    


    command.command = func;
    command.key = key; 
    command.value = value;

    return command;
}

int main(){
    database_t database = init_database(); 
    struct sockaddr_in address;
    socklen_t address_len = sizeof(address);
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int ret;
    
    signal(SIGINT, signal_handler);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0){
        perror("socket");
        close(server_fd);
        return -1;
    } 
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons(SERVER_PORT);
   
    ret = bind(server_fd, (struct sockaddr*)&address, address_len);
    if (ret < 0){
        perror("bind");
        close(server_fd);
        return -1;
    }
    

    ret = listen(server_fd, MAX_CLIENTS);
    if (ret < 0){
        perror("listen");
        close(server_fd);
        return -1;
    }
    
    while (running){
        struct pollfd pfd = {.fd = server_fd, .events = POLLIN};
        ret = poll(&pfd, 1, 1000);
        if (ret < 0){
            if (errno == EINTR){
                break;
            }
            perror("poll");
            break;
        } else if (ret == 0){
            continue;
        }

        int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_len);
        if (client_fd < 0){
            perror("accept");
            close(server_fd);
            close(client_fd);
            return -1;
        }
        
        struct args client_args = {
            .client_fd = client_fd,
            .database = &database
        };

        handle_client(&client_args);
    }
    
    printf("\nClosing server\n");
    close(server_fd);
    return 0;
}
