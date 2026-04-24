#include "s21_big_decimal.h"
#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  // Функция для проверки, равны ли два числа s21_decimal. Возвращает 1, если
  // числа равны, и 0 в противном случае. Сравнение происходит по следующим
  // шагам:
  // 1. Проверка на ноль: если оба числа равны нулю, то они считаются равными,
  // независимо от их знаков и масштабов.
  // 2. Проверка знаков: если знаки чисел различаются, то они не равны.
  // 3. Нормализация масштабов: если знаки чисел одинаковые, то их мантиссы
  // нормализуются до одинакового масштаба (количество десятичных знаков).
  // 4. Сравнение мантиссы: после нормализации, сравниваются мантиссы чисел.
  // Если мантиссы равны, то числа считаются равными, иначе - неравными.
  if (s21_is_zero(value_1) && s21_is_zero(value_2)) {
    return 1;
  }

  if (s21_get_sign(value_1) != s21_get_sign(value_2)) {
    return 0;
  }

  s21_big_decimal first;
  s21_big_decimal second;

  s21_big_from_decimal(value_1, &first);
  s21_big_from_decimal(value_2, &second);

  if (s21_big_normalize_scales(&first, &second) != 0) {
    return 0;
  }

  return s21_big_compare_mantissa(first, second) == 0;
}