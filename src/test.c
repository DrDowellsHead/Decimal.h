#include <check.h>
#include <limits.h>

#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

static s21_decimal make_decimal(unsigned int lo, unsigned int mid,
                                unsigned int hi, int scale, int sign) {
  s21_decimal value;
  s21_zero_decimal(&value);
  value.bits[0] = (int)lo;
  value.bits[1] = (int)mid;
  value.bits[2] = (int)hi;
  s21_set_scale(&value, scale);
  s21_set_sign(&value, sign);
  return value;
}

static void ck_assert_decimal_words(s21_decimal value, unsigned int lo,
                                    unsigned int mid, unsigned int hi,
                                    int scale, int sign) {
  ck_assert_uint_eq((unsigned int)value.bits[0], lo);
  ck_assert_uint_eq((unsigned int)value.bits[1], mid);
  ck_assert_uint_eq((unsigned int)value.bits[2], hi);
  ck_assert_int_eq(s21_get_scale(value), scale);
  ck_assert_int_eq(s21_get_sign(value), sign);
}

START_TEST(test_round_null_result_returns_error) {
  s21_decimal value = make_decimal(12345u, 0u, 0u, 2, 0);
  ck_assert_int_eq(s21_round(value, NULL), 1);
}
END_TEST

START_TEST(test_round_scale_zero_returns_unchanged) {
  s21_decimal value = make_decimal(12345u, 0u, 0u, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 12345u, 0u, 0u, 0, 1);
}
END_TEST

START_TEST(test_round_basic_bankers_rounding_down) {
  s21_decimal value = make_decimal(12u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 1u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_round_basic_bankers_rounding_up) {
  s21_decimal value = make_decimal(18u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 2u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_round_bankers_half_to_even_even) {
  s21_decimal value = make_decimal(25u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 2u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_round_bankers_half_to_even_odd) {
  s21_decimal value = make_decimal(35u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 4u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_round_negative_number) {
  s21_decimal value = make_decimal(18u, 0u, 0u, 1, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 2u, 0u, 0u, 0, 1);
}
END_TEST

START_TEST(test_round_negative_half_to_even) {
  s21_decimal value = make_decimal(25u, 0u, 0u, 1, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 2u, 0u, 0u, 0, 1);
}
END_TEST

START_TEST(test_round_scale_greater_than_one) {
  s21_decimal value = make_decimal(1234u, 0u, 0u, 3, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 1u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_mul_null_result_returns_error) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(10u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_mul(a, b, NULL), 1);
}
END_TEST

START_TEST(test_mul_zero_returns_zero) {
  s21_decimal zero = make_decimal(0u, 0u, 0u, 0, 0);
  s21_decimal five = make_decimal(5u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(zero, five, &result), 0);
  ck_assert_decimal_words(result, 0u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_mul(five, zero, &result), 0);
  ck_assert_decimal_words(result, 0u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_mul_basic_positive) {
  s21_decimal a = make_decimal(12u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  ck_assert_decimal_words(result, 36u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_mul_basic_negative) {
  s21_decimal a = make_decimal(12u, 0u, 0u, 0, 1);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  ck_assert_decimal_words(result, 36u, 0u, 0u, 0, 1);
}
END_TEST

START_TEST(test_mul_negative_negative) {
  s21_decimal a = make_decimal(12u, 0u, 0u, 0, 1);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  ck_assert_decimal_words(result, 36u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_mul_with_scale) {
  s21_decimal a = make_decimal(12u, 0u, 0u, 1, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  ck_assert_decimal_words(result, 36u, 0u, 0u, 2, 0);
}
END_TEST

START_TEST(test_mul_overflow_returns_error) {
  s21_decimal max = {{0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0}};
  s21_decimal two = make_decimal(2u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(max, two, &result), 1);
}
END_TEST

START_TEST(test_mul_scale_overflow_returns_error) {
  s21_decimal a = make_decimal(1u, 0u, 0u, 15, 0);
  s21_decimal b = make_decimal(1u, 0u, 0u, 15, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(a, b, &result), 1);
}
END_TEST

START_TEST(test_round_192_to_96_fits_within_96_bits) {
  unsigned long long temp[6] = {0x12345678u, 0u, 0u, 0u, 0u, 0u};
  unsigned int mantissa[3];
  ck_assert_int_eq(s21_round_192_to_96(temp, mantissa), 0);
  ck_assert_uint_eq(mantissa[0], 0x12345678u);
  ck_assert_uint_eq(mantissa[1], 0u);
  ck_assert_uint_eq(mantissa[2], 0u);
}
END_TEST

START_TEST(test_round_192_to_96_exact_fit_96_bits) {
  unsigned long long temp[6] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
                                0u,         0u,         0u};
  unsigned int mantissa[3];
  ck_assert_int_eq(s21_round_192_to_96(temp, mantissa), 0);
  ck_assert_uint_eq(mantissa[0], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[1], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[2], 0xFFFFFFFFu);
}
END_TEST

START_TEST(test_round_192_to_96_extra_bits_less_than_half) {
  unsigned long long temp[6] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000001u,
                                0u,         0u,         0u};
  unsigned int mantissa[3];
  ck_assert_int_eq(s21_round_192_to_96(temp, mantissa), 0);
  ck_assert_uint_eq(mantissa[0], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[1], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[2], 0x00000001u);
}
END_TEST

START_TEST(test_round_192_to_96_extra_bits_more_than_half) {
  unsigned long long temp[6] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000002u,
                                0u,         0u,         0u};
  unsigned int mantissa[3];
  ck_assert_int_eq(s21_round_192_to_96(temp, mantissa), 0);
  ck_assert_uint_eq(mantissa[0], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[1], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[2], 0x00000002u);
}
END_TEST

START_TEST(test_round_192_to_96_extra_exact_half_even) {
  unsigned long long temp[6] = {0x00000000u, 0x00000000u, 0x00000000u,
                                0x00000000u, 0x00000000u, 0u};
  unsigned int mantissa[3];
  ck_assert_int_eq(s21_round_192_to_96(temp, mantissa), 0);
}
END_TEST

START_TEST(test_div_null_result_returns_error) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(2u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_div(a, b, NULL), 1);
}
END_TEST

START_TEST(test_div_by_zero_returns_error) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal zero = make_decimal(0u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, zero, &result), 1);
}
END_TEST

START_TEST(test_div_basic_positive) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(2u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert_decimal_words(result, 5u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_div_basic_negative) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 1);
  s21_decimal b = make_decimal(2u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert_decimal_words(result, 5u, 0u, 0u, 0, 1);
}
END_TEST

START_TEST(test_div_negative_negative) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 1);
  s21_decimal b = make_decimal(2u, 0u, 0u, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert_decimal_words(result, 5u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_div_with_scale) {
  s21_decimal a = make_decimal(12u, 0u, 0u, 1, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert_decimal_words(result, 4u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_div_result_has_correct_scale) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert_int_eq(s21_get_scale(result), 28);
}
END_TEST

START_TEST(test_div_one_digit_result) {
  s21_decimal a = make_decimal(99u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(33u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert_decimal_words(result, 3u, 0u, 0u, 0, 0);
}
END_TEST

Suite *test_func(void) {
  Suite *s = suite_create("s21_decimal_mul_div_round");

  TCase *tc = tcase_create("round");
  tcase_add_test(tc, test_round_null_result_returns_error);
  tcase_add_test(tc, test_round_scale_zero_returns_unchanged);
  tcase_add_test(tc, test_round_basic_bankers_rounding_down);
  tcase_add_test(tc, test_round_basic_bankers_rounding_up);
  tcase_add_test(tc, test_round_bankers_half_to_even_even);
  tcase_add_test(tc, test_round_bankers_half_to_even_odd);
  tcase_add_test(tc, test_round_negative_number);
  tcase_add_test(tc, test_round_negative_half_to_even);
  tcase_add_test(tc, test_round_scale_greater_than_one);
  TCase *tc = tcase_create("mul");
  tcase_add_test(tc, test_mul_null_result_returns_error);
  tcase_add_test(tc, test_mul_zero_returns_zero);
  tcase_add_test(tc, test_mul_basic_positive);
  tcase_add_test(tc, test_mul_basic_negative);
  tcase_add_test(tc, test_mul_negative_negative);
  tcase_add_test(tc, test_mul_with_scale);
  tcase_add_test(tc, test_mul_overflow_returns_error);
  tcase_add_test(tc, test_mul_scale_overflow_returns_error);
  TCase *tc = tcase_create("round_192_to_96");
  tcase_add_test(tc, test_round_192_to_96_fits_within_96_bits);
  tcase_add_test(tc, test_round_192_to_96_exact_fit_96_bits);
  tcase_add_test(tc, test_round_192_to_96_extra_bits_less_than_half);
  tcase_add_test(tc, test_round_192_to_96_extra_bits_more_than_half);
  tcase_add_test(tc, test_round_192_to_96_extra_exact_half_even);
  TCase *tc = tcase_create("div");
  tcase_add_test(tc, test_div_null_result_returns_error);
  tcase_add_test(tc, test_div_by_zero_returns_error);
  tcase_add_test(tc, test_div_basic_positive);
  tcase_add_test(tc, test_div_basic_negative);
  tcase_add_test(tc, test_div_negative_negative);
  tcase_add_test(tc, test_div_with_scale);
  tcase_add_test(tc, test_div_result_has_correct_scale);
  tcase_add_test(tc, test_div_one_digit_result);

  suite_add_tcase(s, tc);
  return s;
}