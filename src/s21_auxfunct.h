#ifndef S21_AUXFUNC_H
#define S21_AUXFUNC_H

#include <math.h>
#include <string.h>

#include "s21_decimal.h"

union DateDecLL {
   s21_decimal decNumb;
   unsigned long long int llNumb[2]; 
   int intNumb[4];
 };

int CheckDecimal(s21_decimal numb);
void CheckFloat8(float src, int * mantissa_seven, int * scale_seven);
long double mDecimal(s21_decimal numb); 
void printBits(const void *ptr, size_t size);
void printCharBits(char num);
void printFloatBits(float num);
void printIntBits(int num);
void printDecimalBits(s21_decimal numb);
int getDecimalBit (s21_decimal numb, int bitNumber);
int setDecimalBit(s21_decimal *numb, int bitNumber, int bitDest);
int getSign(s21_decimal numb);
int setSign(s21_decimal *numb, int sign);
int getScale(s21_decimal numb);
int setScale(s21_decimal *numb, int scale);
int bankRoundSeven(int src);
void zeroDecMant(s21_decimal * numb);
void zeroDecNumb(s21_decimal * numb);
s21_decimal mant_div10(s21_decimal numb);
s21_decimal mant_mult10(s21_decimal numb);

#endif