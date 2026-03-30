#ifndef S21_HELPERS_FUNCS_H
#define S21_HELPERS_FUNCS_H

#include "s21_decimal.h"

void s21_zero_decimal(s21_decimal *dst); // Очистка данных result
int s21_is_zero(s21_decimal value); // Проверка на ноль
int s21_get_sign(s21_decimal value); // Получение знака
int s21_set_sign(s21_decimal *value, int sign); // Установка знака (0 - положительный, 1 - отрицательный)
int s21_get_scale(s21_decimal value); // Получение коэффициента масштабирования
int s21_set_scale(s21_decimal *value, int scale); // Установка коэффициента масштабирования (0-28)

#endif