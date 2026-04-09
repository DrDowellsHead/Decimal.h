#include "s21_helpers_funcs.h"

#include "s21_decimal.h"

void s21_zero_decimal(s21_decimal *dst) {
  // Функция для приведения данных s21_decimal к нулям во избежания
  // использования мусорных данных
  if (dst != NULL) {
    dst->bits[0] = 0;
    dst->bits[1] = 0;
    dst->bits[2] = 0;
    dst->bits[3] = 0;
  }
}

int s21_is_zero(s21_decimal value) {
  // Функция для проверки, является ли s21_decimal нулём (т.е. все биты
  // мантиссы равны нулю)
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

int s21_get_sign(s21_decimal value) {
  // Получение показателя знака мантиссы
  return ((unsigned int)value.bits[3] >> 31) & 1u;
}

void s21_set_sign(s21_decimal *value, int sign) {
  // Установка знака мантиссы (0 - положительный, 1 - отрицательный)
  if (value != NULL) {
    if (sign) {
      // В Шестнадцетиричном виде 0x80000000
      // соответствует 1000 0000 0000 0000 0000
      // 0000 0000 0000, что устанавливает старший
      // бит в единицу (отрицательное число)
      value->bits[3] |= (int)0x80000000u;
    } else {
      // В Шестнадцатеричном виде 0x7FFFFFFF
      // соответствует 0111 1111 1111 1111 1111
      // 1111 1111 1111, что устанавливает старший
      // бит в ноль (положительное число)
      value->bits[3] &= (int)0x7FFFFFFFu;
    }
  }
}

int s21_get_scale(s21_decimal value) {
  // Получение показателя точности числа
  return ((unsigned int)value.bits[3] >> 16) &
         0xFFu;  // Маска, которая не позволит прочитать больше 8 бит
}

int s21_set_scale(s21_decimal *value, int scale) {
  // Установка показателя точности мантиссы
  int status = 0;

  if (value == NULL || scale < 0 || scale > 28) {
    status = 1;
  } else {
    unsigned int service = (unsigned int)value->bits[3];

    service &= 0xFF00FFFFu;  // Маска для битов с 16 до 23
    service |= ((unsigned int)scale << 16);

    value->bits[3] = (int)service;
  }

  return status;
}