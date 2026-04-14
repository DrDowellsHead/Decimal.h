#include "s21_decimal.h"

#include "s21_auxfunct.h"
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
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 1;
  if (dst != NULL) { 
    res = 0;
    zeroDecNumb(dst);
    setSign(dst, (src < 0) ? 1 : 0);
    if (src == -2147483648) {
      dst->bits[0] = src;               //1 + (unsigned int)(-1 * (src + 1));
    }
    else {
      dst->bits[0] = src * ((src < 0) ? -1 : 1);
    }
  }
  return res;
}


// Из float
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 1; 
  if (dst != NULL) {  
    int sign, scale, mantissa;
    sign = (signbit(src) == 0) ? 0 : 1; 
    src = (sign) ? src * (-1) : src;
    if (src == 0 || (src > 1e-28 && src < 7.9228168e+28)) {// zero, min & max check 
      res = 0;
      zeroDecNumb(dst); 
      CheckFloat8(src, &mantissa, &scale);   
      setSign(dst, sign); 
      if (mantissa != 0) {
        while (mantissa % 10 == 0 && scale > 0) {
          mantissa /= 10;
          scale--;
        }
        dst->bits[0] = mantissa; 
        while (scale < 0) {
          *dst = mant_mult10(*dst);
          scale++;
        } 
      }
      else {
        scale = 0;
      }
      setScale(dst, scale);
    }
  }
  return res;
}


// В int
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  // max int 2147483647 && min int -2147483648
  int res = 1, sign, scale;
  if (dst != NULL && !CheckDecimal(src)) {
    long double norm_mant;
    sign = (getSign(src)) ? - 1 : 1;
    scale = getScale(src);                                     
    while (scale > 0) {
      src = mant_div10(src);
      scale--;
    }   
    norm_mant = mDecimal(src);                                
    if (sign > 0 && norm_mant < (long double)2147483648) {
      res = 0;
      *dst = (int)norm_mant;
    }
    else if (sign < 0 && norm_mant < (long double)2147483649) {
      res = 0;
      *dst = (int)(sign * norm_mant);
    }
  }
  return res;
}


// В float
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int res = 1, sign;
  if (dst != NULL && !CheckDecimal(src)) {
    res = 0;
    sign = (getSign(src)) ? - 1 : 1;
    *dst = (float)(sign * ((mDecimal(src)) * pow(10.0, - getScale(src))));
  } 
  return res;
}

