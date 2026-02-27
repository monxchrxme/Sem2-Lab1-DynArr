#include "unity/unity.h"

void setUp(void) {
    // Выполняется перед каждым тестом
}

void tearDown(void) {
    // Выполняется после каждого теста
}

void test_example(void) {
    TEST_ASSERT_TRUE(1);
    TEST_ASSERT_EQUAL_INT(1, 1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_example);
    return UNITY_END();
}