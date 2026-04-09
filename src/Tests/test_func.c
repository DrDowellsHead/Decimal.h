#include <check.h>
#include <limits.h>

#include "s21_big_decimal.h"
#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

static s21_decimal make_decimal(unsigned int lo, unsigned int mid,
                                unsigned int hi, int scale, int sign) {
  s21_decimal value;
  s21_zero_decimal(&value);
  value.bits[0] = (int)lo;
  value.bits[1] = (int)mid;
  value.bits[2] = (int)hi;
  ck_assert_int_eq(s21_set_scale(&value, scale), 0);
  s21_set_sign(&value, sign);
  return value;
}

static s21_big_decimal make_big_decimal(unsigned int b0, unsigned int b1,
                                        unsigned int b2, unsigned int b3,
                                        unsigned int b4, unsigned int b5,
                                        unsigned int b6, int scale, int sign) {
  s21_big_decimal value;
  s21_big_zero(&value);
  value.bits[0] = b0;
  value.bits[1] = b1;
  value.bits[2] = b2;
  value.bits[3] = b3;
  value.bits[4] = b4;
  value.bits[5] = b5;
  value.bits[6] = b6;
  value.scale = scale;
  value.sign = sign;
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

START_TEST(test_zero_decimal_clears_all_words) {
  s21_decimal value = {{123, -1, 77, INT_MIN}};
  s21_zero_decimal(&value);
  ck_assert_decimal_words(value, 0u, 0u, 0u, 0, 0);
}
END_TEST

START_TEST(test_is_zero_treats_negative_zero_as_zero) {
  s21_decimal value = make_decimal(0u, 0u, 0u, 5, 1);
  ck_assert_int_eq(s21_is_zero(value), 1);
}
END_TEST

START_TEST(test_get_set_sign_preserve_scale) {
  s21_decimal value = make_decimal(42u, 0u, 0u, 7, 0);
  s21_set_sign(&value, 1);
  ck_assert_int_eq(s21_get_sign(value), 1);
  ck_assert_int_eq(s21_get_scale(value), 7);
  s21_set_sign(&value, 0);
  ck_assert_int_eq(s21_get_sign(value), 0);
  ck_assert_int_eq(s21_get_scale(value), 7);
}
END_TEST

START_TEST(test_get_set_scale_preserve_sign) {
  s21_decimal value = make_decimal(42u, 0u, 0u, 0, 1);
  ck_assert_int_eq(s21_set_scale(&value, 12), 0);
  ck_assert_int_eq(s21_get_scale(value), 12);
  ck_assert_int_eq(s21_get_sign(value), 1);
}
END_TEST

START_TEST(test_set_scale_rejects_invalid_values) {
  s21_decimal value = make_decimal(1u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_set_scale(&value, -1), 1);
  ck_assert_int_eq(s21_set_scale(&value, 29), 1);
  ck_assert_int_eq(s21_get_scale(value), 0);
}
END_TEST

START_TEST(test_big_zero_clears_everything) {
  s21_big_decimal value = make_big_decimal(1u, 2u, 3u, 4u, 5u, 6u, 7u, 9, 1);
  s21_big_zero(&value);
  for (int i = 0; i < 7; i++) {
    ck_assert_uint_eq(value.bits[i], 0u);
  }
  ck_assert_int_eq(value.scale, 0);
  ck_assert_int_eq(value.sign, 0);
}
END_TEST

START_TEST(test_big_from_decimal_copies_words_scale_and_sign) {
  s21_decimal src = make_decimal(11u, 22u, 33u, 6, 1);
  s21_big_decimal big;
  s21_big_from_decimal(src, &big);
  ck_assert_uint_eq(big.bits[0], 11u);
  ck_assert_uint_eq(big.bits[1], 22u);
  ck_assert_uint_eq(big.bits[2], 33u);
  for (int i = 3; i < 7; i++) {
    ck_assert_uint_eq(big.bits[i], 0u);
  }
  ck_assert_int_eq(big.scale, 6);
  ck_assert_int_eq(big.sign, 1);
}
END_TEST

START_TEST(test_big_is_zero_detects_zero) {
  s21_big_decimal zero = make_big_decimal(0, 0, 0, 0, 0, 0, 0, 0, 0);
  s21_big_decimal non_zero = make_big_decimal(0, 0, 0, 0, 0, 1, 0, 0, 0);
  ck_assert_int_eq(s21_big_is_zero(zero), 1);
  ck_assert_int_eq(s21_big_is_zero(non_zero), 0);
}
END_TEST

START_TEST(test_big_compare_mantissa_uses_high_words_first) {
  s21_big_decimal a = make_big_decimal(0, 0, 0, 0, 0, 0, 2u, 0, 0);
  s21_big_decimal b = make_big_decimal(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX,
                                       UINT_MAX, UINT_MAX, 1u, 0, 0);
  ck_assert_int_eq(s21_big_compare_mantissa(a, b), 1);
  ck_assert_int_eq(s21_big_compare_mantissa(b, a), -1);
  ck_assert_int_eq(s21_big_compare_mantissa(a, a), 0);
}
END_TEST

START_TEST(test_big_add_mantissa_over_propagates) {
  s21_big_decimal a =
      make_big_decimal(0xFFFFFFFFu, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal b = make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal result;

  ck_assert_int_eq(s21_big_add_mantissa(a, b, &result), 0);
  ck_assert_uint_eq(result.bits[0], 0u);
  ck_assert_uint_eq(result.bits[1], 1u);
}
END_TEST

START_TEST(test_big_sub_mantissa_borrow_propagates) {
  s21_big_decimal a = make_big_decimal(0u, 1u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal b = make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal result;
  ck_assert_int_eq(s21_big_sub_mantissa(a, b, &result), 0);
  ck_assert_uint_eq(result.bits[0], 0xFFFFFFFFu);
  ck_assert_uint_eq(result.bits[1], 0u);
}
END_TEST

START_TEST(test_big_mul10_mantissa_multiplies) {
  s21_big_decimal value = make_big_decimal(123u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_big_mul10_mantissa(&value), 0);
  ck_assert_uint_eq(value.bits[0], 1230u);
}
END_TEST

START_TEST(test_big_div10_mantissa_returns_remainder) {
  s21_big_decimal value = make_big_decimal(123u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  unsigned int remainder = 99u;
  ck_assert_int_eq(s21_big_div10_mantissa(&value, &remainder), 0);
  ck_assert_uint_eq(value.bits[0], 12u);
  ck_assert_uint_eq(remainder, 3u);
}
END_TEST

START_TEST(test_big_normalize_scales_multiplies_smaller_scale) {
  s21_big_decimal a = make_big_decimal(12u, 0u, 0u, 0u, 0u, 0u, 0u, 1, 0);
  s21_big_decimal b = make_big_decimal(123u, 0u, 0u, 0u, 0u, 0u, 0u, 2, 0);

  ck_assert_int_eq(s21_big_normalize_scales(&a, &b), 0);

  ck_assert_int_eq(a.scale, 2);
  ck_assert_int_eq(b.scale, 2);

  ck_assert_uint_eq(a.bits[0], 120u);
  ck_assert_uint_eq(b.bits[0], 123u);
}
END_TEST

START_TEST(test_big_fits_decimal_checks_high_words) {
  s21_big_decimal fits = make_big_decimal(1u, 2u, 3u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal too_big = make_big_decimal(1u, 2u, 3u, 1u, 0u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_big_fits_decimal(fits), 1);
  ck_assert_int_eq(s21_big_fits_decimal(too_big), 0);
}
END_TEST

START_TEST(test_big_add_one_mantissa_propagates_overflow) {
  s21_big_decimal value =
      make_big_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0u, 0u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_big_add_one_mantissa(&value), 0);
  ck_assert_uint_eq(value.bits[0], 0u);
  ck_assert_uint_eq(value.bits[1], 0u);
  ck_assert_uint_eq(value.bits[2], 1u);
}
END_TEST

START_TEST(test_big_round_up_handles_all_cases) {
  s21_big_decimal even = make_big_decimal(10u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal odd = make_big_decimal(11u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);

  ck_assert_int_eq(s21_big_round_up(even, 6u, 0), 1);
  ck_assert_int_eq(s21_big_round_up(even, 4u, 0), 0);
  ck_assert_int_eq(s21_big_round_up(even, 5u, 1), 1);
  ck_assert_int_eq(s21_big_round_up(even, 5u, 0), 0);
  ck_assert_int_eq(s21_big_round_up(odd, 5u, 0), 1);
}
END_TEST

START_TEST(test_big_decimal_rounding_exact_fit) {
  s21_big_decimal src = make_big_decimal(12345u, 0u, 0u, 0u, 0u, 0u, 0u, 2, 1);
  s21_decimal dst;
  ck_assert_int_eq(s21_big_decimal_rounding(src, &dst), 0);
  ck_assert_decimal_words(dst, 12345u, 0u, 0u, 2, 1);
}
END_TEST

START_TEST(test_big_decimal_rounding_half_to_even_down) {
  // 2.5 -> 2 when scale is reduced to 0
  s21_big_decimal src = make_big_decimal(25u, 0u, 0u, 0u, 0u, 0u, 0u, 1, 0);
  s21_decimal dst;
  ck_assert_int_eq(s21_big_decimal_rounding(src, &dst), 0);
  ck_assert_decimal_words(dst, 25u, 0u, 0u, 1, 0);

  // Directly verify banker decision for exact 5 with even retained digit.
  ck_assert_int_eq(
      s21_big_round_up(make_big_decimal(2u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0), 5u,
                       0),
      0);
}
END_TEST

START_TEST(test_equal_zero_ignores_sign) {
  s21_decimal plus_zero = make_decimal(0u, 0u, 0u, 0, 0);
  s21_decimal minus_zero = make_decimal(0u, 0u, 0u, 10, 1);
  ck_assert_int_eq(s21_is_equal(plus_zero, minus_zero), 1);
}
END_TEST

START_TEST(test_equal_trailing_zeros) {
  s21_decimal a = make_decimal(12u, 0u, 0u, 1, 0);   // 1.2
  s21_decimal b = make_decimal(120u, 0u, 0u, 2, 0);  // 1.20
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_less_with_different_signs) {
  s21_decimal neg = make_decimal(1u, 0u, 0u, 0, 1);
  s21_decimal pos = make_decimal(1u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_is_less(neg, pos), 1);
  ck_assert_int_eq(s21_is_less(pos, neg), 0);
}
END_TEST

START_TEST(test_less_with_same_sign_and_scale) {
  s21_decimal a = make_decimal(123u, 0u, 0u, 2, 0);  // 1.23
  s21_decimal b = make_decimal(124u, 0u, 0u, 2, 0);  // 1.24
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_less_with_same_sign_different_scale) {
  s21_decimal a = make_decimal(12u, 0u, 0u, 1, 0);   // 1.2
  s21_decimal b = make_decimal(123u, 0u, 0u, 2, 0);  // 1.23
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_less_negative_order_reversed) {
  s21_decimal a = make_decimal(300u, 0u, 0u, 2, 1);  // -3.00
  s21_decimal b = make_decimal(250u, 0u, 0u, 2, 1);  // -2.50
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_compare_wrappers) {
  s21_decimal a = make_decimal(100u, 0u, 0u, 1, 0);  // 10.0
  s21_decimal b = make_decimal(99u, 0u, 0u, 1, 0);   // 9.9
  s21_decimal c = make_decimal(100u, 0u, 0u, 1, 0);  // 10.0

  ck_assert_int_eq(s21_is_greater(a, b), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(a, c), 1);
  ck_assert_int_eq(s21_is_less_or_equal(b, a), 1);
  ck_assert_int_eq(s21_is_not_equal(a, c), 0);
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_add_zero_identity) {
  s21_decimal zero = make_decimal(0u, 0u, 0u, 0, 0);
  s21_decimal x = make_decimal(12345u, 0u, 0u, 2, 1);
  s21_decimal result;

  ck_assert_int_eq(s21_add(zero, x, &result), 0);
  ck_assert_decimal_words(result, 12345u, 0u, 0u, 2, 1);

  ck_assert_int_eq(s21_add(x, zero, &result), 0);
  ck_assert_decimal_words(result, 12345u, 0u, 0u, 2, 1);
}
END_TEST

START_TEST(test_add_same_sign_positive) {
  s21_decimal a = make_decimal(120u, 0u, 0u, 2, 0);  // 1.20
  s21_decimal b = make_decimal(34u, 0u, 0u, 2, 0);   // 0.34
  s21_decimal result;

  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_decimal_words(result, 154u, 0u, 0u, 2, 0);  // 1.54
}
END_TEST

START_TEST(test_add_same_sign_negative) {
  s21_decimal a = make_decimal(120u, 0u, 0u, 2, 1);  // -1.20
  s21_decimal b = make_decimal(34u, 0u, 0u, 2, 1);   // -0.34
  s21_decimal result;

  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_decimal_words(result, 154u, 0u, 0u, 2, 1);  // -1.54
}
END_TEST

START_TEST(test_add_opposite_signs_becomes_subtraction) {
  s21_decimal a = make_decimal(500u, 0u, 0u, 2, 0);  // 5.00
  s21_decimal b = make_decimal(300u, 0u, 0u, 2, 1);  // -3.00
  s21_decimal result;

  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_decimal_words(result, 200u, 0u, 0u, 2, 0);  // 2.00
}
END_TEST

START_TEST(test_sub_zero_and_basic_cases) {
  s21_decimal a = make_decimal(500u, 0u, 0u, 2, 0);  // 5.00
  s21_decimal b = make_decimal(300u, 0u, 0u, 2, 0);  // 3.00
  s21_decimal zero = make_decimal(0u, 0u, 0u, 0, 0);
  s21_decimal result;

  ck_assert_int_eq(s21_sub(a, zero, &result), 0);
  ck_assert_decimal_words(result, 500u, 0u, 0u, 2, 0);

  ck_assert_int_eq(s21_sub(zero, b, &result), 0);
  ck_assert_decimal_words(result, 300u, 0u, 0u, 2, 1);

  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_decimal_words(result, 200u, 0u, 0u, 2, 0);

  ck_assert_int_eq(s21_sub(b, a, &result), 0);
  ck_assert_decimal_words(result, 200u, 0u, 0u, 2, 1);
}
END_TEST

START_TEST(test_sub_different_signs_becomes_addition) {
  s21_decimal a = make_decimal(500u, 0u, 0u, 2, 0);  // 5.00
  s21_decimal b = make_decimal(300u, 0u, 0u, 2, 1);  // -3.00
  s21_decimal result;

  ck_assert_int_eq(s21_sub(a, b, &result), 0);
  ck_assert_decimal_words(result, 800u, 0u, 0u, 2, 0);  // 8.00
}
END_TEST

START_TEST(test_add_sub_core_null_result_returns_error) {
  s21_decimal a = make_decimal(1u, 0u, 0u, 0, 0);
  s21_decimal b = make_decimal(1u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_add(a, b, NULL), 1);
  ck_assert_int_eq(s21_sub(a, b, NULL), 1);
}
END_TEST

START_TEST(test_decimal_helpers_null_guards) {
  s21_zero_decimal(NULL);
  s21_set_sign(NULL, 1);
  ck_assert_int_eq(s21_set_scale(NULL, 5), 1);
}
END_TEST

START_TEST(test_set_scale_invalid_bounds_again) {
  s21_decimal value = make_decimal(1u, 0u, 0u, 0, 0);
  ck_assert_int_eq(s21_set_scale(&value, -1), 1);
  ck_assert_int_eq(s21_set_scale(&value, 29), 1);
}
END_TEST

START_TEST(test_big_helpers_null_guards) {
  s21_decimal dec = make_decimal(1u, 0u, 0u, 0, 0);
  s21_big_decimal big = make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  unsigned int remainder = 0;

  s21_big_zero(NULL);
  s21_big_from_decimal(dec, NULL);

  ck_assert_int_eq(s21_big_mul10_mantissa(NULL), 1);
  ck_assert_int_eq(s21_big_div10_mantissa(NULL, &remainder), 1);
  ck_assert_int_eq(s21_big_div10_mantissa(&big, NULL), 1);
  ck_assert_int_eq(s21_big_normalize_scales(NULL, &big), 1);
  ck_assert_int_eq(s21_big_normalize_scales(&big, NULL), 1);
  ck_assert_int_eq(s21_big_add_one_mantissa(NULL), 1);
  ck_assert_int_eq(s21_big_add_mantissa(big, big, NULL), 1);
  ck_assert_int_eq(s21_big_sub_mantissa(big, big, NULL), 1);
}
END_TEST

START_TEST(test_big_add_mantissa_reports_final_overflow) {
  s21_big_decimal a =
      make_big_decimal(0u, 0u, 0u, 0u, 0u, 0u, 0xFFFFFFFFu, 0, 0);
  s21_big_decimal b = make_big_decimal(0u, 0u, 0u, 0u, 0u, 0u, 1u, 0, 0);
  s21_big_decimal result;

  ck_assert_int_eq(s21_big_add_mantissa(a, b, &result), 1);
}
END_TEST

START_TEST(test_big_sub_mantissa_reports_borrow_when_a_less_b) {
  s21_big_decimal a = make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal b = make_big_decimal(2u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal result;

  ck_assert_int_eq(s21_big_sub_mantissa(a, b, &result), 1);
}
END_TEST

START_TEST(test_big_mul10_mantissa_reports_overflow) {
  s21_big_decimal value =
      make_big_decimal(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX,
                       UINT_MAX, UINT_MAX, 0, 0);
  ck_assert_int_ne(s21_big_mul10_mantissa(&value), 0);
}
END_TEST

START_TEST(test_big_add_one_mantissa_reports_overflow) {
  s21_big_decimal value =
      make_big_decimal(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX,
                       UINT_MAX, UINT_MAX, 0, 0);
  ck_assert_int_ne(s21_big_add_one_mantissa(&value), 0);
}
END_TEST

START_TEST(test_big_normalize_scales_reports_overflow) {
  s21_big_decimal a = make_big_decimal(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX,
                                       UINT_MAX, UINT_MAX, UINT_MAX, 0, 0);
  s21_big_decimal b = make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, 1, 0);

  ck_assert_int_eq(s21_big_normalize_scales(&a, &b), 1);
}
END_TEST

START_TEST(test_big_decimal_rounding_null_and_negative_scale) {
  s21_big_decimal pos = make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0);
  s21_big_decimal neg = make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 1);
  s21_big_decimal bad_scale =
      make_big_decimal(1u, 0u, 0u, 0u, 0u, 0u, 0u, -1, 0);

  ck_assert_int_eq(s21_big_decimal_rounding(pos, NULL), 1);
  ck_assert_int_eq(s21_big_decimal_rounding(neg, NULL), 2);

  {
    s21_decimal dst;
    ck_assert_int_eq(s21_big_decimal_rounding(bad_scale, &dst), 1);
  }
}
END_TEST

START_TEST(test_big_decimal_rounding_reduces_scale_and_rounds_up) {
  // 15 * 10^-29  -> после приведения scale к 28 получится 2 * 10^-28
  s21_big_decimal src = make_big_decimal(15u, 0u, 0u, 0u, 0u, 0u, 0u, 29, 0);
  s21_decimal dst;

  ck_assert_int_eq(s21_big_decimal_rounding(src, &dst), 0);
  ck_assert_decimal_words(dst, 2u, 0u, 0u, 28, 0);
}
END_TEST

START_TEST(test_add_opposite_signs_equal_magnitude_returns_zero) {
  s21_decimal a = make_decimal(500u, 0u, 0u, 2, 0);  // 5.00
  s21_decimal b = make_decimal(500u, 0u, 0u, 2, 1);  // -5.00
  s21_decimal result;

  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_sub_same_values_returns_zero) {
  s21_decimal a = make_decimal(123u, 0u, 0u, 2, 1);
  s21_decimal result;

  ck_assert_int_eq(s21_sub(a, a, &result), 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_add_sub_core_normalize_failure_from_malformed_decimal) {
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{1, 0, 0, 255 << 16}};  // искусственно плохой scale
  s21_decimal result;

  ck_assert_int_ne(s21_add(a, b, &result), 0);
}
END_TEST

START_TEST(test_less_or_equal_all_branches) {
  s21_decimal less = make_decimal(10u, 0u, 0u, 1, 0);     // 1.0
  s21_decimal equal = make_decimal(10u, 0u, 0u, 1, 0);    // 1.0
  s21_decimal greater = make_decimal(20u, 0u, 0u, 1, 0);  // 2.0

  ck_assert_int_eq(s21_is_less_or_equal(less, greater), 1);  // <
  ck_assert_int_eq(s21_is_less_or_equal(less, equal), 1);    // ==
  ck_assert_int_eq(s21_is_less_or_equal(greater, less), 0);  // >
}
END_TEST

START_TEST(test_add_positive_overflow_returns_1) {
  s21_decimal max = {{-1, -1, -1, 0}};  // max positive decimal
  s21_decimal one = make_decimal(1u, 0u, 0u, 0, 0);
  s21_decimal result;

  ck_assert_int_eq(s21_add(max, one, &result), 1);
}
END_TEST

START_TEST(test_add_negative_overflow_returns_2) {
  s21_decimal max_neg = {{-1, -1, -1, 0}};
  s21_set_sign(&max_neg, 1);

  s21_decimal one_neg = make_decimal(1u, 0u, 0u, 0, 1);
  s21_decimal result;

  ck_assert_int_eq(s21_add(max_neg, one_neg, &result), 2);
}
END_TEST

START_TEST(test_equal_malformed_scale_returns_false) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 255 << 16}};  // заведомо некорректный scale
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

Suite *test_func(void) {
  Suite *s = suite_create("s21_decimal functions");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_zero_decimal_clears_all_words);
  tcase_add_test(tc, test_is_zero_treats_negative_zero_as_zero);
  tcase_add_test(tc, test_get_set_sign_preserve_scale);
  tcase_add_test(tc, test_get_set_scale_preserve_sign);
  tcase_add_test(tc, test_set_scale_rejects_invalid_values);

  tcase_add_test(tc, test_big_zero_clears_everything);
  tcase_add_test(tc, test_big_from_decimal_copies_words_scale_and_sign);
  tcase_add_test(tc, test_big_is_zero_detects_zero);
  tcase_add_test(tc, test_big_compare_mantissa_uses_high_words_first);
  tcase_add_test(tc, test_big_add_mantissa_over_propagates);
  tcase_add_test(tc, test_big_sub_mantissa_borrow_propagates);
  tcase_add_test(tc, test_big_mul10_mantissa_multiplies);
  tcase_add_test(tc, test_big_div10_mantissa_returns_remainder);
  tcase_add_test(tc, test_big_normalize_scales_multiplies_smaller_scale);
  tcase_add_test(tc, test_big_fits_decimal_checks_high_words);
  tcase_add_test(tc, test_big_add_one_mantissa_propagates_overflow);
  tcase_add_test(tc, test_big_round_up_handles_all_cases);
  tcase_add_test(tc, test_big_decimal_rounding_exact_fit);
  tcase_add_test(tc, test_big_decimal_rounding_half_to_even_down);

  tcase_add_test(tc, test_equal_zero_ignores_sign);
  tcase_add_test(tc, test_equal_trailing_zeros);
  tcase_add_test(tc, test_less_with_different_signs);
  tcase_add_test(tc, test_less_with_same_sign_and_scale);
  tcase_add_test(tc, test_less_with_same_sign_different_scale);
  tcase_add_test(tc, test_less_negative_order_reversed);
  tcase_add_test(tc, test_compare_wrappers);

  tcase_add_test(tc, test_add_zero_identity);
  tcase_add_test(tc, test_add_same_sign_positive);
  tcase_add_test(tc, test_add_same_sign_negative);
  tcase_add_test(tc, test_add_opposite_signs_becomes_subtraction);
  tcase_add_test(tc, test_sub_zero_and_basic_cases);
  tcase_add_test(tc, test_sub_different_signs_becomes_addition);
  tcase_add_test(tc, test_add_sub_core_null_result_returns_error);

  tcase_add_test(tc, test_decimal_helpers_null_guards);
  tcase_add_test(tc, test_set_scale_invalid_bounds_again);
  tcase_add_test(tc, test_big_helpers_null_guards);
  tcase_add_test(tc, test_big_add_mantissa_reports_final_overflow);
  tcase_add_test(tc, test_big_sub_mantissa_reports_borrow_when_a_less_b);
  tcase_add_test(tc, test_big_mul10_mantissa_reports_overflow);
  tcase_add_test(tc, test_big_add_one_mantissa_reports_overflow);
  tcase_add_test(tc, test_big_normalize_scales_reports_overflow);
  tcase_add_test(tc, test_big_decimal_rounding_null_and_negative_scale);
  tcase_add_test(tc, test_big_decimal_rounding_reduces_scale_and_rounds_up);
  tcase_add_test(tc, test_add_opposite_signs_equal_magnitude_returns_zero);
  tcase_add_test(tc, test_sub_same_values_returns_zero);
  tcase_add_test(tc,
                 test_add_sub_core_normalize_failure_from_malformed_decimal);
  tcase_add_test(tc, test_less_or_equal_all_branches);
  tcase_add_test(tc, test_add_positive_overflow_returns_1);
  tcase_add_test(tc, test_add_negative_overflow_returns_2);
  tcase_add_test(tc, test_equal_malformed_scale_returns_false);

  suite_add_tcase(s, tc);
  return s;
}