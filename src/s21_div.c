#include "s21_decimal.h"
#include "s21_decimal_utils.h"

// Вспомогательные функции для 96-битных чисел
static int is_greater_or_equal_96(s21_decimal rem, s21_decimal div) {
  int exitcode = 1;
  for (int i = 2; i >= 0 && exitcode; i--) {
    if (rem.bits[i] < div.bits[i]) exitcode = 0;
  }
  return exitcode;
}

static void sub_96(s21_decimal *rem, s21_decimal div) {
  unsigned long long borrow = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long diff =
        (unsigned long long)rem->bits[i] - div.bits[i] - borrow;
    rem->bits[i] = (unsigned int)diff;
    borrow = (diff >> 32) & 1;
  }
}

static void shift_left_96(s21_decimal *num) {
  unsigned long long carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long val = ((unsigned long long)num->bits[i] << 1) | carry;
    num->bits[i] = (unsigned int)val;
    carry = val >> 32;
  }
}

static s21_decimal copy_96(s21_decimal src) {
  s21_decimal dst = s21_zero();
  for (int i = 0; i < 3; i++) {
    dst.bits[i] = src.bits[i];
  }
  return dst;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int exitcode = 0;
  if (result == NULL) {
    exitcode = 1;
  } else {
    *result = s21_zero();

    // Проверка деления на ноль
    if (s21_is_zero(value_2)) {
      exitcode = 1;
    } else {
      // Получаем знаки и масштабы
      int sign1 = s21_get_sign(value_1);
      int sign2 = s21_get_sign(value_2);
      int result_sign = sign1 ^ sign2;

      int scale1 = s21_get_scale(value_1);
      int scale2 = s21_get_scale(value_2);
      int result_scale = scale1 - scale2;

      // Очищаем знак и масштаб для мантисс
      s21_decimal dividend = value_1;
      s21_decimal divisor = value_2;
      s21_set_sign(&dividend, 0);
      s21_set_scale(&dividend, 0);
      s21_set_sign(&divisor, 0);
      s21_set_scale(&divisor, 0);

      // Нормализация: умножаем делимое на 10^result_scale если result_scale < 0
      if (result_scale < 0) {
        int power = -result_scale;
        s21_decimal ten = s21_zero();
        ten.bits[0] = 10;

        for (int i = 0; i < power && !exitcode; i++) {
          s21_decimal temp = s21_zero();
          if (s21_mul(dividend, ten, &temp) != 0) {
            exitcode = 1;
          }
          if (!exitcode) dividend = temp;
        }

        if (!exitcode) {
          result_scale = 0;
        }
      }

      if (!exitcode) {
        // Деление 96-битных чисел
        s21_decimal quotient = s21_zero();
        s21_decimal remainder = s21_zero();
        s21_decimal temp_divisor = copy_96(divisor);

        for (int bit = 95; bit >= 0; bit--) {
          shift_left_96(&remainder);
          remainder.bits[0] |= s21_get_bit(dividend, bit);

          if (is_greater_or_equal_96(remainder, temp_divisor)) {
            sub_96(&remainder, temp_divisor);
            s21_set_bit(&quotient, bit, 1);
          }
        }

        // Проверка переполнения
        if (quotient.bits[2] > 0xFFFFFFFF) {
          exitcode = 1;
        } else {
          result->bits[0] = quotient.bits[0];
          result->bits[1] = quotient.bits[1];
          result->bits[2] = quotient.bits[2];
          s21_set_sign(result, result_sign);
          s21_set_scale(result, result_scale);
        }
      }
    }
  }
  return exitcode;
}