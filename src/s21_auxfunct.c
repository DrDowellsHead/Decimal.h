#include "s21_auxfunct.h"


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
    printf("ERROR: scale can be from 0 to 28, not %d\n", scale);
    res = -1;
  }
  return res; 
}


int getDecimalBit(s21_decimal numb, int bitNumber) {
  int result;
  if (bitNumber >= 0 && bitNumber < 128) {
    int DecByt = bitNumber / 32, DecBit = bitNumber % 32;
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
    result = -1;
  }
return result;
}

void printDecimalBits(s21_decimal numb) {
  for (int j = 0; j < 4; j++) {
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
}

void printCharBits(char num) {
  for (int i = 7; i >= 0; i--) {  // Для 8-битного числа (char)
    printf("%d", (num >> i) & 1);
  }
  printf("\n");
}

void printIntBits(int num) {
  for (int i = 31, count = 1; i >= 0; i--, count++) {  // Для 32-битного числа
    printf("%d", (num >> i) & 1);
    if (count == 8) {
      printf(" ");
      count = 0;
    }
  }
  printf("\n");
}


void printFloatBits(float fnum) {
    int * bits = (int *)&fnum;

    printf("Число: %20.7f |   ", fnum);
    printf("Биты: ");
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
    
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (bytes[i] >> j) & 1);
        }
        if (i > 0) printf(" ");
    }
    printf("\n");
}