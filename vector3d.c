#include "vector3d.h"
#include <string.h>
#include <math.h>

void vector3d_init(Vector3D *vec, TypeInfo *type) {
    for (int i = 0; i < 3; i++) {
        vec->elements[i] = NULL;
    }
    vec->type = type;
}

void vector3d_set(Vector3D *vec, size_t index, void *elem) {
    if (index >= 3 || !vec || !elem) return;
    vec->elements[index] = elem;
}

double vector3d_normal(const Vector3D *vec) {
    if (!vec || !vec->type) return 0.0;

    double sum = 0.0;

    for (int i = 0; i < 3; i++) {
        if (vec->elements[i]) {
            // Для строк: длина строки
            if (vec->type == get_string_type_info()) {
                char *str = (char*)vec->elements[i];
                if (str) {
                    double len = (double)strlen(str);
                    sum += len * len;
                }
            }
                // Для функций: 1 если не NULL
            else if (vec->type == get_function_type_info()) {
                void (*func)(void*) = (void (*)(void*))vec->elements[i];
                if (func) {
                    sum += 1.0;
                }
            }
        }
    }
    return sqrt(sum);
}

int vector3d_compare_by_normal(const Vector3D *a, const Vector3D *b) {
    if (!a || !b) return 0;

    double normal_a = vector3d_normal(a);
    double normal_b = vector3d_normal(b);

    if (normal_a < normal_b) return -1;
    if (normal_a > normal_b) return 1;

    // Если нормали равны — сравниваем поэлементно через type->compare
    if (a->type->compare) {
        for (int i = 0; i < 3; i++) {
            int cmp = a->type->compare(&a->elements[i], &b->elements[i]);
            if (cmp != 0) return cmp;
        }
    }
    return 0;
}

void vector3d_destroy(Vector3D *vec) {
    if (!vec) return;
    for (int i = 0; i < 3; i++) {
        if (vec->elements[i] && vec->type->free) {
            vec->type->free(&vec->elements[i]);
            vec->elements[i] = NULL;
        }
    }
}