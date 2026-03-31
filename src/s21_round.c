#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int exitcode = 0;
  if (result == NULL) {
    exitcode = 1;
  } else {
    s21_zero_decimal(result);

    int sign = s21_get_sign(value);
    int scale = s21_get_scale(value);

    if (scale == 0) {
      *result = value;
    } else {
      // Копируем мантиссу (bits[0-2] уже есть в value)
      unsigned int mantissa[3] = {(unsigned int)value.bits[0],
                                  (unsigned int)value.bits[1],
                                  (unsigned int)value.bits[2]};

      // Вычисляем делитель 10^scale
      unsigned int divisor = 1;
      for (int i = 0; i < scale; i++) {
        divisor *= 10;
      }

      // Делим 96-битное число на 32-битный делитель
      unsigned long long remainder = 0;
      unsigned int result_mantissa[3] = {0, 0, 0};

      for (int i = 2; i >= 0; i--) {
        unsigned long long current = (remainder << 32) | mantissa[i];
        result_mantissa[i] = (unsigned int)(current / divisor);
        remainder = current % divisor;
      }

      unsigned long long half = divisor / 2;

      // Банковское округление
      int need_increment = 0;
      if (remainder > half) {
        need_increment = 1;
      } else if (remainder == half) {
        // Проверяем четность младшего бита целой части
        if (result_mantissa[0] & 1) {
          need_increment = 1;
        }
      }

      // Увеличиваем если нужно
      if (need_increment) {
        unsigned long long carry = 1;
        for (int i = 0; i < 3 && carry; i++) {
          unsigned long long sum =
              (unsigned long long)result_mantissa[i] + carry;
          result_mantissa[i] = (unsigned int)sum;
          carry = sum >> 32;
        }
        if (carry) exitcode = 1;
      }
      if (!exitcode) {
        // Записываем результат
        result->bits[0] = result_mantissa[0];
        result->bits[1] = result_mantissa[1];
        result->bits[2] = result_mantissa[2];
        s21_set_scale(result, 0);
        s21_set_sign(result, sign);
      }
    }
  }
  return exitcode;
}