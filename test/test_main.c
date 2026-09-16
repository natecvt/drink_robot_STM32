#include <unity.h>
#include <button.h>

void setUp(void) {
    // Runs before each test
}

void tearDown(void) {
    // Runs after each test
}

void test_button() {
    UNITY_TEST_ASSERT_EQUAL_UINT8(button_init(0, 0), BUTTON_OK, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(button_init(2, 0), BUTTON_UNKNOWN, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(button_get_state(NULL, 0), BUTTON_OK, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(button_get_state(NULL, 2), BUTTON_UNKNOWN, __LINE__, "");
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_button);
    return UNITY_END();
}