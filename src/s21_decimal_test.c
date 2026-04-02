#include "s21_decimal.h"

#include <check.h>

#include "s21_decimal_utils.h"

START_TEST(test_negate_positive) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_decimal result;
  int err = s21_negate(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
  ck_assert_int_eq(result.bits[0], 123456789);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_set_sign(&value, 1);
  s21_decimal result;
  int err = s21_negate(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;
  int err = s21_negate(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_negate_null) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  int err = s21_negate(value, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_truncate_no_fraction) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_decimal result;
  int err = s21_truncate(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 123456789);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_truncate_positive) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_set_scale(&value, 3);
  s21_decimal result;
  int err = s21_truncate(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_truncate_negative) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_set_sign(&value, 1);
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_truncate(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 12345678);
  ck_assert_int_eq(s21_get_sign(result), 1);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_truncate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_set_scale(&value, 5);
  s21_decimal result;
  int err = s21_truncate(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_truncate_null) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  int err = s21_truncate(value, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_floor_positive) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_floor(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 12345678);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_floor_negative_with_fraction) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_set_sign(&value, 1);
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_floor(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 12345679);
  ck_assert_int_eq(s21_get_sign(result), 1);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_floor_no_fraction) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_decimal result;
  int err = s21_floor(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 123456789);
}
END_TEST

START_TEST(test_floor_negative_no_fraction) {
  s21_decimal value = {{123456780, 0, 0, 0}};
  s21_set_scale(&value, 1);
  s21_set_sign(&value, 1);
  s21_decimal result;
  int err = s21_floor(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 12345678);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_set_scale(&value, 3);
  s21_decimal result;
  int err = s21_floor(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_floor_null) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  int err = s21_floor(value, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_round_no_fraction) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_decimal result;
  int err = s21_round(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 123456789);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_round_less_then_half) {
  s21_decimal value = {{124, 0, 0, 0}};
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_round(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_round_greater_then_half) {
  s21_decimal value = {{126, 0, 0, 0}};
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_round(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 13);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_round_half_even_down) {
  s21_decimal value = {{125, 0, 0, 0}};
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_round(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_round_half_even_up) {
  s21_decimal value = {{135, 0, 0, 0}};
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_round(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 14);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_round_negative) {
  s21_decimal value = {{126, 0, 0, 0}};
  s21_set_sign(&value, 1);
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_round(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 13);
  ck_assert_int_eq(s21_get_sign(result), 1);
  ck_assert_int_eq(s21_get_scale(result), 0);
}
END_TEST

START_TEST(test_round_null) {
  s21_decimal value = {{126, 0, 0, 0}};
  int err = s21_round(value, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST

Suite *s21_decimal_suite(void) {
  Suite *s = suite_create("s21_decimal");
  TCase *tc_negate = tcase_create("negate");
  tcase_add_test(tc_negate, test_negate_positive);
  tcase_add_test(tc_negate, test_negate_negative);
  tcase_add_test(tc_negate, test_negate_zero);
  tcase_add_test(tc_negate, test_negate_null);
  suite_add_tcase(s, tc_negate);
  TCase *tc_truncate = tcase_create("truncate");
  tcase_add_test(tc_truncate, test_truncate_no_fraction);
  tcase_add_test(tc_truncate, test_truncate_positive);
  tcase_add_test(tc_truncate, test_truncate_negative);
  tcase_add_test(tc_truncate, test_truncate_zero);
  tcase_add_test(tc_truncate, test_truncate_null);
  suite_add_tcase(s, tc_truncate);
  TCase *tc_floor = tcase_create("floor");
  tcase_add_test(tc_floor, test_floor_positive);
  tcase_add_test(tc_floor, test_floor_negative_with_fraction);
  tcase_add_test(tc_floor, test_floor_no_fraction);
  tcase_add_test(tc_floor, test_floor_negative_no_fraction);
  tcase_add_test(tc_floor, test_floor_zero);
  tcase_add_test(tc_floor, test_floor_null);
  suite_add_tcase(s, tc_floor);
  TCase *tc_round = tcase_create("round");
  tcase_add_test(tc_round, test_round_no_fraction);
  tcase_add_test(tc_round, test_round_less_then_half);
  tcase_add_test(tc_round, test_round_greater_then_half);
  tcase_add_test(tc_round, test_round_half_even_down);
  tcase_add_test(tc_round, test_round_half_even_up);
  tcase_add_test(tc_round, test_round_negative);
  tcase_add_test(tc_round, test_round_null);
  suite_add_tcase(s, tc_round);
  return s;
}

int main() {
  Suite *s = s21_decimal_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return failed == 0 ? 0 : 1;
}