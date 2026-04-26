#include <stddef.h>

#include "s21_big_decimal.h"
#include "s21_decimal.h"
#include "s21_decimal_utils.h"

// --- 224-bit arithmetic for division ---
// Используем 7 блоков по 32 бита = 224 бит

typedef struct {
  unsigned int b[7];
} u224;

static void u224_zero(u224 *a) {
  for (int i = 0; i < 7; i++) a->b[i] = 0;
}

static int u224_is_zero(u224 a) {
  for (int i = 0; i < 7; i++)
    if (a.b[i]) return 0;
  return 1;
}

static int u224_cmp(u224 a, u224 b) {
  for (int i = 6; i >= 0; i--) {
    if (a.b[i] > b.b[i]) return 1;
    if (a.b[i] < b.b[i]) return -1;
  }
  return 0;
}

static void u224_sub(u224 *a, u224 b) {
  unsigned long long borrow = 0;
  for (int i = 0; i < 7; i++) {
    unsigned long long d = (unsigned long long)a->b[i] - b.b[i] - borrow;
    a->b[i] = (unsigned int)d;
    borrow = (d >> 63) & 1;
  }
}

// Shift left by 1 bit
static void u224_shl1(u224 *a) {
  unsigned int carry = 0;
  for (int i = 0; i < 7; i++) {
    unsigned int nc = a->b[i] >> 31;
    a->b[i] = (a->b[i] << 1) | carry;
    carry = nc;
  }
}

// Set bit n of u224
static void u224_setbit(u224 *a, int n) { a->b[n / 32] |= (1u << (n % 32)); }

// Get bit n of u224
static int u224_getbit(u224 a, int n) { return (a.b[n / 32] >> (n % 32)) & 1; }

// Multiply u224 by 10
static int u224_mul10(u224 *a) {
  unsigned long long carry = 0;
  for (int i = 0; i < 7; i++) {
    unsigned long long v = (unsigned long long)a->b[i] * 10 + carry;
    a->b[i] = (unsigned int)(v & 0xFFFFFFFF);
    carry = v >> 32;
  }
  return carry != 0;
}

// Add u32 to u224
static int u224_add_u32(u224 *a, unsigned int v) {
  unsigned long long carry = v;
  for (int i = 0; i < 7 && carry; i++) {
    unsigned long long s = (unsigned long long)a->b[i] + carry;
    a->b[i] = (unsigned int)(s & 0xFFFFFFFF);
    carry = s >> 32;
  }
  return carry != 0;
}

// fits in 96 bits?

// 96-bit from u224 (low 3 words)

// From 96-bit to u224
static u224 u224_from96(unsigned int a[3]) {
  u224 r;
  u224_zero(&r);
  r.b[0] = a[0];
  r.b[1] = a[1];
  r.b[2] = a[2];
  return r;
}

// Divide u224 by u224 using binary long division -> quotient u224, remainder
// u224
static void u224_divmod(u224 dividend, u224 divisor, u224 *quotient,
                        u224 *remainder) {
  u224_zero(quotient);
  u224_zero(remainder);
  for (int bit = 223; bit >= 0; bit--) {
    u224_shl1(remainder);
    if (u224_getbit(dividend, bit)) remainder->b[0] |= 1;
    if (u224_cmp(*remainder, divisor) >= 0) {
      u224_sub(remainder, divisor);
      u224_setbit(quotient, bit);
    }
  }
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) return 1;
  *result = s21_zero();
  if (s21_is_zero(value_2)) return 3;

  int sign = s21_get_sign(value_1) ^ s21_get_sign(value_2);
  int scale1 = s21_get_scale(value_1);
  int scale2 = s21_get_scale(value_2);
  int result_scale = scale1 - scale2;

  unsigned int d1[3] = {(unsigned int)value_1.bits[0],
                        (unsigned int)value_1.bits[1],
                        (unsigned int)value_1.bits[2]};
  unsigned int d2[3] = {(unsigned int)value_2.bits[0],
                        (unsigned int)value_2.bits[1],
                        (unsigned int)value_2.bits[2]};

  u224 dividend = u224_from96(d1);
  u224 divisor = u224_from96(d2);
  u224 quotient, remainder;

  u224_divmod(dividend, divisor, &quotient, &remainder);

  // Набираем дробные цифры пока remainder != 0 и quotient вмещает
  // (максимум 29 итераций)
  int extra_digits = 0;
  while (!u224_is_zero(remainder) && extra_digits < 29) {
    // quotient *= 10
    u224_mul10(&quotient);
    // remainder *= 10
    u224_mul10(&remainder);
    // digit = remainder / divisor
    u224 digit, rem2;
    u224_divmod(remainder, divisor, &digit, &rem2);
    // quotient += digit (digit маленькое, 0-9)
    u224_add_u32(&quotient, digit.b[0]);
    remainder = rem2;
    extra_digits++;
    result_scale++;
  }

  // Если result_scale < 0, умножаем quotient на 10^(-result_scale)
  while (result_scale < 0) {
    if (u224_mul10(&quotient)) return sign ? 2 : 1;
    result_scale++;
  }

  // Собираем big_decimal и применяем rounding
  s21_big_decimal big;
  s21_big_zero(&big);
  for (int i = 0; i < 7; i++) big.bits[i] = quotient.b[i];
  big.scale = result_scale;
  big.sign = sign;

  int err = s21_big_decimal_rounding(big, result);
  if (err == 2) return 2;
  return err ? 1 : 0;
}