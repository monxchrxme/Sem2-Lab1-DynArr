#include "vector.h"
#include "type_info.h"
#include "utils.h"
#include "console_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global not dynamic arrays for storage vectors
#define MAX_VECTORS 10
Vector *string_vectors[MAX_VECTORS] = {NULL};
Vector *function_vectors[MAX_VECTORS] = {NULL};
int current_string_vector = 0;
int current_function_vector = 0;

// Menu func prototypes
void main_menu();
void string_operations_menu();
void function_operations_menu();
void display_vector(const Vector* vec, int is_string);
void clear_input_buffer();

/**
 * Основное меню программы
 */
void main_menu() {
    int choice;

    while (1) {
        printf("\n");
        printf("============================================\n");
        printf("      ПОЛИМОРФНЫЙ ДИНАМИЧЕСКИЙ МАССИВ\n");
        printf("============================================\n");
        printf("Текущие векторы:\n");
        printf("  Строки: вектор %d\n", current_string_vector);
        printf("  Функции: вектор %d\n", current_function_vector);
        printf("\n");
        printf("1. Работа со строками\n");
        printf("2. Работа с функциями\n");
        printf("3. Управление векторами\n");
        printf("4. Выход\n");
        printf("Выберите опцию: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                string_operations_menu();
                break;
            case 2:
                function_operations_menu();
                break;
            case 3: {
                int sub_choice;
                printf("\nУправление векторами:\n");
                printf("1. Создать новый вектор строк\n");
                printf("2. Создать новый вектор функций\n");
                printf("3. Переключиться на существующий вектор строк\n");
                printf("4. Переключиться на существующий вектор функций\n");
                printf("5. Уничтожить вектор по индексу\n");
                printf("6. Вернуться в главное меню\n");
                printf("Выберите опцию: ");

                if (scanf("%d", &sub_choice) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                if (sub_choice == 1) {
                    int i;
                    for (i = 0; i < MAX_VECTORS; i++) {
                        if (string_vectors[i] == NULL) {
                            string_vectors[i] = vector_create(get_string_type_info());
                            vector_init(string_vectors[i], 4);
                            current_string_vector = i;
                            printf("Создан новый вектор строк #%d\n", i);
                            break;
                        }
                    }
                    if (i == MAX_VECTORS) {
                        printf("Достигнуто максимальное количество векторов строк.\n");
                    }
                } else if (sub_choice == 2) {
                    int i;
                    for (i = 0; i < MAX_VECTORS; i++) {
                        if (function_vectors[i] == NULL) {
                            function_vectors[i] = vector_create(get_function_type_info());
                            vector_init(function_vectors[i], 4);
                            current_function_vector = i;
                            printf("Создан новый вектор функций #%d\n", i);
                            break;
                        }
                    }
                    if (i == MAX_VECTORS) {
                        printf("Достигнуто максимальное количество векторов функций.\n");
                    }
                } else if (sub_choice == 3) {
                    printf("Доступные векторы строк:\n");
                    for (int i = 0; i < MAX_VECTORS; i++) {
                        if (string_vectors[i] != NULL) {
                            printf("  %d: размер = %zu, емкость = %zu\n",
                                   i, string_vectors[i]->size, string_vectors[i]->capacity);
                        }
                    }
                    printf("Выберите номер вектора: ");
                    if (scanf("%d", &sub_choice) == 1 && sub_choice >= 0 && sub_choice < MAX_VECTORS && string_vectors[sub_choice] != NULL) {
                        current_string_vector = sub_choice;
                        printf("Переключено на вектор строк #%d\n", sub_choice);
                    } else {
                        printf("Неверный выбор.\n");
                    }
                    clear_input_buffer();
                } else if (sub_choice == 4) {
                    printf("Доступные векторы функций:\n");
                    for (int i = 0; i < MAX_VECTORS; i++) {
                        if (function_vectors[i] != NULL) {
                            printf("  %d: размер = %zu, емкость = %zu\n",
                                   i, function_vectors[i]->size, function_vectors[i]->capacity);
                        }
                    }
                    printf("Выберите номер вектора: ");
                    if (scanf("%d", &sub_choice) == 1 && sub_choice >= 0 && sub_choice < MAX_VECTORS && function_vectors[sub_choice] != NULL) {
                        current_function_vector = sub_choice;
                        printf("Переключено на вектор функций #%d\n", sub_choice);
                    } else {
                        printf("Неверный выбор.\n");
                    }
                    clear_input_buffer();

                } else if (sub_choice == 5) {
                    int destroy_choice;
                    printf("\nЧто вы хотите уничтожить?\n");
                    printf("1. Вектор строк\n");
                    printf("2. Вектор функций\n");
                    printf("Выберите тип вектора: ");

                    if (scanf("%d", &destroy_choice) != 1) {
                        printf("Ошибка ввода.\n");
                        clear_input_buffer();
                        break;
                    }
                    clear_input_buffer();

                    if (destroy_choice == 1) {
                        int index;
                        printf("Введите индекс вектора строк для удаления: ");
                        if (scanf("%d", &index) != 1 || index < 0 || index >= MAX_VECTORS) {
                            printf("Неверный индекс.\n");
                            clear_input_buffer();
                            break;
                        }
                        clear_input_buffer();

                        if (string_vectors[index] != NULL) {
                            vector_destroy(string_vectors[index]);
                            string_vectors[index] = NULL;
                            printf("Вектор строк #%d уничтожен.\n", index);


                            if (current_string_vector == index) {
                                int new_index = -1;
                                for (int i = 0; i < MAX_VECTORS; i++) {
                                    if (string_vectors[i] != NULL) {
                                        new_index = i;
                                        break;
                                    }
                                }
                                if (new_index == -1) {
                                    string_vectors[0] = vector_create(get_string_type_info());
                                    if (string_vectors[0]) {
                                        vector_init(string_vectors[0], 4);
                                        current_string_vector = 0;
                                        printf("Текущий вектор строк больше не существует, создан новый вектор 0.\n");
                                    } else {
                                        printf("Ошибка создания нового вектора строк.\n");
                                    }
                                } else {
                                    current_string_vector = new_index;
                                    printf("Текущий вектор строк установлен на #%d.\n", new_index);
                                }
                            }
                        } else {
                            printf("Вектор строк #%d уже уничтожен или не существует.\n", index);
                        }
                    } else if (destroy_choice == 2) {

                        int index;
                        printf("Введите индекс вектора функций для удаления: ");
                        if (scanf("%d", &index) != 1 || index < 0 || index >= MAX_VECTORS) {
                            printf("Неверный индекс.\n");
                            clear_input_buffer();
                            break;
                        }
                        clear_input_buffer();

                        if (function_vectors[index] != NULL) {
                            vector_destroy(function_vectors[index]);
                            function_vectors[index] = NULL;
                            printf("Вектор функций #%d уничтожен.\n", index);

                            if (current_function_vector == index) {
                                int new_index = -1;
                                for (int i = 0; i < MAX_VECTORS; i++) {
                                    if (function_vectors[i] != NULL) {
                                        new_index = i;
                                        break;
                                    }
                                }
                                if (new_index == -1) {
                                    function_vectors[0] = vector_create(get_function_type_info());
                                    if (function_vectors[0]) {
                                        vector_init(function_vectors[0], 4);
                                        current_function_vector = 0;
                                        printf("Текущий вектор функций больше не существует, создан новый вектор 0.\n");
                                    } else {
                                        printf("Ошибка создания нового вектора функций.\n");
                                    }
                                } else {
                                    current_function_vector = new_index;
                                    printf("Текущий вектор функций установлен на #%d.\n", new_index);
                                }
                            }
                        } else {
                            printf("Вектор функций #%d уже уничтожен или не существует.\n", index);
                        }
                    } else {
                        printf("Неверный выбор.\n");
                    }
                    break;
                }
                break;
            }
            case 4:
                // Release all memory before exit
                for (int i = 0; i < MAX_VECTORS; i++) {
                    if (string_vectors[i]) {
                        vector_destroy(string_vectors[i]);
                        string_vectors[i] = NULL;
                    }
                    if (function_vectors[i]) {
                        vector_destroy(function_vectors[i]);
                        function_vectors[i] = NULL;
                    }
                }
                printf("Выход из программы...\n");
                return;
            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }
}

/**
 * Menu operations with strings
 */
void string_operations_menu() {
    int choice;

    while (1) {
        printf("\n");
        printf("============================================\n");
        printf("       РАБОТА С ВЕКТОРОМ СТРОК #%d\n", current_string_vector);
        printf("============================================\n");

        // Show vector content
        if (string_vectors[current_string_vector] && string_vectors[current_string_vector]->size > 0) {
            printf("Текущее содержимое вектора:\n");
            display_vector(string_vectors[current_string_vector], 1);
        } else {
            printf("Вектор пуст\n");
        }

        printf("\n");
        printf("1. Добавить строку\n");
        printf("2. Применить map\n");
        printf("3. Применить where\n");
        printf("4. Конкатенация с другим вектором строк\n");
        printf("5. Показать содержимое вектора\n");
        printf("6. Вернуться в главное меню\n");
        printf("Выберите опцию: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: {
                char buffer[256];
                printf("Введите строку: ");
                fgets(buffer, sizeof(buffer), stdin);

                size_t len = strlen(buffer);
                if (len > 0 && buffer[len-1] == '\n') {
                    buffer[len-1] = '\0';
                    len--;
                }

                char *new_string = malloc(strlen(buffer) + 1);
                if (!new_string) {
                    printf("Ошибка выделения памяти для строки.\n");
                    break;
                }
                strcpy(new_string, buffer);

                // make temp ptr to string
                char **temp = &new_string;

                if (vector_push_back(string_vectors[current_string_vector], temp) == 0) {
                    printf("Ошибка добавления строки в вектор.\n");
                    free(new_string);
                } else {
                    printf("Строка добавлена.\n");
                }
                free(new_string);
                break;
            }
            case 2: {
                int func_choice;
                printf("\nДоступные функции для map:\n");
                printf("1. Преобразование в верхний регистр\n");
                printf("2. Преобразование в нижний регистр\n");
                printf("3. Отмена\n");
                printf("Выберите функцию: ");

                if (scanf("%d", &func_choice) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                if (func_choice == 1) {
                    Vector *result = vector_map(string_vectors[current_string_vector], to_uppercase);
                    printf("Результат применения map (в верхний регистр):\n");
                    display_vector(result, 1);
                    vector_destroy(result);
                } else if (func_choice == 2) {
                    Vector *result = vector_map(string_vectors[current_string_vector], to_lowercase);
                    printf("Результат применения map (в нижний регистр):\n");
                    display_vector(result, 1);
                    vector_destroy(result);
                } else if (func_choice != 3) {
                    printf("Неверный выбор функции.\n");
                }
                break;
            }
            case 3: {
                int func_choice;
                printf("\nДоступные функции для where:\n");
                printf("1. Фильтр по длине (> 10 символов)\n");
                printf("2. Фильтр по длине (< 10 символов)\n");
                printf("3. Отмена\n");
                printf("Выберите функцию: ");

                if (scanf("%d", &func_choice) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                if (func_choice == 1) {
                    Vector *result = vector_where(string_vectors[current_string_vector], is_long_string);
                    printf("Результат применения where (длина > 10):\n");
                    display_vector(result, 1);
                    vector_destroy(result);
                } else if (func_choice == 2) {
                    Vector *result = vector_where(string_vectors[current_string_vector], is_short_string);
                    printf("Результат применения where (длина < 10):\n");
                    display_vector(result, 1);
                    vector_destroy(result);
                } else if (func_choice != 3) {
                    printf("Неверный выбор функции.\n");
                }
                break;
            }
            case 4: {
                printf("Доступные векторы строк для конкатенации:\n");
                for (int i = 0; i < MAX_VECTORS; i++) {
                    if (i != current_string_vector && string_vectors[i] != NULL) {
                        printf("  %d: размер = %zu\n", i, string_vectors[i]->size);
                    }
                }

                int target;
                printf("Введите номер вектора для конкатенации: ");
                if (scanf("%d", &target) != 1 || target < 0 || target >= MAX_VECTORS ||
                    target == current_string_vector || string_vectors[target] == NULL) {
                    printf("Неверный выбор.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                Vector *result = vector_concat(string_vectors[current_string_vector], string_vectors[target]);
                if (result) {
                    printf("Результат конкатенации:\n");
                    display_vector(result, 1);
                    vector_destroy(result);
                }
                break;
            }
            case 5:
                if (string_vectors[current_string_vector] && string_vectors[current_string_vector]->size > 0) {
                    printf("Содержимое вектора:\n");
                    display_vector(string_vectors[current_string_vector], 1);
                } else {
                    printf("Вектор пуст\n");
                }
                break;
            case 6:
                return;
            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }
}

/**
 * Menu operations with funcs
 */
void function_operations_menu() {
    int choice;

    while (1) {
        printf("\n");
        printf("============================================\n");
        printf("      РАБОТА С ВЕКТОРОМ ФУНКЦИЙ #%d\n", current_function_vector);
        printf("============================================\n");

        // Show vector content
        if (function_vectors[current_function_vector] && function_vectors[current_function_vector]->size > 0) {
            printf("Текущее содержимое вектора:\n");
            display_vector(function_vectors[current_function_vector], 0);
        } else {
            printf("Вектор пуст\n");
        }

        printf("\n");
        printf("1. Добавить функцию\n");
        printf("2. Применить map\n");
        printf("3. Применить where\n");
        printf("4. Конкатенация с другим вектором функций\n");
        printf("5. Показать содержимое вектора\n");
        printf("6. Вернуться в главное меню\n");
        printf("Выберите опцию: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: {
                int func_choice;
                printf("\nДоступные функции:\n");
                printf("1. example_function\n");
                printf("2. to_uppercase\n");
                printf("3. to_lowercase\n");
                printf("4. print_function_address\n");
                printf("5. NULL (нулевая функция)\n");
                printf("Выберите функцию: ");

                if (scanf("%d", &func_choice) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                if (func_choice == 1) {
                    vector_push_back(function_vectors[current_function_vector], &example_function);
                    printf("Функция example_function добавлена.\n");
                } else if (func_choice == 2) {
                    vector_push_back(function_vectors[current_function_vector], &to_uppercase);
                    printf("Функция to_uppercase добавлена.\n");
                } else if (func_choice == 3) {
                    vector_push_back(function_vectors[current_function_vector], &to_lowercase);
                    printf("Функция to_lowercase добавлена.\n");
                } else if (func_choice == 4) {
                    vector_push_back(function_vectors[current_function_vector], &print_function_address);
                    printf("Функция print_function_address добавлена.\n");
                } else if (func_choice == 5) {
                    void *null_func = NULL;
                    vector_push_back(function_vectors[current_function_vector], &null_func);
                    printf("NULL функция добавлена.\n");
                } else {
                    printf("Неверный выбор функции.\n");
                }
                break;
            }
            case 2: {
                int func_choice;
                printf("\nДоступные функции для map:\n");
                printf("1. Вывод адреса функции\n");
                printf("2. Отмена\n");
                printf("Выберите функцию: ");

                if (scanf("%d", &func_choice) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                if (func_choice == 1) {
                    Vector *result = vector_map(function_vectors[current_function_vector], print_function_address);
                    vector_destroy(result);
                } else if (func_choice != 2) {
                    printf("Неверный выбор функции.\n");
                }
                break;
            }
            case 3: {
                int func_choice;
                printf("\nДоступные функции для where:\n");
                printf("1. Фильтр ненулевых функций\n");
                printf("2. Отмена\n");
                printf("Выберите функцию: ");

                if (scanf("%d", &func_choice) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                if (func_choice == 1) {
                    Vector *result = vector_where(function_vectors[current_function_vector], is_non_null_function);
                    printf("Результат применения where (ненулевые функции):\n");
                    display_vector(result, 0);
                    vector_destroy(result);
                } else if (func_choice != 2) {
                    printf("Неверный выбор функции.\n");
                }
                break;
            }
            case 4: {
                printf("Доступные векторы функций для конкатенации:\n");
                for (int i = 0; i < MAX_VECTORS; i++) {
                    if (i != current_function_vector && function_vectors[i] != NULL) {
                        printf("  %d: размер = %zu\n", i, function_vectors[i]->size);
                    }
                }

                int target;
                printf("Введите номер вектора для конкатенации: ");
                if (scanf("%d", &target) != 1 || target < 0 || target >= MAX_VECTORS ||
                    function_vectors[target] == NULL || target == current_function_vector) {
                    printf("Неверный выбор.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                Vector *result = vector_concat(function_vectors[current_function_vector], function_vectors[target]);
                if (result) {
                    printf("Результат конкатенации:\n");
                    display_vector(result, 0);
                    vector_destroy(result);
                }
                break;
            }
            case 5:
                if (function_vectors[current_function_vector] && function_vectors[current_function_vector]->size > 0) {
                    printf("Содержимое вектора:\n");
                    display_vector(function_vectors[current_function_vector], 0);
                } else {
                    printf("Вектор пуст\n");
                }
                break;
            case 6:
                return;
            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }
}

/**
 * Show vector content function
 * @param vec Vector to show
 * @param is_string 1 for str, 0 for func
 */
void display_vector(const Vector *vec, int is_string) {
    if (!vec || vec->size == 0) {
        printf("Вектор п1уст\n");
        return;
    }

    printf("Размер: %zu, Емкость: %zu\n", vec->size, vec->capacity);
    printf("Содержимое:\n");

    for (size_t i = 0; i < vec->size; i++) {
        printf("  [%zu]: ", i);
        void *elem = (char*)vec->data + i * vec->type->size;

        if (is_string) {
            printf("\"%s\"", *(char**)elem);
        } else {
            void (**func)(void*) = (void (**)(void*))elem;
            printf("Функция по адресу: 0x%lx", (unsigned long)(uintptr_t)*func);
        }
        printf("\n");
    }
}

/**
 * Clear input buffer
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Program entry point
 */
int main() {
    setup_console_encoding();

    string_vectors[0] = vector_create(get_string_type_info());
    vector_init(string_vectors[0], 4);
    function_vectors[0] = vector_create(get_function_type_info());
    vector_init(function_vectors[0], 4);

    printf("Лабораторная работа 1 (вариант 20) - полиморфная коллекция на основе динамического массива.\n");
    printf("Эта программа позволяет работать со строками и указателями на функции.\n");

    main_menu();

    return 0;
}





