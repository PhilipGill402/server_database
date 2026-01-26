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


