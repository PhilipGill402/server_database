#ifndef INC_DATABASE_H_
#define INC_DATABASE_H_

#include <stdio.h>

#define STARTING_CAPACITY 100

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
    entry_t* database;
} database;
#endif
