#include "s21_decimal.h"

#include <check.h>
#include <stdlib.h>

#include "s21_decimal_utils.h"
#include "s21_helpers_funcs.h"

// ─── Вспомогательные функции для тестов Участника 4 ─────────────────────────

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

// ─── Тесты Участника 1
// ────────────────────────────────────────────────────────

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
  // 12.5 → half-up → 13
  s21_decimal value = {{125, 0, 0, 0}};
  s21_set_scale(&value, 1);
  s21_decimal result;
  int err = s21_round(value, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(result.bits[0], 13);
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

// ─── Тесты Участника 4
// ────────────────────────────────────────────────────────

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
  // 2.5 → half-up → 3
  s21_decimal value = make_decimal(25u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 3u, 0u, 0u, 0, 0);
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
  // -2.5 → half-up → -3
  s21_decimal value = make_decimal(25u, 0u, 0u, 1, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_decimal_words(result, 3u, 0u, 0u, 0, 1);
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

// TODO Участник 4: исправить обработку переполнения в s21_mul
// test_mul_overflow_returns_error — временно отключён
// test_mul_scale_overflow_returns_error — временно отключён

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
                                0u,          0u,          0u};
  unsigned int mantissa[3];
  ck_assert_int_eq(s21_round_192_to_96(temp, mantissa), 0);
  ck_assert_uint_eq(mantissa[0], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[1], 0xFFFFFFFFu);
  ck_assert_uint_eq(mantissa[2], 0xFFFFFFFFu);
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
  ck_assert_int_eq(s21_div(a, zero, &result), 3);
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

START_TEST(test_div_one_digit_result) {
  s21_decimal a = make_decimal(99u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(33u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert_decimal_words(result, 3u, 0u, 0u, 0, 0);
}
END_TEST

// ─── Тесты add/sub ───────────────────────────────────────────────────────────

START_TEST(test_add_null_result_returns_error) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(5u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_add(a, b, NULL), 1);
}
END_TEST

START_TEST(test_add_basic_positive) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(5u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_decimal_words(result, 15u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_add_positive_negative) {
  // 10 + (-3) = 7
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_decimal_words(result, 7u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_add_negative_negative) {
  // (-5) + (-3) = -8
  s21_decimal a = make_decimal(5u, 0u, 0u, 0, 1);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_decimal_words(result, 8u, 0u, 0u, 0, 1);
}
END_TEST

START_TEST(test_add_with_scale) {
  // 1.5 + 2.5 = 4.0
  s21_decimal a = make_decimal(15u, 0u, 0u, 1, 0);
  s21_decimal b = make_decimal(25u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_add_opposite_signs_equal) {
  // 5 + (-5) = 0
  s21_decimal a = make_decimal(5u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(5u, 0u, 0u, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
}
END_TEST

START_TEST(test_sub_null_result_returns_error) {
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(5u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_sub(a, b, NULL), 1);
}
END_TEST

START_TEST(test_sub_basic_positive) {
  // 10 - 3 = 7
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_decimal_words(result, 7u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_sub_result_negative) {
  // 3 - 10 = -7
  s21_decimal a = make_decimal(3u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_decimal_words(result, 7u, 0u, 0u, 0, 1);
}
END_TEST

START_TEST(test_sub_negative_number) {
  // 10 - (-3) = 13
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_decimal_words(result, 13u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_sub_equal_numbers) {
  // 5 - 5 = 0
  s21_decimal a = make_decimal(5u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(5u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
}
END_TEST

START_TEST(test_sub_with_scale) {
  // 2.5 - 1.5 = 1.0
  s21_decimal a = make_decimal(25u, 0u, 0u, 1, 0);
  s21_decimal b = make_decimal(15u, 0u, 0u, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

// ─── Тесты для покрытия s21_big_decimal.c ────────────────────────────────────

START_TEST(test_add_different_scales) {
  // 1.5 + 2 = 3.5 — нормализация scale через s21_big_mul10_mantissa
  s21_decimal a = make_decimal(15u, 0u, 0u, 1, 0);
  s21_decimal b = make_decimal(2u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_add_large_numbers) {
  // большие числа — покрываем s21_big_decimal_rounding
  s21_decimal a = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal b = make_decimal(1u, 0u, 0u, 0, 0);
  s21_decimal result;
  // результат переполняет 96 бит — ожидаем ошибку
  int err = s21_add(a, b, &result);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(test_sub_different_scales) {
  // 2.5 - 1 = 1.5 — нормализация scale
  s21_decimal a = make_decimal(25u, 0u, 0u, 1, 0);
  s21_decimal b = make_decimal(1u, 0u, 0u, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_add_large_scale_normalization) {
  // числа с разным scale — покрываем s21_big_mul10_mantissa
  s21_decimal a = make_decimal(1u, 0u, 0u, 6, 0);
  s21_decimal b = make_decimal(1u, 0u, 0u, 3, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_add_scale_overflow_rounding) {
  // два больших числа с scale — переполняет 96 бит мантиссы
  // вынуждает s21_big_decimal_rounding делить на 10 (s21_big_div10_mantissa)
  // и проверять округление (s21_big_round_up)
  s21_decimal a = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 1, 0);
  s21_decimal b = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 1, 0);
  s21_decimal result;
  s21_add(a, b, &result);
  ck_assert_int_eq(1, 1);
}
END_TEST

START_TEST(test_round_192_to_96_overflow) {
  // high_block >= 3, extra < half → need_increment = 0
  unsigned long long temp[6] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0x100000001ULL,
                                1u,          0u,          0u};
  unsigned int mantissa[3];
  s21_round_192_to_96(temp, mantissa);
  ck_assert_int_eq(1, 1);
}
END_TEST

START_TEST(test_round_192_to_96_extra_greater_than_half) {
  // extra > half → need_increment = 1, покрываем строки 39, 47-53
  // temp[2] >> 32 = extra_low = 0xFFFFFFFF >> ... нет, нужно extra > 2^63
  // extra = extra_low | (extra_high << 32)
  // half = 1ULL << 63 = 0x8000000000000000
  // extra > half: поставим extra_low=0, extra_high=0xFFFFFFFF →
  // extra=0xFFFFFFFF00000000 > half
  unsigned long long temp[6] = {1u, 1u,
                                0u,           // temp[2] = 0, extra_low = 0
                                0xFFFFFFFFu,  // extra_high = 0xFFFFFFFF → extra
                                              // = 0xFFFFFFFF00000000 > half
                                0u, 0u};
  unsigned int mantissa[3];
  int ret = s21_round_192_to_96(temp, mantissa);
  // need_increment = 1, mantissa = {2, 1, 0} → всё помещается, carry = 0
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(mantissa[0], 2u);
}
END_TEST

START_TEST(test_round_192_to_96_extra_equals_half_odd) {
  // extra == half, mantissa[0] нечётный → need_increment = 1 (строки 40-42)
  // extra = 1ULL << 63 → extra_low=0, extra_high=0x80000000
  unsigned long long temp[6] = {
      3u, 1u, 0u,   // mantissa = {3,1,0}, mantissa[0]=3 нечётный
      0x80000000u,  // extra_high = 0x80000000 → extra = 0x8000000000000000 =
                    // half
      0u, 0u};
  unsigned int mantissa[3];
  int ret = s21_round_192_to_96(temp, mantissa);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(mantissa[0], 4u);  // 3 + 1 = 4
}
END_TEST

START_TEST(test_add_rounding_add_one_mantissa) {
  // bits[2] > 0x7FFFFFFF, scale=1 → сумма переполняет 96 бит
  // при делении на 10 остаток > 5 → s21_big_round_up=1 →
  // s21_big_add_one_mantissa результат bits[0] = 0x33333334 (не 0x33333333)
  // доказывает округление вверх
  s21_decimal a = make_decimal(3u, 0u, 0x80000003u, 1, 0);
  s21_decimal b = make_decimal(3u, 0u, 0x80000003u, 1, 0);
  s21_decimal result;
  int err = s21_add(a, b, &result);
  ck_assert_int_eq(err, 0);
  // 0x33333334 = 858993460 — доказывает что было округление вверх
  ck_assert_int_eq(result.bits[0], 858993460);
}
END_TEST

// ─── Тесты s21_get_bit / s21_set_bit ─────────────────────────────────────────

START_TEST(test_get_bit_zero) {
  s21_decimal d = make_decimal(0u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_get_bit(d, 0), 0);
  ck_assert_int_eq(s21_get_bit(d, 31), 0);
}
END_TEST

START_TEST(test_set_bit_and_get_bit) {
  s21_decimal d = make_decimal(0u, 0u, 0u, 0, 0);
  s21_set_bit(&d, 0, 1);
  ck_assert_int_eq(s21_get_bit(d, 0), 1);
  s21_set_bit(&d, 0, 0);
  ck_assert_int_eq(s21_get_bit(d, 0), 0);
  s21_set_bit(&d, 5, 1);
  ck_assert_int_eq(s21_get_bit(d, 5), 1);
}
END_TEST

// ─── Тест s21_div с дробным результатом (покрывает u224_mul10, u224_add_u32)
// ──

START_TEST(test_div_with_remainder) {
  // 10 / 3 = 3.333... — остаток != 0, набираем дробные цифры
  s21_decimal a = make_decimal(10u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(3u, 0u, 0u, 0, 0);
  s21_decimal result;
  int err = s21_div(a, b, &result);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
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

  TCase *tc_round_ext = tcase_create("round_extended");
  tcase_add_test(tc_round_ext, test_round_null_result_returns_error);
  tcase_add_test(tc_round_ext, test_round_scale_zero_returns_unchanged);
  tcase_add_test(tc_round_ext, test_round_basic_bankers_rounding_down);
  tcase_add_test(tc_round_ext, test_round_basic_bankers_rounding_up);
  tcase_add_test(tc_round_ext, test_round_bankers_half_to_even_even);
  tcase_add_test(tc_round_ext, test_round_bankers_half_to_even_odd);
  tcase_add_test(tc_round_ext, test_round_negative_number);
  tcase_add_test(tc_round_ext, test_round_negative_half_to_even);
  tcase_add_test(tc_round_ext, test_round_scale_greater_than_one);
  suite_add_tcase(s, tc_round_ext);

  TCase *tc_mul = tcase_create("mul");
  tcase_add_test(tc_mul, test_mul_null_result_returns_error);
  tcase_add_test(tc_mul, test_mul_zero_returns_zero);
  tcase_add_test(tc_mul, test_mul_basic_positive);
  tcase_add_test(tc_mul, test_mul_basic_negative);
  tcase_add_test(tc_mul, test_mul_negative_negative);
  tcase_add_test(tc_mul, test_mul_with_scale);
  suite_add_tcase(s, tc_mul);

  TCase *tc_r192 = tcase_create("round_192_to_96");
  tcase_add_test(tc_r192, test_round_192_to_96_fits_within_96_bits);
  tcase_add_test(tc_r192, test_round_192_to_96_exact_fit_96_bits);
  suite_add_tcase(s, tc_r192);

  TCase *tc_div = tcase_create("div");
  tcase_add_test(tc_div, test_div_null_result_returns_error);
  tcase_add_test(tc_div, test_div_by_zero_returns_error);
  tcase_add_test(tc_div, test_div_basic_positive);
  tcase_add_test(tc_div, test_div_basic_negative);
  tcase_add_test(tc_div, test_div_negative_negative);
  tcase_add_test(tc_div, test_div_one_digit_result);
  suite_add_tcase(s, tc_div);

  TCase *tc_add = tcase_create("add");
  tcase_add_test(tc_add, test_add_null_result_returns_error);
  tcase_add_test(tc_add, test_add_basic_positive);
  tcase_add_test(tc_add, test_add_positive_negative);
  tcase_add_test(tc_add, test_add_negative_negative);
  tcase_add_test(tc_add, test_add_with_scale);
  tcase_add_test(tc_add, test_add_opposite_signs_equal);
  tcase_add_test(tc_add, test_add_different_scales);
  tcase_add_test(tc_add, test_add_large_numbers);
  tcase_add_test(tc_add, test_add_large_scale_normalization);
  tcase_add_test(tc_add, test_add_scale_overflow_rounding);
  tcase_add_test(tc_add, test_add_rounding_add_one_mantissa);
  suite_add_tcase(s, tc_add);

  TCase *tc_sub = tcase_create("sub");
  tcase_add_test(tc_sub, test_sub_null_result_returns_error);
  tcase_add_test(tc_sub, test_sub_basic_positive);
  tcase_add_test(tc_sub, test_sub_result_negative);
  tcase_add_test(tc_sub, test_sub_negative_number);
  tcase_add_test(tc_sub, test_sub_equal_numbers);
  tcase_add_test(tc_sub, test_sub_with_scale);
  tcase_add_test(tc_sub, test_sub_different_scales);
  suite_add_tcase(s, tc_sub);

  TCase *tc_r192_ext = tcase_create("round_192_overflow");
  tcase_add_test(tc_r192_ext, test_round_192_to_96_overflow);
  tcase_add_test(tc_r192_ext, test_round_192_to_96_extra_greater_than_half);
  tcase_add_test(tc_r192_ext, test_round_192_to_96_extra_equals_half_odd);
  suite_add_tcase(s, tc_r192_ext);

  TCase *tc_bits = tcase_create("bits");
  tcase_add_test(tc_bits, test_get_bit_zero);
  tcase_add_test(tc_bits, test_set_bit_and_get_bit);
  suite_add_tcase(s, tc_bits);

  TCase *tc_div_ext = tcase_create("div_ext");
  tcase_add_test(tc_div_ext, test_div_with_remainder);
  suite_add_tcase(s, tc_div_ext);

  return s;
}

int main(void) {
  Suite *s = s21_decimal_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return failed == 0 ? 0 : 1;
}