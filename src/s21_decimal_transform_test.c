#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_auxfunct.h"
#include "s21_decimal.h"

// int s21_from_int_to_decimal(int src, s21_decimal *dst)
START_TEST(test_s21_from_int_to_decimal_zero) {
  int x = 0, res;
  s21_decimal x_dec_res, x_dec_purp = {0};
  res = s21_from_int_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_max_int) {
  int x = 2147483647, res;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0;
  x_dec_purp.bits[2] = 0;
  x_dec_purp.bits[1] = 0;
  x_dec_purp.bits[0] = 0x7FFFFFFF;
  res = s21_from_int_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_min_int) {
  int x = -2147483648, res;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x80000000;
  x_dec_purp.bits[2] = 0;
  x_dec_purp.bits[1] = 0;
  x_dec_purp.bits[0] = 0x80000000;
  res = s21_from_int_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_pos_int) {
  int x = 483648, res;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0;
  x_dec_purp.bits[2] = 0;
  x_dec_purp.bits[1] = 0;
  x_dec_purp.bits[0] = 0x00076140;
  res = s21_from_int_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_neg_int) {
  int x = -8, res;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x80000000;
  x_dec_purp.bits[2] = 0;
  x_dec_purp.bits[1] = 0;
  x_dec_purp.bits[0] = 0x00000008;
  res = s21_from_int_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_bad_sorce) {
  int x = 0, res;
  s21_decimal *ptr_x_dec_res = NULL;
  res = s21_from_int_to_decimal(x, ptr_x_dec_res);
  ck_assert_int_eq(res, 1);
}
END_TEST

// int s21_from_float_to_decimal(float src, s21_decimal
// *dst)_______________________________________________
START_TEST(test_s21_from_float_to_decimal_zero) {
  int res;
  float x = 0.0f;
  s21_decimal x_dec_res, x_dec_purp = {0};
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_neg_zero) {
  int res;
  float x = -0.0f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x80000000;
  x_dec_purp.bits[2] = 0;
  x_dec_purp.bits[1] = 0;
  x_dec_purp.bits[0] = 0;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_max_for_dec) {
  int res;
  float x = 7.92281622514264337593543950335e+28f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0;
  x_dec_purp.bits[2] = 0xFFFFFF77;
  x_dec_purp.bits[1] = 0xB392B21A;
  x_dec_purp.bits[0] = 0x20000000;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_max_neg_for_dec) {
  int res;
  float x = -7.92281622514264337593543950335e+28f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x80000000;
  x_dec_purp.bits[2] = 0xFFFFFF77;
  x_dec_purp.bits[1] = 0xB392B21A;
  x_dec_purp.bits[0] = 0x20000000;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_min_for_dec) {
  int res;
  float x = 1.0e-28f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x001C0000;
  x_dec_purp.bits[2] = 0x00000000;
  x_dec_purp.bits[1] = 0x00000000;
  x_dec_purp.bits[0] = 0x00000001;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_pos_small_for_dec) {
  int res;
  float x = 231.0e-18f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x00120000;
  x_dec_purp.bits[2] = 0x00000000;
  x_dec_purp.bits[1] = 0x00000000;
  x_dec_purp.bits[0] = 0x000000E7;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_neg_small_for_dec) {
  int res;
  float x = -231.0e-18f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x80120000;
  x_dec_purp.bits[2] = 0x00000000;
  x_dec_purp.bits[1] = 0x00000000;
  x_dec_purp.bits[0] = 0x000000E7;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_pos_big_dec) {
  int res;
  float x = 231.0e+18f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x00000000;
  x_dec_purp.bits[2] = 0x0000000C;
  x_dec_purp.bits[1] = 0x85C4DC1C;
  x_dec_purp.bits[0] = 0x0B3C0000;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_neg_big_dec) {
  int res;
  float x = -231.0e+18f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x80000000;
  x_dec_purp.bits[2] = 0x0000000C;
  x_dec_purp.bits[1] = 0x85C4DC1C;
  x_dec_purp.bits[0] = 0x0B3C0000;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_bigger_than_max_for_dec) {
  int res;
  float x = 7.922817e+28;
  ;
  s21_decimal x_dec_res = {0}, x_dec_purp = {0};
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_neg_bigger_than_max_for_dec) {
  int res;
  float x = -7.922817e+28;
  ;
  s21_decimal x_dec_res = {0}, x_dec_purp = {0};
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_smaller_than_min_for_dec) {
  int res;
  float x = 7.9e-29;
  s21_decimal x_dec_res, x_dec_purp = {0};
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_neg_smaller_than_min_for_dec) {
  int res;
  float x = -7.9e-29;
  s21_decimal x_dec_res, x_dec_purp = {0};
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_bad_sorce) {
  int res;
  float x = 8.9;
  s21_decimal *ptr_x_dec_res = NULL;
  res = s21_from_float_to_decimal(x, ptr_x_dec_res);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_for_bank) {
  int res;
  float x = 2.4659255e-11;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x00110000;
  x_dec_purp.bits[2] = 0x00000000;
  x_dec_purp.bits[1] = 0x00000000;
  x_dec_purp.bits[0] = 0x0025A086;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_for_bank2) {
  int res;
  float x = 2.46592659e-11f;
  s21_decimal x_dec_res, x_dec_purp;
  x_dec_purp.bits[3] = 0x00110000;
  x_dec_purp.bits[2] = 0x00000000;
  x_dec_purp.bits[1] = 0x00000000;
  x_dec_purp.bits[0] = 0x0025A086;
  res = s21_from_float_to_decimal(x, &x_dec_res);
  ck_assert_mem_eq(&x_dec_res, &x_dec_purp, sizeof(s21_decimal));
  ck_assert_int_eq(res, 0);
}
END_TEST

// int s21_from_decimal_to_int(s21_decimal src, int *dst)______________________________
START_TEST(test_s21_from_decimal_to_int_zero) {
  int res, x_res, x_purp;
  x_purp = 0;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x00000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x00000000;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_neg_zero) {
  int res, x_res, x_purp;
  x_purp = 0;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x00000000;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_min) {
  int res, x_res, x_purp;
  x_purp = -2147483648;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x80000000;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_max) {
  int res, x_res, x_purp;
  x_purp = 2147483647;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x00000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x7FFFFFFF;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_pos) {
  int res, x_res, x_purp;
  x_purp = 2145448191;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x00000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x7FE0F0FF;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_neg) {
  int res, x_res, x_purp;
  x_purp = -2145448191;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x7FE0F0FF;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_not_integer) {
  int res, x_res, x_purp;
  x_purp = -21454;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80050000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x7FE0F0FF;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_bigger_than_int) {
  int res, x_res = 7, x_purp;
  x_purp = x_res;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x00000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x8FE0F0FF;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_bad_dec) {
  int res, x_res = 7, x_purp;
  x_purp = x_res;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x00000001;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x0000FFFF;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_bad_scale) {
  int res, x_res = 7, x_purp;
  x_purp = x_res;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x001D0000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x0000FFFF;
  res = s21_from_decimal_to_int(x_dec, &x_res);
  ck_assert_int_eq(x_res, x_purp);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_bad_ptr_dec) {
  int res;
  int *ptr_x_res = NULL;
  s21_decimal x_dec;
  res = s21_from_decimal_to_int(x_dec, ptr_x_res);
  ck_assert_int_eq(res, 1);
}
END_TEST

// int s21_from_decimal_to_float(s21_decimal src, float *dst)___________________________
START_TEST(test_s21_from_decimal_to_float_zero) {
  int res;
  float x_res, x_purp = 0.0f;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x00000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x00000000;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_neg_zero) {
  int res;
  float x_res, x_purp = -0.0f;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80000000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x00000000;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_max_dec) {
  int res;
  float x_res, x_purp = 7.92281625e+28f;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x00000000;
  x_dec.bits[2] = 0xFFFFFFFF;
  x_dec.bits[1] = 0xFFFFFFFF;
  x_dec.bits[0] = 0xFFFFFFFF;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_neg_max_dec) {
  int res;
  float x_res, x_purp = -7.92281625e+28f;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80000000;
  x_dec.bits[2] = 0xFFFFFFFF;
  x_dec.bits[1] = 0xFFFFFFFF;
  x_dec.bits[0] = 0xFFFFFFFF;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_min_dec) {
  int res;
  float x_res, x_purp = 1.0e-28f;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x001C0000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x00000000;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_neg_min_dec) {
  int res;
  float x_res, x_purp = -1.0e-28f;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x801C0000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00000000;
  x_dec.bits[0] = 0x00000000;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_num) {
  int res;
  float x_res, x_purp = 1.93428129e-01;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x001A0000;
  x_dec.bits[2] = 0x00100000;
  x_dec.bits[1] = 0x00900000;
  x_dec.bits[0] = 0x00000001;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_neg_num) {
  int res;
  float x_res, x_purp = -7.03687429e-01;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80100000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00190000;
  x_dec.bits[0] = 0x00000000;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_float_eq_tol(x_res, x_purp, 1e-6f);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_bad_dec) {
  int res;
  float x_res;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x80100001;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00190000;
  x_dec.bits[0] = 0x00400000;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_bad_scale) {
  int res;
  float x_res;
  s21_decimal x_dec;
  x_dec.bits[3] = 0x001D0000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00190000;
  x_dec.bits[0] = 0x00400000;
  res = s21_from_decimal_to_float(x_dec, &x_res);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_bad_ptr_float) {
  int res;
  float *ptr_x_res = NULL;
  s21_decimal x_dec = {0};
  res = s21_from_decimal_to_float(x_dec, ptr_x_res);
  ck_assert_int_eq(res, 1);
}
END_TEST

Suite *s21_decimal_suite() {
  Suite *s = suite_create("s21_decimal");
  TCase *tc;

  tc = tcase_create("s21_from_int_to_decimal");
  tcase_add_test(tc, test_s21_from_int_to_decimal_zero);
  tcase_add_test(tc, test_s21_from_int_to_decimal_max_int);
  tcase_add_test(tc, test_s21_from_int_to_decimal_min_int);
  tcase_add_test(tc, test_s21_from_int_to_decimal_pos_int);
  tcase_add_test(tc, test_s21_from_int_to_decimal_neg_int);
  tcase_add_test(tc, test_s21_from_int_to_decimal_bad_sorce);
  suite_add_tcase(s, tc);

  tc = tcase_create("s21_from_float_to_decimal");
  tcase_add_test(tc, test_s21_from_float_to_decimal_zero);
  tcase_add_test(tc, test_s21_from_float_to_decimal_neg_zero);
  tcase_add_test(tc, test_s21_from_float_to_decimal_max_for_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_max_neg_for_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_min_for_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_pos_small_for_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_neg_small_for_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_pos_big_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_neg_big_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_bigger_than_max_for_dec);
  tcase_add_test(tc,
                 test_s21_from_float_to_decimal_neg_bigger_than_max_for_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_smaller_than_min_for_dec);
  tcase_add_test(tc,
                 test_s21_from_float_to_decimal_neg_smaller_than_min_for_dec);
  tcase_add_test(tc, test_s21_from_float_to_decimal_bad_sorce);
  tcase_add_test(tc, test_s21_from_float_to_decimal_for_bank);
  tcase_add_test(tc, test_s21_from_float_to_decimal_for_bank2);
  suite_add_tcase(s, tc);

  tc = tcase_create("s21_from_decimal_to_int");
  tcase_add_test(tc, test_s21_from_decimal_to_int_zero);
  tcase_add_test(tc, test_s21_from_decimal_to_int_neg_zero);
  tcase_add_test(tc, test_s21_from_decimal_to_int_min);
  tcase_add_test(tc, test_s21_from_decimal_to_int_max);
  tcase_add_test(tc, test_s21_from_decimal_to_int_pos);
  tcase_add_test(tc, test_s21_from_decimal_to_int_neg);
  tcase_add_test(tc, test_s21_from_decimal_to_int_not_integer);
  tcase_add_test(tc, test_s21_from_decimal_to_int_bigger_than_int);
  tcase_add_test(tc, test_s21_from_decimal_to_int_bad_dec);
  tcase_add_test(tc, test_s21_from_decimal_to_int_bad_scale);
  tcase_add_test(tc, test_s21_from_decimal_to_int_bad_ptr_dec);
  suite_add_tcase(s, tc);

  tc = tcase_create("s21_from_decimal_to_float");
  tcase_add_test(tc, test_s21_from_decimal_to_float_zero);
  tcase_add_test(tc, test_s21_from_decimal_to_float_neg_zero);
  tcase_add_test(tc, test_s21_from_decimal_to_float_max_dec);
  tcase_add_test(tc, test_s21_from_decimal_to_float_neg_max_dec);
  tcase_add_test(tc, test_s21_from_decimal_to_float_min_dec);
  tcase_add_test(tc, test_s21_from_decimal_to_float_neg_min_dec);
  tcase_add_test(tc, test_s21_from_decimal_to_float_num);
  tcase_add_test(tc, test_s21_from_decimal_to_float_neg_num);
  tcase_add_test(tc, test_s21_from_decimal_to_float_bad_dec);
  tcase_add_test(tc, test_s21_from_decimal_to_float_bad_scale);
  tcase_add_test(tc, test_s21_from_decimal_to_float_bad_ptr_float);
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