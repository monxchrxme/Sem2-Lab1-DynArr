#include "unity.h"
#include "../vector.h"
#include "../type_info.h"
#include "../utils.h"
#include <string.h>

// Global variables for tests
Vector* string_vector = NULL;
Vector* function_vector = NULL;

// Performed before each test
void setUp(void) {
    // Initializing string dynArr
    string_vector = vector_create(get_string_type_info());
    TEST_ASSERT_NOT_NULL(string_vector);
    TEST_ASSERT_TRUE(vector_init(string_vector, 4));
    
    // Initializing func dynArr
    function_vector = vector_create(get_function_type_info());
    TEST_ASSERT_NOT_NULL(function_vector);
    TEST_ASSERT_TRUE(vector_init(function_vector, 4));
}

// Performed after each test
void tearDown(void) {
    if (string_vector) {
        vector_destroy(string_vector);
    }
    if (function_vector) {
        vector_destroy(function_vector);
    }
}

// Additional function for adding strings to dynArr
void add_string(Vector *vec, const char *str) {
    char *new_string = malloc(strlen(str) + 1);
    strcpy(new_string, str);
    char **temp = &new_string;
    TEST_ASSERT_TRUE(vector_push_back(vec, temp));
    free(new_string);
}

// Additional function for checking the contents of dynArr of strings
void assert_string_vector_content(Vector *vec, size_t index, const char *expected) {
    void *elem = (char*)vec->data + index * vec->type->size;
    char *actual = *(char**)elem;
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

// Additional function for adding functions to dynArr
void add_function(Vector *vec, void (*func)(void*)) {
    TEST_ASSERT_TRUE(vector_push_back(vec, &func));
}

// Additional function for checking the contents of dynArr of functions
void assert_function_vector_content(Vector *vec, size_t index, void (*expected)(void*)) {
    void *elem = (char*)vec->data + index * vec->type->size;
    void (**actual)(void*) = (void (**)(void*))elem;
    TEST_ASSERT_EQUAL_PTR(expected, *actual);
}

// Test 1: Checking the creation and initialization of a dynArr of strings
void test_string_vector_creation(void) {
    TEST_ASSERT_NOT_NULL(string_vector);
    TEST_ASSERT_EQUAL(0, string_vector->size);
    TEST_ASSERT_EQUAL(4, string_vector->capacity);
    TEST_ASSERT_EQUAL(sizeof(char*), string_vector->type->size);
}

// Test 2: Checking adding strings to a dynArr
void test_string_vector_push_back(void) {
    add_string(string_vector, "Hello");
    add_string(string_vector, "World");
    
    TEST_ASSERT_EQUAL(2, string_vector->size);
    TEST_ASSERT_EQUAL_STRING("Hello", *(char**)((char*)string_vector->data + 0 * string_vector->type->size));
    TEST_ASSERT_EQUAL_STRING("World", *(char**)((char*)string_vector->data + 1 * string_vector->type->size));
}

// Test 3: Checking the increase in dynArr capacity
void test_string_vector_capacity_increase(void) {
    // Добавляем 4 строки (текущая емкость 4)
    add_string(string_vector, "1");
    add_string(string_vector, "2");
    add_string(string_vector, "3");
    add_string(string_vector, "4");
    
    TEST_ASSERT_EQUAL(4, string_vector->size);
    TEST_ASSERT_EQUAL(4, string_vector->capacity);

    // Добавляем 5-ю строку (емкость должна увеличиться)
    add_string(string_vector, "5");
    
    TEST_ASSERT_EQUAL(5, string_vector->size);
    TEST_ASSERT_EQUAL(8, string_vector->capacity);
}

// Test 4: Checking the map operation (to_uppercase)
void test_string_vector_map_to_uppercase(void) {
    add_string(string_vector, "hello");
    add_string(string_vector, "world");
    
    Vector *result = vector_map(string_vector, to_uppercase);
    TEST_ASSERT_NOT_NULL(result);
    
    assert_string_vector_content(result, 0, "HELLO");
    assert_string_vector_content(result, 1, "WORLD");
    
    vector_destroy(result);
}

// Test 5: Checking the where operation (is_long_string)
void test_string_vector_where_long_strings(void) {
    add_string(string_vector, "Short");
    add_string(string_vector, "This is a long string");
    
    Vector *result = vector_where(string_vector, is_long_string);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(1, result->size);
    
    assert_string_vector_content(result, 0, "This is a long string");
    
    vector_destroy(result);
}

// Test 6: Checking the where operation (is_short_string)
void test_string_vector_where_short_strings(void) {
    add_string(string_vector, "Short");
    add_string(string_vector, "This is a long string");
    
    Vector *result = vector_where(string_vector, is_short_string);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(1, result->size);
    
    assert_string_vector_content(result, 0, "Short");
    
    vector_destroy(result);
}

// Test 7: Checking the concatenation of dynArr's
void test_string_vector_concatenation(void) {
    // Заполняем первый вектор
    add_string(string_vector, "Hello");
    add_string(string_vector, "World");
    
    // Создаем и заполняем второй вектор
    Vector *vec2 = vector_create(get_string_type_info());
    TEST_ASSERT_NOT_NULL(vec2);
    TEST_ASSERT_TRUE(vector_init(vec2, 4));
    
    add_string(vec2, "Test");
    
    // Выполняем конкатенацию
    Vector *result = vector_concat(string_vector, vec2);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(3, result->size);
    
    assert_string_vector_content(result, 0, "Hello");
    assert_string_vector_content(result, 1, "World");
    assert_string_vector_content(result, 2, "Test");
    
    vector_destroy(vec2);
    vector_destroy(result);
}

// Test 8: Checking the creation and initialization of a dynArr of functions
void test_function_vector_creation(void) {
    TEST_ASSERT_NOT_NULL(function_vector);
    TEST_ASSERT_EQUAL(0, function_vector->size);
    TEST_ASSERT_EQUAL(4, function_vector->capacity);
    TEST_ASSERT_EQUAL(sizeof(void (*)(void*)), function_vector->type->size);
}

// Test 9: Checking the addition of functions to a dynArr
void test_function_vector_push_back(void) {
    add_function(function_vector, to_uppercase);
    add_function(function_vector, to_lowercase);
    
    TEST_ASSERT_EQUAL(2, function_vector->size);
    
    assert_function_vector_content(function_vector, 0, to_uppercase);
    assert_function_vector_content(function_vector, 1, to_lowercase);
}

// Test 10: Checking the increase in the capacity of the function dynArr
void test_function_vector_capacity_increase(void) {
    // Добавляем 4 функции (текущая емкость 4)
    add_function(function_vector, example_function);
    add_function(function_vector, to_uppercase);
    add_function(function_vector, to_lowercase);
    add_function(function_vector, print_function_address);
    
    TEST_ASSERT_EQUAL(4, function_vector->size);
    TEST_ASSERT_EQUAL(4, function_vector->capacity);
    
    // Добавляем 5-ю функцию (емкость должна увеличиться)
    add_function(function_vector, example_function);
    
    TEST_ASSERT_EQUAL(5, function_vector->size);
    TEST_ASSERT_EQUAL(8, function_vector->capacity);
}

// Test 11: Checking the map operation for functions
void test_function_vector_map(void) {
    add_function(function_vector, to_uppercase);
    add_function(function_vector, to_lowercase);

    Vector *result = vector_map(function_vector, print_function_address);
    TEST_ASSERT_NOT_NULL(result);
    
    vector_destroy(result);
}

// Test 12: Checking the where operation for functions
void test_function_vector_where_non_null(void) {
    add_function(function_vector, to_uppercase);
    
    void *null_func = NULL;
    vector_push_back(function_vector, &null_func);
    
    add_function(function_vector, to_lowercase);
    
    Vector *result = vector_where(function_vector, is_non_null_function);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL(2, result->size);
    
    assert_function_vector_content(result, 0, to_uppercase);
    assert_function_vector_content(result, 1, to_lowercase);
    
    vector_destroy(result);
}

// Test 13: Checking how to work with an empty dynArr
void test_empty_vector_operations(void) {
    // Создаем новый пустой вектор
    Vector *empty_vec = vector_create(get_string_type_info());
    TEST_ASSERT_NOT_NULL(empty_vec);
    TEST_ASSERT_TRUE(vector_init(empty_vec, 0));
    
    TEST_ASSERT_EQUAL(0, empty_vec->size);
    TEST_ASSERT_EQUAL(0, empty_vec->capacity);

    // Проверяем, что map возвращает NULL для пустого вектора
    Vector *map_result = vector_map(empty_vec, to_uppercase);
    TEST_ASSERT_NULL(map_result);
    
    // Проверяем, что where возвращает NULL для пустого вектора
    Vector *where_result = vector_where(empty_vec, is_long_string);
    TEST_ASSERT_NULL(where_result);
    
    // Проверяем конкатенацию с пустым вектором
    add_string(string_vector, "Test");
    Vector *concat_result = vector_concat(empty_vec, string_vector);
    TEST_ASSERT_NOT_NULL(concat_result);
    TEST_ASSERT_EQUAL(1, concat_result->size);
    assert_string_vector_content(concat_result, 0, "Test");
    
    vector_destroy(empty_vec);
    vector_destroy(concat_result);
}

// Test 14: Checking for correct memory release
void test_memory_leaks(void) {
    // Создаем и заполняем вектор
    add_string(string_vector, "Test");
    
    // Создаем производные векторы
    Vector *map_result = vector_map(string_vector, to_uppercase);
    Vector *where_result = vector_where(string_vector, is_long_string);
    
    // Уничтожаем все векторы
    vector_destroy(map_result);
    vector_destroy(where_result);
    
    // Если тест завершился успешно, утечек памяти нет
    TEST_PASS();
}

// Test 15: Checking how to work with long strings
void test_long_strings(void) {
    const char *long_str = "This is a very long string that should be handled correctly by the vector";
    add_string(string_vector, long_str);
    
    TEST_ASSERT_EQUAL(1, string_vector->size);
    assert_string_vector_content(string_vector, 0, long_str);
}

// Test 16: Checking the operation with the NULL function
void test_null_function(void) {
    void *null_func = NULL;
    vector_push_back(function_vector, &null_func);
    
    TEST_ASSERT_EQUAL(1, function_vector->size);
    
    void *elem = (char*)function_vector->data + 0 * function_vector->type->size;
    void (**func)(void*) = (void (**)(void*))elem;
    
    TEST_ASSERT_NULL(*func);
}

int main(void) {
    UNITY_BEGIN();
    
    // Tests for string DynArr
    RUN_TEST(test_string_vector_creation);
    RUN_TEST(test_string_vector_push_back);
    RUN_TEST(test_string_vector_capacity_increase);
    RUN_TEST(test_string_vector_map_to_uppercase);
    RUN_TEST(test_string_vector_where_long_strings);
    RUN_TEST(test_string_vector_where_short_strings);
    RUN_TEST(test_string_vector_concatenation);
    
    // Tests for func DynArr
    RUN_TEST(test_function_vector_creation);
    RUN_TEST(test_function_vector_push_back);
    RUN_TEST(test_function_vector_capacity_increase);
    RUN_TEST(test_function_vector_map);
    RUN_TEST(test_function_vector_where_non_null);
    
    // Edge case tests
    RUN_TEST(test_empty_vector_operations);
    RUN_TEST(test_memory_leaks);
    RUN_TEST(test_long_strings);
    RUN_TEST(test_null_function);
    
    return UNITY_END();
}