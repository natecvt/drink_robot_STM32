#include "unity.h"

void setUp(void) {
    // Runs before each test
}

void tearDown(void) {
    // Runs after each test
}

void test_calculator_addition(void) {
    TEST_ASSERT_EQUAL(32, 20 + 12);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_calculator_addition);
    return UNITY_END();
}