#include "dynamic_array.h"
#include <stdlib.h>
#include <stdio.h>

int dynamic_array_init(DynArr *da, TypeInfo *type, size_t initial_capacity) {
    if (!da) {
        fprintf(stderr, "Invalid dynamic array pointer\n");
        return 0;
    }

    // Initializing fields of struct
    da->type = type;
    da->capacity = 0;
    da->size = 0;
    da->data = NULL;

    if (!type) {
        fprintf(stderr, "Error: NULL TypeInfo\n");
        return 0;
    }

    if (initial_capacity > 0) {
        da->data = calloc(initial_capacity, da->type->size); // to initialize memory with 0
        if (!da->data) {
            fprintf(stderr, "Memory allocation failed for dynamic array data\n");
            return 0;
        }
        da->capacity = initial_capacity;
    }
    return 1;
}

void dynamic_array_destroy(DynArr *da) {
    if (!da) return;

    // Release memory from elements
    if (da->data) {
        for (size_t i = 0; i < da->size; i++) {
            void *elem = (char*)da->data + i * da->type->size;
            da->type->free(elem);
        }
        free(da->data);
    }
}

int dynamic_array_push_back(DynArr *da, const void *elem) {
    if (!da || !elem) return 0;

    // if array is full increase capacity
    if (da->size == da->capacity) {
        size_t new_capacity = (da->capacity == 0) ? 4 : da->capacity * 2;
        void *new_data = realloc(da->data, new_capacity * da->type->size);

        if (!new_data) {
            fprintf(stderr, "Memory allocation failed for dynamic array data\n");
            return 0;
        }

        da->data = new_data;
        da->capacity = new_capacity;
    }

    // count address for new elem
    void *dest = (char*)da->data + da->size * da->type->size;

    // copy elem there
    da->type->copy(dest, elem);
    da->size++;

    return 1;
}

void *dynamic_array_get(const DynArr *da, size_t index) {
    if (!da || index >= da->size) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }
    return (char*)da->data + index * da->type->size;
}

int dynamic_array_map(const DynArr *source, DynArr *result, void (*func)(void *elem)) {
    if (!source || !result || !func) {
        return 0;
    }

    if (!dynamic_array_init(result, source->type, source->size)) {
        return 0;
    }

    for (size_t i = 0; i < source->size; i++) {
        void *source_elem = (char*)source->data + i * source->type->size;
        void *result_elem = (char*)result->data + i * result->type->size;
        source->type->copy(result_elem, source_elem);
        func(result_elem);
        result->size++;
    }

    return 1;
}

int dynamic_array_where(const DynArr *source, DynArr *result, int (*predicate)(const void *elem)) {
    if (!source || !result || !predicate) {
        return 0;
    }

    if (!dynamic_array_init(result, source->type, 4)) {
        return 0;
    }

    for (size_t i = 0; i < source->size; i++) {
        void *elem = (char*)source->data + i * source->type->size;
        if (predicate(elem)) {
            void *dest = (char*)result->data + result->size * result->type->size;
            source->type->copy(dest, elem);
            result->size++;

            if (result->size == result->capacity) {
                size_t new_capacity = result->capacity * 2;
                void *new_data = realloc(result->data, new_capacity * result->type->size);
                if (!new_data) {
                    return 0;
                }
                result->data = new_data;
                result->capacity = new_capacity;
            }
        }
    }
    return 1;
}

int dynamic_array_concat(const DynArr *da1, const DynArr *da2, DynArr *result) {
    if (!da1 || !da2 || !result) {
        return 0;
    }

    if (da1->type != da2->type) {
        return 0;
    }

    if (!dynamic_array_init(result, da1->type, da1->size + da2->size)) {
        return 0;
    }

    for (size_t i = 0; i < da1->size; i++) {
        void *source_elem = (char*)da1->data + i * da1->type->size;
        void *dest_elem = (char*)result->data + i * result->type->size;
        da1->type->copy(dest_elem, source_elem);
    }

    for (size_t i = 0; i < da2->size; i++) {
        void *source_elem = (char*)da2->data + i * da2->type->size;
        void *dest_elem = (char*)result->data + (da1->size + i) * result->type->size;
        da2->type->copy(dest_elem, source_elem);
    }

    result->size = da1->size + da2->size;
    return 1;
}