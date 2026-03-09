#include "dynamic_array.h"
#include "type_info.h"
#include "utils.h"
#include "console_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector3d.h"

// Global not dynamic arrays for storage dynamic arrays
#define MAX_DYNAMIC_ARRAYS 10
DynArr *string_dynamic_arrays[MAX_DYNAMIC_ARRAYS] = {NULL};
DynArr *function_dynamic_arrays[MAX_DYNAMIC_ARRAYS] = {NULL};
int current_string_dyn_arr = 0;
int current_function_dyn_arr = 0;

// Menu func prototypes
void main_menu();
void string_operations_menu();
void function_operations_menu();
void display_dyn_arr(const DynArr *da, int is_string);
void clear_input_buffer();
void vector3d_comparison_menu();

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
        printf("Текущие динамические массивы:\n");
        printf("  Строки: массив %d\n", current_string_dyn_arr);
        printf("  Функции: массив %d\n", current_function_dyn_arr);
        printf("\n");
        printf("1. Работа со строками\n");
        printf("2. Работа с функциями\n");
        printf("3. Управление динамическими массивами\n");
        printf("4. БОНУС: сравнение трехмерных векторов по норме\n");
        printf("5. Выход\n");
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
                printf("\nУправление динамическими массивами:\n");
                printf("1. Создать новый массив строк\n");
                printf("2. Создать новый массив функций\n");
                printf("3. Переключиться на существующий массив строк\n");
                printf("4. Переключиться на существующий массив функций\n");
                printf("5. Уничтожить массив по индексу\n");
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
                    for (i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                        if (string_dynamic_arrays[i] == NULL) {
                            string_dynamic_arrays[i] = malloc(sizeof(DynArr));
                            if (string_dynamic_arrays[i]) {
                                dynamic_array_init(string_dynamic_arrays[i], get_string_type_info(), 4);
                                current_string_dyn_arr = i;
                                printf("Создан новый массив строк #%d\n", i);
                                break;
                            }
                        }
                    }
                    if (i == MAX_DYNAMIC_ARRAYS) {
                        printf("Достигнуто максимальное количество массивов строк.\n");
                    }
                } else if (sub_choice == 2) {
                    int i;
                    for (i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                        if (function_dynamic_arrays[i] == NULL) {
                            function_dynamic_arrays[i] = malloc(sizeof(DynArr));
                            if (function_dynamic_arrays[i]) {
                                dynamic_array_init(function_dynamic_arrays[i], get_function_type_info(), 4);
                                current_function_dyn_arr = i;
                                printf("Создан новый массив функций #%d\n", i);
                                break;
                            }
                        }
                    }
                    if (i == MAX_DYNAMIC_ARRAYS) {
                        printf("Достигнуто максимальное количество массивов функций.\n");
                    }
                } else if (sub_choice == 3) {
                    printf("Доступные массивы строк:\n");
                    for (int i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                        if (string_dynamic_arrays[i] != NULL) {
                            printf("  %d: размер = %zu, емкость = %zu\n",
                                   i, string_dynamic_arrays[i]->size, string_dynamic_arrays[i]->capacity);
                        }
                    }
                    printf("Выберите номер массива: ");
                    if (scanf("%d", &sub_choice) == 1 && sub_choice >= 0 && sub_choice < MAX_DYNAMIC_ARRAYS && string_dynamic_arrays[sub_choice] != NULL) {
                        current_string_dyn_arr = sub_choice;
                        printf("Переключено на массив строк #%d\n", sub_choice);
                    } else {
                        printf("Неверный выбор.\n");
                    }
                    clear_input_buffer();
                } else if (sub_choice == 4) {
                    printf("Доступные массивы функций:\n");
                    for (int i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                        if (function_dynamic_arrays[i] != NULL) {
                            printf("  %d: размер = %zu, емкость = %zu\n",
                                   i, function_dynamic_arrays[i]->size, function_dynamic_arrays[i]->capacity);
                        }
                    }
                    printf("Выберите номер массива: ");
                    if (scanf("%d", &sub_choice) == 1 && sub_choice >= 0 && sub_choice < MAX_DYNAMIC_ARRAYS && function_dynamic_arrays[sub_choice] != NULL) {
                        current_function_dyn_arr = sub_choice;
                        printf("Переключено на массив функций #%d\n", sub_choice);
                    } else {
                        printf("Неверный выбор.\n");
                    }
                    clear_input_buffer();

                } else if (sub_choice == 5) {
                    int destroy_choice;
                    printf("\nЧто вы хотите уничтожить?\n");
                    printf("1. массив строк\n");
                    printf("2. массив функций\n");
                    printf("Выберите тип массива: ");

                    if (scanf("%d", &destroy_choice) != 1) {
                        printf("Ошибка ввода.\n");
                        clear_input_buffer();
                        break;
                    }
                    clear_input_buffer();

                    if (destroy_choice == 1) {
                        int index;
                        printf("Введите индекс массива строк для удаления: ");
                        if (scanf("%d", &index) != 1 || index < 0 || index >= MAX_DYNAMIC_ARRAYS) {
                            printf("Неверный индекс.\n");
                            clear_input_buffer();
                            break;
                        }
                        clear_input_buffer();

                        if (string_dynamic_arrays[index] != NULL) {
                            dynamic_array_destroy(string_dynamic_arrays[index]);
                            string_dynamic_arrays[index] = NULL;
                            printf("Массив строк #%d уничтожен.\n", index);

                            if (current_string_dyn_arr == index) {
                                int new_index = -1;
                                for (int i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                                    if (string_dynamic_arrays[i] != NULL) {
                                        new_index = i;
                                        break;
                                    }
                                }
                                if (new_index == -1) {
                                    string_dynamic_arrays[0] = malloc(sizeof(DynArr));
                                    if (string_dynamic_arrays[0]) {
                                        dynamic_array_init(string_dynamic_arrays[0], get_string_type_info(), 4);
                                        current_string_dyn_arr = 0;
                                        printf("Текущий массив строк больше не существует, создан новый массив 0.\n");
                                    } else {
                                        printf("Ошибка создания нового массива строк.\n");
                                    }
                                } else {
                                    current_string_dyn_arr = new_index;
                                    printf("Текущий массив строк установлен на #%d.\n", new_index);
                                }
                            }
                        } else {
                            printf("Массив строк #%d уже уничтожен или не существует.\n", index);
                        }
                    } else if (destroy_choice == 2) {

                        int index;
                        printf("Введите индекс массива функций для удаления: ");
                        if (scanf("%d", &index) != 1 || index < 0 || index >= MAX_DYNAMIC_ARRAYS) {
                            printf("Неверный индекс.\n");
                            clear_input_buffer();
                            break;
                        }
                        clear_input_buffer();

                        if (function_dynamic_arrays[index] != NULL) {
                            dynamic_array_destroy(function_dynamic_arrays[index]);
                            function_dynamic_arrays[index] = NULL;
                            printf("Массив функций #%d уничтожен.\n", index);

                            if (current_function_dyn_arr == index) {
                                int new_index = -1;
                                for (int i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                                    if (function_dynamic_arrays[i] != NULL) {
                                        new_index = i;
                                        break;
                                    }
                                }
                                if (new_index == -1) {
                                    function_dynamic_arrays[0] = malloc(sizeof(DynArr));
                                    if (function_dynamic_arrays[0]) {
                                        dynamic_array_init(function_dynamic_arrays[0], get_function_type_info(),4);
                                        current_function_dyn_arr = 0;
                                        printf("Текущий массив функций больше не существует, создан новый массив 0.\n");
                                    } else {
                                        printf("Ошибка создания нового массива функций.\n");
                                    }
                                } else {
                                    current_function_dyn_arr = new_index;
                                    printf("Текущий массив функций установлен на #%d.\n", new_index);
                                }
                            }
                        } else {
                            printf("Массив функций #%d уже уничтожен или не существует.\n", index);
                        }
                    } else {
                        printf("Неверный выбор.\n");
                    }
                    break;
                }
                break;
            }
            case 4:
                vector3d_comparison_menu();
                break;
            case 5:
                // Release all memory before exit
                for (int i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                    if (string_dynamic_arrays[i]) {
                        dynamic_array_destroy(string_dynamic_arrays[i]);
                        string_dynamic_arrays[i] = NULL;
                    }
                    if (function_dynamic_arrays[i]) {
                        dynamic_array_destroy(function_dynamic_arrays[i]);
                        function_dynamic_arrays[i] = NULL;
                    }
                }
                printf("Выход из программы...\n");
                return;
            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    } // while
} // func itself

/**
 * Menu operations with strings
 */
void string_operations_menu() {
    int choice;

    while (1) {
        printf("\n");
        printf("============================================\n");
        printf("       РАБОТА С МАССИВОМ СТРОК #%d\n", current_string_dyn_arr);
        printf("============================================\n");

        // Show array content
        if (string_dynamic_arrays[current_string_dyn_arr] && string_dynamic_arrays[current_string_dyn_arr]->size > 0) {
            printf("Текущее содержимое массива:\n");
            display_dyn_arr(string_dynamic_arrays[current_string_dyn_arr], 1);
        } else {
            printf("Массив пуст\n");
        }

        printf("\n");
        printf("1. Добавить строку\n");
        printf("2. Применить map\n");
        printf("3. Применить where\n");
        printf("4. Конкатенация с другим массивом строк\n");
        printf("5. Показать содержимое массива\n");
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

                if (dynamic_array_push_back(string_dynamic_arrays[current_string_dyn_arr], temp) == 0) {
                    printf("Ошибка добавления строки в массив.\n");
                    free(new_string);
                } else {
                    printf("Строка добавлена.\n");
                }
                free(new_string); // TODO - think about it
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
                    DynArr *result = malloc(sizeof(DynArr));
                    if (result && dynamic_array_map(string_dynamic_arrays[current_string_dyn_arr], result, to_uppercase)) {
                        printf("Результат применения map (в верхний регистр):\n");
                        display_dyn_arr(result, 1);
                        dynamic_array_destroy(result);
                        free(result);
                    } else {
                        printf("Ошибка применения map.\n");
                        if (result) free(result);
                    }
                } else if (func_choice == 2) {
                    DynArr *result = malloc(sizeof(DynArr));
                    if (result && dynamic_array_map(string_dynamic_arrays[current_string_dyn_arr], result, to_lowercase)) {
                        printf("Результат применения map (в нижний регистр):\n");
                        display_dyn_arr(result, 1);
                        dynamic_array_destroy(result);
                        free(result);
                    } else {
                        printf("Ошибка применения map.\n");
                        if (result) free(result);
                    }
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
                    DynArr *result = malloc(sizeof(DynArr));
                    if (result && dynamic_array_where(string_dynamic_arrays[current_string_dyn_arr], result, is_long_string)) {
                        printf("Результат применения where (длина > 10):\n");
                        display_dyn_arr(result, 1);
                        dynamic_array_destroy(result);
                        free(result);
                    } else {
                        printf("Ошибка применения where или результат пуст.\n");
                        if (result) {
                            dynamic_array_destroy(result);
                            free(result);
                        }
                    }
                } else if (func_choice == 2) {
                    DynArr *result = malloc(sizeof(DynArr));
                    if (result && dynamic_array_where(string_dynamic_arrays[current_string_dyn_arr], result, is_short_string)) {
                        printf("Результат применения where (длина < 10):\n");
                        display_dyn_arr(result, 1);
                        dynamic_array_destroy(result);
                        free(result);
                    } else {
                        printf("Ошибка применения where или результат пуст.\n");
                        if (result) {
                            dynamic_array_destroy(result);
                            free(result);
                        }
                    }
                } else if (func_choice != 3) {
                    printf("Неверный выбор функции.\n");
                }
                break;
            }
            case 4: {
                printf("Доступные массивы строк для конкатенации:\n");
                for (int i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                    if (i != current_string_dyn_arr && string_dynamic_arrays[i] != NULL) {
                        printf("  %d: размер = %zu\n", i, string_dynamic_arrays[i]->size);
                    }
                }

                int target;
                printf("Введите номер массива для конкатенации: ");
                if (scanf("%d", &target) != 1 || target < 0 || target >= MAX_DYNAMIC_ARRAYS ||
                    target == current_string_dyn_arr || string_dynamic_arrays[target] == NULL) {
                    printf("Неверный выбор.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                DynArr *result = malloc(sizeof(DynArr));
                if (result && dynamic_array_concat(string_dynamic_arrays[current_string_dyn_arr], string_dynamic_arrays[target], result)) {
                    printf("Результат конкатенации:\n");
                    display_dyn_arr(result, 1);
                    dynamic_array_destroy(result);
                    free(result);
                } else {
                    printf("Ошибка конкатенации.\n");
                    if (result) free(result);
                }
                break;
            }
            case 5:
                if (string_dynamic_arrays[current_string_dyn_arr] && string_dynamic_arrays[current_string_dyn_arr]->size > 0) {
                    printf("Содержимое массива:\n");
                    display_dyn_arr(string_dynamic_arrays[current_string_dyn_arr], 1);
                } else {
                    printf("массив пуст\n");
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
        printf("      РАБОТА С МАССИВОМ ФУНКЦИЙ #%d\n", current_function_dyn_arr);
        printf("============================================\n");

        // Show array content
        if (function_dynamic_arrays[current_function_dyn_arr] && function_dynamic_arrays[current_function_dyn_arr]->size > 0) {
            printf("Текущее содержимое массива:\n");
            display_dyn_arr(function_dynamic_arrays[current_function_dyn_arr], 0);
        } else {
            printf("Массив пуст\n");
        }

        printf("\n");
        printf("1. Добавить функцию\n");
        printf("2. Применить map\n");
        printf("3. Применить where\n");
        printf("4. Конкатенация с другим массивом функций\n");
        printf("5. Показать содержимое массива\n");
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
                    dynamic_array_push_back(function_dynamic_arrays[current_function_dyn_arr], &example_function);
                    printf("Функция example_function добавлена.\n");
                } else if (func_choice == 2) {
                    dynamic_array_push_back(function_dynamic_arrays[current_function_dyn_arr], &to_uppercase);
                    printf("Функция to_uppercase добавлена.\n");
                } else if (func_choice == 3) {
                    dynamic_array_push_back(function_dynamic_arrays[current_function_dyn_arr], &to_lowercase);
                    printf("Функция to_lowercase добавлена.\n");
                } else if (func_choice == 4) {
                    dynamic_array_push_back(function_dynamic_arrays[current_function_dyn_arr], &print_function_address);
                    printf("Функция print_function_address добавлена.\n");
                } else if (func_choice == 5) {
                    void *null_func = NULL;
                    dynamic_array_push_back(function_dynamic_arrays[current_function_dyn_arr], &null_func);
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
                    DynArr *result = malloc(sizeof(DynArr));
                    if (result && dynamic_array_map(function_dynamic_arrays[current_function_dyn_arr], result, print_function_address)) {
                        dynamic_array_destroy(result);
                        free(result);
                    } else {
                        printf("Ошибка применения map.\n");
                        if (result) free(result);
                    }
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
                    DynArr *result = malloc(sizeof(DynArr));
                    if (result && dynamic_array_where(function_dynamic_arrays[current_function_dyn_arr], result, is_non_null_function)) {
                        printf("Результат применения where (ненулевые функции):\n");
                        display_dyn_arr(result, 0);
                        dynamic_array_destroy(result);
                        free(result);
                    } else {
                        printf("Ошибка применения where или результат пуст.\n");
                        if (result) {
                            dynamic_array_destroy(result);
                            free(result);
                        }
                    }
                } else if (func_choice != 2) {
                    printf("Неверный выбор функции.\n");
                }
                break;
            }
            case 4: {
                printf("Доступные массивы функций для конкатенации:\n");
                for (int i = 0; i < MAX_DYNAMIC_ARRAYS; i++) {
                    if (i != current_function_dyn_arr && function_dynamic_arrays[i] != NULL) {
                        printf("  %d: размер = %zu\n", i, function_dynamic_arrays[i]->size);
                    }
                }

                int target;
                printf("Введите номер массива для конкатенации: ");
                if (scanf("%d", &target) != 1 || target < 0 || target >= MAX_DYNAMIC_ARRAYS ||
                    function_dynamic_arrays[target] == NULL || target == current_function_dyn_arr) {
                    printf("Неверный выбор.\n");
                    clear_input_buffer();
                    break;
                }
                clear_input_buffer();

                DynArr *result = malloc(sizeof(DynArr));
                if (result && dynamic_array_concat(function_dynamic_arrays[current_function_dyn_arr], function_dynamic_arrays[target], result)) {
                    printf("Результат конкатенации:\n");
                    display_dyn_arr(result, 0);
                    dynamic_array_destroy(result);
                    free(result);
                } else {
                    printf("Ошибка конкатенации.\n");
                    if (result) free(result);
                }
                break;
            }
            case 5:
                if (function_dynamic_arrays[current_function_dyn_arr] && function_dynamic_arrays[current_function_dyn_arr]->size > 0) {
                    printf("Содержимое массива:\n");
                    display_dyn_arr(function_dynamic_arrays[current_function_dyn_arr], 0);
                } else {
                    printf("Массив пуст\n");
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
 * Меню сравнения трёхмерных векторов по норме
 */
void vector3d_comparison_menu() {
    int choice;
    while (1) {
        printf("\n");
        printf("============================================\n");
        printf("   БОНУС: СРАВНЕНИЕ 3D ВЕКТОРОВ ПО НОРМЕ\n");
        printf("============================================\n");
        printf("1. Создать и сравнить векторы из строк\n");
        printf("2. Создать и сравнить векторы из функций\n");
        printf("3. Вернуться в главное меню\n");
        printf("Выберите опцию: ");
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: {
                Vector3D vec1, vec2;
                vector3d_init(&vec1, get_string_type_info());
                vector3d_init(&vec2, get_string_type_info());

                printf("\n--- Вектор 1 (3 строки) ---\n");
                for (int i = 0; i < 3; i++) {
                    char buffer[256];
                    printf("Элемент %d: ", i);
                    fgets(buffer, sizeof(buffer), stdin);
                    size_t len = strlen(buffer);
                    if (len > 0 && buffer[len-1] == '\n') {
                        buffer[len-1] = '\0';
                    }
                    char *str = malloc(strlen(buffer) + 1);
                    if (str) {
                        strcpy(str, buffer);
                        vector3d_set(&vec1, i, str);
                    }
                }

                printf("\n--- Вектор 2 (3 строки) ---\n");
                for (int i = 0; i < 3; i++) {
                    char buffer[256];
                    printf("Элемент %d: ", i);
                    fgets(buffer, sizeof(buffer), stdin);
                    size_t len = strlen(buffer);
                    if (len > 0 && buffer[len-1] == '\n') {
                        buffer[len-1] = '\0';
                    }
                    char *str = malloc(strlen(buffer) + 1);
                    if (str) {
                        strcpy(str, buffer);
                        vector3d_set(&vec2, i, str);
                    }
                }

                printf("\n--- Результат сравнения ---\n");
                int cmp = vector3d_compare_by_normal(&vec1, &vec2);
                if (cmp < 0) {
                    printf("Вектор 1 < Вектор 2 (норма вектора 1 меньше)\n");
                } else if (cmp > 0) {
                    printf("Вектор 1 > Вектор 2 (норма вектора 1 больше)\n");
                } else {
                    printf("Вектора равны (нормы и элементы совпадают)\n");
                }

                vector3d_destroy(&vec1);
                vector3d_destroy(&vec2);
                break;
            }
            case 2: {
                Vector3D vec1, vec2;
                vector3d_init(&vec1, get_function_type_info());
                vector3d_init(&vec2, get_function_type_info());

                printf("\n--- Вектор 1 (3 функции) ---\n");
                for (int i = 0; i < 3; i++) {
                    printf("Функция %d:\n", i);
                    printf("  1. to_uppercase\n");
                    printf("  2. to_lowercase\n");
                    printf("  3. example_function\n");
                    printf("  4. print_function_address\n");
                    printf("  5. NULL\n");
                    printf("Выберите: ");
                    int func_choice;
                    if (scanf("%d", &func_choice) != 1) {
                        clear_input_buffer();
                        i--;
                        continue;
                    }
                    clear_input_buffer();

                    void (*func)(void*) = NULL;
                    switch (func_choice) {
                        case 1: func = to_uppercase; break;
                        case 2: func = to_lowercase; break;
                        case 3: func = example_function; break;
                        case 4: func = print_function_address; break;
                        default: func = NULL; break;
                    }
                    vector3d_set(&vec1, i, func);
                }

                printf("\n--- Вектор 2 (3 функции) ---\n");
                for (int i = 0; i < 3; i++) {
                    printf("Функция %d:\n", i);
                    printf("  1. to_uppercase\n");
                    printf("  2. to_lowercase\n");
                    printf("  3. example_function\n");
                    printf("  4. print_function_address\n");
                    printf("  5. NULL\n");
                    printf("Выберите: ");
                    int func_choice;
                    if (scanf("%d", &func_choice) != 1) {
                        clear_input_buffer();
                        i--;
                        continue;
                    }
                    clear_input_buffer();

                    void (*func)(void*) = NULL;
                    switch (func_choice) {
                        case 1: func = to_uppercase; break;
                        case 2: func = to_lowercase; break;
                        case 3: func = example_function; break;
                        case 4: func = print_function_address; break;
                        default: func = NULL; break;
                    }
                    vector3d_set(&vec2, i, func);
                }

                printf("\n--- Результат сравнения ---\n");
                int cmp = vector3d_compare_by_normal(&vec1, &vec2);
                if (cmp < 0) {
                    printf("Вектор 1 < Вектор 2 (меньше ненулевых функций)\n");
                } else if (cmp > 0) {
                    printf("Вектор 1 > Вектор 2 (больше ненулевых функций)\n");
                } else {
                    printf("Вектора равны (одинаковое количество ненулевых функций)\n");
                }
                break;
            }
            case 3:
                return;
            default:
                printf("Неверный выбор.\n");
        }
    }
}

/**
 * Show array content function
 * @param da DynArr to show
 * @param is_string 1 for str, 0 for func
 */
void display_dyn_arr(const DynArr *da, int is_string) {
    if (!da || da->size == 0) {
        printf("Массив пуст\n");
        return;
    }

    printf("Размер: %zu, Емкость: %zu\n", da->size, da->capacity);
    printf("Содержимое:\n");

    for (size_t i = 0; i < da->size; i++) {
        printf("  [%zu]: ", i);
        void *elem = (char*)da->data + i * da->type->size;

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

    // Инициализация первого массива строк
    string_dynamic_arrays[0] = malloc(sizeof(DynArr));
    if (string_dynamic_arrays[0]) {
        dynamic_array_init(string_dynamic_arrays[0], get_string_type_info(), 4);
    }
    // Инициализация первого массива функций
    function_dynamic_arrays[0] = malloc(sizeof(DynArr));
    if (function_dynamic_arrays[0]) {
        dynamic_array_init(function_dynamic_arrays[0], get_function_type_info(), 4);
    }

    printf("Лабораторная работа 1 (вариант 20) - полиморфная коллекция на основе динамического массива.\n");
    printf("Эта программа позволяет работать со строками и указателями на функции.\n");

    main_menu();

    return 0;
}
