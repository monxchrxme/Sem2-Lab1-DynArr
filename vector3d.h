#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "type_info.h"

typedef struct {
    void *elements[3];  // 3 элемента (строки ИЛИ функции)
    TypeInfo *type;     // тип элементов
} Vector3D;

void vector3d_init(Vector3D *vec, TypeInfo *type);
// Установка элемента (0, 1, 2)
void vector3d_set(Vector3D *vec, size_t index, void *elem);
double vector3d_normal(const Vector3D *vec);
int vector3d_compare_by_normal(const Vector3D *a, const Vector3D *b);
void vector3d_destroy(Vector3D *vec);

#endif