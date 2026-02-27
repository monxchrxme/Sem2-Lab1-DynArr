#include "type_info.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Static variables for lazy initialization
static TypeInfo *STRING_TYPE_INFO = NULL;
static TypeInfo *FUNCTION_TYPE_INFO = NULL;

//================ STRING FUNCTIONS REALIZATION ================

static void string_copy(void *destination, const void *source) {
    // Convert void pointers to strings pointers
    char **d = (char**)destination;         // двойной указатель потому что в нашем векторе хранятся указатели на строки (*char)
    char **s = (char**)source;
    // Clear destination string if it exists
    if (*s == NULL) {
        *d = NULL;
        return;
    }

    if (*d != NULL) {
        free(*d);
    }

    *d = malloc(strlen(*s) + 1);   // добавляем символ конца строки "\0"
    if (*d) {
        strcpy(*d, *s);
    }
}

static void string_free(void *elem) {
    char **e = (char**)elem;  // elem это указатель на строку (char*), поэтому приводим к char**
    free(*e);         // *e это адрес выделенной памяти для самой строки
}

static int string_compare(const void *a, const void *b) {
    char **sa = (char**)a;
    char **sb = (char**)b;
    return strcmp(*sa, *sb);
}

//================ FUNC FUNCTIONS REALIZATION ================

static void function_copy(void *destination, const void *source) {
    void (**d)(void*) = (void (**)(void*))destination;
    void (**s)(void*) = (void (**)(void*))source;
    *d = *s;
}

// Nothing to do, functions do not require memory release
static void function_free(void *elem) {
    (void)elem;  // Указываем что параметр намеренно не используется
}

static int function_compare(const void *a, const void *b) {
    void (**fa)(void*) = (void (**)(void*))a;
    void (**fb)(void*) = (void (**)(void*))b;

    if (*fa == *fb) return 0;
    // Для упорядочивания можно использовать адреса как целые числа
    uintptr_t addr_a = (uintptr_t)*fa;
    uintptr_t addr_b = (uintptr_t)*fb;
    return (addr_a > addr_b) - (addr_a < addr_b);
}

// initialization of string TypeInfo
TypeInfo *get_string_type_info() {
    if (!STRING_TYPE_INFO) {
        STRING_TYPE_INFO = malloc(sizeof(TypeInfo));
        STRING_TYPE_INFO->size = sizeof(char*);
        STRING_TYPE_INFO->copy = string_copy;
        STRING_TYPE_INFO->free = string_free;
        STRING_TYPE_INFO->compare = string_compare;
    }
    return STRING_TYPE_INFO;
}

// initialization of function TypeInfo
TypeInfo *get_function_type_info() {
    if (!FUNCTION_TYPE_INFO) {
        FUNCTION_TYPE_INFO = malloc(sizeof(TypeInfo));
        FUNCTION_TYPE_INFO->size = sizeof(void(*)(void*));    // указатель на функцию, которая возвращает void и в сигнатуре функции аргумент на вход прописан как void (размер указателя на функцию не зависит от аргументов, принимаемых функцией на вход)
        FUNCTION_TYPE_INFO->copy = function_copy;
        FUNCTION_TYPE_INFO->free = function_free;
        FUNCTION_TYPE_INFO->compare = function_compare;
    }
    return FUNCTION_TYPE_INFO;
}

