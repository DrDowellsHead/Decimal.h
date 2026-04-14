#ifndef S21_AUXFUNC_H
#define S21_AUXFUNC_H

#include <math.h>
#include <string.h>

#include "s21_decimal.h"

int CheckDecimalTR(s21_decimal numb);
void CheckFloatTR(float src, int *mantissa_seven, int *scale_seven);
long double MantissaDecimalTR(s21_decimal numb);
int BankRoundTR(int src);
void ZeroDecimal(s21_decimal *numb);
s21_decimal MantissaDiv10(s21_decimal numb);
s21_decimal MantissaMult10(s21_decimal numb);
int getSign(s21_decimal numb);
int setSign(s21_decimal *numb, int sign);
int getScale(s21_decimal numb);
int setScale(s21_decimal *numb, int scale);
int getDecimalBit(s21_decimal numb, int bitNumber);
int setDecimalBit(s21_decimal *numb, int bitNumber, int bitDest);
// void printDecimalBits(s21_decimal numb);

#endif