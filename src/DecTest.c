#include "s21_decimal.h"
#include "s21_auxfunct.h"


union DateDecLL {
   s21_decimal decNumb;
   long long int llNumb[2]; 
 };


int main() {
  s21_decimal s21Numb = {{0xFFFFFFFF, 0x000000FF, 0, 0}};
  //float flnum = 79832191.12345679;

  //0x7FFFFFFF
  //int src = -2147483647, srcA = -99;//0x9F9F9F9F;

  //unsigned int srcu = src;
  //long long int srcll = src;
  //printFloatBits(flnum);
  //printBits(&flnum, sizeof(flnum));
  //printFloatBits(-flnum);
  //printf(" <- bit float \n");
 // printDecimalBits(s21Numb);

  //printf("src   %d   src16   %x\n", src, src);
  //printIntBits(src);
  //src *= -1;
  //printIntBits(src);
  //printf("srcu  %u\n",srcu);
  //printIntBits(srcu);
  //printf("srcll %lld\n",srcll);
  //printBits(&srcll, sizeof(srcll));
  //srcll *= -1;
  //printBits(&srcll, sizeof(srcll));
  

  //setScale(&s21Numb, 21);
  //printDecimalBits(s21Numb);
 
  //printIntBits(src);
  //s21_from_int_to_decimal(src, &s21Numb);
  //printDecimalBits(s21Numb);
  //printf("src             %16d     src16   %x\n", src, src);
  //printf("s21Numb.bits[0] %16u       16X   %x\n", s21Numb.bits[0], s21Numb.bits[0]);


  /* from int to dec & back 
  s21Numb.bits[3] = 0xFFFFFFFF;
  int src = 2147483649, srcA = -99;
  printf("trouble num unsign = %u  int = %d\n", src, src);
  printIntBits(src);
  s21_from_int_to_decimal(src, &s21Numb);
  //setScale(&s21Numb, 8);
  printDecimalBits(s21Numb);
  printf("convertation from dec to int = %d  (0 - ok / 1 - error)\n", s21_from_decimal_to_int(s21Numb, &srcA));
  printIntBits(srcA);
  //printIntBits(s21Numb.bits[0]);
  */

  /* FLOAAAAAAAAAAAAAAAAAAAAT */
  
  float x = 268435459;
  /*
  unsigned int src = 0x9FFFFFFF;
  int srcA = 0x9FFFFFFF;
  printf("trouble num unsign = %u  int = %d\n", src, src);
  printIntBits(src);
  printf("A trouble num unsign = %u  int = %d\n", srcA, srcA);
  printIntBits(srcA);
  */

  x = 0.341e-28;//
  //x = 1234567890123456789012345678901234567890.123456789012345678901234567890;
  //
  float x2 = 1e+29, x3 = 1e-28; //79228162514264337593543950336.0;
  printf("FLOAT MAX = %f\n", x);
  printFloatBits(x);
  
  /*
  char buffFloat[100];
  // Преобразование float в строку
  // С контролем точности
  sprintf(buffFloat, "%-#.30f", x3); 
  printf("Float string x3= [%s]\n", buffFloat);
  sprintf(buffFloat, "%-#.30f", x); 
  printf("Float string x = [%s]\n", buffFloat);
  sprintf(buffFloat, "%-#.30f", x2); 
  printf("Float string x2= [%s]\n", buffFloat);

  printf("\n - = [%c]  + = [%c]  . = [%c]\n", 45, 43, 46);

  //printf("true ? %d\n", (x < 1e-28));
  printf("true x < x2 & x > 1e-28 ? %d\n", (x2 > x) && (x > 1e-28));
  */


  char buffFloat[100];
  float x5 = -1234567.8901e+38;
  sprintf(buffFloat, "%-f", x5); 
  printf("Float %f string x = [%s]\n", x5, buffFloat);
  printf("true x < x2 & x > 1e-28 ? %d\n", (x5 > 1e-28 && x5 < 1e+29));
  //printf("Checkfloat = %d\n", checkPFloat(x5));
  x = 7.9228162514264337593543950336e+28; // max numb
  x = -0.0000012481846;
  sprintf(buffFloat, "%-f", x); 
  printf("Float   x = %f \nstring x = [%s]\n", x, buffFloat);
  printf("result of float to dec = %d", s21_from_float_to_decimal(x, &s21Numb));




/* проверка деления 
  union DateDecLL DATA1;
  DATA1.decNumb = s21Numb;

  printf("UNION LL %30llu       16X   %llx\n", DATA1.llNumb[0], DATA1.llNumb[0]);
  printDecimalBits(DATA1.decNumb);

  DATA1.decNumb = mant_div10(DATA1.decNumb);

  printf("UNION LL %30llu       16X   %llx\n", DATA1.llNumb[0], DATA1.llNumb[0]);
  printDecimalBits(DATA1.decNumb);
*/

  //s21Numb = mant_div10(s21Numb);
  //printIntBits(src);
  //printf("src             %16d     src16   %x\n", src, src);
  //printf("s21Numb.bits[0] %16u       16X   %x\n", s21Numb.bits[0], s21Numb.bits[0]);

  //printf("\nScale = %d\n", getScale(s21Numb));


  /*
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
*/
  return 0;
}