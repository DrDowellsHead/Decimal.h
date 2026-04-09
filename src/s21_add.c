#include "s21_big_decimal.h"
#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    return s21_add_sub_core(value_1, value_2, result, 0);
}