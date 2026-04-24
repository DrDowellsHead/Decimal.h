#include <stdint.h>

#include "s21_decimal.h"
#include "s21_decimal_utils.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (!result) {
    err = 1;
  } else {
    *result = value;
    s21_set_sign(result, !s21_get_sign(value));
  }
  return err;
}

static int s21_div10(s21_decimal *value) {
  uint64_t remainder = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t cur = remainder * (uint64_t)0x100000000 + (uint32_t)value->bits[i];
    value->bits[i] = (int)(cur / 10);
    remainder = cur % 10;
  }
  return (int)remainder;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (!result) {
    err = 1;
  } else {
    *result = value;
    int scale = s21_get_scale(value);
    for (int i = 0; i < scale; i++) {
      s21_div10(result);
    }
    s21_set_scale(result, 0);
  }
  return err;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (!result) {
    err = 1;
  } else {
    int sign = s21_get_sign(value);
    int scale = s21_get_scale(value);
    *result = value;
    int has_fraction = 0;
    for (int i = 0; i < scale; i++) {
      if (s21_div10(result) != 0) has_fraction = 1;
    }
    s21_set_scale(result, 0);
    if (sign && has_fraction) {
      uint64_t carry = 1;
      for (int i = 0; i < 3 && carry; i++) {
        uint64_t sum = (uint64_t)result->bits[i] + carry;
        result->bits[i] = (int)(sum & 0xFFFFFFFF);
        carry = sum >> 32;
      }
      if (carry) err = 1;
    }
  }
  return err;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (!result) {
    err = 1;
  } else {
    int scale = s21_get_scale(value);
    int sign = s21_get_sign(value);
    if (scale == 0) {
      *result = value;
    } else {
      *result = value;
      s21_set_sign(result, 0);
      int first_digit = 0;
      for (int i = 0; i < scale; i++) {
        int digit = s21_div10(result);
        if (i == scale - 1) {
          first_digit = digit;
        }
      }
      s21_set_scale(result, 0);
      int add_one = 0;
      if (first_digit >= 5) {
        add_one = 1;
      }
      if (add_one) {
        uint64_t carry = 1;
        for (int i = 0; i < 3 && carry; i++) {
          uint64_t sum = (uint64_t)result->bits[i] + carry;
          result->bits[i] = (int)(sum & 0xFFFFFFFF);
          carry = sum >> 32;
        }
        if (carry) err = 1;
      }
      if (!err) s21_set_sign(result, sign);
    }
  }
  return err;
}