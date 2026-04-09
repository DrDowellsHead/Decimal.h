#include "s21_decimal.h"

#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_auxfunct.h"

// int s21_from_int_to_decimal(int src, s21_decimal *dst) 
START_TEST(test_s21_from_int_to_decimal_zero) {
  int x = 0, res;
  s21_decimal x_dec_res, x_dec_purp = {0};
  //x_dec_purp.bits[0] = 0;
  //x_dec_purp.bits[1] = 0;
  //x_dec_purp.bits[2] = 0;
  //x_dec_purp.bits[3] = 0;
  res = s21_from_int_to_decimal(x, &x_dec_res);
  //ck_assert_int_eq(x_dec_res.bits[0], x_dec_purp.bits[0]);
  //ck_assert_int_eq(x_dec_res.bits[1], x_dec_purp.bits[1]);
  //ck_assert_int_eq(x_dec_res.bits[2], x_dec_purp.bits[2]);
  //ck_assert_int_eq(x_dec_res.bits[3], x_dec_purp.bits[3]);
  // Сравниваем всю структуру (128 бит) одной проверкой
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_memchr_not_found) {
  char str[] = "hello world";
  ck_assert_ptr_eq(s21_memchr(str, 'x', 11), memchr(str, 'x', 11));
}
END_TEST

START_TEST(test_memchr_n_zero) {
  char str[] = "hello world";
  ck_assert_ptr_eq(s21_memchr(str, 'h', 0), memchr(str, 'h', 0));
}
END_TEST

START_TEST(test_memchr_null) {
  char str[] = "hello\0world";
  ck_assert_ptr_eq(s21_memchr(str, '\0', 11), memchr(str, '\0', 11));
}
END_TEST

// int s21_from_float_to_decimal(float src, s21_decimal *dst)
START_TEST(test_memcmp_equal) {
  ck_assert_int_eq(s21_memcmp("abc", "abc", 3), memcmp("abc", "abc", 3));
}
END_TEST

START_TEST(test_memcmp_less) {
  int res1 = s21_memcmp("abc", "abd", 3);
  int res2 = memcmp("abc", "abd", 3);
  ck_assert_int_eq((res1 < 0), (res2 < 0));
}
END_TEST

START_TEST(test_memcmp_greater) {
  int res1 = s21_memcmp("abd", "abc", 3);
  int res2 = memcmp("abd", "abc", 3);
  ck_assert_int_eq((res1 > 0), (res2 > 0));
}
END_TEST

START_TEST(test_memcmp_n_zero) {
  ck_assert_int_eq(s21_memcmp("abc", "xyz", 0), memcmp("abc", "xyz", 0));
}
END_TEST

// int s21_from_decimal_to_int(s21_decimal src, int *dst)
START_TEST(test_memcpy_basic) {
  char dst1[20] = {0}, dst2[20] = {0};
  s21_memcpy(dst1, "hello", 5);
  memcpy(dst2, "hello", 5);
  ck_assert_str_eq(dst1, dst2);
}
END_TEST

START_TEST(test_memcpy_n_zero) {
  char dst1[20] = "original", dst2[20] = "original";
  s21_memcpy(dst1, "hello", 0);
  memcpy(dst2, "hello", 0);
  ck_assert_str_eq(dst1, dst2);
}
END_TEST

START_TEST(test_memcpy_null) {
  char str[] = "ab\0cd";
  char dst1[20] = {0}, dst2[20] = {0};
  s21_memcpy(dst1, str, 5);
  memcpy(dst2, str, 5);
  ck_assert_str_eq(dst1, dst2);
}
END_TEST

// int s21_from_decimal_to_float(s21_decimal src, float *dst)
START_TEST(test_memset_basic) {
  char dst1[10], dst2[10];
  s21_memset(dst1, 'x', 10);
  memset(dst2, 'x', 10);
  ck_assert_mem_eq(dst1, dst2, 10);
}
END_TEST

START_TEST(test_memset_zero) {
  char dst1[10] = "hello", dst2[10] = "hello";
  s21_memset(dst1, 0, 10);
  memset(dst2, 0, 10);
  ck_assert_mem_eq(dst1, dst2, 10);
}
END_TEST

Suite *s21_decimal_suite() {
  Suite *s = suite_create("s21_decimal");
  TCase *tc;

  tc = tcase_create("s21_from_int_to_decimal");
  tcase_add_test(tc, test_s21_from_int_to_decimal_zero);
  tcase_add_test(tc, test_memchr_not_found);
  tcase_add_test(tc, test_memchr_n_zero);
  tcase_add_test(tc, test_memchr_null);
  suite_add_tcase(s, tc);

  tc = tcase_create("s21_from_float_to_decimal");
  tcase_add_test(tc, test_memcmp_equal);
  tcase_add_test(tc, test_memcmp_less);
  tcase_add_test(tc, test_memcmp_greater);
  tcase_add_test(tc, test_memcmp_n_zero);
  suite_add_tcase(s, tc);

  tc = tcase_create("s21_from_decimal_to_int");
  tcase_add_test(tc, test_memcpy_basic);
  tcase_add_test(tc, test_memcpy_n_zero);
  tcase_add_test(tc, test_memcpy_null);
  suite_add_tcase(s, tc);

  tc = tcase_create("s21_from_decimal_to_float");
  tcase_add_test(tc, test_memset_basic);
  tcase_add_test(tc, test_memset_zero);
  suite_add_tcase(s, tc);

  return s;
}

int main() {
  int failed;
  Suite *s = s21_decimal_suite();
  SRunner *runner = srunner_create(s);
  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}