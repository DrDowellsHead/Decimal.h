#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int exitcode = 0;
  if (result == NULL) {
    exitcode = 1;
  } else {
    s21_zero_decimal(result);

    // Получаем знаки и масштабы
    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);
    int result_sign = sign1 ^ sign2;

    int scale1 = s21_get_scale(value_1);
    int scale2 = s21_get_scale(value_2);
    int result_scale = scale1 + scale2;

    unsigned int a[3] = {(unsigned int)value_1.bits[0],
                         (unsigned int)value_1.bits[1],
                         (unsigned int)value_1.bits[2]};
    unsigned int b[3] = {(unsigned int)value_2.bits[0],
                         (unsigned int)value_2.bits[1],
                         (unsigned int)value_2.bits[2]};

    // Проверка на ноль
    if ((a[0] == 0 && a[1] == 0 && a[2] == 0) ||
        (b[0] == 0 && b[1] == 0 && b[2] == 0)) {
      s21_set_sign(result, result_sign);
      s21_set_scale(result, result_scale);
    } else {
      // Временный массив для результата умножения (192 бита = 6 блоков)
      unsigned long long temp[6] = {0};

      // Умножение блоков
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          unsigned long long mul = (unsigned long long)a[i] * b[j];
          temp[i + j] += mul;

          temp[i + j + 1] += temp[i + j] >> 32;
          temp[i + j] &= 0xFFFFFFFF;
        }
      }

      // Финишные переносы
      for (int i = 0; i < 5; i++) {
        temp[i + 1] += temp[i] >> 32;
        temp[i] &= 0xFFFFFFFF;
      }

      // Определяем, сколько значащих блоков
      int high_block = 5;
      while (high_block > 0 && temp[high_block] == 0) {
        high_block--;
      }

      // Собираем 96-битную мантиссу
      unsigned int mantissa[3] = {(unsigned int)temp[0], (unsigned int)temp[1],
                                  (unsigned int)temp[2]};

      // Если есть лишние биты, нужно округление
      if (high_block >= 3) {
        // Округляем 192-битное число до 96-битной мантиссы
        if (s21_round_192_to_96(temp, mantissa) != 0) {
          exitcode = 1;
        } else {
          // Записываем результат
          result->bits[0] = mantissa[0];
          result->bits[1] = mantissa[1];
          result->bits[2] = mantissa[2];
          s21_set_sign(result, result_sign);
          s21_set_scale(result, result_scale);
        }
      } else {
        // Результат помещается в 96 бит
        result->bits[0] = mantissa[0];
        result->bits[1] = mantissa[1];
        result->bits[2] = mantissa[2];
        s21_set_sign(result, result_sign);
        s21_set_scale(result, result_scale);
      }
    }
  }
  return exitcode;
}