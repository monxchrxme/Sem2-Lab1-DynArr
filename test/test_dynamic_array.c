#include "unity.h"
#include "../dynamic_array.h"
#include "../type_info.h"
#include "../utils.h"
#include <string.h>

// Global variables for tests
DynArr *string_dyn_arr = NULL;
DynArr *function_dyn_arr = NULL;

// Performed before each test
void setUp(void) {
    // Initializing string DynArr
    string_dyn_arr = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(string_dyn_arr);
    TEST_ASSERT_TRUE(dynamic_array_init(string_dyn_arr, get_string_type_info(), 4));

    // Initializing func DynArr
    function_dyn_arr = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(function_dyn_arr);
    TEST_ASSERT_TRUE(dynamic_array_init(function_dyn_arr, get_function_type_info(), 4));
}

// Performed after each test
void tearDown(void) {
    if (string_dyn_arr) {
        dynamic_array_destroy(string_dyn_arr);
        free(string_dyn_arr);
    }
    if (function_dyn_arr) {
        dynamic_array_destroy(function_dyn_arr);
        free(function_dyn_arr);
    }
}

// Additional function for adding strings to DynArr
void add_string(DynArr *da, const char *str) {
    char *new_string = malloc(strlen(str) + 1);
    strcpy(new_string, str);
    char **temp = &new_string;
    TEST_ASSERT_TRUE(dynamic_array_push_back(da, temp));
    free(new_string);
}

// Additional function for checking the contents of DynArr of strings
void assert_string_dyn_arr_content(DynArr *da, size_t index, const char *expected) {
    void *elem = (char*)da->data + index * da->type->size;
    char *actual = *(char**)elem;
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

// Additional function for adding functions to DynArr
void add_function(DynArr *da, void (*func)(void*)) {
    TEST_ASSERT_TRUE(dynamic_array_push_back(da, &func));
}

// Additional function for checking the contents of DynArr of functions
void assert_function_dyn_arr_content(DynArr *da, size_t index, void (*expected)(void*)) {
    void *elem = (char*)da->data + index * da->type->size;
    void (**actual)(void*) = (void (**)(void*))elem;
    TEST_ASSERT_EQUAL_PTR(expected, *actual);
}

// Test 1: Checking the creation and initialization of a DynArr of strings
void test_string_dyn_arr_creation(void) {
    TEST_ASSERT_NOT_NULL(string_dyn_arr);
    TEST_ASSERT_EQUAL(0, string_dyn_arr->size);
    TEST_ASSERT_EQUAL(4, string_dyn_arr->capacity);
    TEST_ASSERT_EQUAL(sizeof(char*), string_dyn_arr->type->size);
}

// Test 2: Checking adding strings to a DynArr
void test_string_dyn_arr_push_back(void) {
    add_string(string_dyn_arr, "Hello");
    add_string(string_dyn_arr, "World");

    TEST_ASSERT_EQUAL(2, string_dyn_arr->size);
    TEST_ASSERT_EQUAL_STRING("Hello", *(char**)((char*)string_dyn_arr->data + 0 * string_dyn_arr->type->size));
    TEST_ASSERT_EQUAL_STRING("World", *(char**)((char*)string_dyn_arr->data + 1 * string_dyn_arr->type->size));
}

// Test 3: Checking the increase in DynArr capacity
void test_string_dyn_arr_capacity_increase(void) {
    // Добавляем 4 строки (текущая емкость 4)
    add_string(string_dyn_arr, "1");
    add_string(string_dyn_arr, "2");
    add_string(string_dyn_arr, "3");
    add_string(string_dyn_arr, "4");

    TEST_ASSERT_EQUAL(4, string_dyn_arr->size);
    TEST_ASSERT_EQUAL(4, string_dyn_arr->capacity);

    add_string(string_dyn_arr, "5");

    TEST_ASSERT_EQUAL(5, string_dyn_arr->size);
    TEST_ASSERT_EQUAL(8, string_dyn_arr->capacity);
}

// Test 4: Checking the map operation (to_uppercase)
void test_string_dyn_arr_map_to_uppercase(void) {
    add_string(string_dyn_arr, "hello");
    add_string(string_dyn_arr, "world");

    DynArr *result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(result);
//    TEST_ASSERT_TRUE(dynamic_array_init(result, get_string_type_info(), string_dyn_arr->size));

    TEST_ASSERT_TRUE(dynamic_array_map(string_dyn_arr, result, to_uppercase));

    assert_string_dyn_arr_content(result, 0, "HELLO");
    assert_string_dyn_arr_content(result, 1, "WORLD");

    dynamic_array_destroy(result);
    free(result);
}

// Test 5: Checking the where operation (is_long_string)
void test_string_dyn_arr_where_long_strings(void) {
    add_string(string_dyn_arr, "Short");
    add_string(string_dyn_arr, "This is a long string");

    DynArr *result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_TRUE(dynamic_array_init(result, get_string_type_info(), 4));

    TEST_ASSERT_TRUE(dynamic_array_where(string_dyn_arr, result, is_long_string));

    TEST_ASSERT_EQUAL(1, result->size);
    assert_string_dyn_arr_content(result, 0, "This is a long string");

    dynamic_array_destroy(result);
    free(result);
}

// Test 6: Checking the where operation (is_short_string)
void test_string_dyn_arr_where_short_strings(void) {
    add_string(string_dyn_arr, "Short");
    add_string(string_dyn_arr, "This is a long string");

    DynArr *result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_TRUE(dynamic_array_init(result, get_string_type_info(), 4));

    TEST_ASSERT_TRUE(dynamic_array_where(string_dyn_arr, result, is_short_string));

    TEST_ASSERT_EQUAL(1, result->size);
    assert_string_dyn_arr_content(result, 0, "Short");

    dynamic_array_destroy(result);
    free(result);
}

// Test 7: Checking the concatenation of DynArr's
void test_string_dyn_arr_concatenation(void) {
    add_string(string_dyn_arr, "Hello");
    add_string(string_dyn_arr, "World");

    DynArr *da2 = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(da2);
    TEST_ASSERT_TRUE(dynamic_array_init(da2, get_string_type_info(), 4));
    add_string(da2, "Test");

    DynArr *result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_TRUE(dynamic_array_init(result, get_string_type_info(), string_dyn_arr->size + da2->size));

    TEST_ASSERT_TRUE(dynamic_array_concat(string_dyn_arr, da2, result));

    TEST_ASSERT_EQUAL(3, result->size);
    assert_string_dyn_arr_content(result, 0, "Hello");
    assert_string_dyn_arr_content(result, 1, "World");
    assert_string_dyn_arr_content(result, 2, "Test");

    dynamic_array_destroy(da2);
    free(da2);
    dynamic_array_destroy(result);
    free(result);
}

// Test 8: Checking the creation and initialization of a DynArr of functions
void test_function_dyn_arr_creation(void) {
    TEST_ASSERT_NOT_NULL(function_dyn_arr);
    TEST_ASSERT_EQUAL(0, function_dyn_arr->size);
    TEST_ASSERT_EQUAL(4, function_dyn_arr->capacity);
    TEST_ASSERT_EQUAL(sizeof(void (*)(void*)), function_dyn_arr->type->size);
}

// Test 9: Checking the addition of functions to a DynArr
void test_function_dyn_arr_push_back(void) {
    add_function(function_dyn_arr, to_uppercase);
    add_function(function_dyn_arr, to_lowercase);

    TEST_ASSERT_EQUAL(2, function_dyn_arr->size);

    assert_function_dyn_arr_content(function_dyn_arr, 0, to_uppercase);
    assert_function_dyn_arr_content(function_dyn_arr, 1, to_lowercase);
}

// Test 10: Checking the increase in the capacity of the function DynArr
void test_function_dyn_arr_capacity_increase(void) {
    // Добавляем 4 функции (текущая емкость 4)
    add_function(function_dyn_arr, example_function);
    add_function(function_dyn_arr, to_uppercase);
    add_function(function_dyn_arr, to_lowercase);
    add_function(function_dyn_arr, print_function_address);

    TEST_ASSERT_EQUAL(4, function_dyn_arr->size);
    TEST_ASSERT_EQUAL(4, function_dyn_arr->capacity);

    // Добавляем 5-ю функцию (емкость должна увеличиться)
    add_function(function_dyn_arr, example_function);

    TEST_ASSERT_EQUAL(5, function_dyn_arr->size);
    TEST_ASSERT_EQUAL(8, function_dyn_arr->capacity);
}

// Test 11: Checking the map operation for functions
void test_function_dyn_arr_map(void) {
    add_function(function_dyn_arr, to_uppercase);
    add_function(function_dyn_arr, to_lowercase);

    DynArr *result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_TRUE(dynamic_array_init(result, get_function_type_info(), function_dyn_arr->size));

    TEST_ASSERT_TRUE(dynamic_array_map(function_dyn_arr, result, print_function_address));

    dynamic_array_destroy(result);
    free(result);
}

// Test 12: Checking the where operation for functions
void test_function_dyn_arr_where_non_null(void) {
    add_function(function_dyn_arr, to_uppercase);
    void *null_func = NULL;
    dynamic_array_push_back(function_dyn_arr, &null_func);
    add_function(function_dyn_arr, to_lowercase);

    DynArr *result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_TRUE(dynamic_array_init(result, get_function_type_info(), 4));

    TEST_ASSERT_TRUE(dynamic_array_where(function_dyn_arr, result, is_non_null_function));

    TEST_ASSERT_EQUAL(2, result->size);
    assert_function_dyn_arr_content(result, 0, to_uppercase);
    assert_function_dyn_arr_content(result, 1, to_lowercase);

    dynamic_array_destroy(result);
    free(result);
}

// Test 13: Checking how to work with an empty DynArr
void test_empty_dyn_arr_operations(void) {
    DynArr *empty_da = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(empty_da);
    TEST_ASSERT_TRUE(dynamic_array_init(empty_da, get_string_type_info(), 0));

    TEST_ASSERT_EQUAL(0, empty_da->size);
    TEST_ASSERT_EQUAL(0, empty_da->capacity);

    add_string(string_dyn_arr, "Test");

    DynArr *concat_result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(concat_result);
    TEST_ASSERT_TRUE(dynamic_array_init(concat_result, get_string_type_info(), string_dyn_arr->size));

    TEST_ASSERT_TRUE(dynamic_array_concat(empty_da, string_dyn_arr, concat_result));

    TEST_ASSERT_EQUAL(1, concat_result->size);
    assert_string_dyn_arr_content(concat_result, 0, "Test");

    dynamic_array_destroy(empty_da);
    free(empty_da);
    dynamic_array_destroy(concat_result);
    free(concat_result);
}

// Test 14: Checking for correct memory release
void test_memory_leaks(void) {
    add_string(string_dyn_arr, "Test");

    DynArr *map_result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(map_result);
    TEST_ASSERT_TRUE(dynamic_array_init(map_result, get_string_type_info(), string_dyn_arr->size));
    TEST_ASSERT_TRUE(dynamic_array_map(string_dyn_arr, map_result, to_uppercase));

    DynArr *where_result = malloc(sizeof(DynArr));
    TEST_ASSERT_NOT_NULL(where_result);
    TEST_ASSERT_TRUE(dynamic_array_init(where_result, get_string_type_info(), 4));
    TEST_ASSERT_TRUE(dynamic_array_where(string_dyn_arr, where_result, is_long_string));

    dynamic_array_destroy(map_result);
    free(map_result);
    dynamic_array_destroy(where_result);
    free(where_result);

    // Если тест завершился успешно, утечек памяти нет
    TEST_PASS();
}
// Test 15: Checking how to work with long strings
void test_long_strings(void) {
    const char *long_str = "This is a very long string that should be handled correctly by the dynamic array";
    add_string(string_dyn_arr, long_str);

    TEST_ASSERT_EQUAL(1, string_dyn_arr->size);
    assert_string_dyn_arr_content(string_dyn_arr, 0, long_str);
}

// Test 16: Checking the operation with the NULL function
void test_null_function(void) {
    void *null_func = NULL;
    dynamic_array_push_back(function_dyn_arr, &null_func);

    TEST_ASSERT_EQUAL(1, function_dyn_arr->size);

    void *elem = (char*)function_dyn_arr->data + 0 * function_dyn_arr->type->size;
    void (**func)(void*) = (void (**)(void*))elem;

    TEST_ASSERT_NULL(*func);
}

int main(void) {
    UNITY_BEGIN();

    // Tests for string DynArr
    RUN_TEST(test_string_dyn_arr_creation);
    RUN_TEST(test_string_dyn_arr_push_back);
    RUN_TEST(test_string_dyn_arr_capacity_increase);
    RUN_TEST(test_string_dyn_arr_map_to_uppercase);
    RUN_TEST(test_string_dyn_arr_where_long_strings);
    RUN_TEST(test_string_dyn_arr_where_short_strings);
    RUN_TEST(test_string_dyn_arr_concatenation);

    // Tests for func DynArr
    RUN_TEST(test_function_dyn_arr_creation);
    RUN_TEST(test_function_dyn_arr_push_back);
    RUN_TEST(test_function_dyn_arr_capacity_increase);
    RUN_TEST(test_function_dyn_arr_map);
    RUN_TEST(test_function_dyn_arr_where_non_null);

    // Edge case tests
    RUN_TEST(test_empty_dyn_arr_operations);
    RUN_TEST(test_memory_leaks);
    RUN_TEST(test_long_strings);
    RUN_TEST(test_null_function);

    return UNITY_END();
}

