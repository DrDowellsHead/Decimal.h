#include "s21_auxfunct.h"

long double mDecimal(s21_decimal numb) {
  long double res = 0;
  for (int i = 0; i < 96; i++) {
    res += getDecimalBit( numb, i) * pow(2.0, i);  //printf("i=%d, NUMB = %lf\n", i, modul_numb);
  }
  return res;
}

int bankRoundSeven(int src) {
  // округляет 8 значное целое до 7ми значащих цифр банковским округлением
  int res = 0, ostatok;
  ostatok = src % 10; //printf("ostatok = %d\n", ostatok);
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

void CheckFloat8(float src, int * mantissa_seven, int * scale_seven) {
  //находим 7 значащих цифр и порядок этого числа
    char string_float[100] = {0};  
    int digit_found = 0, delitel = 0; 
    *scale_seven = 0;                        //printf("\n\nS21 F to Dec \nsorce sign = %d, float(.28) = %-.28f\n", sign, src);
    sprintf(string_float, "%-.36f", src);                   //printf("Float   x = %f \nstring x = [%s]\n", src, string_float);
    for (int i = 0, flag_dot = 0, flag_digit = 0, int_digit_found = 0, flag = 1; flag; i++) { // && scale < 28; i++) {  
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
      flag = ((digit_found < 8 || flag_dot == 0) && *scale_seven < 28) ? flag : 0;  // выход из цикла: (поиск до 28 порядка) или (8 значащих цифр и найденная целая часть числа)  
      //printf("%2d[%c] dot= %d dig= %d scale= %2d, digitF= %2d, intdigitF= %2d  delitel = %d  flag = %d\n", i, string_float[i] ,flag_dot , flag_digit, *scale_seven, digit_found, int_digit_found, delitel, flag);
    }
    *scale_seven = *scale_seven - delitel ;  //printf("NEW scale (-delitel) = %d, (int)src*10^(%d) = %d \n", *scale_seven, *scale_seven, (int)((double)src*pow(10, *scale_seven)));
      if (digit_found >= 8) { 
        //если цифр больше 8 округляем до 7ми банк округлением
        *mantissa_seven = (bankRoundSeven((int)((double)src*pow(10, *scale_seven)))) / 10;
        (*scale_seven) = (*scale_seven) - 1; //        printf("BANKROUND DigitFound = %d, mantissa BANKround = %d, mantissa before round = %d\n", digit_found, *mantissa_seven, (int)((double)src*pow(10, (*scale_seven) + 1)));
      }
      else {
        *mantissa_seven = ((int)((double)src*pow(10, *scale_seven)));
      }                                        
      //printf ("FINAL CHEKFLOAT mantissa = %d in %d scale src float = %d\n", *mantissa_seven, *scale_seven, (int)((double)src*pow(10, *scale_seven + 1)) );
}

int checkDecimal(s21_decimal numb){
  //Decimal проверка числа на scale <= 28 && запрещенные биты = 0
  // 0 - ok    1 - error
  int res = 0, scale;
  for (int i = 96; i < 112; i++) {
      res = getDecimalBit(numb, i) ? 1 : res;
  }
  for (int i = 120; i < 127; i++) {
      res = getDecimalBit(numb, i) ? 1 : res;
  }
  scale = getScale(numb);
  if (scale > 28 || scale < 0) {
    res = 1;
  }
  return res;
}

// num[3] — 96-битное число (uint32_t), result[3] — частное
s21_decimal mant_div10(s21_decimal numb) {
    s21_decimal res = numb;
    zeroDecMant(&res);
    int reminder = 0;
    for (int i = 95; i >= 0; i--) {
        reminder <<= 1;
        int byte = i / 32;
        int bit = i % 32;
       // printf("Step N %d rem =  %d\n", i, reminder);
       // printBits(&reminder, sizeof (reminder));
        if (numb.bits[byte] & (1UL << bit))
            reminder |= 1;
        if (reminder >= 10) {
            reminder -= 10;
            res.bits[byte] |= (1UL << bit);
        }
    }
    //setScale(&res, (getScale(numb) - 1)); 
    return res;
}

// num[3] — 96-битное число (uint32_t), result[3] — частное
s21_decimal mant_mult10(s21_decimal numb) {
    s21_decimal res = numb, m8 = {0}, m2 = {0};
    zeroDecMant(&res);
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

void zeroDecMant(s21_decimal * numb) {
  numb->bits[0] = 0;
  numb->bits[1] = 0;
  numb->bits[2] = 0;
}

void zeroDecNumb(s21_decimal * numb) {
  numb->bits[0] = 0;
  numb->bits[1] = 0;
  numb->bits[2] = 0;
  numb->bits[3] = 0;
}

int getSign(s21_decimal numb) {
  return getDecimalBit(numb, 127);
}

int setSign(s21_decimal *numb, int sign) {
  //sign 
  // 0 = +  
  // 1 = -
  return setDecimalBit(numb, 127, sign); 
}

int getScale(s21_decimal numb) {
  char * ptr8bit = (char *)&numb;
  return *(ptr8bit+14);
  
  //double res = 0;
  //for (int bitN = 112; bitN < 120; bitN++) {
  //  res += getDecimalBit(numb, bitN) * pow(2, ((double)bitN - 112));
  //}
  //return (int)res;
}

int setScale(s21_decimal *numb, int scale) {
  char * ptr8bit = (char *)numb;
  int res = 0;
  if (scale >= 0 && scale <= 28) {
    *(ptr8bit+14) = scale;
  }
  else {
    //printf("ERROR: scale can be from 0 to 28, not %d\n", scale);
    res = 1;
  }
  return res; 
}

int getDecimalBit(s21_decimal numb, int bitNumber) {
  int result;
  if (bitNumber >= 0 && bitNumber < 128) {
    unsigned int DecByt = bitNumber / 32, DecBit = bitNumber % 32;
  //  printf("Decimal byte %d bit #%d current byte  &  mask:\n", DecByt, DecBit);
  //  printIntBits(numb.bits[DecByt]);
  //  printIntBits(1 << DecBit);
    result = ((numb.bits[DecByt] & (1 << DecBit)) == (1 << DecBit));
  //  printf("BIT [%d] = %d \n", bitNumber, result);
  } else {
    printf("ERROR type Decimal haven't bit # %d [0 - 127]\n", bitNumber);
    result = -1;
  }
  return result;
}

int setDecimalBit(s21_decimal *numb, int bitNumber, int bitDest) {
  int result = 0;
  if (bitNumber >= 0 && bitNumber < 128 && (bitDest == 0 || bitDest == 1)) {
    int DecByt = bitNumber / 32, DecBit = bitNumber % 32;
    if (bitDest == 0) {
        numb->bits[DecByt] = numb->bits[DecByt] & ~(1 << DecBit);
    } 
    else {
        numb->bits[DecByt] = numb->bits[DecByt] | (1 << DecBit);
    }
  } 
  else {
    printf("ERROR type Decimal haven't bit # %d [0 - 127], and can set only 0 or 1, not %d\n", bitNumber, bitDest);
    result = 1; //-1;
  }
  return result;
}

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

void printCharBits(char numb) {
  printf("Char number : %12u  16x    %x \nBits :   ", numb, numb);
  for (int i = 7; i >= 0; i--) {  // Для 8-битного числа (char)
    printf("%d", (numb >> i) & 1);
  }
  printf("\n");
}

void printIntBits(int numb) {
  printf("Int number : %12d  16x   %x \nBits :   ", numb, numb);
  for (int i = 31, count = 1; i >= 0; i--, count++) {  // Для 32-битного числа
    printf("%d", (numb >> i) & 1);
    if (count == 8) {
      printf(" ");
      count = 0;
    }
  }
  printf("\n");
}


void printFloatBits(float numb) {
    int * bits = (int *)&numb;
    printf("Float number : %-.8e \nBits :   ", numb);
    for (int i = 31; i >= 0; i--) {
      if (i < 23) {
        printf("\033[32m%d\033[0m", (*bits >> i) & 1);
      }
      else if (i > 22 && i < 31) {
        printf("\033[36m%d\033[0m", (*bits >> i) & 1);
      }
      else {
        printf("%d", (*bits >> i) & 1);
      }
      if ( i == 8 || i == 16 || i == 24 ) printf(" ");  // разделители для знака/экспоненты/мантиссы


 //   printf("Знак: %d (%s)\n", sign, sign ? "отрицательное" : "положительное");
 //   printf("Экспонента: %d (0x%02X)\n", exponent, exponent);
 //   printf("Мантисса: 0x%06X\n", mantissa);

    }
    printf("\n");
}


void printBits(const void *ptr, size_t size) {
    const unsigned char *bytes = (const unsigned char*)ptr;
    printf("Void type Bits : \n");
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (bytes[i] >> j) & 1);
        }
        if (i > 0) printf(" ");
        if (i % 4 == 0) printf ("\n");
    }
    printf("\n");
}