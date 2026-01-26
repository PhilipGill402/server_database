#include "redis.h"

int main() {
    
    value_t i = make_int(10);
    value_t d = make_double(10.98);
    value_t s = make_string("hello");
    entry_t ei = create_entry("int", i); 
    entry_t ed = create_entry("double", d);
    entry_t es = create_entry("string", s);
    

    
    
    return 0;
}
