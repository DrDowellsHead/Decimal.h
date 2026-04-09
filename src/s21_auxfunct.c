#include "s21_auxfunct.h"

void getDecimalBit(s21_decimal numb, int bitNumber) {
  if (bitNumber >= 0 && bitNumber < 128) {
    int DecByt = bitNumber / 32, DecBit = bitNumber % 32;
    printf("Decimal byte %d bit #%d current byte  &  mask:\n", DecByt, DecBit);
    printIntBits(numb.bits[DecByt]);
    printIntBits(1 << DecBit);
    printf("BIT [%d] = %d \n", bitNumber,
           (numb.bits[DecByt] & (1 << DecBit)) == (1 << DecBit));
  } else {
    printf("type Decimal haven't bit # %d [0 - 127]\n", bitNumber);
  }
}

void printDecimalBits(s21_decimal numb) {
  for (int j = 0; j < 4; j++) {
    int temp = numb.bits[j];
    printf("bits [%d] % 4d - % 4d: ", j, (32 * (j + 1)) - 1, 32 * j);
    for (int i = 31, count = 1; i >= 0; i--, count++) {
      printf("%d", (temp >> i) & 1);
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