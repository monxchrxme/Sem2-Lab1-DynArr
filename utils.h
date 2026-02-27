#ifndef UTILS_H
#define UTILS_H

//================ FUNCS FOR STRINGS ================

void to_uppercase(void *elem);
void to_lowercase(void *elem);
int is_long_string(const void *elem);
int is_short_string(const void *elem);

//================ FUNCS FOR POINTERS TO FUNCS ================

void print_function_address(void *elem);
int is_non_null_function(const void *elem);

// Example func for testing
void example_function(void *data);

#endif // UTILS_H