#include <Arduino.h>
#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
}

void test_string_example(void) {
  String hello = "Hello ";
  String world = "world";
  TEST_ASSERT_EQUAL_STRING("Hello world", (hello + world).c_str());
}

void setup() {
  delay(2000);
  UNITY_BEGIN();

  RUN_TEST(test_string_example);

  UNITY_END();
}

void loop() {
  // Unused for tests
}
