#include "s21_auxfunct.h"
#include "s21_decimal.h"

int main() {
  s21_decimal s21Numb = {{-1, -2, -3, 1}};
  printDecimalBits(s21Numb);

  int bitN = 31;
  while (bitN < 128) {
    scanf("%d", &bitN);
    getDecimalBit(s21Numb, bitN);
  }

  return 0;
}