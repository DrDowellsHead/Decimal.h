#ifndef S21_HELPERS_FUNCS_H
#define S21_HELPERS_FUNCS_H

#include "s21_decimal.h"
#include "s21_decimal_utils.h"

void s21_zero_decimal(s21_decimal *dst);
int s21_round_192_to_96(unsigned long long temp[6], unsigned int mantissa[3]);

#endif
