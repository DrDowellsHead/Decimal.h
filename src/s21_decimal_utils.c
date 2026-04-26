#include "s21_decimal_utils.h"

#include <string.h>

int s21_get_bit(s21_decimal value, int n) {
  int arr = n / 32;
  int pos = n % 32;
  return (value.bits[arr] >> pos) & 1;
}

void s21_set_bit(s21_decimal *value, int n, int bit) {
  int arr = n / 32;
  int pos = n % 32;
  if (bit)
    value->bits[arr] |= (1 << pos);
  else
    value->bits[arr] &= ~(1 << pos);
}

int s21_get_sign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

void s21_set_sign(s21_decimal *value, int sign) {
  if (sign)
    value->bits[3] |= (1u << 31);
  else
    value->bits[3] &= ~(1u << 31);
}

int s21_get_scale(s21_decimal value) { return (value.bits[3] >> 16) & 0xFF; }

void s21_set_scale(s21_decimal *value, int scale) {
  value->bits[3] &= ~(0xFF << 16);
  value->bits[3] |= (scale << 16);
}

s21_decimal s21_zero(void) {
  s21_decimal result;
  memset(result.bits, 0, sizeof(result.bits));
  return result;
}

int s21_is_zero(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}