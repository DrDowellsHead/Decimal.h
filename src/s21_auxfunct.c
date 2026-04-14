#include "s21_auxfunct.h"

int CheckDecimalTR(s21_decimal numb){
  //Decimal проверка числа на scale <= 28 && запрещенные биты = 0
  // 0 - ok    1 - error
  int res = 0, scale;
  for (int i = 96; i < 112 && res != 1; i++) {
      res = getDecimalBit(numb, i) ? 1 : res;
  }
  for (int i = 120; i < 127 && res != 1; i++) {
      res = getDecimalBit(numb, i) ? 1 : res;
  }
  scale = getScale(numb);
  if (res != 1 && (scale > 28 || scale < 0)) {
    res = 1;
  }
  return res;
}

void CheckFloatTR(float src, int * mantissa_seven, int * scale_seven) {
  //находим 7 значащих цифр и порядок этого числа
    char string_float[100] = {0};  
    int digit_found = 0, delitel = 0; 
    *scale_seven = 0;               
    sprintf(string_float, "%-.36f", src);            
    for (int i = 0, flag_dot = 0, flag_digit = 0, int_digit_found = 0, flag = 1; flag; i++) {   
      if (string_float[i] == 46) {
        flag_dot = 1;
      }
      else {
        flag_digit = (string_float[i] != 48) ? 1 : flag_digit;
        *scale_seven = (flag_dot) ? *scale_seven + 1 : *scale_seven;
        int_digit_found = (flag_dot) ? int_digit_found : int_digit_found + 1;
        digit_found = (flag_digit) ? digit_found + 1 : digit_found;  
      }
      delitel = (int_digit_found > 8) ? int_digit_found - 8 : 0;                    // чтобы получить 8ми значное число, нужно src / (10 ^ delitel)
      flag = ((digit_found < 8 || flag_dot == 0) && *scale_seven < 28) ? flag : 0;  // выход из цикла: (поиск до 28 порядка) или (8 значащих цифр и найденной целой части числа)  
    }
    *scale_seven = *scale_seven - delitel ; 
    if (digit_found >= 8) { 
      //если цифр больше 8 округляем до 7ми банк округлением
      *mantissa_seven = (BankRoundTR((int)((double)src*pow(10, *scale_seven)))) / 10;
      (*scale_seven) = (*scale_seven) - 1; 
    }
    else {
      *mantissa_seven = ((int)((double)src*pow(10, *scale_seven)));
    }                                        
}

long double MantissaDecimalTR(s21_decimal numb) {
  long double res = 0;
  for (int i = 0; i < 96; i++) {
    res += getDecimalBit(numb, i) * pow(2.0, i); 
  }
  return res;
}

int BankRoundTR(int src) {
  // округляет 8 значное целое до 7ми значащих цифр банковским округлением
  int res = 0, ostatok;
  ostatok = src % 10; 
  if (ostatok == 5) {
    res = src / 10;
    if (res % 2 == 0) {
      res = res * 10;
    }
    else {
      res = (res + 1) * 10;
    }
  }
  else {
    res = (ostatok < 5) ? src - ostatok : src + (10 - ostatok);
  }
  return res;
}

void ZeroDecimal(s21_decimal * numb) {
  numb->bits[0] = 0;
  numb->bits[1] = 0;
  numb->bits[2] = 0;
  numb->bits[3] = 0;
}

s21_decimal MantissaDiv10(s21_decimal numb) {
    s21_decimal res = numb;
    res.bits[0] = 0;
    res.bits[1] = 0;
    res.bits[2] = 0;
    int reminder = 0;
    for (int i = 95; i >= 0; i--) {
        reminder <<= 1;
        int byte = i / 32;
        int bit = i % 32;
        if (numb.bits[byte] & (1UL << bit))
            reminder |= 1;
        if (reminder >= 10) {
            reminder -= 10;
            res.bits[byte] |= (1UL << bit);
        }
    }
    return res;
}

s21_decimal MantissaMult10(s21_decimal numb) {
    s21_decimal res = numb, m8 = {0}, m2 = {0};
    res.bits[0] = 0;
    res.bits[1] = 0;
    res.bits[2] = 0;
    for (int i = 95; i >= 0; i--) {
      setDecimalBit( &m8, i, ((i >= 3) ? getDecimalBit( numb, i-3) : 0 ));
      setDecimalBit( &m2, i, ((i >= 1) ? getDecimalBit( numb, i-1) : 0 ));
    }
    for (int i = 0, ps = 0, bit2, bit8, psnew; i <96; i++) {
      bit2 = getDecimalBit( m2, i);
      bit8 = getDecimalBit( m8, i);
      setDecimalBit( &res, i, bit2 ^ bit8 ^ ps); 
      psnew = (bit2 & bit8) | (bit2 & ps) | (bit8 & ps);
      ps = psnew;
    }
    return res;
}

int getSign(s21_decimal numb) {
  return getDecimalBit(numb, 127);
}

int setSign(s21_decimal *numb, int sign) {
  return setDecimalBit(numb, 127, sign); 
}

int getScale(s21_decimal numb) {
  char * ptr8bit = (char *)&numb;
  return (int)*(ptr8bit+14);
}

int setScale(s21_decimal *numb, int scale) {
  char * ptr8bit = (char *)numb;
  int res = 1;
  if (scale >= 0 && scale <= 28) {
    *(ptr8bit+14) = scale;
    res = 0;
  }
  return res; 
}

int getDecimalBit(s21_decimal numb, int bitNumber) {
  int result = -1;
  if (bitNumber >= 0 && bitNumber < 128) {
    unsigned int DecByt = bitNumber / 32, DecBit = bitNumber % 32;
    result = ((numb.bits[DecByt] & (1 << DecBit)) == (1 << DecBit));
  }
  return result;
}

int setDecimalBit(s21_decimal *numb, int bitNumber, int bitDest) {
  int res = 1;
  if (bitNumber >= 0 && bitNumber < 128 && (bitDest == 0 || bitDest == 1)) {
    int DecByt = bitNumber / 32, DecBit = bitNumber % 32;
    if (bitDest == 0) {
        numb->bits[DecByt] = numb->bits[DecByt] & ~(1 << DecBit);
    } 
    else {
        numb->bits[DecByt] = numb->bits[DecByt] | (1 << DecBit);
    }
    res = 0;
  } 
  return res;
}

/*
void printDecimalBits(s21_decimal numb) {
  for (int j = 3; j >= 0; j--) {
    int temp = numb.bits[j];
    printf("bits [%d] % 4d - % 4d: ", j, (32 * (j + 1)) - 1, 32 * j);
    for (int i = 31, count = 1; i >= 0; i--, count++) {
      if (j < 3) {
        printf("\033[32m%d\033[0m", (temp >> i) & 1);
      }
      else if (i > 15 && i < 24) {
        printf("\033[36m%d\033[0m", (temp >> i) & 1);
      }
      else if (i == 31) {
        printf("%d", (temp >> i) & 1);
      }
      else {
        printf("\033[31m%d\033[0m", (temp >> i) & 1);
      }
      if (count == 8) {
        printf(" ");
        count = 0;
      }
    }
    printf("\n");
  }
  printf("Sign is [%d]  Scale is [%d]\n", getSign(numb), getScale(numb));
}
*/