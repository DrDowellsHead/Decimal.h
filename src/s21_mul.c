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

    int a[3] = {value_1.bits[0], value_1.bits[1], value_1.bits[2]};
    int b[3] = {value_2.bits[0], value_2.bits[1], value_2.bits[2]};

    // Проверка на ноль
    if ((a[0] == 0 && a[1] == 0 && a[2] == 0) ||
        (b[0] == 0 && b[1] == 0 && b[2] == 0)) {
      s21_set_sign(result, result_sign);
      s21_set_scale(result, result_scale);
      exitcode = 0;
    } else {
      // Временный массив для результата умножения (192 бита = 6 блоков)
    long long temp[6] = {0};

      // Умножение блоков
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          long long mul = (long long)a[i] * b[j];
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
      int mantissa[3] = {(int)temp[0], (int)temp[1],
                                  (int)temp[2]};

      // Если есть лишние биты, нужно округление
      if (high_block >= 3) {
        // Создаем временный decimal для округления
        s21_decimal temp_dec;
        s21_zero_decimal(&temp_dec);

        temp_dec.bits[0] = mantissa[0];
        temp_dec.bits[1] = mantissa[1];
        temp_dec.bits[2] = mantissa[2];

        // Устанавливаем масштаб 1 (отбрасываем один десятичный разряд)
        s21_set_scale(&temp_dec, 1);

        // Округляем
        if (s21_round(temp_dec, result) != 0) {
          exitcode = 1;
        } else {
          // Применяем знак
          s21_set_sign(result, result_sign);

          // Масштаб результата: исходный минус 1 (так как округлили)
          s21_set_scale(result, result_scale - 1);

          exitcode = 0;
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