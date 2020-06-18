#include "comparator.h"
#include "test_util.h"
#include <assert.h>

void test_double_less_then() {
  assert(double_less_then(5, 3) == false);
  assert(double_less_then(3, 5) == true);
  assert(double_less_then(5, 5) == false);
}

void test_double_greater_then() {
  assert(double_great_then(5, 3) == true);
  assert(double_great_then(3, 5) == false);
  assert(double_great_then(5, 5) == false);
}

int main(int argc, char *argv[]) {
  // Run all tests if there are no command-line arguments
  bool all_tests = argc == 1;
  // Read test name from file
  char testname[100];
  if (!all_tests) {
      read_testname(argv[1], testname, sizeof(testname));
  }

  DO_TEST(test_double_less_then)
  DO_TEST(test_double_greater_then)

  puts("comparator_test PASS");
  return 0;
}
