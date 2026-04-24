#include <stddef.h>

#include "s21_big_decimal.h"
#include "s21_decimal.h"
#include "s21_decimal_utils.h"
#include "s21_helpers_funcs.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int exitcode = 0;
  if (result == NULL) {
    exitcode = 1;
  } else {
    *result = s21_zero();

    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);
    int result_sign = sign1 ^ sign2;

    int scale1 = s21_get_scale(value_1);
    int scale2 = s21_get_scale(value_2);
    int result_scale = scale1 + scale2;

    if (s21_is_zero(value_1) || s21_is_zero(value_2)) {
      s21_set_sign(result, result_sign);
      int final_scale = result_scale > 28 ? 28 : result_scale;
      s21_set_scale(result, final_scale);
      return 0;
    }

    // 192-битное умножение
    unsigned int a[3] = {(unsigned int)value_1.bits[0],
                         (unsigned int)value_1.bits[1],
                         (unsigned int)value_1.bits[2]};
    unsigned int b[3] = {(unsigned int)value_2.bits[0],
                         (unsigned int)value_2.bits[1],
                         (unsigned int)value_2.bits[2]};

    unsigned long long temp[6] = {0};
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        unsigned long long mul = (unsigned long long)a[i] * b[j];
        temp[i + j] += mul;
        temp[i + j + 1] += temp[i + j] >> 32;
        temp[i + j] &= 0xFFFFFFFF;
      }
    }
    for (int i = 0; i < 5; i++) {
      temp[i + 1] += temp[i] >> 32;
      temp[i] &= 0xFFFFFFFF;
    }

    // Собираем результат в s21_big_decimal
    s21_big_decimal big;
    s21_big_zero(&big);
    for (int i = 0; i < 6 && i < 7; i++) {
      big.bits[i] = (unsigned int)temp[i];
    }
    big.scale = result_scale;
    big.sign = result_sign;

    // Используем rounding для приведения к 96 бит
    exitcode = s21_big_decimal_rounding(big, result);
    // exitcode: 0=OK, 1=TOO_BIG (положительное переполнение), 2=TOO_SMALL
    // (отрицательное)
  }
  return exitcode;
}