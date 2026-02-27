#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//================ FUNCS FOR STRINGS ================

void to_uppercase(void *elem) {
    char **str = (char**)elem;
    for (char *c = *str; *c; c++) {
        *c = toupper(*c);
    }
}

void to_lowercase(void *elem) {
    char **str = (char**)elem;
    for (char *c = *str; *c; c++) {
        *c = tolower(*c);
    }
}

int is_long_string(const void *elem) {
    char **str = (char**)elem;
    return strlen(*str) > 10;
}

int is_short_string(const void *elem) {
    char **str = (char**)elem;
    return strlen(*str) <= 10;
}

//================ FUNCS FOR POINTERS TO FUNCS ================

void print_function_address(void* elem) {
    void (**func)(void*) = (void (**)(void*))elem;
    printf("Function address: %p\n", (void*)(uintptr_t)*func);
}

int is_non_null_function(const void *elem) {
    void (**func)(void*) = (void (**)(void*))elem;
    return *func != NULL;
}

// Example func for testing
void example_function(void *data) {
    printf("Example function called with data: %p\n", data);
}