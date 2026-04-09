#ifndef S21_BIG_DECIMAL_H
#define S21_BIG_DECIMAL_H

#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

typedef struct {
  unsigned int bits[7];  // 224 бита для хранения большой мантиссы
  int scale;  // Поле размера выведено отдельно
  int sign;  // Поле знака выведено отдельно. 0 - положительное, 1 -
             // отрицательное
} s21_big_decimal;

void s21_big_zero(s21_big_decimal *rst);
void s21_big_from_decimal(s21_decimal src, s21_big_decimal *rst);
int s21_big_is_zero(s21_big_decimal value);

int s21_big_compare_mantissa(s21_big_decimal a, s21_big_decimal b);
int s21_big_add_mantissa(s21_big_decimal a, s21_big_decimal b,
                         s21_big_decimal *result);
int s21_big_sub_mantissa(s21_big_decimal a, s21_big_decimal b,
                         s21_big_decimal *result);
int s21_big_mul10_mantissa(s21_big_decimal *value);
int s21_big_div10_mantissa(s21_big_decimal *value, unsigned int *remainder);
int s21_big_fits_decimal(s21_big_decimal value);
int s21_big_round_up(s21_big_decimal value, unsigned int last_removed,
                     int sticky);
int s21_big_decimal_rounding(s21_big_decimal src, s21_decimal *rst);
int s21_big_normalize_scales(s21_big_decimal *a, s21_big_decimal *b);
int s21_big_add_one_mantissa(s21_big_decimal *value);
int s21_add_sub_core(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, int is_sub);

#endif