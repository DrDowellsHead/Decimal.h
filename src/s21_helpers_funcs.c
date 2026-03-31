#include "s21_helpers_funcs.h"
// Округляет 192-битное число (temp[6]) до 96-битной мантиссы
// Возвращает 0 - успех, 1 - переполнение
int s21_round_192_to_96(unsigned long long temp[6], unsigned int mantissa[3]) {
  // Проверяем, сколько значащих блоков
  int exitcode = 0;
  int high_block = 5;
  while (high_block > 0 && temp[high_block] == 0) {
    high_block--;
  }

  // Если помещается в 96 бит, просто копируем
  if (high_block < 3) {
    mantissa[0] = (unsigned int)temp[0];
    mantissa[1] = (unsigned int)temp[1];
    mantissa[2] = (unsigned int)temp[2];
  } else {
    // Извлекаем лишние биты (биты 96-191)
    unsigned long long extra_low = temp[2] >> 32;
    unsigned long long extra_high = temp[3];

    // Оставляем только 96 бит
    mantissa[0] = (unsigned int)(temp[0] & 0xFFFFFFFF);
    mantissa[1] = (unsigned int)(temp[1] & 0xFFFFFFFF);
    mantissa[2] = (unsigned int)(temp[2] & 0xFFFFFFFF);

    // Банковское округление
    unsigned long long extra = extra_low | (extra_high << 32);
    unsigned long long half = 1ULL << 63;

    int need_increment = 0;
    if (extra > half) {
      need_increment = 1;
    } else if (extra == half) {
      if (mantissa[0] & 1) {
        need_increment = 1;
      }
    }

    // Увеличиваем если нужно
    if (need_increment) {
      unsigned long long carry = 1;
      for (int i = 0; i < 3 && carry; i++) {
        unsigned long long sum = (unsigned long long)mantissa[i] + carry;
        mantissa[i] = (unsigned int)sum;
        carry = sum >> 32;
      }
      if (carry) exitcode = 1;  // Переполнение
    }
  }
  return exitcode;
}