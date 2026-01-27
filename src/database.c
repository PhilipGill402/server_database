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

void print_value(value_t val) {
    switch (val.type) {
        case INT: printf("%d", val.value.i); break;
        case DOUBLE: printf("%f", val.value.d); break;
        case STRING: printf("%s", val.value.s); break;
        default: printf("Type not recognized");
    } 
}

/* 
 *
 * ENTRY FUNCTIONS
 *
 * */

entry_t create_entry(char* key, value_t val) {
    entry_t entry = {
        .val = val,
        .key = key
    };

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
unsigned int hash_str(char* key) {
    unsigned int p = 67;
    int hash = 0; 

    for (int i = 0; i < strlen(key); i++) {
        hash += (key[i] - 'a' + 1) * (int)pow((double)p, (double)i);
    }
    
    hash = hash % (unsigned)(MAX_CAPACITY);

    return hash;
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
    database->nodes[idx].next = database->head;   // sets the first node in the free list to the deleted node
    database->head = idx;                        // sets the head to the deleted node
}

void set(database_t* database, char* key, value_t value) {
    entry_t entry = create_entry(key, value);
    unsigned int hash = hash_str(key);
    
    int idx = get_node(database);
    database->nodes[idx].entry = entry;
    database->nodes[idx].next = -1;
    
    //if nothing is at the hash
    if (database->entries[hash] == -1) {
        database->entries[hash] = idx; 
    } else {
        node_t curr_node = database->nodes[database->entries[hash]];
        while(curr_node.next == -1) {
            curr_node = database->nodes[curr_node.next];
        }

        curr_node.next = idx;
    } 
}

value_t get(database_t* database, char* key) {
    printf("GET NOT YET IMPLEMENTED\n");
}

void del(database_t* database, char* key) {
    printf("DEL NOT IMPLEMENTED\n");
}
