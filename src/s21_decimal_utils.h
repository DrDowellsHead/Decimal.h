#ifndef S21_DECIMAL_UTILS_H
#define S21_DECIMAL_UTILS_H

#include "s21_decimal.h"

int s21_get_bit(s21_decimal value, int n);
void s21_set_bit(s21_decimal *value, int n, int bit);

int s21_get_sign(s21_decimal value);
void s21_set_sign(s21_decimal *value, int sign);

int s21_get_scale(s21_decimal value);
void s21_set_scale(s21_decimal *value, int scale);

s21_decimal s21_zero(void);
int s21_is_zero(s21_decimal value);

#endif