#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <stdlib.h>

typedef struct {
    size_t size;                                               // Element size
    void (*copy)(void *destination, const void *source);       // Copy function
    void (*free)(void *elem);                                  // Free mem function
    int (*compare)(const void *a, const void *b);              // Compare function
} TypeInfo;

TypeInfo *get_string_type_info();

TypeInfo *get_function_type_info();

#endif // TYPE_INFO_H