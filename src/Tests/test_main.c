#include <check.h>
#include <stdlib.h>

Suite *test_func(void);

int main(void) {
  int number_failed;
  SRunner *sr = srunner_create(test_func());

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}