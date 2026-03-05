#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include "type_info.h"

typedef struct {
    void *data;        // Array of pointers to elements
    TypeInfo *type;    // Information about type of elements
    size_t capacity;   // Current capacity of the array
    size_t size;       // Current number of elements
} DynArr;

DynArr *dynamic_array_create(TypeInfo *type);
int dynamic_array_init(DynArr *da, size_t initial_capacity);
void dynamic_array_destroy(DynArr *da);

int dynamic_array_push_back(DynArr *da, const void *elem);
void *dynamic_array_get(const DynArr *da, size_t index);

DynArr *dynamic_array_map(const DynArr *da, void (*func)(void *elem));
DynArr *dynamic_array_where(const DynArr *da, int (*predicate)(const void *elem));
DynArr *dynamic_array_concat(const DynArr *da1, const DynArr *da2);

#endif // DYNAMIC_ARRAY_H