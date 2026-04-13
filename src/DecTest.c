#include "s21_decimal.h"
#include "s21_auxfunct.h"

/*
union DateDecLL {
   s21_decimal decNumb;
   long long int llNumb[2]; 
   int intNumb[4];
 };
*/

s21_decimal * ptr_dec_numb;

int main() {
  s21_decimal s21Numb;// = {0};//{{0xFFFFFFFF, 0x000000FF, 0, 0}};
  //printDecimalBits(s21Numb);
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


  // INTEGER  from int to dec & back 
  /*
  //s21Numb.bits[3] = 0xFFFFFFFF;
  int src = 2147483647, srcA = 77;
  //src = 2147483647; //max int
  src = -2147483648; //min int
  //src = -2147483647;
  src = 0;
  //src = 8;
  //src = -8;

  printf("\n\nINTEGER TESTS\n\n ");
  printf("trouble int= %d\n       unsign= %u \n", src, src);
  printIntBits(src);
  printf("\nFrom int to dec = %d  (0 - ok )\n", s21_from_int_to_decimal(src, &s21Numb));
  //setScale(&s21Numb, -5);
  //s21Numb.bits[0] = 0x80000000;
  //s21Numb.bits[3] = 0xFFFFFFFF; // error decimal
  printDecimalBits(s21Numb);
  printf("\n\nMANTISA DEC double = %lf\n\n", mDecimal(s21Numb));
  printf("From dec to int = %d  (0 - ok )\n", s21_from_decimal_to_int(s21Numb, &srcA));
  printf("INT num = %d\n", srcA);
  printIntBits(srcA);
  //printIntBits(s21Numb.bits[0]);
  */


  /* FLOAAAAAAAAAAAAAAAAAAAAT */
  printf("\n\nFLOAT TESTS\n\n ");
  float x = 268435459;
  x = 0.341e-28;//
  //x = 1234567890123456789012345678901234567890.123456789012345678901234567890;
  
  union DateDecLL datai;
  
  
  /*
  char buffFloat[100];
  // Преобразование float в строку
  // С контролем точности
  //float x2 = 1e+29, x3 = 1e-28; //79228162514264337593543950336.0;
  //printf("FLOAT MAX = %f\n", x);
  //printFloatBits(x);
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


  //sprintf(buffFloat, "%-f", x5); 
  //printf("Float %f string x = [%s]\n", x5, buffFloat);
  //printf("true x < x2 & x > 1e-28 ? %d\n", (x5 > 1e-28 && x5 < 1e+29));
  //printf("Checkfloat = %d\n", checkPFloat(x5));
  
  
  //tests for float to dec
  //x = -2147483648;
  //x = 2147483647;
  //(|x| > 79,228,162,514,264,337,593,543,950,335)
  x = 7.92281622514264337593543950335e+28; //max dec
  //printFloatBits(x);
  //float x2 = 7.9228168e+28;
  //printFloatBits(x2);
  // printf(" x = x2  is - %d \n", x == x2);
  //x = -7.92282622514264337593543950335e+28; //biger
  //x = 7.92282622514264337593543950335e+28;  // biger
  //x = 7.92282622514264337593543950335e+35; // much bigger
  //x = 0.9e-28;  //smaller
  //x = 0.5e-35;  //much smaller
  //x = 1e-28; // min border
  //x = 1.0092282622514264337593543950335e-15;
  //x = 2.0092282622514264337593543950335e+19;
  //x = 1.0092282622514264337593543950335e+3;
  //x = 1.0092285622514264337593543950335e+2;
  //x = 1.0092295622514264337593543950335e+2;
  //x = 0.00095295622514264337593543950335e-24;
  //x = 0.00095295632514264337593543950335e+16;
  x = - 8.3e-9;
  //x = 100000;
  //x = -0.0f;
  //x = 0.0f;
  //x = -1.0f;
  //x = -18446744073709551616.0;

  printFloatBits(x);
  //printf("\nX = %60.30f\nXe = %e\nX2= %60.30f\nXe2= %e\n\n", x, x, x2, x2);
  printf("\nX = %60.30f\nXe = %e\n\n", x, x);
  printf("\n\nMAIN Result of float to dec = %d ( 0 - ok )\n", s21_from_float_to_decimal(x, &s21Numb));
  //printDecimalBits(s21Numb);

  //printf("mantisa s21Numb = %30Lf\n", mDecimal(s21Numb));
  //printf("mantisa s21Numb = %.20Le\n", mDecimal(s21Numb));


  //s21Numb.bits[0] = 0x7FFFFFFF; //max int 2147483647
//s21Numb.bits[0] = 0x80000000; //min int 2147483648 with -
//s21Numb.bits[1] = 0xFFFFFFFF;
//s21Numb.bits[2] = 0xFFFFFFFF;
//s21Numb.bits[3] = 0xFFFFFFFF; // error decimal

//datai.llNumb[0] = (unsigned long long int) 17446754073709551616.0;
//printf("\n\nMAIN continue \nx64 [0] = %llu \n",  datai.llNumb[0]);
//printBits(&datai.llNumb[0], 8);
//printBits(&datai.decNumb, 16);
  
//setDecimalBit(&datai.decNumb, 114, 1);
//setScale(&datai.decNumb, 20);
//setSign(&datai.decNumb, 1);







datai.decNumb = s21Numb;
printDecimalBits(datai.decNumb);
 int xifdec = 77;
 x5 = 77;
 printf("\nRES from dec  to  int = %d ( 0 - ok )\n", s21_from_decimal_to_int(datai.decNumb, &xifdec));
 printf("INT   = %d \n", xifdec);
 //printIntBits(xifdec);
 printf("\nRES from dec to float = %d ( 0 - ok )\n", s21_from_decimal_to_float(datai.decNumb, &x5));
 sprintf(buffFloat, "%-.28f", x5); 
 printf("Float = %30f \nFLoat = %e \nstring [%s]\n", x5, x5, buffFloat);
 printf("\n\n DELTA = %e %%\n", (x - x5) / x );

 //datai.decNumb = s21Numb;
 printf("\n\nMAIN continue \nx64 [0] = %llu \n",  datai.llNumb[0]);

 // x5 =  -0.0;//1e-28;
 // x5 *= -1;
 //x5 = -255.0e-15;
 // printf("FLOAT 0 = %f\n", x5);
 // printFloatBits(x5);
 // printf("\nX = %60.30f\nXe= %e\n\n", x5, x5);
 // printf("\n\nMAIN Result of float to dec = %d ( 0 - ok )\n", s21_from_float_to_decimal(x5, &s21Numb));
//  printDecimalBits(s21Numb);

 

  //s21_decimal * ptr_dec_numb;
  //float * ptrf;
  //printf("\n\n %p", ptrf);
  //printf("\n\n %p", NULL);
  //if (ptr_dec_numb) printf("\n\ntrue");
  //else printf("\n\nfalse");

 //printf("    res mDecimal = %20.4lf\n", mDecimal(datai.decNumb));
 //printf("LLI res mDecimal = %20lld\n", (long long int)mDecimal(datai.decNumb));
 /*
 printf("res of s21 to float = %d ( 0 - ok )\n", s21_from_decimal_to_float(datai.decNumb, &x5));
 sprintf(buffFloat, "%-.28f", x5); 
 printf("Float   x = %e \nstring x = [%s]\n", x5, buffFloat);
 printf("\nU int from float = %14u int from bit0 = %14u\n", (int)x5, datai.intNumb[0]);
 printf("\nD int from float = %14d int from bit0 = %14d\n", (int)x5, datai.intNumb[0]);
 printf("\nint from DECIM u = %14u int from DECd = %14d\n", xifdec, xifdec);
 printIntBits((int)x5);
 printIntBits(datai.intNumb[0]);
 */
 
 


 //CHECKDECIMAL?
 //setDecimalBit(&s21Numb, 126, 1);
 //printDecimalBits(s21Numb);
 //printf("\n\n ChekDec = %d (o - ok, 1 - error)\n", checkDecimal(s21Numb));


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