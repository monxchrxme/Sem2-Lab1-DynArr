#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include "type_info.h"

typedef struct {
    void *data;        // Array of pointers to elements
    TypeInfo *type;    // Information about type of elements
    size_t capacity;   // Current capacity of the array
    size_t size;       // Current number of elements
} Vector;

Vector *vector_create(TypeInfo *type);
int vector_init(Vector *vec, size_t initial_capacity);
void vector_destroy(Vector *vec);

int vector_push_back(Vector *vec, const void *elem);
void *vector_get(const Vector *vec, size_t index);

Vector *vector_map(const Vector *vec, void (*func)(void *elem));
Vector *vector_where(const Vector *vec, int (*predicate)(const void *elem));
Vector *vector_concat(const Vector *vec1, const Vector *vec2);

#endif // VECTOR_H