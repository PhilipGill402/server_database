#include "database.h"

/* 
 *
 * VALUE FUNCTIONS
 *
 * */

value_t make_int(int val) {
    value_t ret = {
        .value.i = val,
        .type = INT
    };

    return ret;
}

value_t make_double(double val) {
    value_t ret = {
        .value.d = val,
        .type = DOUBLE 
    };

    return ret;
}

value_t make_string(char* val) {
    value_t ret = {
        .value.s = val,
        .type = STRING
    };

    return ret;
}

int values_equal(value_t a, value_t b) {
    // if the types aren't equal then they aren't equal 
    if (a.type != b.type) {
        return 0;
    }

    switch (a.type) {
        case INT: return a.value.i == b.value.i;
        case DOUBLE: return a.value.d == b.value.d;
        case STRING: return a.value.s == b.value.s;
        default: return 0; 
    }
}

void print_value(value_t val) {
    switch (val.type) {
        case INT: printf("%d", val.value.i); return;
        case DOUBLE: printf("%f", val.value.d); return;
        case STRING: printf("%s", val.value.s); return;
        case NOTFOUND: printf("Could not find value at key: %s", val.value.s); return;
        case NONE: printf("NONE"); return; 
        default: printf("Type not recognized"); return;
    } 
}

char* val_to_str(value_t val) {
    char str[100]; 
    switch (val.type) {
        case INT: sprintf(str, "%d\n", val.value.i); break;
        case DOUBLE: sprintf(str, "%f\n", val.value.d); break;
        case STRING: strcat(str, val.value.s); strcat(str, "\n"); break;
        case NOTFOUND: strcat(str, "NOT FOUND\n"); break;
        case NONE: strcat(str, "NONE\n"); break;
        default: strcat(str, "TYPE NOT RECOGNIZED\n"); break;
    }

    return str;
}

/* 
 *
 * ENTRY FUNCTIONS
 *
 * */

entry_t create_entry(char* key, value_t val) {
    entry_t entry = {
        .val = val,
        .key = strdup(key) 
    };
    
    if (val.type == STRING) {
        entry.val.value.s = strdup(val.value.s);
    }

    return entry; 
}

void print_entry(entry_t entry) {
    printf("{%s: ", entry.key);
    print_value(entry.val);
    printf("}");
}

/* 
 *
 * DATABASE FUNCTIONS
 *
 * */

//TODO: cache powers of p for optimization
int hash_str(char* key) {
    unsigned int p = 67;
    unsigned int hash = 0; 

    for (int i = 0; i < strlen(key); i++) {
        hash += (key[i] - 'a' + 1) * (int)pow((double)p, (double)i);
    }
    
    hash = hash % (unsigned)(MAX_CAPACITY);
    return (int)hash;
}

database_t init_database() {
    database_t database = {
        .size = 0,
        .head = 0
    };
    
    // initializes free list
    for (int i = 0; i < MAX_CAPACITY - 1; i++) {
        database.nodes[i].next = i + 1;
    } 

    database.nodes[MAX_CAPACITY - 1].next = -1;

    // set all entries to have a next of -2 to tell later if they've been filled
    for (int i = 0; i < MAX_CAPACITY; i++) {
        database.entries[i] = -1;
    } 

    return database;
}

int get_node(database_t* database) {
    if (database->head == -1) {
        fprintf(stderr, "Couldn't allocate more space, ran out of memory\n");
        abort();
    }
    
    int idx = database->head;                    // index of next free node
    database->head = database->nodes[idx].next;
    return idx;
}

void free_node(database_t* database, int idx) {
    database->nodes[idx].next = database->head; // sets the first node in the free list to the deleted node
    database->head = idx;                       // sets the head to the deleted node
}

void set(database_t* database, char* key, value_t value) {
    entry_t entry = create_entry(key, value);
    unsigned int hash = hash_str(key);
    
    
    
    //if nothing is at the hash create a new node
    if (database->entries[hash] == -1) {
        int idx = get_node(database);
        database->nodes[idx].entry = entry;
        database->nodes[idx].next = -1;

        database->entries[hash] = idx; 
    } else {
        node_t* curr_node = &database->nodes[database->entries[hash]];
        while(curr_node->next != -1 && strcmp(curr_node->entry.key, key) != 0) {
            curr_node = &database->nodes[curr_node->next];
        }
        
        // if the key already exists then just replace the value
        if (strcmp(curr_node->entry.key, key) == 0) {
            curr_node->entry = entry;
        } else {
            // create new node and assign it to the next node in the chain 
            int idx = get_node(database);
            database->nodes[idx].entry = entry;
            database->nodes[idx].next = -1;

            curr_node->next = idx;
        }

    } 
}

value_t get(database_t* database, char* key) {
    int hash = hash_str(key);
     
    // there is no node with this hash in nodes
    if (database->entries[hash] == -1) {
        value_t ret = {
            .type = NOTFOUND,
            .value.s = key
        };

        return ret; 
    }

    node_t curr_node = database->nodes[database->entries[hash]];
    while(strcmp(key, curr_node.entry.key) != 0 && curr_node.next != -1) {
        curr_node = database->nodes[curr_node.next];
    }
    
    // check if we found the node 
    if (strcmp(key, curr_node.entry.key) == 0) {
        return curr_node.entry.val;
    }
    
    value_t ret = {
        .type = NOTFOUND,
        .value.s = key
    };

    return ret;
}

void del(database_t* database, char* key) {
    int hash = hash_str(key);
    
    // hash not found so just return
    if (database->entries[hash] == -1) {
        return;
    }
    
    node_t* prev_node = NULL;
    node_t* curr_node = &database->nodes[database->entries[hash]];
    while(strcmp(key, curr_node->entry.key) != 0 && curr_node->next != -1) {
        prev_node = curr_node;
        curr_node = &database->nodes[curr_node->next];
    }
    
    
    // check if we found the node 
    if (strcmp(key, curr_node->entry.key) == 0) {
        int idx; 
        if (prev_node == NULL) {
            idx = database->entries[hash];
            database->entries[hash] = -1;   // we need to set the node index at the hash to -1 because there are no longer any nodes that correspond to that hash
        } else {
            idx = prev_node->next;
            prev_node->next = curr_node->next; 
        }

        free_node(database, idx); 
         
    }
    
    return;
}

int exists(database_t* database, char* key) {
    int hash = hash_str(key);

    if (database->entries[hash] == -1) {
        return 0;
    }

    node_t curr_node = database->nodes[database->entries[hash]];
    while (strcmp(key, curr_node.entry.key) != 0 && curr_node.next != -1) {
        curr_node = database->nodes[curr_node.next];
    }

    if (strcmp(key, curr_node.entry.key) == 0) {
        return 1;
    }

    return 0;
}

/*
char msg[strlen(key)];
for (int i = 0; i < strlen(key); i++) {
    msg[i] = *key++;
}
*/
