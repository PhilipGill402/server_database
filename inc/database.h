#ifndef INC_DATABASE_H_
#define INC_DATABASE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_CAPACITY 100000

enum Type {
    INT,
    DOUBLE,
    STRING
};

typedef struct {
    union {
        int i;
        double d;
        char* s;
    } value;
    enum Type type;
} value_t;

value_t make_int(int val);
value_t make_double(double val);
value_t make_string(char* val);
void print_value(value_t val);

typedef struct {
    value_t val;
    char* key;
} entry_t;

entry_t create_entry(char* key, value_t val);
void print_entry(entry_t entry);

typedef struct {
    entry_t entry; 
    int next;
} node_t;

typedef struct {
    node_t nodes[MAX_CAPACITY];         // stores all nodes free or used
    int entries[MAX_CAPACITY];          // stores index to first node in bucket 
    int size;                           // keeps track of size to make sure it doesn't excede MAX_CAPACITY
    int head;                           // keeps track of first free node
} database_t;

unsigned int hash_str(char* key);
database_t init_database();
void set(database_t* database, char* key, value_t value);
value_t get(database_t* database, char* key);
void del(database_t* database, char* key);
#endif
