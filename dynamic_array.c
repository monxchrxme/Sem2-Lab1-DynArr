#include "dynamic_array.h"
#include <stdlib.h>
#include <stdio.h>

DynArr *dynamic_array_create(TypeInfo *type) { //TODO пользователь сам создает маллок, и передает его в инит функцию, креат функция не нужна, все что ниже маллока нужно переместить в функцию инит,
    DynArr *da = malloc(sizeof(DynArr));
    if (!da) {
        fprintf(stderr, "Memory allocation failed for DynArr\n");
        return NULL;
    }

    da->data = NULL; //TODO это перенести в функцию инит, пользователь выделяет с помощью маллок память под структуру DynArr, и передает этот указатель в функцию init, где уже инициализируются поля этой струткуры, и выделяется память под поле data
    da->type = type;
    da->capacity = 0;
    da->size = 0;

    return da;
}

int dynamic_array_init(DynArr *da, size_t initial_capacity) {
    if (!da) {
        fprintf(stderr, "Invalid dynamic array pointer\n");
        return 0;
    }

    if (da->data != NULL) {
        fprintf(stderr, "DynArr is already initialized\n");
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

    // Release memory from dynamic array itself
    free(da);
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

DynArr *dynamic_array_map(const DynArr *da, void (*func)(void *elem)) {
    if (!da || !func) {
        fprintf(stderr, "Error: NULL dynamic array or function pointer\n");
        return NULL;
    }

    if (da->size == 0) {
        return NULL;
    }

    DynArr *result = dynamic_array_create(da->type);
    if (!result) {
        return NULL;
    }


    // initialize dynamic array with enough capacity
    if (!dynamic_array_init(result, da->size)) {
        dynamic_array_destroy(result);
        return NULL;
    }

    for (size_t i = 0; i < da->size; i++) {
        void *source_elem = (char*)da->data + i * da->type->size;

        // count place for result
        void *dest_elem = (char*)result->data + i * result->type->size;

        // copy elem
        da->type->copy(dest_elem, source_elem);

        // use func
        func(dest_elem);

        result->size++;
    }

    return result;
}

DynArr *dynamic_array_where(const DynArr *da, int (*predicate)(const void *elem)) {
    if (!da || !predicate) {
        fprintf(stderr, "Error: NULL dynamic array or predicate function\n");
        return NULL;
    }

    if (da->size == 0) {
        return NULL;
    }

    DynArr *result = dynamic_array_create(da->type);
    if (!result) {
        return NULL;
    }

    if (!dynamic_array_init(result, 4)) {
        dynamic_array_destroy(result);
        return NULL;
    }

    for (size_t i = 0; i < da->size; i++) {
        void* elem = (char*)da->data + i * da->type->size;
        if (predicate(elem)) {
            if (!dynamic_array_push_back(result, elem)) {
                dynamic_array_destroy(result);
                return NULL;
            }
        }
    }

    if (result->size == 0) {
        dynamic_array_destroy(result);
        return NULL;
    }

    return result;
}

DynArr *dynamic_array_concat(const DynArr *da1, const DynArr *da2) {
    if (!da1 || !da2) {
        fprintf(stderr, "Error: NULL dynamic array pointer\n");
        return NULL;
    }

    // Check type compatibility
    if (da1->type != da2->type) {
        fprintf(stderr, "Error: Type mismatch. Cannot concatenate dynamic array of different types\n");
        return NULL;
    }

    DynArr *result = dynamic_array_create(da1->type);
    if (!result) {
        return NULL;
    }

    // initialize dynamic array with capacity equal sum(da1, da2)
    if (!dynamic_array_init(result, da1->size + da2->size)) {
        dynamic_array_destroy(result);
        return NULL;
    }

    // Copy elements from first dynamic array
    for (size_t i = 0; i < da1->size; i++) {
        void *source_elem = (char*)da1->data + i * da1->type->size;
        void *dest_elem = (char*)result->data + i * result->type->size;
        da1->type->copy(dest_elem, source_elem);
    }

    // Copy elements from second dynamic array
    for (size_t i = 0; i < da2->size; i++) {
        void *source_elem = (char*)da2->data + i * da2->type->size;
        void *dest_elem = (char*)result->data + (da1->size + i) * result->type->size;
        da2->type->copy(dest_elem, source_elem);
    }

    result->size = da1->size + da2->size;
    return result;
}