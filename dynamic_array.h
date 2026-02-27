#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include "type_info.h"

typedef struct {
    void *data;        // Array of pointers to elements
    TypeInfo *type;    // Information about type of elements
    size_t capacity;   // Current capacity of the array
    size_t size;       // Current number of elements
} dynArr;

dynArr *dynamic_array_create(TypeInfo *type);
int dynamic_array_init(dynArr *da, size_t initial_capacity);
void dynamic_array_destroy(dynArr *da);

int dynamic_array_push_back(dynArr *da, const void *elem);
void *dynamic_array_get(const dynArr *da, size_t index);

dynArr *dynamic_array_map(const dynArr *da, void (*func)(void *elem));
dynArr *dynamic_array_where(const dynArr *da, int (*predicate)(const void *elem));
dynArr *dynamic_array_concat(const dynArr *da1, const dynArr *da2);

#endif // DYNAMIC_ARRAY_H