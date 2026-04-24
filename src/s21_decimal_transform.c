#include "s21_auxfunct.h"
#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 1;
  if (dst != NULL) {
    res = 0;
    ZeroDecimal(dst);
    setSign(dst, (src < 0) ? 1 : 0);
    if (src == -2147483648) {
      dst->bits[0] = src;  // 1 + (unsigned int)(-1 * (src + 1));
    } else {
      dst->bits[0] = src * ((src < 0) ? -1 : 1);
    }
  }
  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 1;
  if (dst != NULL) {
    int sign = (signbit(src) == 0) ? 0 : 1;
    float abssrc = sign ? -src : src;
    if (abssrc == 0.0f) {
      res = 0;
      ZeroDecimal(dst);
      setSign(dst, sign);
      setScale(dst, 0);
    } else if (abssrc >= 1e-28f && abssrc < 7.9228168e28f) {
      res = 0;
      ZeroDecimal(dst);
      // Получаем 7 значащих цифр через %e
      char buf[64];
      sprintf(buf, "%.6e", (double)abssrc);
      // buf вида "1.234568e-24"
      // Парсим мантиссу и экспоненту

      int exp;
      // Парсим строку вида "1.234568e-24" вручную
      char *eptr = buf;
      while (*eptr && *eptr != 'e' && *eptr != 'E') eptr++;
      exp = 0;
      if (*eptr) sscanf(eptr + 1, "%d", &exp);
      // Парсим мантиссу как целое (убирая точку)
      long long mantissa7 = 0;
      int digits = 0;
      for (char *p = buf; p < eptr; p++) {
        if (*p >= '0' && *p <= '9') {
          mantissa7 = mantissa7 * 10 + (*p - '0');
          digits++;
        }
      }
      // mantissa7 теперь = digits значащих цифр (digits=7 для "1.234568")
      // scale = 6 - exp (т.к. mant_d * 10^exp = src, и mantissa7 = mant_d *
      // 10^6)
      int scale = (digits - 1) - exp;
      // Если scale > 28, нужно обрезать мантиссу
      while (scale > 28) {
        long long last = mantissa7 % 10;
        mantissa7 /= 10;
        if (last >= 5) mantissa7++;
        scale--;
      }
      // Если scale < 0, умножаем мантиссу на 10 используя 96-битное хранение
      // mantissa7 сначала <= 9999999 (7 цифр), помещается в long long
      // но после умножений может вырасти до 96 бит
      unsigned int m[3] = {(unsigned int)(mantissa7 & 0xFFFFFFFF),
                           (unsigned int)((mantissa7 >> 32) & 0xFFFFFFFF), 0u};
      while (scale < 0) {
        // m *= 10
        unsigned long long carry = 0;
        for (int i = 0; i < 3; i++) {
          unsigned long long v = (unsigned long long)m[i] * 10 + carry;
          m[i] = (unsigned int)(v & 0xFFFFFFFF);
          carry = v >> 32;
        }
        scale++;
      }
      // Убираем незначащие нули
      while (scale > 0) {
        // Проверяем делимость на 10
        unsigned long long rem = 0;
        for (int i = 2; i >= 0; i--) {
          unsigned long long cur = rem * 0x100000000ULL + m[i];
          m[i] = (unsigned int)(cur / 10);
          rem = cur % 10;
        }
        if (rem != 0) {
          // не делится — возвращаем обратно (m *= 10)
          unsigned long long carry = rem;
          for (int i = 0; i < 3; i++) {
            unsigned long long v = (unsigned long long)m[i] * 10 + carry;
            m[i] = (unsigned int)(v & 0xFFFFFFFF);
            carry = v >> 32;
          }
          break;
        }
        scale--;
      }
      dst->bits[0] = (int)m[0];
      dst->bits[1] = (int)m[1];
      dst->bits[2] = (int)m[2];
      setSign(dst, sign);
      setScale(dst, scale);
    } else if (abssrc < 1e-28f) {
      res = 1;  // значение слишком маленькое для decimal
      ZeroDecimal(dst);
    }
  }
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  // max int 2147483647 && min int -2147483648
  int res = 1, sign, scale;
  if (dst != NULL && !CheckDecimalTR(src)) {
    long double norm_mant;
    sign = (getSign(src)) ? -1 : 1;
    scale = getScale(src);
    while (scale > 0) {
      src = MantissaDiv10(src);
      scale--;
    }
    norm_mant = MantissaDecimalTR(src);
    if (sign > 0 && norm_mant < (long double)2147483648) {
      res = 0;
      *dst = (int)norm_mant;
    } else if (sign < 0 && norm_mant < (long double)2147483649) {
      res = 0;
      *dst = (int)(sign * norm_mant);
    }
  }
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int res = 1, sign;
  if (dst != NULL && !CheckDecimalTR(src)) {
    res = 0;
    sign = (getSign(src)) ? -1 : 1;
    *dst =
        (float)(sign * ((MantissaDecimalTR(src)) * pow(10.0, -getScale(src))));
  }
  return res;
}

/*
Преобразователи
Возвращаемое значение — код ошибки:
0 — OK;
1 — ошибка конвертации.

Уточнение про преобразование числа типа float:
Если числа слишком малы (0 < |x| < 1e-28),
возвращай ошибку и значение, равное 0.
Если числа слишком велики (|x| > 79,228,162,514,264,337,593,543,950,335)
или равны бесконечности, возвращай ошибку.

При обработке числа с типом float преобразовывай все содержащиеся в нём
значимые десятичные цифры. Если таких цифр больше 7, то значение
числа должно округляться с использованием банковского округления к числу,
у которого не больше 7 значимых цифр.

Уточнение прo преобразование из числа типа decimal в тип int:
Если в числе типа decimal есть дробная часть, то её следует
отбросить (например, 0.9 преобразуется 0).
*/

// Из int