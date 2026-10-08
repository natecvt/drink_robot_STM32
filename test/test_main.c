#include <unity.h>
#include "button.h"
#include "ringbuf.h"

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

void test_ringbuf() {
    ringbuf_init();
    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_produce(0), RINGBUF_OK, __LINE__, "");
    /** by god do not use this pointer */
    ringbuf_produce(1);
    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_produce(0), RINGBUF_ERR, __LINE__, "");

    void *item;
    ringbuf_access_item(&item);
    UNITY_TEST_ASSERT_EQUAL_UINT8(item, 0, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_consume(), RINGBUF_OK, __LINE__, "");
    ringbuf_access_item(&item);
    UNITY_TEST_ASSERT_EQUAL_UINT8(item, 1, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_consume(), RINGBUF_OK, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_consume(), RINGBUF_ERR, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(rb_cindex, 2, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(rb_pindex, 2, __LINE__, "");

    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_produce(0), RINGBUF_OK, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_produce(0), RINGBUF_OK, __LINE__, "");
    UNITY_TEST_ASSERT_EQUAL_UINT8(ringbuf_produce(0), RINGBUF_ERR, __LINE__, "");

    /** #TODO: test index maxxing behavior */
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_button);
    RUN_TEST(test_ringbuf);
    return UNITY_END();
}