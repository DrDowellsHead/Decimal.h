#include "s21_decimal.h"
#include "s21_auxfunct.h"

int main() {
  s21_decimal s21Numb = {{0, 0, -0, 0}};
  //float flnum = 79832191.12345679;
  int src = -8;
  unsigned int srcu = 2147483648;
  long long int srcll = -8;
  //printFloatBits(flnum);
  //printBits(&flnum, sizeof(flnum));
  //printFloatBits(-flnum);
  //printf(" <- bit float \n");
 // printDecimalBits(s21Numb);

  printf("src   %d\n",src);
  printIntBits(src);
  //src *= -1;
  printIntBits(src);
  printf("srcu  %u\n",srcu);
  printIntBits(srcu);
  printf("srcll %lld\n",srcll);
  printBits(&srcll, sizeof(srcll));
  srcll *= -1;
  printBits(&srcll, sizeof(srcll));
  

  setScale(&s21Numb, 21);
  printDecimalBits(s21Numb);
  s21_from_int_to_decimal(src, &s21Numb);
  printDecimalBits(s21Numb);
  printf("\nScale = %d\n", getScale(s21Numb));


  printf("\n%u\n",s21Numb.bits[0]);
  int bitN = 131, bit, sign;
  while (bitN < 128) {
    
    
  //  printf("enter sign 0 = + or 1 = - : ");
    scanf("%d", &sign);
  //  printf(" | Get sign = %d Set sign result = %d \n", getSign(s21Numb), setSign( &s21Numb, sign));

    printf("enter bitNumb & bit : ");
    scanf("%d %d", &bitN, &bit);
  //  printf(" | Set result = %d \n", setDecimalBit(&s21Numb, bitN, bit));
  //  getDecimalBit(s21Numb, bitN);
    setDecimalBit(&s21Numb, bitN, bit);
    printDecimalBits(s21Numb);
    printf("\nScale = %d\n", getScale(s21Numb));
  }

  return 0;
}