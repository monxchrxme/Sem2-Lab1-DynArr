#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

Vector *vector_create(TypeInfo *type) {
    Vector *vec = malloc(sizeof(Vector));
    if (!vec) {
        fprintf(stderr, "Memory allocation failed for Vector\n");
        return NULL;
    }

    vec->data = NULL;
    vec->type = type;
    vec->capacity = 0;
    vec->size = 0;

    return vec;
}

int vector_init(Vector *vec, size_t initial_capacity) {
    if (!vec) {
        fprintf(stderr, "Invalid vector pointer\n");
        return 0;
    }

    if (vec->data != NULL) {
        fprintf(stderr, "Vector is already initialized\n");
        return 0;
    }

    if (initial_capacity > 0) {
        vec->data = calloc(initial_capacity, vec->type->size); // to initialize memory with 0
        if (!vec->data) {
            fprintf(stderr, "Memory allocation failed for vector data\n");
            return 0;
        }
        vec->capacity = initial_capacity;
    }

    return 1;
}

void vector_destroy(Vector *vec) {
    if (!vec) return;

    // Release memory from elements
    if (vec->data) {
        for (size_t i = 0; i < vec->size; i++) {
            void *elem = (char*)vec->data + i * vec->type->size;
            vec->type->free(elem);
        }
        free(vec->data);
    }

    // Release memory from vector itself
    free(vec);
}

int vector_push_back(Vector *vec, const void *elem) {
    if (!vec || !elem) return 0;

    // if array is full increase capacity
    if (vec->size == vec->capacity) {
        size_t new_capacity = (vec->capacity == 0) ? 4 : vec->capacity * 2;
        void *new_data = realloc(vec->data, new_capacity * vec->type->size);

        if (!new_data) {
            fprintf(stderr, "Memory allocation failed for vector data\n");
            return 0;
        }

        vec->data = new_data;
        vec->capacity = new_capacity;
    }

    // count address for new elem
    void *dest = (char*)vec->data + vec->size * vec->type->size;

    // copy elem there
    vec->type->copy(dest, elem);
    vec->size++;

    return 1;
}

void *vector_get(const Vector *vec, size_t index) {
    if (!vec || index >= vec->size) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }
    return (char*)vec->data + index * vec->type->size;
}

Vector *vector_map(const Vector *vec, void (*func)(void *elem)) {
    if (!vec || !func) {
        fprintf(stderr, "Error: NULL vector or function pointer\n");
        return NULL;
    }

    if (vec->size == 0) {
        return NULL;
    }

    Vector *result = vector_create(vec->type);
    if (!result) {
        return NULL;
    }


    // initialize vector with enough capacity
    if (!vector_init(result, vec->size)) {
        vector_destroy(result);
        return NULL;
    }

    for (size_t i = 0; i < vec->size; i++) {
        void *source_elem = (char*)vec->data + i * vec->type->size;

        // count place for result
        void *dest_elem = (char*)result->data + i * result->type->size;

        // copy elem
        vec->type->copy(dest_elem, source_elem);

        // use func
        func(dest_elem);

        result->size++;
    }

    return result;
}

Vector *vector_where(const Vector *vec, int (*predicate)(const void *elem)) {
    if (!vec || !predicate) {
        fprintf(stderr, "Error: NULL vector or predicate function\n");
        return NULL;
    }

    if (vec->size == 0) {
        return NULL;
    }

    Vector *result = vector_create(vec->type);
    if (!result) {
        return NULL;
    }

    if (!vector_init(result, 4)) {
        vector_destroy(result);
        return NULL;
    }

    for (size_t i = 0; i < vec->size; i++) {
        void* elem = (char*)vec->data + i * vec->type->size;
        if (predicate(elem)) {
            if (!vector_push_back(result, elem)) {
                vector_destroy(result);
                return NULL;
            }
        }
    }

    if (result->size == 0) {
        vector_destroy(result);
        return NULL;
    }

    return result;
}

Vector *vector_concat(const Vector *vec1, const Vector *vec2) {
    if (!vec1 || !vec2) {
        fprintf(stderr, "Error: NULL vector pointer\n");
        return NULL;
    }

    // Check type compatibility
    if (vec1->type != vec2->type) {
        fprintf(stderr, "Error: Type mismatch. Cannot concatenate vectors of different types\n");
        return NULL;
    }

    Vector *result = vector_create(vec1->type);
    if (!result) {
        return NULL;
    }

    // initialize vector with capacity equal sum(vec1, vec2)
    if (!vector_init(result, vec1->size + vec2->size)) {
        vector_destroy(result);
        return NULL;
    }

    // Copy elements from first vector
    for (size_t i = 0; i < vec1->size; i++) {
        void *source_elem = (char*)vec1->data + i * vec1->type->size;
        void *dest_elem = (char*)result->data + i * result->type->size;
        vec1->type->copy(dest_elem, source_elem);
    }

    // Copy elements from second vector
    for (size_t i = 0; i < vec2->size; i++) {
        void* source_elem = (char*)vec2->data + i * vec2->type->size;
        void* dest_elem = (char*)result->data + (vec1->size + i) * result->type->size;
        vec2->type->copy(dest_elem, source_elem);
    }

    result->size = vec1->size + vec2->size;
    return result;
}