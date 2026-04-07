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
  int res = 0;
  unsigned int srcD;
  zeroDecNumb(dst);
  res = setScale(dst, 0);
  if (src < 0) {
    setSign(dst, 1);
    srcD = (unsigned int)(-1 * (src + 1));
    srcD++;
  }
  else {
    setSign(dst, 0);
    srcD = (unsigned int)src;
  }
  dst->bits[0] = srcD;
  res = (res != 0) ? 1 : 0;
  return res;
}


// Из float
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 0, sign = 0, scale = 0, mantissa, delitel = 0, digitFound = 0;
  sign = (src < 0) ? 1 : 0;
  src = (sign) ? src * ( -1 ) : src;
                         printf("\n\nS21 F to Dec \nsorce sign = %d, float(.28) = %-.28f\n", sign, src);
  if ((src != 0 && src < 1e-28) || src > 1e+29) {
    res = 1;
    zeroDecNumb(dst);
  }
  else {
    char stringFloat[100];
    zeroDecNumb(dst); 
    setSign(dst, sign);  
    sprintf(stringFloat, "%-.36f", src);        //printf("Float   x = %f \nstring x = [%s]\n", src, stringFloat);
      for (int i = 0, flagDot = 0, flagDigit = 0, intdigitfound = 0, flag = 1; flag && scale < 28;) {
        if (stringFloat[i] == 46) {
          flagDot = 1;                          // printf ("[%c] i = %d, scale = %d, digitFound = %d, flagdig = %d, flagdot = %d, intdigitfound = %d\n", stringFloat[i], i, scale, digitFound, flagDigit, flagDot, intdigitfound);
          i++;
        }
        else {
          flagDigit = ( stringFloat[i] != 48 ) ? 1 : flagDigit;
          scale = ( flagDot ) ? scale + 1 : scale;
          intdigitfound = ( flagDot ) ? intdigitfound : intdigitfound + 1;
          digitFound = ( flagDigit ) ? digitFound + 1 : digitFound;  // printf ("[%c] i= %2d, scale= %2d, digitFound= %2d, flagdig= %2d, flagdot= %2d, intdigitfound= %2d\n", stringFloat[i], i, scale, digitFound, flagDigit, flagDot, intdigitfound);
          i++;
        }
        flag = (digitFound < 8 || flagDot == 0) ? flag : 0; 
        delitel = (intdigitfound > 8) ? intdigitfound - 8 : 0; //printf("digitFound < 9 && !flagDot = %d\n", digitFound < 9 && !flagDot);       
      }
      scale = scale - delitel ;
      if (digitFound >= 8) {
        mantissa = (bankRoundSeven((int)((double)src*pow(10, scale)))) / 10;
        scale--;
        printf("DigitFound = %d, mantissa round = %d, mantissa before round = %d\n", digitFound, mantissa, (int)((double)src*pow(10, scale + 1)));
      }
      else {
        mantissa = ((int)((double)src*pow(10, scale)));
      }
      printf ("AFTER ROUND mantissa = %d in %d scale src float = %d\n", mantissa, scale, (int)((double)src*pow(10, scale + 1)) );
      
      if (scale < -20 && mantissa > 7922816) {
        printf("numb is bigger then max decimal\n");
        res = 1;
        zeroDecNumb(dst);  
      }
      else if (mantissa != 0) {
        while (mantissa % 10 == 0) {
          mantissa /= 10;
          scale--;
        }
        dst->bits[0] = mantissa;
        while (scale < 0) {
          *dst = mant_mult10(*dst);
          scale++;
        } 
        setScale(dst, scale); 
      }
  }
  printf("\n\nFLOAT TO DEC final: \n");
  printDecimalBits(*dst);
  return res;
}


// В int
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  // max int 2147483647 && min int -2147483648
  int res = 0, sign, scale;
  double norm_mant;
  sign = (getSign(src)) ? - 1 : 1;
  scale = getScale(src);                                     //printf("%30lf\n", mDecimal(src));
  while (scale > 0) {
    src = mant_div10(src);
    scale--;
  }                                                          //  printDecimalBits(src);
  norm_mant = mDecimal(src);                                 //printf("%30lf\n", mDecimal(src)); //printf("%-20lf\n%-20lu\n", norm_mant, 2147483648);
  if (sign > 0 && norm_mant < (double)2147483648) {
      *dst = (int)norm_mant;
    }
  else if (sign < 0 && norm_mant < (double)2147483649) {
      *dst = (int)(sign * norm_mant);
    }
  else {
      *dst = 0;
      res = 1;
  }     
  return res;
}


// В float
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int res = 0, sign;
  if (checkDecimal(src)) {
    *dst = 0;
    res = 1;
  }
  else {
    sign = (getSign(src)) ? - 1 : 1;
    *dst = (float)(sign * ((mDecimal(src)) * pow(10.0, - getScale(src))));
  } 
  return res;
}

