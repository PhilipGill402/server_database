#include "redis.h"

int main() {
    
    value_t i = make_int(10);
    value_t d = make_double(10.98);
    value_t s = make_string("hello");
    
    database_t database = init_database();
    
    set(&database, "hello", d);

    value_t val = get(&database, "hello");
    print_value(val);
    del(&database, "hello");

    return 0;
}
