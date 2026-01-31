#include "redis.h"

int main() {
    
    value_t i = make_int(10);
    value_t d = make_double(10.98);
    value_t s = make_string("hello");

    value_t ii = make_int(1000);
    value_t dd = make_double(1.99999);
    value_t ss = make_string("hello world!");
    
    database_t database = init_database();
    
    set(&database, "hello", i);
    set(&database, "world", d);
    set(&database, "what", s);

    value_t val = get(&database, "hello");
    print_value(val);
    printf("\n"); 
    value_t val1 = get(&database, "world");
    print_value(val1);
    printf("\n");
    value_t val2 = get(&database, "what");
    print_value(val2);
    printf("\n");
    
    set(&database, "hello", ii);
    set(&database, "world", dd);
    set(&database, "what", ss);

    val = get(&database, "hello");
    print_value(val);
    printf("\n"); 
    val1 = get(&database, "world");
    print_value(val1);
    printf("\n");
    val2 = get(&database, "what");
    print_value(val2);
    printf("\n");

    printf("%d\n", exists(&database, "hello"));
    printf("%d\n", exists(&database, "world"));
    printf("%d\n", exists(&database, "what"));
    printf("%d\n", exists(&database, "up"));
    return 0;
}
