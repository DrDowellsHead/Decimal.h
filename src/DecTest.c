#include "s21_decimal.h"
#include "s21_auxfunct.h"


int main() {
  s21_decimal s21Numb = {0};// = {0};//{{0xFFFFFFFF, 0x000000FF, 0, 0}};
  
 /*   // INTEGER  from int to dec & back 
  s21Numb.bits[3] = 0x001D0000;
  s21Numb.bits[2] = 0x00000000;
  s21Numb.bits[1] = 0x00000000;
  s21Numb.bits[0] = 0x0000FFFF; 


  //s21Numb.bits[3] = 0xFFFFFFFF; // error decimal
    
  int src = 2147483647, src_back = 29;
  //src = 2147483647; //max int
  src = -2147483648; //min int
  //src = -214748367;
  //src = 214748367;
  //src = 483648;
  //src = 8;
  //src = -8;

  
  printf("\n\nINTEGER TESTS\n\n ");
  printf("trouble int = %d\n      unsign  = %u \n", src,(unsigned int)s21Numb.bits[0]);
  
  printIntBits(src);
  printf("\nFrom int to dec = %d  (0 - ok )\n", s21_from_int_to_decimal(src, &s21Numb));
  //setScale(&s21Numb, -5);
  //s21Numb.bits[0] = 0x80000000;
  //s21Numb.bits[3] = 0xFFFFFFFF; // error decimal
  

  printDecimalBits(s21Numb);
  printf("DEC\n3 0x%08X = %u\n2 0x%08X = %u\n1 0x%08X = %u\n0 0x%08X = %u\n\n",s21Numb.bits[3] ,s21Numb.bits[3] , s21Numb.bits[2] ,s21Numb.bits[2] , s21Numb.bits[1], s21Numb.bits[1], s21Numb.bits[0], s21Numb.bits[0]);

  printf("\n\nMANTISA DEC double = %Lf\n\n", mDecimal(s21Numb));
  printf("From dec to int = %d  (0 - ok )\n", s21_from_decimal_to_int(s21Numb, &src_back));
  printf("INT num = %d\n", src_back);
  printIntBits(src_back);
  //printIntBits(s21Numb.bits[0]);
*/


    /// FLOAT __________________________________________________________________________________________
 
    printf("\n\nFLOAT TESTS\n\n ");
    union DateDecLL datai;
    float x = 268435459, x5 = -1234567.8901e+38;
    x = 0.341e-28;
    char buffFloat[100];
      //(|x| > 79,228,162,514,264,337,593,543,950,335)
    x = 7.92281622514264337593543950335e+28f; //max dec
    //printFloatBits(x);
    //x = -7.92282622514264337593543950335e+28; //biger
    x = 7.92281692514264337593543950335e+28;  // biger
    x =   7.922817e+28;  
    //x = 7.92282622514264337593543950335e+35; // much bigger
    //x = 0.9e-28;  //smaller
    //x = 0.5e-35;  //much smaller
    //x = 1e-28; // min border
    //x =  -8.3e-9;
    //x = -100000;
    //x = -0.0f;
    //x = 0.0f;
    x = -1.0f;
    //x = -18446744073709551616.0;
    x = - 7.9e-29;
    //x = 231.0e+16f;
    x = 2.46592659e-11f;
   
   
   
   
    int xifdec = 77;
//s21Numb.bits[0] = 0x0000C000;


    printf("\nX = %60.30f\nXe = %e\n\n", x, x);
    printf("\n\nMAIN Result of float to dec = %d ( 0 - ok )\n\n", s21_from_float_to_decimal(x, &s21Numb));
    //printDecimalBits(s21Numb);
      //printf("mantisa s21Numb = %30Lf\n", mDecimal(s21Numb));
      //printf("mantisa s21Numb = %.20Le\n", mDecimal(s21Numb));

    //s21Numb.bits[3] = 0xFFFFFFFF; // error decimal
    datai.decNumb = s21Numb;

//datai.llNumb[0] = (unsigned long long int) 17446754073709551616.0;

    printDecimalBits(datai.decNumb);
    printf("DEC\n3 0x%08X = %d\n2 0x%08X = %d\n1 0x%08X = %d\n0 0x%08X = %d\n\n",s21Numb.bits[3] ,s21Numb.bits[3] , s21Numb.bits[2] ,s21Numb.bits[2] , s21Numb.bits[1], s21Numb.bits[1], s21Numb.bits[0], s21Numb.bits[0]);

    printf("\n\nMANTISA DEC double = %Lf or %Le\n\n", mDecimal(s21Numb), mDecimal(s21Numb));


    

 printf("\nRES from dec  to  int = %d ( 0 - ok )\n", s21_from_decimal_to_int(datai.decNumb, &xifdec));
 printf("INT   = %d \n", xifdec);
 //printIntBits(xifdec);
 printf("\nRES from dec to float = %d ( 0 - ok )\n", s21_from_decimal_to_float(datai.decNumb, &x5));
 sprintf(buffFloat, "%-.28f", x5); 
 printf("Float = %30f \nFLoat = %e \nstring [%s]\n", x5, x5, buffFloat);
 printf("\n\n DELTA = %e %%\n", (x - x5) / x );

 //datai.decNumb = s21Numb;
 printf("\n\nMAIN continue \nx64 [0] = %llu \n",  datai.llNumb[0]);


  s21Numb.bits[3] = 0x80100000;
  s21Numb.bits[2] = 0x00000000;
  s21Numb.bits[1] = 0x00190000;
  s21Numb.bits[0] = 0x00000000; 

  s21_decimal x_dec;
  x_dec.bits[3] = 0x001D0000;
  x_dec.bits[2] = 0x00000000;
  x_dec.bits[1] = 0x00190000;
  x_dec.bits[0] = 0x00400000;

  
  x =  -7.03687429e-01;
  x5 = x;
  
  datai.decNumb = x_dec;
  printDecimalBits(datai.decNumb);
  printf("DEC\n3 0x%08X = %d\n2 0x%08X = %d\n1 0x%08X = %d\n0 0x%08X = %d\n\n",s21Numb.bits[3] ,s21Numb.bits[3] , s21Numb.bits[2] ,s21Numb.bits[2] , s21Numb.bits[1], s21Numb.bits[1], s21Numb.bits[0], s21Numb.bits[0]);

  printf("\nRES from dec to float = %d ( 0 - ok )\n", s21_from_decimal_to_float(x_dec, &x5));
  //sprintf(buffFloat, "%-.28f", x5); 
  //printf("Float = %30f \nFLoat = %e \nstring [%s]\n", x5, x5, buffFloat);
  printFloatBits(x5);
  printFloatBits(x);
  printf("\n\nMANTISA DEC double = %Lf or %Le\n\n", mDecimal(s21Numb), mDecimal(s21Numb));
  //printf("\n\n DELTA = %e %%\n", (x - x5) / x );



    return 0;
} 

//gcc DecTest.c s21_auxfunct.c s21_decimal_transform.c -lm